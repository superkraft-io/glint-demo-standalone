#pragma once

inline void glint_demos_window::buildCheckboxes()
{
  auto addHeading = [&](const char* text, float marginBottom = 6.f) {
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

  // ── Basic states ────────────────────────────────────────────────────────
  addHeading("Checked / unchecked");

  const struct { const char* label; bool checked; } basicDefs[] = {
    { "Unchecked (default)",         false },
    { "Checked",                     true  },
    { "Keep-checked (click to try)", true  },
  };

  for (int i = 0; i < 3; ++i)
  {
    const auto& d = basicDefs[i];
    glint_checkbox* cb      = nullptr;
    glint_element*  stateEl = nullptr;

    mContent->add.div([&](glint_component_style& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 12.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 4.f;

      row.add.fromClass<glint_checkbox>([&](glint_checkbox& c) {
        c.text        = d.label;
        c.checked     = d.checked;
        c.size        = 16.f;
        c.keepChecked = (i == 2);
      }, &cb);

      row.add.div([&](glint_component_style& state) {
        state.innerText      = d.checked ? "Y" : "N";
        state.style.color    = d.checked ? glint_demo_theme::success : glint_demo_theme::danger;
        state.style.fontSize = 13.f;
        state.style.width    = 16.f;
      }, &stateEl);
    });

    cb->onChange = [stateEl](bool v) {
      stateEl->innerText   = v ? "Y" : "N";
      stateEl->style.color = v ? glint_demo_theme::success : glint_demo_theme::danger;
      stateEl->setDirty(false);
    };
  }

  addSpacer(12.f);

  // ── Colour themes ────────────────────────────────────────────────────────
  addHeading("Colour themes");

  const struct {
    const char* label;
    const char* checkedBg;
    const char* borderCol;
  } themeDefs[] = {
    { "Default", glint_demo_theme::primary,   "#6e7080" },
    { "Green",   glint_demo_theme::successBg, "#6e7080" },
    { "Red",     glint_demo_theme::dangerBg,  "#6e7080" },
    { "Slate",   glint_demo_theme::slate,     "#6e7080" },
    { "Amber",   glint_demo_theme::amberBg,   "#6e7080" },
  };

  for (const auto& td : themeDefs)
  {
    mContent->add.div([&](glint_component_style& r) {
      r.style.display       = "flex";
      r.style.flexDirection = "row";
      r.style.alignItems    = "center";
      r.style.width         = "100%";
      r.style.marginBottom  = 6.f;

      r.add.fromClass<glint_checkbox>([&](glint_checkbox& cb) {
        cb.text      = td.label;
        cb.checked   = true;
        cb.size      = 16.f;
        cb.checkedBg = td.checkedBg;
        cb.borderCol = td.borderCol;
      });
    });
  }

  addSpacer(12.f);

  // ── Size variants ────────────────────────────────────────────────────────
  addHeading("Size variants (size = 12 / 16 / 20 / 26)");

  const float szSizes[] = { 12.f, 16.f, 20.f, 26.f };

  mContent->add.div([&](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems    = "center";
    row.style.gap           = 20.f;
    row.style.width         = "100%";

    for (float sz : szSizes)
    {
      row.add.fromClass<glint_checkbox>([sz](glint_checkbox& cb) {
        cb.text      = std::to_string(static_cast<int>(sz)) + "px";
        cb.checked   = true;
        cb.size      = sz;
        cb.checkedBg = glint_demo_theme::primary;
      });
    }
  });
}

