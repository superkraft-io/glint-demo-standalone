#pragma once

/**
 * demos/window.hpp
 * Shared Glint component demo presenter.
 *
 * On desktop this is hosted by glint_window.
 * On iOS the same presenter is attached to a glint_view-backed document.
 */

#include "glint/glint_standalone.hpp"
#include "glint/platform/glint_apple_platform.hpp"
#if !GLINT_PLATFORM_IOS
#include "glint/glint_window.hpp"
#endif
#include "glint/components/glint_button.hpp"
#include "glint/components/glint_checkbox.hpp"
#include "glint/components/glint_colorpicker.hpp"
#include "glint/components/glint_gradient_editor.hpp"
#include "glint/components/glint_list/glint_list.hpp"
#include "glint/components/glint_select.hpp"
#include "glint/shaders/glint_shaders.hpp"
#include "glint_user_code/cpp/ui/glint_switch.hpp"

#include <cmath>
#include <functional>
#include <string>

#if defined(GLINT_BUNDLE_SHALLOW) || defined(GLINT_BUNDLE_DEEP)
#include "glint_user_code/cpp/bundle/glint_bundle_library.hpp"
#endif

#if GLINT_PLATFORM_MAC
extern "C" void glint_demo_terminate_app();
#endif

namespace glint_demo_theme {
inline constexpr const char* canvasBg = "#181818";
inline constexpr const char* headerBg = "#202020";
inline constexpr const char* sidebarBg = "#1c1c1c";
inline constexpr const char* panelBg = "#252526";
inline constexpr const char* surface = "#2d2d2d";
inline constexpr const char* surfaceAlt = "#232323";
inline constexpr const char* surfaceHover = "#383838";
inline constexpr const char* border = "#3c3c3c";
inline constexpr const char* text = "#e6e6e6";
inline constexpr const char* heading = "#ffffff";
inline constexpr const char* muted = "#b8b8b8";
inline constexpr const char* subtle = "#8a8a8a";
inline constexpr const char* primary = "#4c4c4c";
inline constexpr const char* primaryHover = "#5a5a5a";
inline constexpr const char* success = "#7ec07e";
inline constexpr const char* successBg = "#244224";
inline constexpr const char* danger = "#d07b7b";
inline constexpr const char* dangerBg = "#5a3030";
inline constexpr const char* warning = "#d0b16f";
inline constexpr const char* amberBg = "#6a4620";
inline constexpr const char* slate = "#5f6872";
inline constexpr const char* slateHover = "#6d7782";
inline constexpr const char* accent = "#1a6fc4";
inline constexpr const char* accentHover = "#2179cf";
}

enum class DemoSection
{
  Text = 0,
  WhiteSpace,
  Buttons,
  Switches,
  Checkboxes,
  Inputs,
  Lists,
  Scroll,
  TouchDemo,
  Colors,
  Generic,
  Transforms,
  Filters,
  BackdropFilters,
  Transitions,
  Shaders,
  Fonts,
  Select,
  Masks,
  Stroke,
  Images,
  BlendModes,
  Textarea,
  Radios,
  Progress,
  Tooltips,
  DatePicker,
  Cursors,
  _Count
};

enum class DemoMode
{
  CoreComponents = 0,
  UserComponents,
};

enum class DemoLayout
{
  Desktop = 0,
  IOSCompact,
};

static constexpr const char* kSectionNames[] = {
  "Text",
  "White Space",
  "Buttons",
  "Switches",
  "Checkboxes",
  "Inputs",
  "Lists",
  "Scroll",
  "Touch Demo",
  "Colors",
  "Generic",
  "Transforms",
  "Filters",
  "Backdrop Filters",
  "Transitions",
  "Shaders",
  "Fonts",
  "Select",
  "Masks",
  "Stroke",
  "Images",
  "Blend Modes",
  "Textarea",
  "Radios",
  "Progress",
  "Tooltips",
  "Date Picker",
  "Cursors",
};

class glint_demos_window
#if !GLINT_PLATFORM_IOS
  : public glint_window
