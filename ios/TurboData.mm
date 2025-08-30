#import "TurboData.h"

@implementation TurboData
RCT_EXPORT_MODULE()

// The actual implementation is in the C++ files
// This is just the bridge to register the module

- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params
{
    return std::make_shared<facebook::react::NativeTurboDataSpecJSI>(params);
}

@end
