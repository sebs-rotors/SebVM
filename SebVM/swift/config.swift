import Foundation
import Virtualization

func buildConfig(diskPath: String, cpuCount: Int, memoryGB: Int) throws -> VZVirtualMachineConfiguration {
    // Base config
    let config = VZVirtualMachineConfiguration()
    config.cpuCount = cpuCount
    config.memorySize = UInt64(memoryGB) * 1024 * 1024 * 1024
    config.platform = VZGenericPlatformConfiguration()

    // Storage
    let diskURL = URL(fileURLWithPath: diskPath)
    let diskAttachment = try VZDiskImageStorageDeviceAttachment(url: diskURL, readOnly: false)
    let disk = VZVirtioBlockDeviceConfiguration(attachment: diskAttachment)
    config.storageDevices = [disk]

    // EFI Bootloader
    let bootloader = VZEFIBootLoader()
    let appSupportURL = FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!
    let SebVMDirURL = appSupportURL.appendingPathComponent("SebVM")
    try FileManager.default.createDirectory(at: SebVMDirURL, withIntermediateDirectories: true)
    let efiStoreURL = SebVMDirURL.appendingPathComponent("efi-variable-store")
    
    if FileManager.default.fileExists(atPath: efiStoreURL.path) {
        bootloader.variableStore = VZEFIVariableStore(url: efiStoreURL)
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
