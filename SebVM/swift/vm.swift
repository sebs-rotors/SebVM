//
//  vm.swift
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

import Virtualization
import AppKit

private var virtualMachine: VZVirtualMachine?
private var window: NSWindow?

@_cdecl("startVM")
public func startVM(_ diskPathPtr: UnsafePointer<CChar>, _ cpuCount: Int32, _ memoryGB: Int32) {
    let diskPath = String(cString: diskPathPtr)
    DispatchQueue.main.async {
        do {
            let config = try buildConfig(diskPath: diskPath, cpuCount: Int(cpuCount), memoryGB: Int(memoryGB))
            try config.validate()
            
            virtualMachine = VZVirtualMachine(configuration: config, queue: .main)
            let vmView = buildView(virtualMachine:  virtualMachine!)
            
            window = NSWindow(
                contentRect: NSRect(x: 0, y: 0, width: 1280, height: 720),
                styleMask: [.titled, .closable, .resizable],
                backing: .buffered,
                defer: false
            )
            
            window!.title = "SebVM"
            window!.contentView = vmView
            window!.center()
            window!.makeKeyAndOrderFront(nil)

            virtualMachine!.start { result in
                switch result {
                case .success:
                    print("We did it boys.")
                case .failure(let error):
                    print("Lord have mercy, we messed up: \(error)")
                }
            }
        } catch {
            print("We REALLY messed up: \(error)")
        }
    }
}
