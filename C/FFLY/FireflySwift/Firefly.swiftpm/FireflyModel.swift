import SwiftUI
import Observation

/// Owns the simulation and drives it at a fixed rate.
///
/// The 1993 program ran `CalcFlash` + `DrawFlies` as fast as the CPU allowed in its message
/// loop; here a timer loop does one tick every 1/`stepsPerSecond` seconds instead.
@MainActor
@Observable
final class FireflyModel {

    /// Current settings. Change them through `update(_:)` / `handleKey(_:)`.
    private(set) var parameters: FireflyParameters

    /// Bumped after every change so the canvas redraws.
    private(set) var frame = 0

    var isPaused = false

    /// Simulation ticks per second.
    var stepsPerSecond: Double = 20

    /// Draw the little firefly bitmaps instead of plain squares
    /// (the original switched to bitmaps when started with a third command-line argument).
    var useSprites = false

    @ObservationIgnored let engine: FireflyEngine
    @ObservationIgnored private var tickTask: Task<Void, Never>?

    init() {
        let initial = FireflyParameters()
        parameters = initial
        engine = FireflyEngine(parameters: initial)
        var generator = SystemRandomNumberGenerator()
        engine.randomize(using: &generator)
    }

    // MARK: Tick loop

    func start() {
        guard tickTask == nil else { return }
        tickTask = Task { [self] in
            await run()
        }
    }

    func stop() {
        tickTask?.cancel()
        tickTask = nil
    }

    private func run() async {
        let clock = ContinuousClock()
        var next = clock.now
        while !Task.isCancelled {
            if !isPaused {
                engine.step()
                frame &+= 1
            }
            next = next.advanced(by: .seconds(1.0 / max(1.0, stepsPerSecond)))
            if next < clock.now {
                next = clock.now   // fell behind (e.g. app was suspended): don't try to catch up
            }
            try? await clock.sleep(until: next)
        }
    }

    // MARK: Settings

    /// Changes the settings, e.g. `model.update { $0.cellsAround = 6 }`.
    func update(_ change: (inout FireflyParameters) -> Void) {
        var changed = parameters
        change(&changed)
        apply(changed)
    }

    /// Same keys as the original (c/C e/E t/T r/R g/G). Returns true when the key was used.
    @discardableResult
    func handleKey(_ key: Character) -> Bool {
        var changed = parameters
        guard changed.handleKey(key) else { return false }
        apply(changed)
        return true
    }

    private func apply(_ newValue: FireflyParameters) {
        let normalized = newValue.normalized()
        guard normalized != parameters else { return }
        parameters = normalized
        engine.setParameters(normalized)
        frame &+= 1
    }

    /// Back to the default Cellsaround / Energie / Thresholddiv (the `r` key).
    func resetSettings() {
        handleKey("r")
    }

    /// Fresh random energies on the same grid.
    func restart() {
        var generator = SystemRandomNumberGenerator()
        engine.randomize(using: &generator)
        frame &+= 1
    }
}
