# SebVM

A macOS virtual machine application for running aarch64 Linux on Apple Silicon, built on Apple's Virtualization framework with a Qt-based GUI and a Swift/C++ interoperability architecture.

---

## Overview

SebVM boots any aarch64 Linux distribution from a raw disk image using Apple's native Virtualization framework. The application is designed around a strict separation of concerns: the GUI and application logic are written in C++ using Qt, while the VM configuration and lifecycle are handled in Swift via Apple's Virtualization APIs. The two layers are connected via a direct C symbol bridge using Swift's `@_cdecl` export mechanism.

---

## Architecture

```
Qt C++ (GUI + Logic) ↔ C Bridge (vmbridge_api.h) ↔ Swift (Virtualization Framework)
```

### Layer Breakdown

**Swift — Virtualization Layer** (`SebVM/swift/`)
- Builds the `VZVirtualMachineConfiguration` with all required devices
- Manages `VZVirtualMachine` lifecycle (start, graceful stop, forced stop)
- Exposes `VZVirtualMachineView` for display rendering
- Handles EFI variable store creation and persistence
- Exports C-callable symbols via `@_cdecl` (`startVM`, `stopVM`, `setVMStoppedCallback`)
- Uses `VZVirtualMachineDelegate` to notify the C++ layer when the VM stops

**C Bridge** (`SebVM/bridge/vmbridge_api.h`)
- Header-only C interface declaring the symbols exported from Swift
- `extern "C"` guarded for C++ inclusion
- No implementation — Swift exports directly as C symbols

**C++/Qt — Application Layer** (`SebVM/cpp/`)
- `main.cpp` — `QApplication` entry point; config-aware launch flow with disk image validation
- `setupWindow` — First-run Qt window for image path, CPU, and memory configuration
- `settingsWindow` — Post-launch settings panel for updating VM configuration
- `vmConfigForm` — Shared form widget (disk path, CPU, memory) embedded in both windows
- `vmconfig` — Config struct, JSON serialization/deserialization via nlohmann/json

---

## VM Device Configuration

| Device | Implementation |
|---|---|
| Platform | `VZGenericPlatformConfiguration` |
| Bootloader | `VZEFIBootLoader` with persistent variable store |
| Storage | `VZVirtioBlockDeviceConfiguration` → raw disk image |
| Network | `VZVirtioNetworkDeviceConfiguration` → NAT |
| Graphics | `VZVirtioGraphicsDeviceConfiguration` (1920×1080 scanout) |
| Keyboard | `VZUSBKeyboardConfiguration` |
| Pointing | `VZUSBScreenCoordinatePointingDeviceConfiguration` |
| Entropy | `VZVirtioEntropyDeviceConfiguration` |

---

## Application Flow

```
Launch
  │
  ├─ config.json exists?
  │     │
  │     ├─ Yes → Disk image still present?
  │     │             │
  │     │             ├─ Yes → Load config → Start VM → Show tray icon
  │     │             │
  │     │             └─ No  → Warn user → Show SetupWindow
  │     │
  │     └─ No  → Show SetupWindow
  │                   │
  │                   └─ User inputs image path, CPU, memory
  │                         │
  │                         └─ Validate → Save config → Start VM → Show tray icon
  │
  └─ VM running
        │
        ├─ Tray → Settings → SettingsWindow (edits saved to config.json, apply on next boot)
        │
        └─ Tray → Quit → ACPI shutdown signal → wait for guest → exit
                              │
                              └─ 15s timeout → force stop → exit
```

Config is persisted to `~/Library/Application Support/SebVM/config.json`:
```json
{
    "diskPath": "/path/to/linux.raw",
    "cpuCount": 2,
    "memoryGB": 4
}
```

---

## Compatibility

**Host requirements:**
- Apple Silicon Mac (aarch64)
- macOS 13 Ventura or later

**Guest requirements:**
- aarch64 Linux distribution
- Raw disk image format (`.raw`)
- EFI bootable
- Virtio drivers (included by default in all modern Linux kernels)

**Tested with:** Fedora 43 KDE Desktop (aarch64), Fedora 43 XFCE (aarch64)

Other compatible distributions include Ubuntu, Debian, Arch Linux ARM, Alpine, and openSUSE — any aarch64 `.raw` image that boots via EFI will work. Images in other formats (`.qcow2`, `.vmdk`) can be converted using `qemu-img`. ISO images can be attached as secondary storage devices.

---

## Dependencies

| Dependency | Purpose | Source |
|---|---|---|
| Apple Virtualization framework | VM configuration and lifecycle | macOS system framework |
| Qt 6 (QtWidgets, QtCore, QtGui) | Cross-platform GUI | Homebrew (`brew install qt`) |
| nlohmann/json | JSON config serialization | Header-only (`json.hpp`) |

---

## Project Structure

```
SebVM/
└── SebVM/
    ├── swift/
    │   ├── config.swift              # VZVirtualMachineConfiguration builder
    │   └── vm.swift                  # VM lifecycle: start, stop, delegate, C exports
    ├── cpp/
    │   ├── main.cpp                  # QApplication entry point + launch flow
    │   ├── setupWindow.hpp/cpp       # First-run setup UI
    │   ├── settingsWindow.hpp/cpp    # Post-launch settings UI
    │   ├── vmConfigForm.hpp/cpp      # Shared form widget (used by both windows)
    │   ├── vmconfig.hpp/cpp          # Config struct + JSON persistence
    │   └── json.hpp                  # nlohmann/json (header-only)
    ├── bridge/
    │   └── vmbridge_api.h            # C interface to Swift-exported symbols
    ├── arc/
    │   └── VMBridge.mm               # Archived ObjC++ bridge (superseded by @_cdecl)
    └── SebVM.entitlements
```

---

## Build Setup

### Qt (Homebrew)

```bash
brew install qt
```

**Xcode Build Settings:**

| Setting | Value |
|---|---|
| Framework Search Paths | `/opt/homebrew/opt/qt/lib` |
| Runpath Search Paths | `/opt/homebrew/opt/qt/lib` |
| Other Linker Flags | `-F/opt/homebrew/opt/qt/lib -framework QtWidgets -framework QtCore -framework QtGui` |
| C++ Language Dialect | C++17 |

> On Intel Macs, replace `/opt/homebrew` with `/usr/local`.

### Entitlements

| Entitlement | Reason |
|---|---|
| `com.apple.security.virtualization` | Required to use the Virtualization framework |
| `com.apple.security.cs.disable-library-validation` | Required to load Qt frameworks under Hardened Runtime |

---

## References

- [Apple Virtualization Framework](https://developer.apple.com/documentation/virtualization)
- [Qt Documentation](https://doc.qt.io)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Fedora Linux](https://www.fedoraproject.org)
