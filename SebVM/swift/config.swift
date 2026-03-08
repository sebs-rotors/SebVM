import Foundation
import Virtualization

let cpuCount = 2
let gbRam = UInt64(2)

func buildConfig() throws -> VZVirtualMachineConfiguration {
    // Base config
    let config = VZVirtualMachineConfiguration()
    config.cpuCount = cpuCount
    config.memorySize = (gbRam * 1024 * 1024 * 1024)
    config.platform = VZGenericPlatformConfiguration()

    // Storage
    let diskURL = URL(
        fileURLWithPath:
            "/Users/sebisidor/Code/Personal/Tasks/virtual-fedora/resources/Fedora-KDE-Desktop-Disk-43-1.6.aarch64.raw"
    )
    let diskAttachment = try VZDiskImageStorageDeviceAttachment(url: diskURL, readOnly: false)
    let disk = VZVirtioBlockDeviceConfiguration(attachment: diskAttachment)
    config.storageDevices = [disk]

    // EFI Bootloader
    let bootloader = VZEFIBootLoader()
    let efiStoreURL = URL(
        fileURLWithPath:
            "/Users/sebisidor/Code/Personal/Tasks/virtual-fedora/resources/efi-variable-store")
    if FileManager.default.fileExists(atPath: efiStoreURL.path) {
        bootloader.variableStore = try VZEFIVariableStore(url: efiStoreURL)
    } else {
        bootloader.variableStore = try VZEFIVariableStore(creatingVariableStoreAt: efiStoreURL)
    }
    config.bootLoader = bootloader

    // source of the heat death of the universe: entropy
    config.entropyDevices = [VZVirtioEntropyDeviceConfiguration()]

    // Network
    let networkDevice = VZVirtioNetworkDeviceConfiguration()
    networkDevice.attachment = VZNATNetworkDeviceAttachment()
    config.networkDevices = [networkDevice]
    
    // Graphics
    let graphics = VZVirtioGraphicsDeviceConfiguration()
    graphics.scanouts = [VZVirtioGraphicsScanoutConfiguration(widthInPixels: 1920, heightInPixels: 1080)]
    config.graphicsDevices = [graphics]
    
    // Input
    config.keyboards = [VZUSBKeyboardConfiguration()]
    config.pointingDevices = [VZUSBScreenCoordinatePointingDeviceConfiguration()]

    return config
}

func buildView(virtualMachine: VZVirtualMachine) -> VZVirtualMachineView {
    let vmView = VZVirtualMachineView()
    vmView.virtualMachine = virtualMachine
    return vmView
}
