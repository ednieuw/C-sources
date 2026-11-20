# Fireflies - revived

A Swift/SwiftUI revival of **FIREFLY** (Ed Nieuwenhuys, 1993), the Windows 3.x program whose
source is in `../FIREFLY.C`. A grid of fireflies charges up, flashes and nudges its neighbours;
the flashes ripple across the screen in waves. The original `firefly.exe` is a 16-bit binary that
no longer runs, so this app re-implements it for iPad / iPhone.

## Run it on your iPad (Swift Playgrounds)

`Firefly.swiftpm` is a Swift Playgrounds **App Playground**.

1. On the iPad open Safari and download
   `https://github.com/ednieuw/C-sources/raw/claude/firefly-ios-macos-swift-o5gxxa/C/FFLY/FireflySwift/Firefly.swiftpm.zip`
   (or open `Firefly.swiftpm.zip` in the GitHub app/website and tap *Download*).
2. In the **Files** app, tap the zip to unzip it. You get a `Firefly` playground.
3. Open it with **Swift Playgrounds** (free from the App Store, version 4.4 or newer) and tap **Run**.

Alternative: clone the repository with a Git app (e.g. *Working Copy*) and open the folder
`C/FFLY/FireflySwift/Firefly.swiftpm` in Swift Playgrounds.

On a Mac you can open the same `Firefly.swiftpm` in Xcode (it runs on iPhone/iPad simulators, and
on Apple-silicon Macs as an iPad app).

Requires iOS / iPadOS 17 or newer.

## Using it

Tap the picture to show or hide the control panel.

| Control | Original key | What it does |
|---|---|---|
| Cells around | `c` / `C` | Size of the neighbourhood a flash reaches (min 1) |
| Energie | `e` / `E` | How fast the fireflies charge up (110 % = the original speed) |
| Threshold divisor | `t` / `T` | Divides how hard a flash kicks its neighbours: higher = gentler, calmer (min 1) |
| Reset settings | `r` | Back to Cellsaround 4, Energie 110 %, Thresholddiv 4 |
| Glow | `g` / `G` | Off / on: leave a dim afterglow frame after each flash |
| Firefly bitmaps | 3rd command-line argument | Draw the little 16x16 firefly pictures instead of squares |
| Columns / Rows | 1st / 2nd command-line argument | Grid size (default 120 x 100) |
| Speed | - | Simulation steps per second (the original ran as fast as the PC allowed) |
| Restart | - | New random starting state |
| Pause / Resume | - | Freeze the picture |

With an iPad hardware keyboard the original keys `c C e E t T r g G` work directly.

Try **Threshold divisor 40** with **Cells around 4** for the calmer, more "synchronising" regime
that the old commented-out settings in `FIREFLY.C` were aiming at; the default (4) gives the dense
shimmering waves.

The panel's caption shows the window title of the original:
`Cellsaround 4 Energie 110% Thresholddiv 4 Glow FIREFLY`.

## How it maps to the C original

Every tick runs `CalcFlash` and then `DrawFlies`, as in `FIREFLY.C`:

* A cell whose energy has reached **1000** *fires*. Every other cell within `Cellsaround` (a square
  window, clipped at the edges, no wrap-around) gains
  `(int)(0.5 + 0.5 * 1000 / d) / Thresholddiv`, where `d` is the squared distance. The firing cell
  drops back to 0.
* Cells that do not fire charge up by 10 per tick.
* All cells are updated at the same time. A cell is **bright** `(0,255,0)` in the tick it fires and
  **dim** `(0,48,0)` in the next one (with Glow).

Deliberate differences:

* The original shows **Energie** in its title and has keys for it but never uses it (the charge is a
  hard-coded 10). Here Energie scales the charge (`10 * Energie / 110`), which is exactly 10 at the
  default of 110 %.
* A timer (default 20 steps/s) replaces the CPU-bound loop.
* Cell size is `width / columns` exactly (the original lost a few pixels to integer division) and
  the grid may be larger than the original limit of 16 000 cells (now 40 000).
* Touch controls instead of keyboard only.

## Layout

```
FireflySwift/
  Firefly.swiftpm/          the app (Swift Playgrounds App Playground)
    Engine/                 pure-Swift simulation: engine, parameters/keys, RNG, sprites
    *.swift                 SwiftUI: app, canvas renderer, controls panel, model
    Assets.xcassets/        app icon (drawn from the original FIREFLY.BMP)
  Firefly.swiftpm.zip       the same playground, zipped, for easy download on the iPad
  Package.swift             plain SwiftPM package that builds only Engine/ for unit tests
  Tests/FireflyEngineTests/ XCTest suite
  Reference/firefly_ref.c   portable C port of the original simulation core (gcc)
```

### Tests

The engine is verified against the original algorithm: `Reference/firefly_ref.c` is a literal
port of `CalcFlash` and the `DrawFlies` cell state machine; it was also cross-checked against an
independent NumPy implementation, and its output (counts and hashes of the complete state at
selected ticks) is stored in `Tests/FireflyEngineTests`.

```sh
cd C/FFLY/FireflySwift
swift test                                   # Swift engine vs. golden values
cc -O2 -o firefly_ref Reference/firefly_ref.c
./firefly_ref 12 10 4 4 1 60 42              # cols rows cellaround thresholddiv glow ticks seed
```

To rebuild the zip after changing the playground:

```sh
cd C/FFLY/FireflySwift && rm -f Firefly.swiftpm.zip && zip -qr Firefly.swiftpm.zip Firefly.swiftpm -x '*.DS_Store'
```
