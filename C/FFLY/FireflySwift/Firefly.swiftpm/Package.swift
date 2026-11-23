// swift-tools-version: 5.9

// This is a Swift Playgrounds "App Playground" (open the Firefly.swiftpm folder in
// Swift Playgrounds on iPad or Mac, or in Xcode). It is self-contained: the simulation
// engine lives in the Engine folder next to the SwiftUI files.

import PackageDescription
import AppleProductTypes

let package = Package(
    name: "Firefly",
    platforms: [
        .iOS("17.0")
    ],
    products: [
        .iOSApplication(
            name: "Firefly",
            targets: ["AppModule"],
            bundleIdentifier: "com.example.Firefly",
            teamIdentifier: "",
            displayVersion: "1.0",
            bundleVersion: "1",
            appIcon: .asset("AppIcon"),
            accentColor: .presetColor(.green),
            supportedDeviceFamilies: [
                .pad,
                .phone
            ],
            supportedInterfaceOrientations: [
                .portrait,
                .landscapeRight,
                .landscapeLeft,
                .portraitUpsideDown(.when(deviceFamilies: [.pad]))
            ]
        )
    ],
    targets: [
        .executableTarget(
            name: "AppModule",
            path: "."
        )
    ]
)
