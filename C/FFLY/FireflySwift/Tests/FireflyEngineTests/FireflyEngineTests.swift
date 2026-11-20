import XCTest
@testable import FireflyEngine

final class FireflyEngineTests: XCTestCase {

    // MARK: helpers

    private func fnv1a(_ bytes: [UInt8]) -> UInt64 {
        var hash: UInt64 = 0xCBF2_9CE4_8422_2325
        for byte in bytes {
            hash ^= UInt64(byte)
            hash = hash &* 0x0000_0100_0000_01B3   // 64-bit FNV prime 0x100000001B3
        }
        return hash
    }

    private func levelHash(_ engine: FireflyEngine) -> UInt64 {
        fnv1a(engine.level)
    }

    /// Hash over the energies as little-endian Int32, like Reference/firefly_ref.c.
    private func energyHash(_ engine: FireflyEngine) -> UInt64 {
        var bytes: [UInt8] = []
        bytes.reserveCapacity(engine.energy.count * 4)
        for value in engine.energy {
            let v = UInt32(bitPattern: Int32(value))
            bytes.append(UInt8(v & 255))
            bytes.append(UInt8((v >> 8) & 255))
            bytes.append(UInt8((v >> 16) & 255))
            bytes.append(UInt8((v >> 24) & 255))
        }
        return fnv1a(bytes)
    }

    private func count(_ engine: FireflyEngine, level: UInt8) -> Int {
        engine.level.reduce(0) { $0 + ($1 == level ? 1 : 0) }
    }

    private func index(_ engine: FireflyEngine, _ column: Int, _ row: Int) -> Int {
        column + engine.columns * row
    }

    // MARK: RNG

    func testSplitMix64MatchesReferenceValues() {
        var generator = SplitMix64(seed: 42)
        XCTAssertEqual(generator.next(), 0xbdd7_3226_2feb_6e95)
        XCTAssertEqual(generator.next(), 0x28ef_e333_b266_f103)
        XCTAssertEqual(generator.next(), 0x4752_6757_130f_9f52)
    }

    // MARK: coupling kernel

    /// Values printed by the original expression (default cellaround 4, thresholddiv 4).
    func testNeighbourGainsMatchOriginalFormula() {
        let expected: [Int: Int] = [1: 125, 2: 62, 4: 31, 5: 25, 8: 15, 9: 14, 10: 12,
                                    13: 9, 16: 7, 18: 7, 20: 6, 25: 5, 32: 4]
        for (distance, gain) in expected {
            XCTAssertEqual(
                FireflyEngine.gain(distanceSquared: distance, cellsAround: 4, thresholdDiv: 4),
                gain, "distance^2 = \(distance)")
        }
    }

    // MARK: single firefly

    private func engineWithOneFiringCell(glow: Bool, column: Int, row: Int) -> FireflyEngine {
        var parameters = FireflyParameters()
        parameters.columns = 12
        parameters.rows = 10
        parameters.glow = glow
        let engine = FireflyEngine(parameters: parameters)   // all energies start at 0
        engine.setEnergy(FireflyParameters.threshold, column: column, row: row)
        return engine
    }

    func testFiringCellKicksNeighboursAndResets() {
        let engine = engineWithOneFiringCell(glow: true, column: 6, row: 5)
        engine.step()

        XCTAssertEqual(engine.level[index(engine, 6, 5)], 2)
        XCTAssertEqual(engine.energy[index(engine, 6, 5)], 0)            // reset, gets no charge
        XCTAssertEqual(engine.energy[index(engine, 7, 5)], 10 + 125)     // d=1: charge + kick
        XCTAssertEqual(engine.energy[index(engine, 7, 6)], 10 + 62)      // d=2
        XCTAssertEqual(engine.energy[index(engine, 8, 5)], 10 + 31)      // d=4
        XCTAssertEqual(engine.energy[index(engine, 10, 9)], 10 + 4)      // d=32, window corner
        XCTAssertEqual(engine.energy[index(engine, 0, 0)], 10)           // out of reach: charge only
        XCTAssertEqual(count(engine, level: 2), 1)
    }

    func testWindowIsClippedAtTheEdge() {
        let engine = engineWithOneFiringCell(glow: true, column: 0, row: 0)
        engine.step()   // must not crash or wrap around

        XCTAssertEqual(engine.energy[index(engine, 1, 0)], 10 + 125)
        XCTAssertEqual(engine.energy[index(engine, 0, 1)], 10 + 125)
        XCTAssertEqual(engine.energy[index(engine, 1, 1)], 10 + 62)
        XCTAssertEqual(engine.energy[index(engine, 4, 4)], 10 + 4)
        XCTAssertEqual(engine.energy[index(engine, 5, 0)], 10)           // beyond cellaround
        XCTAssertEqual(engine.energy[index(engine, 11, 9)], 10)          // opposite corner: no wrap
    }