#endif
{
public:
#if !GLINT_PLATFORM_IOS
  static void open()
  {
    if (sInstance && sInstance->isRunning()) return;
    if (!sInstance) sInstance = new glint_demos_window();
    sInstance->startThread();
  }

  static void close()
  {
    if (sInstance) sInstance->stopThread();
  }

  static bool isOpen()
  {
    return sInstance && sInstance->isRunning();
  }
#endif

  glint_demos_window() = default;

  void attachToDocument(glint_document& document,
                        std::function<void()> requestRedraw,
                        std::function<void(std::function<void()>)> dispatchMain = {})
  {
    mRoot = &document;
    mRequestRedraw = std::move(requestRedraw);
    mDispatchMain = std::move(dispatchMain);
    buildUI();
  }

  void updateHostMetrics(float width,
                         float height,
                         float safeTop = 0.f,
                         float safeRight = 0.f,
                         float safeBottom = 0.f,
                         float safeLeft = 0.f)
  {
    const bool viewportChanged = mViewportWidth != width || mViewportHeight != height;
    mViewportWidth = width;
    mViewportHeight = height;

#if GLINT_PLATFORM_IOS
    const DemoLayout nextLayout = layoutForWidth(width);
    const bool safeAreaChanged = mSafeTop != safeTop || mSafeRight != safeRight
                              || mSafeBottom != safeBottom || mSafeLeft != safeLeft;
    const bool layoutChanged = mLayout != nextLayout;

    mSafeTop = safeTop;
    mSafeRight = safeRight;
    mSafeBottom = safeBottom;
    mSafeLeft = safeLeft;
    mLayout = nextLayout;

    if (mRoot && (layoutChanged || safeAreaChanged || viewportChanged))
      rebuildShell();
#else
    (void)safeTop;
    (void)safeRight;
    (void)safeBottom;
    (void)safeLeft;
#endif
  }

  void completeDocumentSetup(bool deferStylesheetLoad = false)
  {
    if (!mRoot) return;

#if defined(GLINT_BUNDLE_SHALLOW) || defined(GLINT_BUNDLE_DEEP)
    mRoot->onRequest = [this](glint_resource_request& req) {
      mBundle.dispatch(req);
    };
#endif

    if (deferStylesheetLoad && mDispatchMain)
    {
      mDispatchMain([this]() {
        loadStylesheetAndPopulate();
      });
      return;
    }

    loadStylesheetAndPopulate();
  }

  void detachDocument()
  {
    if (mRoot) mRoot->showInspector(false);
    mRoot = nullptr;
    mContent = nullptr;
    mSidebar = nullptr;
    mBottomSheet = nullptr;
    mCoreModeBtn = nullptr;
    mUserModeBtn = nullptr;
    mRequestRedraw = {};
    mDispatchMain = {};
  }

private:
  static constexpr int kHeaderH = 40;
  static constexpr float kSidebarW = 180.f;
  static constexpr float kIOSRegularSidebarW = 220.f;

  DemoMode mMode = DemoMode::CoreComponents;
  DemoMode mSheetMode = DemoMode::CoreComponents;
  DemoLayout mLayout = GLINT_PLATFORM_IOS ? DemoLayout::IOSCompact : DemoLayout::Desktop;
  DemoSection mSection = DemoSection::Text;
  DemoSection mCoreSection = DemoSection::Text;
  DemoSection mUserSection = DemoSection::Switches;
  glint_document* mRoot = nullptr;
  glint_element* mContent = nullptr;
  glint_list* mSidebar = nullptr;
  glint_element* mBottomSheet = nullptr;
  glint_button* mCoreModeBtn = nullptr;
  glint_button* mUserModeBtn = nullptr;
  std::function<void()> mRequestRedraw;
  std::function<void(std::function<void()>)> mDispatchMain;
  float mViewportWidth = 0.f;
  float mViewportHeight = 0.f;
  float mSafeTop = 0.f;
  float mSafeRight = 0.f;
  float mSafeBottom = 0.f;
  float mSafeLeft = 0.f;
  bool mBottomSheetOpen = false;
  bool mSuppressSectionSelection = false;

#if !GLINT_PLATFORM_IOS
  static glint_demos_window* sInstance;
#endif

#if defined(GLINT_BUNDLE_SHALLOW) || defined(GLINT_BUNDLE_DEEP)
  glint_bundle::glint_bundle_library mBundle;
#endif

  static std::string insetString(float top, float right, float bottom, float left)
  {
    return std::to_string((int) std::lround(top)) + " "
         + std::to_string((int) std::lround(right)) + " "
         + std::to_string((int) std::lround(bottom)) + " "
         + std::to_string((int) std::lround(left));
  }

  DemoLayout layoutForWidth(float width) const
  {
#if GLINT_PLATFORM_IOS
    (void) width;
    return DemoLayout::IOSCompact;
#else
    (void) width;
    return DemoLayout::Desktop;
#endif
  }

  bool isCompactLayout() const
  {
    return mLayout == DemoLayout::IOSCompact;
  }

  bool isIOSLayout() const
  {
    return mLayout == DemoLayout::IOSCompact;
  }

  bool usesSidebarNavigation() const
  {
    return mLayout == DemoLayout::Desktop;
  }

  std::string rootClassName() const
  {
    if (mLayout == DemoLayout::IOSCompact)
      return "demo-shell demo-platform-ios demo-layout-bottom-nav";
    return "demo-shell demo-platform-desktop demo-layout-desktop";
  }

  std::string contentClassName() const
  {
    if (mLayout == DemoLayout::IOSCompact)
      return "demo-content demo-platform-ios demo-layout-bottom-nav";
    return "demo-content demo-platform-desktop demo-layout-desktop";
  }

  std::string navClassName() const
  {
    if (mLayout == DemoLayout::IOSCompact)
      return "demo-section-list demo-platform-ios demo-layout-sheet";
    return "demo-section-list demo-platform-desktop demo-layout-desktop";
  }

  DemoMode modeForSection(DemoSection section) const
  {
    return isUserSection(section) ? DemoMode::UserComponents : DemoMode::CoreComponents;
  }

  DemoMode activeModeButton() const
  {
    return isIOSLayout() && mBottomSheetOpen ? mSheetMode : mMode;
  }

  float bottomNavHeight() const
  {
    return 58.f + mSafeBottom;
  }

  float bottomSheetHeight() const
  {
    const float viewport = mViewportHeight > 0.f ? mViewportHeight : 640.f;
    return std::max(240.f, std::min(420.f, viewport * 0.48f));
  }

  void syncBottomSheet()
  {
    if (!mBottomSheet) return;
    mBottomSheet->style.height = mBottomSheetOpen ? bottomSheetHeight() : 0.f;
    mBottomSheet->style.borderWidth = mBottomSheetOpen ? 1.f : 0.f;
    mBottomSheet->style.padding = mBottomSheetOpen
                                ? insetString(12.f, 12.f + mSafeRight, 12.f, 12.f + mSafeLeft)
                                : "0 0 0 0";
    mBottomSheet->setDirty(false);
  }

  void rebuildShell()
  {
    if (!mRoot) return;
    buildUI();
    if (isIOSLayout())
    {
      if (!mBottomSheetOpen)
        mSheetMode = mMode;
      populateSidebar(false);
      rebuildContent();
      syncBottomSheet();
      syncModeButtons();
    }
    else
    {
      populateSidebar();
    }
    scheduleRedraw();
  }

  void buildDesktopShell()
  {
    mRoot->add.div([this](glint_component_style& header) {
      header.style.backgroundColor = glint_demo_theme::headerBg;
      header.style.height = static_cast<float>(kHeaderH);
      header.style.width = "100%";
      header.style.display = "flex";
      header.style.alignItems = "center";
      header.style.padding = "0 16";
      header.style.gap = 8.f;

      header.add.button([this](glint_button& btn) {
        const bool active = (mMode == DemoMode::CoreComponents);
        btn.innerText = "Core components";
        btn.style.height = 28.f;
        btn.style.padding = "0 12";
        btn.style.borderWidth = 1.f;
        btn.style.borderRadius = 4.f;
        btn.style.fontSize = 12.f;
        btn.style.backgroundColor = active ? glint_demo_theme::accent : glint_demo_theme::surface;
        btn.style.borderColor = active ? glint_demo_theme::accent : glint_demo_theme::border;
        btn.style.color = active ? glint_demo_theme::heading : glint_demo_theme::muted;
        btn.hover.backgroundColor = active ? glint_demo_theme::accentHover : glint_demo_theme::surfaceHover;
        btn.hover.borderColor = active ? glint_demo_theme::accentHover : glint_demo_theme::slate;
        btn.hover.borderWidth = 1.f;
        btn.hover.borderRadius = 4.f;
        btn.hover.color = glint_demo_theme::heading;
        btn.pressed.backgroundColor = glint_demo_theme::accent;
        btn.pressed.borderColor = glint_demo_theme::accent;
        btn.pressed.borderWidth = 1.f;
        btn.pressed.borderRadius = 4.f;
        btn.pressed.color = glint_demo_theme::heading;
        btn.onClick = [this] { setMode(DemoMode::CoreComponents); };
      }, &mCoreModeBtn);

      header.add.button([this](glint_button& btn) {
        const bool active = (mMode == DemoMode::UserComponents);
        btn.innerText = "User components";
        btn.style.height = 28.f;
        btn.style.padding = "0 12";
        btn.style.borderWidth = 1.f;
        btn.style.borderRadius = 4.f;
        btn.style.fontSize = 12.f;
        btn.style.backgroundColor = active ? glint_demo_theme::accent : glint_demo_theme::surface;
        btn.style.borderColor = active ? glint_demo_theme::accent : glint_demo_theme::border;
        btn.style.color = active ? glint_demo_theme::heading : glint_demo_theme::muted;
        btn.hover.backgroundColor = active ? glint_demo_theme::accentHover : glint_demo_theme::surfaceHover;
        btn.hover.borderColor = active ? glint_demo_theme::accentHover : glint_demo_theme::slate;
        btn.hover.borderWidth = 1.f;
        btn.hover.borderRadius = 4.f;
        btn.hover.color = glint_demo_theme::heading;
        btn.pressed.backgroundColor = glint_demo_theme::accent;
        btn.pressed.borderColor = glint_demo_theme::accent;
        btn.pressed.borderWidth = 1.f;
        btn.pressed.borderRadius = 4.f;
        btn.pressed.color = glint_demo_theme::heading;
        btn.onClick = [this] { setMode(DemoMode::UserComponents); };
      }, &mUserModeBtn);
    });

    mRoot->add.div([](glint_component_style& separator) {
      separator.style.backgroundColor = glint_demo_theme::border;
      separator.style.height = 1.f;
      separator.style.width = "100%";
    });

    mRoot->add.div([this](glint_component_style& bodyRow) {
      bodyRow.style.display = "flex";
      bodyRow.style.flexDirection = "row";
      bodyRow.style.flexGrow = 1.f;
      bodyRow.style.width = "100%";
      bodyRow.style.overflow = "hidden";

      bodyRow.add.fromClass<glint_list>([this](glint_list& sidebar) {
        sidebar.className = navClassName();
        sidebar.style.width = kSidebarW;
        sidebar.style.height = "100%";
        sidebar.style.backgroundColor = glint_demo_theme::sidebarBg;
        sidebar.highlightOnSelect = true;
        sidebar.onItemSelected = [this](glint_list_item* item) {
          if (!item || mSuppressSectionSelection) return;
          for (int sectionIndex = 0; sectionIndex < static_cast<int>(DemoSection::_Count); ++sectionIndex)
          {
            if (item->id == kSectionNames[sectionIndex])
            {
              mSection = static_cast<DemoSection>(sectionIndex);
              mMode = modeForSection(mSection);
              rememberSectionForMode(mSection);
              rebuildContent();
              syncModeButtons();
              return;
            }
          }
        };
      }, &mSidebar);

      bodyRow.add.div([](glint_component_style& divider) {
        divider.style.width = 1.f;
        divider.style.height = "100%";
        divider.style.backgroundColor = glint_demo_theme::border;
      });

      bodyRow.add.div([](glint_component_style& content) {
        content.className = "demo-content demo-platform-desktop demo-layout-desktop";
        content.style.flexGrow = 1.f;
        content.style.height = "100%";
        content.style.backgroundColor = glint_demo_theme::panelBg;
        content.style.overflowY = "auto";
        content.style.padding = "16";
      }, &mContent);
    });
  }

  void buildIOSShell()
  {
    mRoot->add.div([this](glint_component_style& shell) {
      shell.className = "demo-ios-shell";
      shell.style.width = "100%";
      shell.style.height = "100%";
      shell.style.flexGrow = 1.f;
      shell.style.display = "flex";
      shell.style.flexDirection = "column";
      shell.style.overflow = "hidden";

      shell.add.div([this](glint_component_style& content) {
        content.className = contentClassName();
        content.style.flexGrow = 1.f;
        content.style.width = "100%";
        content.style.backgroundColor = glint_demo_theme::panelBg;
        content.style.overflowY = "auto";
        content.style.padding = insetString(16.f + mSafeTop, 16.f + mSafeRight, 16.f, 16.f + mSafeLeft);
      }, &mContent);

      shell.add.div([this](glint_component_style& drawer) {
        drawer.className = "demo-ios-sheet";
        drawer.style.width = "100%";
        drawer.style.height = mBottomSheetOpen ? bottomSheetHeight() : 0.f;
        drawer.style.display = "flex";
        drawer.style.flexDirection = "column";
        drawer.style.overflow = "hidden";
        drawer.style.backgroundColor = glint_demo_theme::sidebarBg;
        drawer.style.borderColor = glint_demo_theme::border;
        drawer.style.borderWidth = mBottomSheetOpen ? 1.f : 0.f;
        drawer.style.borderTopLeftRadius = 18.f;
        drawer.style.borderTopRightRadius = 18.f;
        drawer.style.transition = "height 220ms ease-out, border-width 220ms ease-out";
        drawer.style.padding = mBottomSheetOpen
                             ? insetString(12.f, 12.f + mSafeRight, 12.f, 12.f + mSafeLeft)
                             : "0 0 0 0";

        drawer.add.div([this](glint_component_style& title) {
          title.innerText = mSheetMode == DemoMode::CoreComponents ? "Core pages" : "User pages";
          title.className = "demo-ios-sheet-title";
          title.style.color = glint_demo_theme::heading;
          title.style.fontSize = 14.f;
          title.style.width = "100%";
          title.style.marginBottom = 8.f;
          title.style.textAlign = EAlign::Near;
        });

        drawer.add.fromClass<glint_list>([this](glint_list& sidebar) {
          sidebar.className = navClassName();
          sidebar.style.flexGrow = 1.f;
          sidebar.style.width = "100%";
          sidebar.style.height = "100%";
          sidebar.style.backgroundColor = glint_demo_theme::sidebarBg;
          sidebar.highlightOnSelect = true;
          sidebar.onItemSelected = [this](glint_list_item* item) {
            if (!item || mSuppressSectionSelection) return;
            for (int sectionIndex = 0; sectionIndex < static_cast<int>(DemoSection::_Count); ++sectionIndex)
            {
              if (item->id == kSectionNames[sectionIndex])
              {
                mSection = static_cast<DemoSection>(sectionIndex);
                mMode = modeForSection(mSection);
                mSheetMode = mMode;
                rememberSectionForMode(mSection);
                rebuildContent();
                mBottomSheetOpen = false;
                syncBottomSheet();
                syncModeButtons();
                scheduleRedraw();
                return;
              }
            }
          };
        }, &mSidebar);
      }, &mBottomSheet);

      shell.add.div([this](glint_component_style& nav) {
        nav.className = "demo-ios-bottom-nav";
        nav.style.width = "100%";
        nav.style.height = bottomNavHeight();
        nav.style.display = "flex";
        nav.style.flexDirection = "row";
        nav.style.alignItems = "center";
        nav.style.gap = 10.f;
        nav.style.backgroundColor = glint_demo_theme::headerBg;
        nav.style.borderColor = glint_demo_theme::border;
        nav.style.borderTopWidth = 1.f;
        nav.style.padding = insetString(8.f, 12.f + mSafeRight, 8.f + mSafeBottom, 12.f + mSafeLeft);

        nav.add.button([this](glint_button& btn) {
          const bool active = activeModeButton() == DemoMode::CoreComponents;
          btn.innerText = "Core";
          btn.style.flexGrow = 1.f;
          btn.style.height = 42.f;
          btn.style.borderWidth = 1.f;
          btn.style.borderRadius = 12.f;
          btn.style.fontSize = 14.f;
          btn.style.backgroundColor = active ? glint_demo_theme::accent : glint_demo_theme::surface;
          btn.style.borderColor = active ? glint_demo_theme::accent : glint_demo_theme::border;
          btn.style.color = active ? glint_demo_theme::heading : glint_demo_theme::muted;
          btn.hover.backgroundColor = active ? glint_demo_theme::accentHover : glint_demo_theme::surfaceHover;
          btn.hover.borderColor = active ? glint_demo_theme::accentHover : glint_demo_theme::slate;
          btn.hover.borderWidth = 1.f;
          btn.hover.borderRadius = 12.f;
          btn.hover.color = glint_demo_theme::heading;
          btn.pressed.backgroundColor = glint_demo_theme::accent;
          btn.pressed.borderColor = glint_demo_theme::accent;
          btn.pressed.borderWidth = 1.f;
          btn.pressed.borderRadius = 12.f;
          btn.pressed.color = glint_demo_theme::heading;
          btn.onClick = [this] { setMode(DemoMode::CoreComponents); };
        }, &mCoreModeBtn);

        nav.add.button([this](glint_button& btn) {
          const bool active = activeModeButton() == DemoMode::UserComponents;
          btn.innerText = "User";
          btn.style.flexGrow = 1.f;
          btn.style.height = 42.f;
          btn.style.borderWidth = 1.f;
          btn.style.borderRadius = 12.f;
          btn.style.fontSize = 14.f;
          btn.style.backgroundColor = active ? glint_demo_theme::accent : glint_demo_theme::surface;
          btn.style.borderColor = active ? glint_demo_theme::accent : glint_demo_theme::border;
          btn.style.color = active ? glint_demo_theme::heading : glint_demo_theme::muted;
          btn.hover.backgroundColor = active ? glint_demo_theme::accentHover : glint_demo_theme::surfaceHover;
          btn.hover.borderColor = active ? glint_demo_theme::accentHover : glint_demo_theme::slate;
          btn.hover.borderWidth = 1.f;
          btn.hover.borderRadius = 12.f;
          btn.hover.color = glint_demo_theme::heading;
          btn.pressed.backgroundColor = glint_demo_theme::accent;
          btn.pressed.borderColor = glint_demo_theme::accent;
          btn.pressed.borderWidth = 1.f;
          btn.pressed.borderRadius = 12.f;
          btn.pressed.color = glint_demo_theme::heading;
          btn.onClick = [this] { setMode(DemoMode::UserComponents); };
        }, &mUserModeBtn);
      });
    });
  }

#if !GLINT_PLATFORM_IOS
  const wchar_t* windowClassName() const override { return L"glint_demos"; }
  const wchar_t* windowTitle() const override { return L"glint Component Demos"; }
  int defaultWidth() const override { return 900; }
  int defaultHeight() const override { return 600; }
#if defined(_WIN32)
  COLORREF bgColor() const override { return RGB(20, 20, 20); }
#endif
  SkColor clearColor() const override { return SkColorSetARGB(255, 24, 24, 24); }

  void onCreated() override
  {
    completeDocumentSetup(GLINT_PLATFORM_MAC);
  }

  void onThreadEnded() override
  {
    detachDocument();
  }

#if GLINT_PLATFORM_MAC
  void afterRun() override
  {
    glint_demo_terminate_app();
  }
#endif

  void buildUI() override
#else
  void buildUI()
#endif
  {
#if !GLINT_PLATFORM_IOS
    mRoot = mOwnRoot.get();
#if GLINT_PLATFORM_MAC
    mDispatchMain = [this](std::function<void()> fn) {
      _dispatchMain(std::move(fn));
    };
#else
    mDispatchMain = {};
#endif
#endif
    if (!mRoot)
      return;

    mLayout = layoutForWidth(mViewportWidth > 0.f ? mViewportWidth : mRoot->mCanvas.mRect.W());

    mRoot->mCanvas.clearChildren();
    mContent = nullptr;
    mSidebar = nullptr;
    mBottomSheet = nullptr;
    mCoreModeBtn = nullptr;
    mUserModeBtn = nullptr;

    mRoot->name = "Component Demos";
    mRoot->mCanvas.className = rootClassName();
    mRoot->mCanvas.style.backgroundColor = glint_demo_theme::canvasBg;
    mRoot->mCanvas.style.width = "100%";
    mRoot->mCanvas.style.height = "100%";
    mRoot->mCanvas.style.flexGrow = 1.f;
    mRoot->mCanvas.style.alignItems = "stretch";
    mRoot->mCanvas.style.justifyContent = "flex-start";

    if (isIOSLayout())
      buildIOSShell();
    else
      buildDesktopShell();

    mRoot->onRequest = [](glint_resource_request& req) {
      static const std::filesystem::path kRepoRootDir =
        std::filesystem::path(__FILE__).parent_path().parent_path().parent_path();
      static const std::filesystem::path kStylesDir =
        kRepoRootDir / "glint_user_code" / "web" / "styles";
      static const std::filesystem::path kWebAssetsDir =
        kRepoRootDir / "glint_user_code" / "web";

      const std::string& url = req.url;
      const bool isCss = url.size() >= 4 &&
                         url.compare(url.size() - 4, 4, ".css") == 0;
      if (isCss)
      {
        const auto p = kStylesDir / std::filesystem::path(req.url).filename();
        req.fromFile(p.string());
        return;
      }

      std::string p = req.pathname;
      while (!p.empty() && (p.front() == '/' || p.front() == '\\')) p.erase(p.begin());
      req.fromFile((kWebAssetsDir / p).string());
    };

#ifndef NDEBUG
    mRoot->setStylesheetHotReloadEnabled(true);
#endif
  }

  void loadStylesheetAndPopulate()
  {
    if (!mRoot)
      return;

    mRoot->loadStylesheet("/styles/main.css");
    if (isIOSLayout())
    {
      mSheetMode = mMode;
      populateSidebar(false);
      rebuildContent();
      syncBottomSheet();
      syncModeButtons();
    }
    else
    {
      populateSidebar();
    }
    scheduleRedraw();
  }

  void populateSidebar(bool rebuildCurrentContent = true)
  {
    if (!mSidebar) return;

    const DemoMode sidebarMode = isIOSLayout() ? mSheetMode : mMode;

    mSidebar->clear();
    for (int sectionIndex = 0; sectionIndex < static_cast<int>(DemoSection::_Count); ++sectionIndex)
    {
      const DemoSection section = static_cast<DemoSection>(sectionIndex);
      if (!sectionBelongsToMode(section, sidebarMode))
        continue;

      mSidebar->items.add([sectionIndex](glint_list_item& item) {
        item.innerText = kSectionNames[sectionIndex];
        item.id = kSectionNames[sectionIndex];
        item.className = "demo-section-item";
        item.style.height = 36.f;
        item.style.fontSize = 13.f;
        item.style.paddingLeft = 14.f;
        item.style.color = glint_demo_theme::text;
        item.selectedStyle.backgroundColor = glint_demo_theme::accent;
        item.selectedStyle.color = glint_demo_theme::heading;
        item.selectedStyle.fontSize = 13.f;
        item.selectedStyle.height = 36.f;
        item.selectedStyle.paddingLeft = 14.f;
      });
    }

    const float itemHeight = isIOSLayout() ? 46.f : 36.f;
    const float itemFont = isIOSLayout() ? 15.f : 13.f;
    const float itemPad = isIOSLayout() ? 16.f : 14.f;
    for (auto* child : mSidebar->mList)
    {
      if (!child) continue;
      child->style.height = itemHeight;
      child->style.fontSize = itemFont;
      child->style.paddingLeft = itemPad;
      child->selectedStyle.height = itemHeight;
      child->selectedStyle.fontSize = itemFont;
      child->selectedStyle.paddingLeft = itemPad;
    }

    const DemoSection highlightedSection = preferredSectionForMode(sidebarMode);
    mSuppressSectionSelection = true;
    mSidebar->selectItemById(kSectionNames[static_cast<int>(highlightedSection)]);
    mSuppressSectionSelection = false;

    if (rebuildCurrentContent)
    {
      mSection = highlightedSection;
      if (!isIOSLayout())
        mMode = sidebarMode;
      rebuildContent();
    }
  }

  void rebuildContent()
  {
    if (!mContent) return;

    mContent->mScrollTop = 0.f;
    mContent->mScrollLeft = 0.f;
    mContent->clearChildren();

    switch (mSection)
    {
      case DemoSection::Text: buildText(); break;
      case DemoSection::WhiteSpace: buildWhiteSpace(); break;
      case DemoSection::Buttons: buildButtons(); break;
      case DemoSection::Switches: buildSwitches(); break;
      case DemoSection::Checkboxes: buildCheckboxes(); break;
      case DemoSection::Inputs: buildInputs(); break;
      case DemoSection::Lists: buildLists(); break;
      case DemoSection::Scroll: buildScroll(); break;
      case DemoSection::TouchDemo: buildTouchDemo(); break;
      case DemoSection::Colors: buildColors(); break;
      case DemoSection::Generic: buildGeneric(); break;
      case DemoSection::Transforms: buildTransforms(); break;
      case DemoSection::Filters: buildFilters(); break;
      case DemoSection::BackdropFilters: buildBackdropFilters(); break;
      case DemoSection::Transitions: buildTransitions(); break;
      case DemoSection::Shaders: buildShaders(); break;
      case DemoSection::Fonts: buildFonts(); break;
      case DemoSection::Select: buildSelects(); break;
      case DemoSection::Masks: buildMasks(); break;
      case DemoSection::Stroke: buildStroke(); break;
      case DemoSection::Images: buildImages(); break;
      case DemoSection::BlendModes: buildBlendModes(); break;
      case DemoSection::Textarea: buildTextarea(); break;
      case DemoSection::Radios: buildRadios(); break;
      case DemoSection::Progress: buildProgress(); break;
      case DemoSection::Tooltips: buildTooltips(); break;
      case DemoSection::DatePicker: buildDatePicker(); break;
      case DemoSection::Cursors: buildCursors(); break;
      default: break;
    }

    scheduleRedraw();
  }

  void scheduleRedraw()
  {
#if !GLINT_PLATFORM_IOS && defined(_WIN32)
    if (HWND h = mHWNDAtom.load()) ::InvalidateRect(h, nullptr, FALSE);
#elif !GLINT_PLATFORM_IOS
    requestRedraw();
#else
    if (mRequestRedraw) mRequestRedraw();
#endif
  }

  void addHeading(const char* text)
  {
    mContent->add.div([=](glint_component_style& lbl) {
      lbl.innerText = text;
      lbl.style.color = glint_demo_theme::heading;
      lbl.style.fontSize = 15.f;
      lbl.style.width = "100%";
      lbl.style.textAlign = EAlign::Near;
      lbl.style.marginBottom = 6.f;
    });
  }

  glint_element* addRow(float gap = 10.f)
  {
    return mContent->add.div([gap](glint_component_style& row) {
      row.style.display = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems = "center";
      row.style.gap = gap;
      row.style.width = "100%";
      row.style.marginBottom = 12.f;
    });
  }

  void addSpacer(float h = 16.f)
  {
    mContent->add.div([h](glint_component_style& sp) {
      sp.style.height = h;
      sp.style.width = "100%";
    });
  }

  static bool isUserSection(DemoSection section)
  {
    return section == DemoSection::Switches || section == DemoSection::Shaders;
  }

  static bool sectionBelongsToMode(DemoSection section, DemoMode mode)
  {
    return mode == DemoMode::UserComponents ? isUserSection(section)
                                            : !isUserSection(section);
  }

  static DemoSection firstSectionForMode(DemoMode mode)
  {
    return mode == DemoMode::UserComponents ? DemoSection::Switches
                                            : DemoSection::Text;
  }

  DemoSection preferredSectionForMode(DemoMode mode) const
  {
    const DemoSection preferred = mode == DemoMode::UserComponents ? mUserSection : mCoreSection;
    return sectionBelongsToMode(preferred, mode) ? preferred : firstSectionForMode(mode);
  }

  void rememberSectionForMode(DemoSection section)
  {
    if (isUserSection(section))
      mUserSection = section;
    else
      mCoreSection = section;
  }

  void syncModeButtons()
  {
    auto apply = [](glint_button* btn, bool active) {
      if (!btn) return;
      btn->style.backgroundColor = active ? glint_demo_theme::accent : glint_demo_theme::surface;
      btn->style.borderColor = active ? glint_demo_theme::accent : glint_demo_theme::border;
      btn->style.color = active ? glint_demo_theme::heading : glint_demo_theme::muted;
      btn->hover.backgroundColor = active ? glint_demo_theme::accentHover : glint_demo_theme::surfaceHover;
      btn->hover.borderColor = active ? glint_demo_theme::accentHover : glint_demo_theme::slate;
      btn->hover.color = glint_demo_theme::heading;
      btn->pressed.backgroundColor = glint_demo_theme::accent;
      btn->pressed.borderColor = glint_demo_theme::accent;
      btn->pressed.color = glint_demo_theme::heading;
      btn->setDirty(false);
    };

    apply(mCoreModeBtn, activeModeButton() == DemoMode::CoreComponents);
    apply(mUserModeBtn, activeModeButton() == DemoMode::UserComponents);
  }

  void setMode(DemoMode mode)
  {
    if (isIOSLayout())
    {
      if (mBottomSheetOpen && mSheetMode == mode)
      {
        mBottomSheetOpen = false;
        syncBottomSheet();
        syncModeButtons();
        scheduleRedraw();
        return;
      }

      mSheetMode = mode;
      populateSidebar(false);
      mBottomSheetOpen = true;
      syncBottomSheet();
      syncModeButtons();
      scheduleRedraw();
      return;
    }

    if (mMode == mode) return;
    mMode = mode;
    syncModeButtons();
    populateSidebar();
  }

  void buildText();
  void buildWhiteSpace();
  void buildButtons();
  void buildSwitches();
  void buildCheckboxes();
  void buildInputs();
  void buildLists();
  void buildScroll();
  void buildTouchDemo();
  void buildColors();
  void buildGeneric();
  void buildFilters();
  void buildBackdropFilters();
  void buildTransforms();
  void buildTransitions();
  void buildShaders();
  void buildFonts();
  void buildSelects();
  void buildMasks();
  void buildStroke();
  void buildImages();
  void buildBlendModes();
  void buildTextarea();
  void buildRadios();
  void buildProgress();
  void buildTooltips();
  void buildDatePicker();
  void buildCursors();
};

