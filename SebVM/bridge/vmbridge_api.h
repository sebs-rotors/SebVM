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

typedef void (*VMStoppedCallback)(void);

void startVM(const char* diskPath, int cpuCount, int memoryGB);
void stopVM(void);
void setVMStoppedCallback(VMStoppedCallback callback);

#ifdef __cplusplus
}
#endif
