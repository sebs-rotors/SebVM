//
//  vmbridge_api.h
//  SebVM
//
//  Created by Sebastian Sidor on 3/9/26.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void startVM(const char* diskPath, int cpuCount, int memoryGB);

#ifdef __cplusplus
}
#endif