    func testGlowLifecycle() {
        let engine = engineWithOneFiringCell(glow: true, column: 6, row: 5)
        let cell = index(engine, 6, 5)
        engine.step(); XCTAssertEqual(engine.level[cell], 2)   // bright
        engine.step(); XCTAssertEqual(engine.level[cell], 1)   // dim
        engine.step(); XCTAssertEqual(engine.level[cell], 0)   // dark again
    }

    func testNoGlowSkipsTheDimFrame() {
        let engine = engineWithOneFiringCell(glow: false, column: 6, row: 5)
        let cell = index(engine, 6, 5)
        engine.step(); XCTAssertEqual(engine.level[cell], 2)
        engine.step(); XCTAssertEqual(engine.level[cell], 0)
    }

    // MARK: Energie (made functional; 110 is exactly the original charge of 10)

    func testChargeScalesWithEnergie() {
        var parameters = FireflyParameters()
        parameters.columns = 8
        parameters.rows = 8
        parameters.energie = 55
        let engine = FireflyEngine(parameters: parameters)
        engine.step()
        XCTAssertTrue(engine.energy.allSatisfy { $0 == 5 })

        parameters.energie = FireflyParameters.defaultEnergie
        engine.setParameters(parameters)
        engine.step()
        XCTAssertTrue(engine.energy.allSatisfy { $0 == 15 })
    }

    // MARK: golden vectors from Reference/firefly_ref.c

    private struct Golden {
        let tick: Int
        let bright: Int
        let dim: Int
        let levelHash: UInt64
        let energyHash: UInt64
    }

    private func run(columns: Int, rows: Int, cellsAround: Int, thresholdDiv: Int,
                     glow: Bool, seed: UInt64, golden: [Golden],
                     file: StaticString = #filePath, line: UInt = #line) {
        var parameters = FireflyParameters()
        parameters.columns = columns
        parameters.rows = rows
        parameters.cellsAround = cellsAround
        parameters.thresholdDiv = thresholdDiv
        parameters.glow = glow
        let engine = FireflyEngine(parameters: parameters)
        engine.randomize(seed: seed)

        var tick = 0
        for expected in golden {
            while tick < expected.tick {
                engine.step()
                tick += 1
            }
            XCTAssertEqual(count(engine, level: 2), expected.bright, "bright @\(tick)", file: file, line: line)
            XCTAssertEqual(count(engine, level: 1), expected.dim, "dim @\(tick)", file: file, line: line)
            XCTAssertEqual(levelHash(engine), expected.levelHash, "level hash @\(tick)", file: file, line: line)
            XCTAssertEqual(energyHash(engine), expected.energyHash, "energy hash @\(tick)", file: file, line: line)
        }
    }

    func testGoldenSmallGridDefaults() {
        run(columns: 12, rows: 10, cellsAround: 4, thresholdDiv: 4, glow: true, seed: 42, golden: [
            Golden(tick: 1, bright: 0, dim: 0, levelHash: 0xde9f_a0da_6fc2_2a85, energyHash: 0x0fa7_21ec_1596_58bc),
            Golden(tick: 10, bright: 21, dim: 23, levelHash: 0x5949_3151_8036_3210, energyHash: 0x1a0c_2275_aecf_6859),
            Golden(tick: 60, bright: 20, dim: 19, levelHash: 0xf5d9_c159_fabd_98d2, energyHash: 0xc0ec_2fa3_c019_3ad6),
        ])
    }

    func testGoldenBrightCountsOfFirstTicks() {
        var parameters = FireflyParameters()
        parameters.columns = 12
        parameters.rows = 10
        let engine = FireflyEngine(parameters: parameters)
        engine.randomize(seed: 42)
        var bright: [Int] = []
        for _ in 0..<12 {
            engine.step()
            bright.append(count(engine, level: 2))
        }
        XCTAssertEqual(bright, [0, 3, 11, 12, 12, 20, 21, 18, 23, 21, 22, 21])
    }

    func testGoldenCalmNoGlow() {
        run(columns: 30, rows: 20, cellsAround: 2, thresholdDiv: 40, glow: false, seed: 7, golden: [
            Golden(tick: 1, bright: 0, dim: 0, levelHash: 0xb5f2_58c1_5049_1005, energyHash: 0x1a04_e6df_c00a_027f),
            Golden(tick: 30, bright: 6, dim: 0, levelHash: 0x411e_26c7_e72b_d8bd, energyHash: 0xfe30_c86e_d606_ccbc),
            Golden(tick: 100, bright: 6, dim: 0, levelHash: 0x546c_f821_eb25_df25, energyHash: 0x67f8_9f6d_7709_e7bb),
        ])
    }

