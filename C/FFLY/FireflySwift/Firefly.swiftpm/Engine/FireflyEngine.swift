import Foundation

/// Simulation core of FIREFLY.C (Ed Nieuwenhuys, 1993): `CalcFlash()` plus the cell state
/// machine of `DrawFlies()`, without any drawing.
///
/// Every cell has an energy. Each tick a cell that has reached the threshold *fires*: it kicks
/// all cells within `cellsAround` (more strongly the closer they are) and drops back to 0.
/// Cells that do not fire charge up by a fixed amount. All cells are updated synchronously.
///
/// A cell is shown bright for the tick in which it fired, then (with Glow) dim for one tick.
///
/// Index of a cell: `column + columns * row`.
final class FireflyEngine {

    private(set) var parameters: FireflyParameters
    private(set) var columns: Int
    private(set) var rows: Int

    /// `energie[]` in FIREFLY.C.
    private(set) var energy: [Float]
    /// What `DrawFlies()` paints for each cell: 0 dark, 1 dim (glow), 2 bright.
    private(set) var level: [UInt8]

    /// `genergie[]`: energy received during the current tick.
    private var pending: [Float]
    /// `flash[]`: 0 nothing, 1 glow pending, 2 fired this tick.
    private var flash: [UInt8]
    private var kernel: Kernel

    init(parameters: FireflyParameters = FireflyParameters()) {
        let p = parameters.normalized()
        self.parameters = p
        columns = p.columns
        rows = p.rows
        let count = p.columns * p.rows
        energy = [Float](repeating: 0, count: count)
        level = [UInt8](repeating: 0, count: count)
        pending = [Float](repeating: 0, count: count)
        flash = [UInt8](repeating: 0, count: count)
        kernel = Kernel(cellsAround: p.cellsAround, thresholdDiv: p.thresholdDiv)
    }

    // MARK: Setup

    /// `Init()` of FIREFLY.C: every cell gets a random energy in 0..<threshold; nothing is lit.
    ///
    /// The loop order (columns outer, rows inner) and the `% threshold` mapping follow the
    /// original so a seeded generator reproduces `Reference/firefly_ref.c` exactly.
    func randomize<G: RandomNumberGenerator>(using generator: inout G) {
        let limit = UInt64(FireflyParameters.threshold)
        for n in 0..<columns {
            for m in 0..<rows {
                energy[n + columns * m] = Float(generator.next() % limit)
            }
        }
        clearTransientState()
    }

    func randomize(seed: UInt64) {
        var generator = SplitMix64(seed: seed)
        randomize(using: &generator)
    }

    /// Applies new settings. A different grid size reallocates and re-randomises the grid.
    func setParameters(_ newValue: FireflyParameters) {
        let p = newValue.normalized()
        let gridChanged = p.columns != columns || p.rows != rows
        parameters = p
        if gridChanged {
            columns = p.columns
            rows = p.rows
            let count = columns * rows
            energy = [Float](repeating: 0, count: count)
            level = [UInt8](repeating: 0, count: count)
            pending = [Float](repeating: 0, count: count)
            flash = [UInt8](repeating: 0, count: count)
            var generator = SystemRandomNumberGenerator()
            randomize(using: &generator)
        }
        if kernel.cellsAround != p.cellsAround || kernel.thresholdDiv != p.thresholdDiv {
            kernel = Kernel(cellsAround: p.cellsAround, thresholdDiv: p.thresholdDiv)
        }
    }

    /// Test helper: sets the energy of one cell.
    func setEnergy(_ value: Float, column: Int, row: Int) {
        energy[column + columns * row] = value
    }

    private func clearTransientState() {
        for u in 0..<pending.count {
            pending[u] = 0
            flash[u] = 0
            level[u] = 0
        }
    }

    // MARK: Simulation

    /// One tick: `CalcFlash()` followed by the state machine of `DrawFlies()`.
    func step() {
        calcFlash()
        advanceDisplay()
    }

    /// Energy a neighbour at squared distance `d` receives from a firing cell:
    /// `(int)(0.5 + 0.5 * (threshold * (maxdistance / d / maxdistance))) / thresholddiv`
    /// with the same float/double mix as the original.
    static func gain(distanceSquared d: Int, cellsAround: Int, thresholdDiv: Int) -> Int {
        let maxDistance = 2 * cellsAround * cellsAround
        let ratio = Float(maxDistance) / Float(d) / Float(maxDistance)
        let scaled = FireflyParameters.threshold * ratio
        return Int(0.5 + 0.5 * Double(scaled)) / thresholdDiv
    }

    /// `CalcFlash()`.
    private func calcFlash() {
        let nx = columns
        let ny = rows
        let threshold = FireflyParameters.threshold
        let charge = parameters.charge
        let dxs = kernel.dx
        let dys = kernel.dy
        let gains = kernel.gain
        let taps = gains.count

        energy.withUnsafeMutableBufferPointer { e in
            pending.withUnsafeMutableBufferPointer { g in
                flash.withUnsafeMutableBufferPointer { f in
                    // 1. fired cells kick their neighbours, the others charge up
                    for n in 0..<nx {
                        for m in 0..<ny {
                            let u = n + nx * m
                            if e[u] >= threshold {
                                for t in 0..<taps {
                                    let i = n + Int(dxs[t])
                                    let j = m + Int(dys[t])
                                    if i < 0 || i >= nx || j < 0 || j >= ny { continue }
                                    g[i + nx * j] += gains[t]
                                }
                                f[u] = 2
                            } else {
                                g[u] += charge
                            }
                        }
                    }
                    // 2. apply everything at once; fired cells start again from 0
                    for u in 0..<(nx * ny) {
                        e[u] += g[u]
                        g[u] = 0
                        if f[u] == 2 { e[u] = 0 }
                    }
                }
            }
        }
    }

    /// The cell state machine of `DrawFlies()`: bright while fired, then dim (if Glow), then dark.
    private func advanceDisplay() {
        let afterBright: UInt8 = parameters.glow ? 1 : 0
        for u in 0..<flash.count {
            switch flash[u] {
            case 2:
                level[u] = 2
                flash[u] = afterBright
            case 1:
                level[u] = 1
                flash[u] = 0
            default:
                level[u] = 0
            }
        }
    }

    // MARK: Neighbour kernel

    /// Precomputed neighbour offsets and the energy each one receives. Entries whose gain
    /// rounds to zero are left out (adding 0 changes nothing).
    private struct Kernel {
        let cellsAround: Int
        let thresholdDiv: Int
        var dx: [Int32] = []
        var dy: [Int32] = []
        var gain: [Float] = []

        init(cellsAround: Int, thresholdDiv: Int) {
            self.cellsAround = cellsAround
            self.thresholdDiv = thresholdDiv
            // same visiting order as the original loops: x outer, y inner
            for offsetX in -cellsAround...cellsAround {
                for offsetY in -cellsAround...cellsAround {
                    if offsetX == 0 && offsetY == 0 { continue }
                    let d = offsetX * offsetX + offsetY * offsetY
                    let g = FireflyEngine.gain(distanceSquared: d,
                                               cellsAround: cellsAround,
                                               thresholdDiv: thresholdDiv)
                    if g > 0 {
                        dx.append(Int32(offsetX))
                        dy.append(Int32(offsetY))
                        gain.append(Float(g))
                    }
                }
            }
        }
    }
}
