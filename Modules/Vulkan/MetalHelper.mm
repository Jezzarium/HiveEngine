#include "Core/Platform/Platform.h"

#ifdef HIVE_PLATFORM_MACOS
#import <QuartzCore/CAMetalLayer.h>
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>

extern "C" {
    void* create_metal_layer_for_nswindow(void* window) {
        NSWindow* nsWindow = static_cast<NSWindow*>(window);
        NSView* contentView = [nsWindow contentView];

        CAMetalLayer* metalLayer = nil;

        if ([contentView.layer isKindOfClass:[CAMetalLayer class]]) {
            metalLayer = (CAMetalLayer*)contentView.layer;
        } else {
            metalLayer = [CAMetalLayer layer];

            [contentView setWantsLayer:YES];
            [contentView setLayer:metalLayer];

            metalLayer.frame = contentView.bounds;
            metalLayer.device = MTLCreateSystemDefaultDevice();

            metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
            metalLayer.framebufferOnly = YES;
        }

        return metalLayer;
    }
}
#endif
