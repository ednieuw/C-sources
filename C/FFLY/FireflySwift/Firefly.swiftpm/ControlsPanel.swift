import SwiftUI

/// Touch replacement for the keyboard of the original program (c/C, e/E, t/T, r, g/G ...).
struct ControlsPanel: View {
    @Bindable var model: FireflyModel

    var body: some View {
        VStack(spacing: 0) {
            // the original window title
            Text(model.parameters.title)
                .font(.system(.footnote, design: .monospaced))
                .foregroundStyle(.secondary)
                .padding(.vertical, 8)

            ScrollView {
                VStack(spacing: 10) {
                    Stepper(value: setting(\.cellsAround), in: FireflyParameters.cellsAroundRange) {
                        Text("Cells around: \(model.parameters.cellsAround)")
                    }
                    Stepper(value: setting(\.energie), in: FireflyParameters.energieRange) {
                        Text("Energie: \(model.parameters.energie)%")
                    }
                    Stepper(value: setting(\.thresholdDiv), in: FireflyParameters.thresholdDivRange) {
                        Text("Threshold divisor: \(model.parameters.thresholdDiv)")
                    }

                    Toggle("Glow (dim afterglow frame)", isOn: setting(\.glow))
                    Toggle("Firefly bitmaps instead of squares", isOn: $model.useSprites)

                    HStack {
                        Text("Speed")
                        Slider(value: $model.stepsPerSecond, in: 1...60, step: 1)
                        Text("\(Int(model.stepsPerSecond))/s")
                            .monospacedDigit()
                            .frame(width: 48, alignment: .trailing)
                    }

                    Stepper(value: setting(\.columns), in: FireflyParameters.dimensionRange, step: 10) {
                        Text("Columns: \(model.parameters.columns)")
                    }
                    Stepper(value: setting(\.rows), in: FireflyParameters.dimensionRange, step: 10) {
                        Text("Rows: \(model.parameters.rows)")
                    }

                    HStack(spacing: 12) {
                        Button(model.isPaused ? "Resume" : "Pause") {
                            model.isPaused.toggle()
                        }
                        Button("Restart") {
                            model.restart()
                        }
                        Button("Reset settings") {
                            model.resetSettings()
                        }
                    }
                    .buttonStyle(.bordered)

                    Text("Tap the picture to hide this panel. Keyboard: c/C e/E t/T r g/G")
                        .font(.caption2)
                        .foregroundStyle(.secondary)
                }
                .padding(.horizontal)
                .padding(.bottom, 12)
            }
        }
        .frame(maxWidth: 520, maxHeight: 420)
        .background(.regularMaterial, in: RoundedRectangle(cornerRadius: 16))
    }

    /// Two-way binding to one field of the parameters; every change goes through the model so
    /// the engine is updated too.
    private func setting<Value>(_ keyPath: WritableKeyPath<FireflyParameters, Value>) -> Binding<Value> {
        Binding(
            get: { model.parameters[keyPath: keyPath] },
            set: { newValue in
                model.update { $0[keyPath: keyPath] = newValue }
            }
        )
    }
}
