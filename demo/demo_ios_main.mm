// demo_ios_main.mm
// UIKit bootstrap for the Glint component demos on iOS.

#import <UIKit/UIKit.h>

#include <algorithm>
#include <cmath>

#include "demos/window.hpp"
#include "glint/glint_view.hpp"

@interface GlintDemoViewController : UIViewController
@end

@implementation GlintDemoViewController {
  std::unique_ptr<glint::glint_view> _glintView;
  std::unique_ptr<glint_demos_window> _demo;
}

- (void)syncDemoMetricsForBounds:(CGRect)bounds
{
  if (!_demo)
    return;

  const UIEdgeInsets safeInsets = self.view.safeAreaInsets;
  _demo->updateHostMetrics(std::max(1, (int) std::lround(CGRectGetWidth(bounds))),
                           std::max(1, (int) std::lround(CGRectGetHeight(bounds))),
                           safeInsets.top,
                           safeInsets.right,
                           safeInsets.bottom,
                           safeInsets.left);
}

- (void)viewDidLoad
{
  [super viewDidLoad];

  self.view.backgroundColor = [UIColor colorWithRed:24.0 / 255.0
                                              green:24.0 / 255.0
                                               blue:24.0 / 255.0
                                              alpha:1.0];

  glint_font_registry::systemFontMgr();

  _demo = std::make_unique<glint_demos_window>();

  const CGRect bounds = self.view.bounds;
  GlintDemoViewController* controller = self;

  glint::glint_view_options options;
  options.parent = (__bridge void*) self.view;
  options.width = std::max(1, (int) std::lround(CGRectGetWidth(bounds)));
  options.height = std::max(1, (int) std::lround(CGRectGetHeight(bounds)));
  options.gpu = glint::glint_backend::Metal;
  options.clearColor = SkColorSetARGB(255, 24, 24, 24);
  options.onDocumentCreated = [controller](glint_document& document) {
    controller->_demo->attachToDocument(
      document,
      [controller]() {
        if (controller->_glintView)
          controller->_glintView->requestRedraw();
      },
      [](std::function<void()> fn) {
        dispatch_async(dispatch_get_main_queue(), ^{
          fn();
        });
      });
    [controller syncDemoMetricsForBounds:controller.view.bounds];
    controller->_demo->completeDocumentSetup(false);
  };

  _glintView = glint::createView(options);
  if (_glintView)
  {
    UIView* view = (__bridge UIView*) _glintView->nativeHandle();
    view.translatesAutoresizingMaskIntoConstraints = NO;
    [NSLayoutConstraint activateConstraints:@[
      [view.topAnchor constraintEqualToAnchor:self.view.topAnchor],
      [view.leadingAnchor constraintEqualToAnchor:self.view.leadingAnchor],
      [view.trailingAnchor constraintEqualToAnchor:self.view.trailingAnchor],
      [view.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor],
    ]];
  }
}

- (void)viewDidLayoutSubviews
{
  [super viewDidLayoutSubviews];

  if (!_glintView)
    return;

  const CGRect bounds = self.view.bounds;
  _glintView->resize(std::max(1, (int) std::lround(CGRectGetWidth(bounds))),
                     std::max(1, (int) std::lround(CGRectGetHeight(bounds))));
  [self syncDemoMetricsForBounds:bounds];
}

- (void)dealloc
{
  if (_demo)
    _demo->detachDocument();
  _glintView.reset();
  _demo.reset();
  [super dealloc];
}

@end

@interface GlintDemoAppDelegate : UIResponder <UIApplicationDelegate>
@property(nonatomic, retain) UIWindow* window;
@end

@implementation GlintDemoAppDelegate

@synthesize window = _window;

- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
  (void)application;
  (void)launchOptions;

  self.window = [[[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds] autorelease];
  GlintDemoViewController* controller = [[[GlintDemoViewController alloc] init] autorelease];
  self.window.rootViewController = controller;
  [self.window makeKeyAndVisible];
  return YES;
}

- (void)dealloc
{
  [_window release];
  [super dealloc];
}

@end

int main(int argc, char* argv[])
{
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, NSStringFromClass([GlintDemoAppDelegate class]));
  }
}