#if !GLINT_PLATFORM_IOS
inline glint_demos_window* glint_demos_window::sInstance = nullptr;
#endif

#include "pages/page_text.hpp"
#include "pages/page_white_space.hpp"
#include "pages/page_buttons.hpp"
#include "pages/page_switches.hpp"
#include "pages/page_checkboxes.hpp"
#include "pages/page_inputs.hpp"
#include "pages/page_lists.hpp"
#include "pages/page_scroll.hpp"
#include "pages/page_touch_demo.hpp"
#include "pages/page_colors.hpp"
#include "pages/page_generic.hpp"
#include "pages/page_filters.hpp"
#include "pages/page_backdrop_filters.hpp"
#include "pages/page_transforms.hpp"
#include "pages/page_transitions.hpp"
#include "pages/page_shaders.hpp"
#include "pages/page_fonts.hpp"
#include "pages/page_selects.hpp"
#include "pages/page_masks.hpp"
#include "pages/page_stroke.hpp"
#include "pages/page_images.hpp"
#include "pages/page_blend_modes.hpp"
#include "pages/page_textarea.hpp"
#include "pages/page_radios.hpp"
#include "pages/page_progress.hpp"
#include "pages/page_tooltips.hpp"
#include "pages/page_datepicker.hpp"
#include "pages/page_cursors.hpp"