# Firefly - web version

`firefly.html` is a single, self-contained page (no libraries, no network) that revives the 1993
FIREFLY program in any modern browser, including Safari on an iPad: open the file in a browser, or
host it anywhere that serves static files.

It uses the same simulation as the Swift version in `../FireflySwift` (a JavaScript port of
`CalcFlash` and the `DrawFlies` cell state machine from `../FIREFLY.C`), with touch controls and the
original keys `c/C e/E t/T r g/G`. Tap the picture to hide the control panel.

The JavaScript engine was checked against `../FireflySwift/Reference/firefly_ref.c`, the portable C
port of the original: the complete state (level and energy of every cell) matched at every tick for
seven different settings, including large neighbourhoods and extreme threshold divisors.
