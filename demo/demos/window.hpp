#pragma once

/**
 * demos/window.hpp
 * glint Component Demos � a standalone Win32 window that showcases every
 * built-in glint component.
 *
 * API:
 *   glint_demos_window::open();   // create + show (no-op if already open)
 *   glint_demos_window::close();  // teardown
 *   glint_demos_window::isOpen(); // query
 *
 * Layout:
 *   +- header (title bar, 40 px) -----------------------------------------+
 *   � +- sidebar (180 px) --+ +- content panel (flex-grow: 1) ----------+ �
 *   � �  glint_list         � �  rebuilt per-section                    � �
 *   � �  � Labels           � �                                         � �
 *   � �  � Buttons          � �                                         � �
 *   � �  � Switches         � �                                         � �
 *   � �  � Checkboxes       � �                                         � �
 *   � �  � Inputs           � �                                         � �
 *   � �  � Lists            � �                                         � �
 *   � �  � Scroll           � �                                         � �
 *   � �  � Filters          � �                                         � �
 *   � �  � Transitions      � �                                         � �
 *   � +---------------------+ +-----------------------------------------+ �
 *   +---------------------------------------------------------------------+
 *
 * File structure:
 *   window.hpp               � class definition, sidebar/content scaffold,
 *                              section helpers (addHeading/addRow/addSpacer)
 *   pages/page_labels.hpp    � buildLabels()
 *   pages/page_buttons.hpp   � buildButtons()
 *   pages/page_switches.hpp  � buildSwitches()
 *   pages/page_checkboxes.hpp� buildCheckboxes()
 *   pages/page_inputs.hpp    � buildInputs()
 *   pages/page_lists.hpp     � buildLists()
 *   pages/page_scroll.hpp    � buildScroll()
 *   pages/page_colors.hpp    � buildColors()
 *   pages/page_filters.hpp   � buildFilters()
 *   pages/page_backdrop_filters.hpp � buildBackdropFilters()
 *   pages/page_transforms.hpp� buildTransforms()
 *   pages/page_transitions.hpp � buildTransitions()
 *   pages/page_shaders.hpp   � buildShaders()
 *
 * Thread model: identical to glint_inspector_window � background thread owns
 * the HWND + message loop + glint_document.  The main plugin thread is never
 * touched from here.
 */

#include "glint/glint_standalone.hpp"
#include "glint/glint_window.hpp"
#include "glint/components/glint_button.hpp"
#include "glint/components/glint_checkbox.hpp"
#include "glint/components/glint_colorpicker.hpp"
#include "glint/components/glint_gradient_editor.hpp"
#include "glint/components/glint_list/glint_list.hpp"
#include "glint/shaders/glint_shaders.hpp"
#include "glint_user_code/cpp/ui/glint_switch.hpp"
#include "glint/components/glint_select.hpp"

#if defined(GLINT_BUNDLE_SHALLOW) || defined(GLINT_BUNDLE_DEEP)
#include "glint_user_code/cpp/bundle/glint_bundle_library.hpp"
#endif

#ifdef __APPLE__
// Implemented in demo_mac_main.mm.  Dispatches [NSApp terminate:nil] on the
// main run loop so the app exits cleanly after the panel is fully closed.
extern "C" void glint_demo_terminate_app();
#endif

namespace glint_demo_theme {
inline constexpr const char* canvasBg      = "#181818";
inline constexpr const char* headerBg      = "#202020";
inline constexpr const char* sidebarBg     = "#1c1c1c";
inline constexpr const char* panelBg       = "#252526";
inline constexpr const char* surface       = "#2d2d2d";
inline constexpr const char* surfaceAlt    = "#232323";
inline constexpr const char* surfaceHover  = "#383838";
inline constexpr const char* border        = "#3c3c3c";
inline constexpr const char* text          = "#e6e6e6";
inline constexpr const char* heading       = "#ffffff";
inline constexpr const char* muted         = "#b8b8b8";
inline constexpr const char* subtle        = "#8a8a8a";
inline constexpr const char* primary       = "#4c4c4c";
inline constexpr const char* primaryHover  = "#5a5a5a";
inline constexpr const char* success       = "#7ec07e";
inline constexpr const char* successBg     = "#244224";
inline constexpr const char* danger        = "#d07b7b";
inline constexpr const char* dangerBg      = "#5a3030";
inline constexpr const char* warning       = "#d0b16f";
inline constexpr const char* amberBg       = "#6a4620";
inline constexpr const char* slate         = "#5f6872";
inline constexpr const char* slateHover    = "#6d7782";
inline constexpr const char* accent        = "#1a6fc4";
inline constexpr const char* accentHover   = "#2179cf";
}

