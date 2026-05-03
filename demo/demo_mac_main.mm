// demo_mac_main.mm
// macOS NSApp setup for the glint demo standalone executable.
// Compiled as Objective-C++ so it can use Cocoa APIs.

#import <Cocoa/Cocoa.h>
#include "demos/window.hpp"

@interface GlintDemoAppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation GlintDemoAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)n {
  (void)n;
  // Initialise the CoreText font manager synchronously on the main thread.
  // This call scans the entire system font catalog (~1 s) and caches the
  // result behind a function-local static.  Doing it here — once, on the
  // main thread, before glint_demos_window::open() — guarantees that no
  // render thread can race against the static initialisation or the first
  // sk_sp copy-construction (which is what caused the SkASSERT crash when
  // the call was dispatched to a background GCD thread).
  glint_font_registry::systemFontMgr();
  glint_demos_window::open();
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
  (void)sender;
  return NO;
}
@end

// Called from glint_demos_window::afterRun() (C++) when the panel is fully
// closed.  Schedules [NSApp terminate:nil] on the next run-loop pass so that
// all Cocoa cleanup from the close delegate has finished before we exit.
extern "C" void glint_demo_terminate_app()
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [NSApp terminate:nil];
  });
}

void glint_demo_run_mac()
{
  [NSApplication sharedApplication];
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

  GlintDemoAppDelegate* delegate = [[GlintDemoAppDelegate alloc] init];
  [NSApp setDelegate:delegate];
  [NSApp activateIgnoringOtherApps:YES];
  [NSApp run];
}
