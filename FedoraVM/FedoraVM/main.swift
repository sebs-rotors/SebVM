import Foundation
import Virtualization
import AppKit

let vmConfig = try! buildConfig()
try vmConfig.validate()

let app = NSApplication.shared
app.setActivationPolicy(.regular)

let virtualMachine = VZVirtualMachine(configuration: vmConfig, queue: .main)
let vmView = buildView(virtualMachine: virtualMachine)

let window = NSWindow(
    contentRect: NSRect(x: 0, y: 0, width: 1280, height: 720),
    styleMask: [.titled, .closable, .resizable],
    backing: .buffered,
    defer: false
)
window.title = "Fedora VM"
window.contentView = vmView
window.center()
window.makeKeyAndOrderFront(nil)
app.activate(ignoringOtherApps: true)

virtualMachine.start { result in
    switch result {
    case .success:
        print("We did it boys.")
    case .failure(let error):
        print("Lord have mercy, we messed up: \(error)")
    }
}

app.run()
