/*
 ==============================================================================
 
 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers. 
 
 See LICENSE.txt for  more info.
 
 ==============================================================================
 
 Freeze95 patch: added crash guards around OpenWindow call.
 
 macOS 15+ AU validation (and some hosts) may call uiViewForAudioUnit in a
 process context where the ObjC runtime is not fully initialized for Cocoa
 view creation (e.g., validation subprocess, headless CI runner). This causes
 a hard SIGSEGV that @try/@catch cannot recover from.
 
 These guards check the environment before creating the view:
 1. Verify we are on the main thread
 2. Verify NSApp exists
 3. Wrap OpenWindow in @try/@catch as a second line of defense
 */
 
#import <Cocoa/Cocoa.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioUnit/AUCocoaUIView.h>

#include "config.h"   // This is your plugin's config.h.
#include "IPlugAPIBase.h"

using namespace iplug;

static const AudioUnitPropertyID kIPlugObjectPropertyID = UINT32_MAX-100;

@interface AUV2_VIEW_CLASS : NSObject <AUCocoaUIBase>
{
  IPlugAPIBase* mPlug;
}
- (id) init;
- (NSView*) uiViewForAudioUnit: (AudioUnit) audioUnit withSize: (NSSize) preferredSize;
- (unsigned) interfaceVersion;
- (NSString*) description;
@end

@implementation AUV2_VIEW_CLASS

- (id) init
{
  TRACE  
  mPlug = nullptr;
  return [super init];
}

- (NSView*) uiViewForAudioUnit: (AudioUnit) audioUnit withSize: (NSSize) preferredSize
{
  TRACE

  void* pointers[1];
  UInt32 propertySize = sizeof (pointers);
  
  if (AudioUnitGetProperty (audioUnit, kIPlugObjectPropertyID,
                            kAudioUnitScope_Global, 0, pointers, &propertySize) == noErr)
  {
    mPlug = (IPlugAPIBase*) pointers[0];
    
    if (mPlug && mPlug->HasUI())
    {
      // macOS 15+ AU validation runs in a subprocess where the ObjC runtime
      // may not be initialized enough for IGraphics/Cocoa interaction. Check
      // the environment before creating the view to avoid SIGSEGV.
      @try {
        if (![NSThread isMainThread]) {
          DBGMSG("uiViewForAudioUnit: not on main thread, cannot create UI\n");
          return 0;
        }
        if (![NSApplication sharedApplication]) {
          DBGMSG("uiViewForAudioUnit: no NSApp, cannot create UI\n");
          return 0;
        }
      } @catch(NSException* e) {
        DBGMSG("uiViewForAudioUnit: ObjC env check failed: %s\n", [[e reason] UTF8String]);
        return 0;
      }
      
      @try {
#if __has_feature(objc_arc)
        NSView* pView = (__bridge NSView*) mPlug->OpenWindow(nullptr);
#else
        NSView* pView = (NSView*) mPlug->OpenWindow(nullptr);
#endif
        return pView;
      } @catch(NSException* e) {
        DBGMSG("uiViewForAudioUnit: OpenWindow failed: %s\n", [[e reason] UTF8String]);
        // Clean up any partial IGraphics state from the failed OpenWindow
        // by calling CloseWindow on the plugin if it created a view.
        // mPlug->CloseWindow() is not directly accessible here, but
        // IGraphicsMac::OpenWindow's own @try/@catch already cleans up.
        return 0;
      }
    }
  }
  return 0;
}

- (unsigned) interfaceVersion
{
  return 0;
}

- (NSString*) description
{
  @try {
    return [NSString stringWithUTF8String:PLUG_NAME " View"];
  } @catch(NSException* e) {
    return @"Freeze95 View (unavailable)";
  }
}

@end