    func testGoldenFullSizeDefaults() {
        run(columns: 120, rows: 100, cellsAround: 4, thresholdDiv: 4, glow: true, seed: 1, golden: [
            Golden(tick: 1, bright: 0, dim: 0, levelHash: 0x1dfe_af77_3f5e_bca5, energyHash: 0x2bee_6f79_9722_7fde),
            Golden(tick: 25, bright: 2973, dim: 2882, levelHash: 0x6925_4f36_f4e8_8b13, energyHash: 0xa540_e460_8bbe_f9c5),
            Golden(tick: 100, bright: 2912, dim: 2925, levelHash: 0x1350_448d_92d3_704a, energyHash: 0x6000_b436_086e_0fe7),
        ])
    }

    func testGoldenLargeWindow() {
        run(columns: 16, rows: 16, cellsAround: 7, thresholdDiv: 12, glow: true, seed: 99, golden: [
            Golden(tick: 1, bright: 0, dim: 0, levelHash: 0xd80a_c658_736b_b725, energyHash: 0x1bf7_1f3b_f7d4_d7ee),
            Golden(tick: 30, bright: 5, dim: 3, levelHash: 0x174d_285f_1859_bc82, energyHash: 0x1e21_11e4_d2fe_ac99),
        ])
    }

    // MARK: parameters

    func testKeyMapFollowsWMChar() {
        var p = FireflyParameters()
        p.handleKey("C"); p.handleKey("C")
        p.handleKey("E")
        p.handleKey("T"); p.handleKey("T"); p.handleKey("T")
        XCTAssertEqual(p.cellsAround, 6)
        XCTAssertEqual(p.energie, 111)
        XCTAssertEqual(p.thresholdDiv, 7)

        p.handleKey("g")
        XCTAssertFalse(p.glow)
        p.handleKey("G")
        XCTAssertTrue(p.glow)

        p.handleKey("r")   // resets the three numbers, not Glow or the grid
        XCTAssertEqual(p.cellsAround, 4)
        XCTAssertEqual(p.energie, 110)
        XCTAssertEqual(p.thresholdDiv, 4)

        XCTAssertFalse(p.handleKey("x"))
    }

    func testKeysNeverGoBelowOne() {
        var p = FireflyParameters()
        for _ in 0..<200 {   // Energie starts at 110, so more presses than that
            p.handleKey("c"); p.handleKey("e"); p.handleKey("t")
        }
        XCTAssertEqual(p.cellsAround, 1)
        XCTAssertEqual(p.energie, 1)
        XCTAssertEqual(p.thresholdDiv, 1)
    }

    func testTitleMatchesOriginalFormat() {
        var p = FireflyParameters()
        XCTAssertEqual(p.title, "Cellsaround 4 Energie 110% Thresholddiv 4 Glow FIREFLY")
        p.glow = false   // the original prints an empty string for "Glow", leaving two spaces
        XCTAssertEqual(p.title, "Cellsaround 4 Energie 110% Thresholddiv 4  FIREFLY")
    }

    func testGridIsClampedAndCapped() {
        let tiny = FireflyParameters.fitGrid(columns: 1, rows: 1)
        XCTAssertEqual(tiny.columns, 4)
        XCTAssertEqual(tiny.rows, 4)

        let huge = FireflyParameters.fitGrid(columns: 300, rows: 300)
        XCTAssertLessThanOrEqual(huge.columns * huge.rows, FireflyParameters.maxCells)
        XCTAssertEqual(huge.columns, huge.rows)

        let normal = FireflyParameters.fitGrid(columns: 120, rows: 100)
        XCTAssertEqual(normal.columns, 120)
        XCTAssertEqual(normal.rows, 100)
    }

    func testChangingTheGridReallocates() {
        let engine = FireflyEngine()
        XCTAssertEqual(engine.level.count, 120 * 100)
        var p = engine.parameters
        p.columns = 50
        p.rows = 40
        engine.setParameters(p)
        XCTAssertEqual(engine.columns, 50)
        XCTAssertEqual(engine.rows, 40)
        XCTAssertEqual(engine.energy.count, 2000)
        XCTAssertEqual(engine.level.count, 2000)
        engine.step()   // must work on the new grid
    }

    // MARK: sprites

    func testSpritesMatchTheOriginalBitmaps() {
        XCTAssertEqual(FireflySprites.lit.count, 16)
        XCTAssertEqual(FireflySprites.dim.count, 16)
        XCTAssertTrue((FireflySprites.lit + FireflySprites.dim).allSatisfy { $0.count == 16 })

        func tally(_ rows: [String]) -> [Character: Int] {
            var counts: [Character: Int] = [:]
            for row in rows { for ch in row { counts[ch, default: 0] += 1 } }
            return counts
        }
        // pixel counts read from FIREFLY.BMP / FIREFLY1.BMP
        XCTAssertEqual(tally(FireflySprites.lit), ["G": 27, "d": 8, "w": 15, "g": 28, "b": 4, ".": 174])
        XCTAssertEqual(tally(FireflySprites.dim), ["d": 27, "w": 15, "g": 36, "b": 4, ".": 174])
        XCTAssertEqual(FireflySprites.rgba(FireflySprites.lit).count, 16 * 16 * 4)
    }
}
