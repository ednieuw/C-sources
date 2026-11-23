import SwiftUI

/// Draws the grid: black background, bright green for a firefly that just flashed,
/// dark green for the afterglow frame (the colours of the original), or the firefly bitmaps.
struct FireflyCanvas: View {
    let model: FireflyModel

    private static let brightColor = Color(red: 0, green: 1, blue: 0)
    private static let dimColor = Color(red: 0, green: 48.0 / 255.0, blue: 0)

    private static let litImage: Image? = FireflyCanvas.sprite(FireflySprites.lit)
    private static let dimImage: Image? = FireflyCanvas.sprite(FireflySprites.dim)

    private static func sprite(_ rows: [String]) -> Image? {
        guard let cgImage = FireflySprites.cgImage(rows) else { return nil }
        return Image(decorative: cgImage, scale: 1).interpolation(.none)
    }

    var body: some View {
        // Reading `frame` makes SwiftUI rebuild this view after every simulation tick.
        let _ = model.frame
        Canvas(opaque: true) { context, size in
            draw(&context, size)
        }
    }

    private func draw(_ context: inout GraphicsContext, _ size: CGSize) {
        context.fill(Path(CGRect(origin: .zero, size: size)), with: .color(.black))

        let engine = model.engine
        let columns = engine.columns
        let rows = engine.rows
        guard columns > 0, rows > 0, size.width > 0, size.height > 0 else { return }

        // The window is divided evenly (the original lost a few pixels to integer division).
        let cellWidth = size.width / CGFloat(columns)
        let cellHeight = size.height / CGFloat(rows)
        let level = engine.level

        if model.useSprites,
           let litImage = Self.litImage,
           let dimImage = Self.dimImage {
            let lit = context.resolve(litImage)
            let dim = context.resolve(dimImage)
            for row in 0..<rows {
                for column in 0..<columns {
                    let value = level[column + columns * row]
                    if value == 0 { continue }
                    let rect = CGRect(x: CGFloat(column) * cellWidth,
                                      y: CGFloat(row) * cellHeight,
                                      width: cellWidth,
                                      height: cellHeight)
                    context.draw(value == 2 ? lit : dim, in: rect)
                }
            }
            return
        }

        // Squares: the original used Rectangle() with the default black pen, which leaves a
        // thin black grid line around every cell.
        let gap = min(1.0, min(cellWidth, cellHeight) * 0.15)
        var bright = Path()
        var dim = Path()
        for row in 0..<rows {
            for column in 0..<columns {
                let value = level[column + columns * row]
                if value == 0 { continue }
                let rect = CGRect(x: CGFloat(column) * cellWidth,
                                  y: CGFloat(row) * cellHeight,
                                  width: cellWidth,
                                  height: cellHeight).insetBy(dx: gap / 2, dy: gap / 2)
                if value == 2 {
                    bright.addRect(rect)
                } else {
                    dim.addRect(rect)
                }
            }
        }
        context.fill(dim, with: .color(Self.dimColor))
        context.fill(bright, with: .color(Self.brightColor))
    }
}
