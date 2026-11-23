// swift-tools-version: 5.9
//
// Plain SwiftPM package that compiles ONLY the simulation engine of the app playground
// (Firefly.swiftpm/Engine) so it can be unit-tested with `swift test` on a Mac or Linux.
// The app itself is the self-contained playground in Firefly.swiftpm (open that in
// Swift Playgrounds on the iPad); this file is not needed to run the app.

import PackageDescription

let package = Package(
    name: "FireflyEngine",
    products: [
        .library(name: "FireflyEngine", targets: ["FireflyEngine"]),
    ],
    targets: [
        .target(
            name: "FireflyEngine",
            path: "Firefly.swiftpm/Engine"
        ),
        .testTarget(
            name: "FireflyEngineTests",
            dependencies: ["FireflyEngine"],
            path: "Tests/FireflyEngineTests"
        ),
    ]
)
