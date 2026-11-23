import SwiftUI

struct ContentView: View {
    @State private var model = FireflyModel()
    @State private var showControls = true
    @FocusState private var hasKeyboardFocus: Bool

    var body: some View {
        ZStack(alignment: .bottom) {
            FireflyCanvas(model: model)
                .ignoresSafeArea()
                .contentShape(Rectangle())
                .onTapGesture {
                    withAnimation(.easeInOut(duration: 0.2)) {
                        showControls.toggle()
                    }
                }

            if showControls {
                ControlsPanel(model: model)
                    .padding()
                    .transition(.move(edge: .bottom).combined(with: .opacity))
            }
        }
        .background(Color.black)
        .preferredColorScheme(.dark)
        .statusBarHidden(true)
        .persistentSystemOverlays(.hidden)
        // hardware keyboard (iPad): same keys as the original program
        .focusable()
        .focused($hasKeyboardFocus)
        .focusEffectDisabled()
        .onKeyPress(phases: [.down, .repeat]) { press in
            var used = false
            for character in press.characters {
                if model.handleKey(character) { used = true }
            }
            return used ? .handled : .ignored
        }
        .onAppear {
            hasKeyboardFocus = true
            model.start()
        }
        .onDisappear {
            model.stop()
        }
    }
}