// -----------------------------------------------------------------------------
// DemoSection enum � drives the sidebar list + right panel rebuild
// -----------------------------------------------------------------------------

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

static constexpr const char* kSectionNames[] = {
  "Text",
  "White Space",
  "Buttons",
  "Switches",
  "Checkboxes",
  "Inputs",
  "Lists",
  "Scroll",
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

// -----------------------------------------------------------------------------
// glint_demos_window
// -----------------------------------------------------------------------------

class glint_demos_window : public glint_window
{
public:
  // -- Singleton API ----------------------------------------------------------

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

  static bool isOpen() { return sInstance && sInstance->isRunning(); }

private:
  glint_demos_window() = default;

  // -- Layout constants -------------------------------------------------------
  static constexpr int   kHeaderH  = 40;
  static constexpr float kSidebarW = 180.f;

  // -- State ------------------------------------------------------------------
  DemoMode         mMode        = DemoMode::CoreComponents;
  DemoSection      mSection     = DemoSection::Text;
  DemoSection      mCoreSection = DemoSection::Text;
  DemoSection      mUserSection = DemoSection::Switches;
  glint_element*   mContent     = nullptr;  // right panel � ptr owned by root
  glint_list*      mSidebar     = nullptr;  // left list � ptr owned by root
  glint_button*    mCoreModeBtn = nullptr;
  glint_button*    mUserModeBtn = nullptr;

  static glint_demos_window* sInstance;

#if defined(GLINT_BUNDLE_SHALLOW) || defined(GLINT_BUNDLE_DEEP)
  glint_bundle::glint_bundle_library mBundle;
#endif

  // -- UI construction --------------------------------------------------------
  const wchar_t* windowClassName() const override { return L"glint_demos"; }
  const wchar_t* windowTitle()     const override { return L"glint Component Demos"; }
  int  defaultWidth()  const override { return 900; }
  int  defaultHeight() const override { return 600; }
#if defined(_WIN32)
  COLORREF bgColor() const override { return RGB(20, 20, 20); }
#endif
  SkColor clearColor() const override { return SkColorSetARGB(255, 24, 24, 24); }
  void onCreated() override
  {
#if defined(GLINT_BUNDLE_SHALLOW) || defined(GLINT_BUNDLE_DEEP)
    mOwnRoot->onRequest = [this](glint_resource_request& req) {
      mBundle.dispatch(req);
    };
#endif

#ifdef __APPLE__
    // onCreated() is still on the _createPanelAndView() call stack, so the
    // window cannot draw until we return.  Dispatching asynchronously pushes
    // the expensive stylesheet / @font-face work to the next run-loop cycle,
    // letting the window appear on screen before any fonts are loaded.
    _dispatchMain([this]() {
      mOwnRoot->loadStylesheet("/styles/main.css");
      populateSidebar();
      scheduleRedraw();
    });
#else
    mOwnRoot->loadStylesheet("/styles/main.css");
    populateSidebar();
#endif
  }

  void onThreadEnded() override
  {
    if (mOwnRoot) glint_insp_bridge::close(mOwnRoot.get());
  }

#ifdef __APPLE__
  void afterRun() override
  {
    // The panel is fully closed.  Tell NSApp to quit.
    glint_demo_terminate_app();
  }
#endif

  void buildUI() override
  {
    mContent = nullptr;
    mSidebar = nullptr;
    mCoreModeBtn = nullptr;
    mUserModeBtn = nullptr;

    mOwnRoot->name = "Component Demos";

    // Set canvas background color (base initRoot sets display:flex/column)
    mOwnRoot->mCanvas.style.backgroundColor = glint_demo_theme::canvasBg;

    // -- Header --------------------------------------------------------------
    mOwnRoot->add.div([this](glint_component_style& header) {
      header.style.backgroundColor = glint_demo_theme::headerBg;
      header.style.height          = static_cast<float>(kHeaderH);
      header.style.width           = "100%";
      header.style.display         = "flex";
      header.style.alignItems      = "center";
      header.style.padding         = "0 16";
      header.style.gap             = 8.f;

      header.add.button([this](glint_button& btn) {
        const bool active            = (mMode == DemoMode::CoreComponents);
        btn.innerText                = "Core components";
        btn.style.height             = 28.f;
        btn.style.padding            = "0 12";
        btn.style.borderWidth        = 1.f;
        btn.style.borderRadius       = 4.f;
        btn.style.fontSize           = 12.f;
        btn.style.backgroundColor    = active ? glint_demo_theme::accent      : glint_demo_theme::surface;
        btn.style.borderColor        = active ? glint_demo_theme::accent      : glint_demo_theme::border;
        btn.style.color              = active ? glint_demo_theme::heading      : glint_demo_theme::muted;
        btn.hover.backgroundColor    = active ? glint_demo_theme::accentHover : glint_demo_theme::surfaceHover;
        btn.hover.borderColor        = active ? glint_demo_theme::accentHover : glint_demo_theme::slate;
        btn.hover.borderWidth        = 1.f;
        btn.hover.borderRadius       = 4.f;
        btn.hover.color              = glint_demo_theme::heading;
        btn.pressed.backgroundColor  = glint_demo_theme::accent;
        btn.pressed.borderColor      = glint_demo_theme::accent;
        btn.pressed.borderWidth      = 1.f;
        btn.pressed.borderRadius     = 4.f;
        btn.pressed.color            = glint_demo_theme::heading;
        btn.onClick = [this] { setMode(DemoMode::CoreComponents); };
      }, &mCoreModeBtn);

      header.add.button([this](glint_button& btn) {
        const bool active            = (mMode == DemoMode::UserComponents);
        btn.innerText                = "User components";
        btn.style.height             = 28.f;
        btn.style.padding            = "0 12";
        btn.style.borderWidth        = 1.f;
        btn.style.borderRadius       = 4.f;
        btn.style.fontSize           = 12.f;
        btn.style.backgroundColor    = active ? glint_demo_theme::accent      : glint_demo_theme::surface;
        btn.style.borderColor        = active ? glint_demo_theme::accent      : glint_demo_theme::border;
        btn.style.color              = active ? glint_demo_theme::heading      : glint_demo_theme::muted;
        btn.hover.backgroundColor    = active ? glint_demo_theme::accentHover : glint_demo_theme::surfaceHover;
        btn.hover.borderColor        = active ? glint_demo_theme::accentHover : glint_demo_theme::slate;
        btn.hover.borderWidth        = 1.f;
        btn.hover.borderRadius       = 4.f;
        btn.hover.color              = glint_demo_theme::heading;
        btn.pressed.backgroundColor  = glint_demo_theme::accent;
        btn.pressed.borderColor      = glint_demo_theme::accent;
        btn.pressed.borderWidth      = 1.f;
        btn.pressed.borderRadius     = 4.f;
        btn.pressed.color            = glint_demo_theme::heading;
        btn.onClick = [this] { setMode(DemoMode::UserComponents); };
      }, &mUserModeBtn);
    });

    // 1 px header separator
    mOwnRoot->add.div([](glint_component_style& _c) {
      _c.style.backgroundColor = glint_demo_theme::border;
      _c.style.height          = 1.f;
      _c.style.width           = "100%";
    });

    // -- Body row: sidebar + content -----------------------------------------
    mOwnRoot->add.div([this](glint_component_style& bodyRow) {
      bodyRow.style.display       = "flex";
      bodyRow.style.flexDirection = "row";
      bodyRow.style.flexGrow      = 1.f;
      bodyRow.style.width         = "100%";
      bodyRow.style.overflow      = "hidden";

      // -- Sidebar ----------------------------------------------------------
      bodyRow.add.fromClass<glint_list>([this](glint_list& sidebar) {
        sidebar.style.width           = kSidebarW;
        sidebar.style.height          = "100%";
        sidebar.style.backgroundColor = glint_demo_theme::sidebarBg;
        sidebar.highlightOnSelect     = true;
        sidebar.onItemSelected = [this](glint_list_item* item) {
          if (!item) return;
          for (int s = 0; s < static_cast<int>(DemoSection::_Count); ++s)
          {
            if (item->id == kSectionNames[s])
            {
              mSection = static_cast<DemoSection>(s);
              rememberSectionForMode(mSection);
              rebuildContent();
              return;
            }
          }
        };
      }, &mSidebar);

      // -- Vertical divider -------------------------------------------------
      bodyRow.add.div([](glint_component_style& divider) {
        divider.style.width           = 1.f;
        divider.style.height          = "100%";
        divider.style.backgroundColor = glint_demo_theme::border;
      });

      // -- Content panel ----------------------------------------------------
      bodyRow.add.div([](glint_component_style& content) {
        content.style.flexGrow        = 1.f;
        content.style.height          = "100%";
        content.style.backgroundColor = glint_demo_theme::panelBg;
        content.style.overflowY       = "auto";
        content.style.padding         = "16";
      }, &mContent);
    });

    mOwnRoot->onRequest = [](glint_resource_request& req) {
      static const std::filesystem::path kRepoRootDir =
        std::filesystem::path(__FILE__).parent_path().parent_path().parent_path();
      static const std::filesystem::path kStylesDir =
        kRepoRootDir / "glint_user_code" / "web" / "styles";
      static const std::filesystem::path kWebAssetsDir =
        kRepoRootDir / "glint_user_code" / "web";


      // Route .css requests to the shared user-code styles directory.
      const std::string& url = req.url;
      const bool isCss = url.size() >= 4 &&
                         url.compare(url.size() - 4, 4, ".css") == 0;
      if (isCss)
      {
        const auto p = kStylesDir / std::filesystem::path(req.url).filename();
        req.fromFile(p.string());
        return;
      }
      // All other assets route through the active standalone web asset tree.
      std::string p = req.pathname;
      while (!p.empty() && (p.front() == '/' || p.front() == '\\')) p.erase(p.begin());
      req.fromFile((kWebAssetsDir / p).string());
    };

    // Hot-reload is enabled in debug builds; the actual loadStylesheet() call
    // is deferred to onCreated() so the window appears before fonts are loaded.
  #ifndef NDEBUG
    mOwnRoot->setStylesheetHotReloadEnabled(true);
  #endif
  }

  // called after UI is built (mSidebar + mContent are valid)
  void populateSidebar()
  {
    if (!mSidebar) return;

    mSidebar->clear();

    for (int s = 0; s < static_cast<int>(DemoSection::_Count); ++s)
    {
      const DemoSection section = static_cast<DemoSection>(s);
      if (!sectionBelongsToMode(section, mMode))
        continue;

      mSidebar->items.add([s](glint_list_item& item) {
        item.innerText                   = kSectionNames[s];
        item.id                          = kSectionNames[s];
        item.style.height                = 36.f;
        item.style.fontSize              = 13.f;
        item.style.paddingLeft           = 14.f;
        item.style.color                 = glint_demo_theme::text;
        item.selectedStyle.backgroundColor = glint_demo_theme::accent;
        item.selectedStyle.color           = glint_demo_theme::heading;
        item.selectedStyle.fontSize        = 13.f;
        item.selectedStyle.height          = 36.f;
        item.selectedStyle.paddingLeft     = 14.f;
      });
    }

    mSection = preferredSectionForMode(mMode);
    mSidebar->selectItemById(kSectionNames[static_cast<int>(mSection)]);
  }

  // -- Content rebuild --------------------------------------------------------
  void rebuildContent()
  {
    if (!mContent) return;
    mContent->mScrollTop  = 0.f;
    mContent->mScrollLeft = 0.f;
    mContent->clearChildren();

    switch (mSection)
    {
    case DemoSection::Text:     buildText();     break;
    case DemoSection::WhiteSpace: buildWhiteSpace(); break;
    case DemoSection::Buttons:  buildButtons();  break;
    case DemoSection::Switches:   buildSwitches();   break;
    case DemoSection::Checkboxes: buildCheckboxes(); break;
    case DemoSection::Inputs:     buildInputs();     break;
    case DemoSection::Lists:    buildLists();    break;
    case DemoSection::Scroll:      buildScroll();      break;
    case DemoSection::Colors:      buildColors();      break;
    case DemoSection::Generic:     buildGeneric();     break;
    case DemoSection::Transforms:   buildTransforms();   break;
    case DemoSection::Filters:          buildFilters();          break;
    case DemoSection::BackdropFilters:   buildBackdropFilters();  break;
    case DemoSection::Transitions:       buildTransitions();      break;
    case DemoSection::Shaders:           buildShaders();          break;
    case DemoSection::Fonts:             buildFonts();            break;
    case DemoSection::Select:            buildSelects();          break;
    case DemoSection::Masks:             buildMasks();            break;
    case DemoSection::Stroke:            buildStroke();           break;
    case DemoSection::Images:            buildImages();           break;
    case DemoSection::BlendModes:        buildBlendModes();       break;
    case DemoSection::Textarea:           buildTextarea();         break;
    case DemoSection::Radios:             buildRadios();           break;
    case DemoSection::Progress:           buildProgress();         break;
    case DemoSection::Tooltips:           buildTooltips();         break;
    case DemoSection::DatePicker:         buildDatePicker();       break;
    case DemoSection::Cursors:             buildCursors();          break;
    default: break;
    }

    scheduleRedraw();
  }

  void scheduleRedraw()
  {
#if defined(_WIN32)
    if (HWND h = mHWNDAtom.load()) ::InvalidateRect(h, nullptr, FALSE);
#else
    requestRedraw();
#endif
  }

  // -- Section helpers --------------------------------------------------------

  /** Add a section heading inside mContent */
  void addHeading(const char* text)
  {
    mContent->add.div([=](glint_component_style& lbl) {
      lbl.innerText          = text;
      lbl.style.color        = glint_demo_theme::heading;
      lbl.style.fontSize     = 15.f;
      lbl.style.width        = "100%";
      lbl.style.textAlign    = EAlign::Near;
      lbl.style.marginBottom = 6.f;
    });
  }

  /** Row helper � horizontal flex strip */
  glint_element* addRow(float gap = 10.f)
  {
    return mContent->add.div([gap](glint_component_style& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = gap;
      row.style.width         = "100%";
      row.style.marginBottom  = 12.f;
    });
  }

  /** Spacer between sections */
  void addSpacer(float h = 16.f)
  {
    mContent->add.div([h](glint_component_style& sp) {
      sp.style.height = h;
      sp.style.width  = "100%";
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
      btn->style.backgroundColor   = active ? glint_demo_theme::primaryHover : glint_demo_theme::surface;
      btn->style.borderColor       = active ? glint_demo_theme::slateHover : glint_demo_theme::border;
      btn->style.color             = active ? glint_demo_theme::heading : glint_demo_theme::muted;
      btn->hover.backgroundColor   = active ? glint_demo_theme::primaryHover : glint_demo_theme::surfaceHover;
      btn->hover.borderColor       = active ? glint_demo_theme::slateHover : glint_demo_theme::slate;
      btn->hover.color             = glint_demo_theme::heading;
      btn->pressed.backgroundColor = glint_demo_theme::primary;
      btn->pressed.borderColor     = glint_demo_theme::slateHover;
      btn->pressed.color           = glint_demo_theme::heading;
      btn->setDirty(false);
    };

    apply(mCoreModeBtn, mMode == DemoMode::CoreComponents);
    apply(mUserModeBtn, mMode == DemoMode::UserComponents);
  }

  void setMode(DemoMode mode)
  {
    if (mMode == mode) return;
    mMode = mode;
    syncModeButtons();
    populateSidebar();
  }

  // -- Section builders (declarations only; bodies in pages/) ------------------
  void buildText();
  void buildWhiteSpace();
  void buildButtons();
  void buildSwitches();
  void buildCheckboxes();
  void buildInputs();
  void buildLists();
  void buildScroll();
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

inline glint_demos_window* glint_demos_window::sInstance = nullptr;

// -- Page implementations (out-of-class method definitions) -------------------
#include "pages/page_text.hpp"
#include "pages/page_white_space.hpp"
#include "pages/page_buttons.hpp"
#include "pages/page_switches.hpp"
#include "pages/page_checkboxes.hpp"
#include "pages/page_inputs.hpp"
#include "pages/page_lists.hpp"
#include "pages/page_scroll.hpp"
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
