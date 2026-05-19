#pragma once

inline void glint_demos_window::buildSwitches()
{
  const bool compactLayout = isCompactLayout();

  auto addHeading = [&](const char* text) {
    mContent->add.div([=](glint_component_style& d) {
      d.innerText          = text;
      d.style.color        = glint_demo_theme::heading;
      d.style.fontSize     = 15.f;
      d.style.width        = "100%";
      d.style.textAlign    = EAlign::Near;
      d.style.marginBottom = 12.f;
    });
  };

  // ── Toggle switches ──────────────────────────────────────────────────────
  addHeading("Toggle switches");

  const struct {
    const char* label;
    bool        on;
    const char* trackOff;
    const char* trackOn;
  } switchDefs[] = {
    { "Default (off)", false, glint_demo_theme::surfaceHover, glint_demo_theme::primary   },
    { "Default (on)",  true,  glint_demo_theme::surfaceHover, glint_demo_theme::primary   },
    { "Green theme",   true,  glint_demo_theme::surface,      glint_demo_theme::successBg },
    { "Red theme",     false, glint_demo_theme::surface,      glint_demo_theme::dangerBg  },
    { "Slate theme",   true,  glint_demo_theme::surface,      glint_demo_theme::slate     },
  };

  for (const auto& sd : switchDefs)
  {
    glint_switch*  sw      = nullptr;
    glint_element* stateEl = nullptr;

    mContent->add.div([&, compactLayout](glint_component_style& row) {
      row.style.display       = "flex";
      row.style.flexDirection = compactLayout ? "column" : "row";
      row.style.alignItems    = compactLayout ? "stretch" : "center";
      row.style.gap           = 14.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 4.f;

      row.add.div([&, compactLayout](glint_component_style& lbl) {
        lbl.innerText       = sd.label;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 13.f;
        if (compactLayout) lbl.style.width = "100%";
        else lbl.style.width = 140.f;
        lbl.style.textAlign = EAlign::Near;
      });

      row.add.template fromClass<glint_switch>([&](glint_switch& ctrl) {
        ctrl.size     = 20.f;
        ctrl.trackOff = sd.trackOff;
        ctrl.trackOn  = sd.trackOn;
        ctrl.thumb    = glint_demo_theme::text;
        ctrl.SetToggleValue(sd.on, false);
      }, &sw);

      row.add.div([&](glint_component_style& state) {
        state.innerText      = sd.on ? "ON" : "OFF";
        state.style.color    = sd.on ? glint_demo_theme::success : glint_demo_theme::danger;
        state.style.fontSize = 11.f;
        if (compactLayout) state.style.width = "100%";
        else state.style.width = 32.f;
        state.style.textAlign = EAlign::Near;
      }, &stateEl);
    });

    sw->onChange = [stateEl](bool v) {
      stateEl->innerText   = v ? "ON" : "OFF";
      stateEl->style.color = v ? glint_demo_theme::success : glint_demo_theme::danger;
      stateEl->setDirty(false);
    };
  }

  addSpacer(16.f);

  // ── Size variants ────────────────────────────────────────────────────────
  addHeading("Size variants (size = 12 / 16 / 20 / 26)");

  const float szSizes[] = { 12.f, 16.f, 20.f, 26.f };

  mContent->add.div([&, compactLayout](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = compactLayout ? "column" : "row";
    row.style.alignItems    = compactLayout ? "flex-start" : "center";
    row.style.gap           = 20.f;
    row.style.width         = "100%";

    for (float sz : szSizes)
    {
      row.add.template fromClass<glint_switch>([sz](glint_switch& ctrl) {
        ctrl.size     = sz;
        ctrl.trackOn  = glint_demo_theme::primary;
        ctrl.trackOff = glint_demo_theme::surfaceHover;
        ctrl.thumb    = glint_demo_theme::text;
        ctrl.SetToggleValue(true, false);
      });
    }
  });
}

