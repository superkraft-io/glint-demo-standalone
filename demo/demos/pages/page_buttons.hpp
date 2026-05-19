#pragma once

inline void glint_demos_window::buildButtons()
{
  const bool compactLayout = isCompactLayout();
  const float compactButtonRowHeight = (34.f * 4.f) + (10.f * 3.f);
  const float compactInteractiveRowHeight = 34.f + 12.f + 20.f;
  const float compactClassRowHeight = (30.f * 4.f) + (8.f * 3.f);

  auto addHeading = [&](const char* text, float marginBottom = 10.f) {
    mContent->add.div([=](glint_component_style& heading) {
      heading.innerText = text;
      heading.style.color = glint_demo_theme::heading;
      heading.style.fontSize = 15.f;
      heading.style.width = "100%";
      heading.style.textAlign = EAlign::Near;
      heading.style.marginBottom = marginBottom;
    });
  };

  auto addSpacer = [&](float h) {
    mContent->add.div([=](glint_component_style& spacer) {
      spacer.style.height = h;
      spacer.style.width = "100%";
    });
  };

  auto addButton = [compactLayout](glint_element* parent,
                                   const char* label,
                                   float width,
                                   float height,
                                   const char* bg,
                                   const char* hoverBg,
                                   const char* text,
                                   const char* hoverText,
                                   const char* border,
                                   float radius,
                                   float fontSize) {
    return parent->add.button([=](glint_button& btn) {
      btn.innerText = label;
      if (compactLayout) btn.style.width = "100%";
      else btn.style.width = width;
      btn.style.height = height;
      btn.style.backgroundColor = bg;
      btn.style.color = text;
      btn.style.borderRadius = radius;
      btn.style.fontSize = fontSize;
      btn.style.borderColor = border;
      btn.style.borderWidth = 1.f;
      btn.hover.backgroundColor = hoverBg;
      btn.hover.color = hoverText;
      btn.hover.borderRadius = radius;
      btn.hover.fontSize = fontSize;
      btn.hover.borderColor = border;
      btn.hover.borderWidth = 1.f;
      btn.pressed.backgroundColor = hoverBg;
      btn.pressed.color = hoverText;
      btn.pressed.borderRadius = radius;
      btn.pressed.fontSize = fontSize;
      btn.pressed.borderColor = border;
      btn.pressed.borderWidth = 1.f;
    });
  };

  addHeading("Variants");

  auto* row1 = mContent->add.div([compactLayout, compactButtonRowHeight](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = compactLayout ? "column" : "row";
    row.style.alignItems = compactLayout ? "stretch" : "center";
    row.style.gap = 10.f;
    row.style.width = "100%";
    row.style.height = compactLayout ? compactButtonRowHeight : 36.f;
    row.style.marginBottom = 10.f;
  });

  addButton(row1, "Primary", 110.f, 34.f, glint_demo_theme::primary, glint_demo_theme::primaryHover, "#fff", "#fff", "#00000000", 6.f, 13.f);
  addButton(row1, "Secondary", 110.f, 34.f, glint_demo_theme::surface, glint_demo_theme::surfaceHover, glint_demo_theme::text, glint_demo_theme::text, "#00000000", 6.f, 13.f);
  addButton(row1, "Danger", 110.f, 34.f, glint_demo_theme::dangerBg, "#714040", "#fff", "#fff", "#00000000", 6.f, 13.f);
  addButton(row1, "Ghost", 110.f, 34.f, "#00000000", glint_demo_theme::surface, glint_demo_theme::heading, glint_demo_theme::heading, glint_demo_theme::border, 6.f, 13.f);

  addSpacer(4.f);
  addHeading("Border radius");

  auto* row2 = mContent->add.div([compactLayout, compactButtonRowHeight](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = compactLayout ? "column" : "row";
    row.style.alignItems = compactLayout ? "stretch" : "center";
    row.style.gap = 10.f;
    row.style.width = "100%";
    row.style.height = compactLayout ? compactButtonRowHeight : 36.f;
    row.style.marginBottom = 16.f;
  });

  const struct { const char* lbl; float r; } radii[] = {
    {"r = 0",  0.f},
    {"r = 4",  4.f},
    {"r = 8",  8.f},
    {"r = 17", 17.f},
  };
  for (const auto& rv : radii)
    addButton(row2, rv.lbl, 100.f, 34.f, glint_demo_theme::surfaceHover, glint_demo_theme::primaryHover,
              glint_demo_theme::text, glint_demo_theme::text, "#00000000", rv.r, 12.f);

  addHeading("Interactive");

  auto* row3 = mContent->add.div([compactLayout, compactInteractiveRowHeight](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = compactLayout ? "column" : "row";
    row.style.alignItems = compactLayout ? "stretch" : "center";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.height = compactLayout ? compactInteractiveRowHeight : 40.f;
  });

  auto* btn = addButton(row3, "Click me!", 110.f, 34.f, glint_demo_theme::successBg, "#2d542d",
                        glint_demo_theme::success, glint_demo_theme::success, "#00000000", 6.f, 13.f);

  auto* counter = row3->add.div([compactLayout](glint_component_style& counter) {
    counter.innerText = "Clicks: 0";
    counter.style.color = glint_demo_theme::success;
    counter.style.fontSize = 14.f;
    if (compactLayout) counter.style.width = "100%";
    else counter.style.width = 120.f;
    counter.style.textAlign = EAlign::Near;
  });

  glint_element* ctrPtr = counter;
  int* clicks = new int(0);
  btn->onClick = [ctrPtr, clicks] {
    (*clicks)++;
    ctrPtr->innerText = std::string("Clicks: ") + std::to_string(*clicks);
    ctrPtr->setDirty(false);
  };

  addSpacer(20.f);
  addHeading("classList  (add / remove / toggle / contains)");

  // Status label
  glint_element* clsStatusPtr = nullptr;
  mContent->add.div([](glint_component_style& s) {
    s.innerText          = "No classes applied";
    s.style.color        = glint_demo_theme::muted;
    s.style.fontSize     = 12.f;
    s.style.width        = "100%";
    s.style.textAlign    = EAlign::Near;
    s.style.marginBottom = 8.f;
  }, &clsStatusPtr);

  // Target box whose classes we'll mutate.
  // Only layout properties are set inline; visual properties (color, bg, border)
  // come entirely from the CSS classes (.demo-default, .demo-active, etc.).
  glint_element* clsBoxPtr = nullptr;
  mContent->add.div([](glint_component_style& box) {
    box.innerText             = "Target element";
    box.className             = "demo-default";
    box.style.width           = "100%";
    box.style.height          = 52.f;
    box.style.borderRadius    = 6.f;
    box.style.display         = "flex";
    box.style.alignItems      = "center";
    box.style.justifyContent  = "center";
    box.style.fontSize        = 13.f;
    box.style.marginBottom    = 10.f;
  }, &clsBoxPtr);

  // Helper that updates only the status label text.
  // Visual changes on clsBoxPtr are driven entirely by CSS class rules —
  // classList._notifyChange() re-cascades and redraws automatically.
  auto refreshStatus = [clsBoxPtr, clsStatusPtr]() {
    const bool active  = clsBoxPtr->classList.contains("demo-active");
    const bool outline = clsBoxPtr->classList.contains("demo-outline");
    const bool faded   = clsBoxPtr->classList.contains("demo-faded");

    std::string msg;
    if (!active && !outline && !faded)
      msg = "No classes applied";
    else
    {
      if (active)  msg += ".demo-active  ";
      if (outline) msg += ".demo-outline  ";
      if (faded)   msg += ".demo-faded";
    }
    clsStatusPtr->innerText = msg;
    clsStatusPtr->setDirty(false);
  };

  auto* clsBtnRow = mContent->add.div([compactLayout, compactClassRowHeight](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = compactLayout ? "column" : "row";
    row.style.alignItems    = compactLayout ? "stretch" : "center";
    row.style.gap           = 8.f;
    row.style.width         = "100%";
    if (compactLayout) row.style.height = compactClassRowHeight;
  });

  struct ClsBtn { const char* label; std::function<void()> action; };
  std::vector<ClsBtn> clsBtns;

  // Capture shared_ptr so lambdas stay valid even if clsBtns is destroyed
  auto refreshFn = std::make_shared<std::function<void()>>(refreshStatus);

  clsBtns = {
    { "add .demo-active",     [clsBoxPtr, refreshFn]{ clsBoxPtr->classList.add("demo-active");     (*refreshFn)(); } },
    { "remove .demo-active",  [clsBoxPtr, refreshFn]{ clsBoxPtr->classList.remove("demo-active");  (*refreshFn)(); } },
    { "toggle .demo-outline", [clsBoxPtr, refreshFn]{ clsBoxPtr->classList.toggle("demo-outline"); (*refreshFn)(); } },
    { "toggle .demo-faded",   [clsBoxPtr, refreshFn]{ clsBoxPtr->classList.toggle("demo-faded");   (*refreshFn)(); } },
  };

  for (const auto& cb : clsBtns)
  {
    clsBtnRow->add.template fromClass<glint_button>([cb, compactLayout](glint_button& btn) {
      btn.innerText             = cb.label;
      btn.style.height          = 30.f;
      if (compactLayout) btn.style.width = "100%";
      btn.style.padding         = "0 10";
      btn.style.borderRadius    = 4.f;
      btn.style.fontSize        = 12.f;
      btn.style.backgroundColor = glint_demo_theme::surface;
      btn.style.color           = glint_demo_theme::text;
      btn.style.borderColor     = glint_demo_theme::border;
      btn.style.borderWidth     = 1.f;
      btn.hover.backgroundColor = glint_demo_theme::surfaceHover;
      btn.hover.color           = glint_demo_theme::text;
      btn.hover.borderColor     = glint_demo_theme::border;
      btn.hover.borderWidth     = 1.f;
      btn.hover.borderRadius    = 4.f;
      btn.onClick               = cb.action;
    });
  }
}
