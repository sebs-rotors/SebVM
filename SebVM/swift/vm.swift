import Virtualization
import AppKit

private var virtualMachine: VZVirtualMachine?
private var window: NSWindow?

private class VMDelegate: NSObject, VZVirtualMachineDelegate {
    var onStop: (() -> Void)?
    
    func guestDidStop(_ virtualMachine: VZVirtualMachine) {
        onStop?()
    }
    
    func virtualMachine(_ virtualMachine: VZVirtualMachine, didStopWithError error: Error) {
        print("VM stopped with error: \(error)")
        onStop?()
    }
}

private var vmDelegate: VMDelegate?
private var vmStoppedCallBack: (@convention(c) () -> Void)?

@_cdecl("startVM")
public func startVM(_ diskPathPtr: UnsafePointer<CChar>, _ cpuCount: Int32, _ memoryGB: Int32) {
    let diskPath = String(cString: diskPathPtr)
    DispatchQueue.main.async {
        do {
            let config = try buildConfig(diskPath: diskPath, cpuCount: Int(cpuCount), memoryGB: Int(memoryGB))
            try config.validate()
            
            virtualMachine = VZVirtualMachine(configuration: config, queue: .main)
            vmDelegate = VMDelegate()
            virtualMachine!.delegate = vmDelegate
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

@_cdecl("setVMStoppedCallback")
public func setVMStoppedCallback(_ callback: @escaping @convention(c) () -> Void) {
    vmStoppedCallBack = callback
}

@_cdecl("stopVM")
public func stopVM() {
    guard let vm = virtualMachine else {
        vmStoppedCallBack?()
        return
    }
    DispatchQueue.main.async {
        vmDelegate?.onStop = { vmStoppedCallBack?() }
        
        // 15 second fallback, force stop if guest ignores ACPI
        DispatchQueue.main.asyncAfter(deadline: .now() + 15) {
            if vm.state != .stopped {
                print("Guest did not shutdown in time, forcing stop")
                vm.stop { _ in
                    DispatchQueue.main.async {vmStoppedCallBack?() }
                }
            }
        }
        
        do {
            try vm.requestStop()
        } catch {
            print("ACPI shutdown failed: \(error), forcing stop")
            vm.stop { _ in
                DispatchQueue.main.async { vmStoppedCallBack?() }
            }
        }
    }
}
