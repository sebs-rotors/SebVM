//
//  AppDelegate.swift
//  SebVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

import AppKit
import Virtualization

class AppDelegate: NSObject, NSApplicationDelegate {
    var window: NSWindow!
    var virtualMachine: VZVirtualMachine!
    
    func applicationDidFinishLaunching(_ notification: Notification) {
        do {
            let vmConfig = try buildConfig()
            try vmConfig.validate()

            self.virtualMachine = VZVirtualMachine(configuration: vmConfig, queue: .main)
            let vmView = buildView(virtualMachine: virtualMachine)

            self.window = NSWindow(
                contentRect: NSRect(x: 0, y: 0, width: 1280, height: 720),
                styleMask: [.titled, .closable, .resizable],
                backing: .buffered,
                defer: false
            )
            
            window.title = "SebVM"
            window.contentView = vmView
            window.center()
            window.makeKeyAndOrderFront(nil)

            virtualMachine.start { result in
                switch result {
                case .success:
                    print("We did it boys.")
                case .failure(let error):
                    print("Lord have mercy, we messed up: \(error)")
                }
            }
        } catch {
            print("Failed: \(error)")
            NSApplication.shared.terminate(nil)
        }
    }
    
    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true
    }
}
