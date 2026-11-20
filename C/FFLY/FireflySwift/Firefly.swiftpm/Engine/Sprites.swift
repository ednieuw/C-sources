import Foundation
#if canImport(CoreGraphics)
import CoreGraphics
#endif

/// The 16x16 firefly bitmaps of the original program, used when "sprites" are switched on
/// (the third command-line argument of FIREFLY.EXE).
///
/// Generated from FIREFLY.BMP (lit), FIREFLY1.BMP (dim) and FIREFLY2.BMP (all black - not
/// needed, a dark cell is simply the black background).
///
/// Legend:  .  black    w  light grey (192)   g  grey (128)
///          b  blue     d  green (0,191,0)    G  bright green (0,255,0)
/// Row 0 is the top row.
enum FireflySprites {

    static let size = 16

    struct RGB {
        let red: UInt8
        let green: UInt8
        let blue: UInt8
    }

    static let black = RGB(red: 0, green: 0, blue: 0)

    static let palette: [Character: RGB] = [
        ".": black,
        "w": RGB(red: 192, green: 192, blue: 192),
        "g": RGB(red: 128, green: 128, blue: 128),
        "b": RGB(red: 0, green: 0, blue: 255),
        "d": RGB(red: 0, green: 191, blue: 0),
        "G": RGB(red: 0, green: 255, blue: 0),
    ]

    /// FIREFLY.BMP: the firefly that just flashed (bright abdomen).
    static let lit: [String] = [
        "................",
        "......w...w.....",
        "......w..w......",
        "ggggg.bddb......",
        ".gggggbddbgggggg",
        "..ggggddddgggggg",
        ".....GGGGGG..gg.",
        "....GGGGGGGw....",
        "...wGGGGGGGGw...",
        "..w..GGGGGG.w...",
        ".....w...w......",
        "....w.....w.....",
        "...w......w.....",
        "................",
        "................",
        "................",
    ]

    /// FIREFLY1.BMP: the afterglow frame (dim abdomen).
    static let dim: [String] = [
        "................",
        "......w...w.....",
        "......w..w......",
        "ggggg.bggb......",
        ".gggggbggbgggggg",
        "..gggggggggggggg",
        ".....dddddd..gg.",
        "....dddddddw....",
        "...wddddddddw...",
        "..w..dddddd.w...",
        ".....w...w......",
        "....w.....w.....",
        "...w......w.....",
        "................",
        "................",
        "................",
    ]

    /// RGBA bytes (row-major, 4 bytes per pixel, alpha 255) of a sprite given as `size` strings.
    static func rgba(_ rows: [String]) -> [UInt8] {
        var bytes: [UInt8] = []
        bytes.reserveCapacity(size * size * 4)
        for row in rows {
            for character in row {
                let color = palette[character] ?? black
                bytes.append(color.red)
                bytes.append(color.green)
                bytes.append(color.blue)
                bytes.append(255)
            }
        }
        return bytes
    }

    #if canImport(CoreGraphics)
    /// A 16x16 `CGImage` of the sprite.
    static func cgImage(_ rows: [String]) -> CGImage? {
        let bytes = rgba(rows)
        guard let provider = CGDataProvider(data: Data(bytes) as CFData) else { return nil }
        return CGImage(
            width: size,
            height: size,
            bitsPerComponent: 8,
            bitsPerPixel: 32,
            bytesPerRow: size * 4,
            space: CGColorSpaceCreateDeviceRGB(),
            bitmapInfo: CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue),
            provider: provider,
            decode: nil,
            shouldInterpolate: false,
            intent: .defaultIntent
        )
    }
    #endif
}
