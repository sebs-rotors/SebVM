//
//  VMBridge.mm
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

#import "vmbridge_api.h"
#import "SebVM-Swift.h"

static VMController* controller = nil;

void startVM(const char* diskPath, int cpuCount, int memoryGB) {
    NSString* path = [NSString stringWithUTF8String:diskPath];
    dispatch_async(dispatch_get_main_queue(), ^{
        controller = [[VMController alloc] init];
        [controller startVMWithDiskPath:path cpuCount:cpuCount memoryGB:memoryGB];
    });
}
