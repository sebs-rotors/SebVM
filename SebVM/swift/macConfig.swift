//
//  macConfig.swift
//  SebVM
//
//  Created by Sebastian Sidor on 3/15/26.
//

import Foundation
import Virtualization

enum VMError : Error {
    case missingHardwareModel
    case missingMachineIdentifier
}

private func macSebvmDir() throws -> URL {
    let appSupportURL = FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!
    let dir = appSupportURL.appendingPathComponent("SebVM")
    try FileManager.default.createDirectory(at: dir, withIntermediateDirectories: true)
    return dir
}
