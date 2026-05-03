#pragma once

inline void glint_demos_window::buildButtons()
{
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

  auto addButton = [](glint_element* parent,
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
      btn.style.width = width;
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

  auto* row1 = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.gap = 10.f;
    row.style.width = "100%";
    row.style.height = 36.f;
    row.style.marginBottom = 10.f;
  });

  addButton(row1, "Primary", 110.f, 34.f, glint_demo_theme::primary, glint_demo_theme::primaryHover, "#fff", "#fff", "#00000000", 6.f, 13.f);
  addButton(row1, "Secondary", 110.f, 34.f, glint_demo_theme::surface, glint_demo_theme::surfaceHover, glint_demo_theme::text, glint_demo_theme::text, "#00000000", 6.f, 13.f);
  addButton(row1, "Danger", 110.f, 34.f, glint_demo_theme::dangerBg, "#714040", "#fff", "#fff", "#00000000", 6.f, 13.f);
  addButton(row1, "Ghost", 110.f, 34.f, "#00000000", glint_demo_theme::surface, glint_demo_theme::heading, glint_demo_theme::heading, glint_demo_theme::border, 6.f, 13.f);

  addSpacer(4.f);
  addHeading("Border radius");

  auto* row2 = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.gap = 10.f;
    row.style.width = "100%";
    row.style.height = 36.f;
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

  auto* row3 = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "center";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.height = 40.f;
  });

  auto* btn = addButton(row3, "Click me!", 110.f, 34.f, glint_demo_theme::successBg, "#2d542d",
                        glint_demo_theme::success, glint_demo_theme::success, "#00000000", 6.f, 13.f);

  auto* counter = row3->add.div([](glint_component_style& counter) {
    counter.innerText = "Clicks: 0";
    counter.style.color = glint_demo_theme::success;
    counter.style.fontSize = 14.f;
    counter.style.width = 120.f;
    counter.style.textAlign = EAlign::Near;
  });

  glint_element* ctrPtr = counter;
  int* clicks = new int(0);
  btn->onClick = [ctrPtr, clicks] {
    (*clicks)++;
    ctrPtr->innerText = std::string("Clicks: ") + std::to_string(*clicks);
    ctrPtr->setDirty(false);
  };
}
