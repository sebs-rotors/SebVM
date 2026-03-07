//
//  main.swift
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/6/26.
//

import Foundation
import Virtualization

let vmConfig = try! buildConfig()
try vmConfig.validate()
let virtualMachine = VZVirtualMachine(configuration: vmConfig)
let vmView = buildView(virtualMachine: virtualMachine)

