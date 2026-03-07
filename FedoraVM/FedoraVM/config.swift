//
//  config.swift
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//
import Foundation
import Virtualization

let cpuCount = 2
let gbRam = UInt64(2)

func buildConfig() -> VZVirtualMachineConfiguration {
    let config = VZVirtualMachineConfiguration()
    config.cpuCount = cpuCount
    config.memorySize = (gbRam * 1024 * 1024 * 1024)
    return config
}

func buildView(virtualMachine: VZVirtualMachine) -> VZVirtualMachineView {
    let vmView = VZVirtualMachineView()
    vmView.virtualMachine = virtualMachine
    return vmView
}
