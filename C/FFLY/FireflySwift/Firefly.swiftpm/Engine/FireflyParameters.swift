import Foundation

/// Every user-adjustable setting of the simulation: the globals at the top of FIREFLY.C
/// (`cellaround`, `Energie`, `thresholddiv`, `Glow`, `nooffliesx`, `nooffliesy`).
struct FireflyParameters: Equatable {

    // MARK: Defaults (the #defines / initial values of FIREFLY.C)

    static let defaultColumns = 120        // nooffliesx
    static let defaultRows = 100           // nooffliesy
    static let defaultCellsAround = 4      // CELLAROUND
    static let defaultEnergie = 110        // ENERGIE
    static let defaultThresholdDiv = 4     // THRESHOLDDIV

    /// A cell fires when its energy reaches this value (`threshold` in FIREFLY.C).
    static let threshold: Float = 1000

    /// Energy a non-firing cell gains per tick at the default Energie
    /// (the literal `genergie[u] += 10` in CalcFlash).
    static let baseCharge: Float = 10

    // MARK: Limits
    // The original only enforced the lower bounds. The upper bounds keep the neighbour
    // kernel and the grid at a sensible size on a phone/tablet.

    static let cellsAroundRange = 1...20
    static let energieRange = 1...999
    static let thresholdDivRange = 1...500
    static let dimensionRange = 4...300
    /// FIREFLY.C allowed 16 000 cells (Win16 64K segments); there is no such limit anymore.
    static let maxCells = 40_000

    // MARK: Values

    var columns = FireflyParameters.defaultColumns
    var rows = FireflyParameters.defaultRows
    var cellsAround = FireflyParameters.defaultCellsAround
    var energie = FireflyParameters.defaultEnergie
    var thresholdDiv = FireflyParameters.defaultThresholdDiv
    var glow = true

    /// Energy gained per tick by a cell that does not fire.
    ///
    /// FIREFLY.C displays "Energie" but never uses it (the charge is a hard-coded 10).
    /// Here it scales the charge, and 110 (the default) gives exactly the original 10.
    var charge: Float {
        FireflyParameters.baseCharge * Float(energie) / Float(FireflyParameters.defaultEnergie)
    }

    /// Window title of the original: "Cellsaround %d Energie %d%% Thresholddiv %d %s FIREFLY"
    var title: String {
        "Cellsaround \(cellsAround) Energie \(energie)% Thresholddiv \(thresholdDiv) \(glow ? "Glow" : "") FIREFLY"
    }

    // MARK: Normalising

    /// Returns a copy with every value forced into its allowed range.
    func normalized() -> FireflyParameters {
        var p = self
        p.cellsAround = FireflyParameters.clamp(p.cellsAround, FireflyParameters.cellsAroundRange)
        p.energie = FireflyParameters.clamp(p.energie, FireflyParameters.energieRange)
        p.thresholdDiv = FireflyParameters.clamp(p.thresholdDiv, FireflyParameters.thresholdDivRange)
        let grid = FireflyParameters.fitGrid(columns: p.columns, rows: p.rows)
        p.columns = grid.columns
        p.rows = grid.rows
        return p
    }

    /// Clamps both dimensions into `dimensionRange` and shrinks them (keeping the aspect
    /// ratio) when the grid would exceed `maxCells`.
    static func fitGrid(columns: Int, rows: Int) -> (columns: Int, rows: Int) {
        var c = clamp(columns, dimensionRange)
        var r = clamp(rows, dimensionRange)
        if c * r > maxCells {
            let scale = (Double(maxCells) / Double(c * r)).squareRoot()
            c = clamp(Int(Double(c) * scale), dimensionRange)
            r = clamp(Int(Double(r) * scale), dimensionRange)
        }
        return (c, r)
    }

    private static func clamp(_ value: Int, _ range: ClosedRange<Int>) -> Int {
        min(max(value, range.lowerBound), range.upperBound)
    }

    // MARK: Keyboard (the WM_CHAR handler of FIREFLY.C)

    /// c/C cellaround -/+, e/E Energie -/+, t/T thresholddiv -/+, r/R reset those three
    /// to their defaults, g/G Glow off/on.  Returns false for any other key.
    @discardableResult
    mutating func handleKey(_ key: Character) -> Bool {
        switch key {
        case "c": cellsAround -= 1
        case "C": cellsAround += 1
        case "e": energie -= 1
        case "E": energie += 1
        case "t": thresholdDiv -= 1
        case "T": thresholdDiv += 1
        case "r", "R":
            energie = FireflyParameters.defaultEnergie
            thresholdDiv = FireflyParameters.defaultThresholdDiv
            cellsAround = FireflyParameters.defaultCellsAround
        case "g": glow = false
        case "G": glow = true
        default: return false
        }
        self = normalized()
        return true
    }
}
