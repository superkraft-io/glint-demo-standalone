#pragma once

inline void glint_demos_window::buildColors()
{
  // ── glint_colorpicker ──────────────────────────────────────────────────
  addHeading("Color Picker  (glint_colorpicker)");

  mContent->add.div([](auto& hdr) {
    hdr.innerText          = "Click the swatch row to expand the picker. Supports RGBA.";
    hdr.style.color        = glint_demo_theme::muted;
    hdr.style.fontSize     = 12.f;
    hdr.style.width        = "100%";
    hdr.style.textAlign    = EAlign::Near;
    hdr.style.marginBottom = 8.f;
  });

  // One picker example
  const struct { glint_color c; const char* label; } pickerDemos[] = {
    { glint_color(255, 96, 96, 96), "#606060  \xe2\x80\x94 neutral"    },
  };

  for (auto& pd : pickerDemos)
  {
    glint_colorpicker* pickerPtr = nullptr;
    mContent->add.div([pd, &pickerPtr](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "flex-start";
      row.style.gap           = 12.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 12.f;

      row.add.div([pd](auto& lbl) {
        lbl.innerText       = pd.label;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 12.f;
        lbl.style.width     = 140.f;
        lbl.style.textAlign = EAlign::Near;
      });

      row.add.template fromClass<glint_colorpicker>([pd](auto& p) {
        p.value       = pd.c;
        p.style.width = 220.f;
      }, &pickerPtr);
    });
    pickerPtr->onChange = [this](glint_color) { scheduleRedraw(); };
  }

  addSpacer(20.f);

  // ── glint_gradient_editor ──────────────────────────────────────────
  addHeading("Gradient Ramp Editor  (glint_gradient_editor)");

  mContent->add.div([](auto& hdr) {
    hdr.innerText          = "Click ramp to add stop \xc2\xb7 Click handle to select \xc2\xb7 Drag to move \xc2\xb7 Dbl-click to remove";
    hdr.style.color        = glint_demo_theme::muted;
    hdr.style.fontSize     = 12.f;
    hdr.style.width        = "100%";
    hdr.style.textAlign    = EAlign::Near;
    hdr.style.marginBottom = 8.f;
  });

  // Default black → white
  {
    mContent->add.div([](auto& lbl) {
      lbl.innerText          = "Black \xe2\x86\x92 White (default)";
      lbl.style.color        = glint_demo_theme::muted;
      lbl.style.fontSize     = 12.f;
      lbl.style.width        = "100%";
      lbl.style.textAlign    = EAlign::Near;
      lbl.style.marginBottom = 4.f;
    });

    mContent->add.gradientEditor([this](glint_gradient_editor& ge) {
      ge.stops  = {{ 0.f, glint_color(255,0,0,0) }, { 1.f, glint_color(255,255,255,255) }};
      ge.style.width = "100%";
      ge.style.marginBottom = 16.f;
      ge.onChange = [this](const auto&) { scheduleRedraw(); };
    });
  }

  // Multi-stop rainbow
  {
    mContent->add.div([](auto& lbl) {
      lbl.innerText          = "Multi-stop ramp";
      lbl.style.color        = glint_demo_theme::muted;
      lbl.style.fontSize     = 12.f;
      lbl.style.width        = "100%";
      lbl.style.textAlign    = EAlign::Near;
      lbl.style.marginBottom = 4.f;
    });

    mContent->add.gradientEditor([this](glint_gradient_editor& ge) {
      ge.stops = {
        { 0.f,    glint_color(255, 255,  60,  60) },
        { 0.33f,  glint_color(255,  60, 220,  80) },
        { 0.66f,  glint_color(255,  60, 140, 255) },
        { 1.f,    glint_color(255, 200,  60, 255) },
      };
      ge.style.width = "100%";
      ge.style.marginBottom = 16.f;
      ge.onChange = [this](const auto&) { scheduleRedraw(); };
    });
  }

  // With alpha
  {
    mContent->add.div([](auto& lbl) {
      lbl.innerText          = "Transparent \xe2\x86\x92 opaque  (alpha visible via checkerboard)";
      lbl.style.color        = glint_demo_theme::muted;
      lbl.style.fontSize     = 12.f;
      lbl.style.width        = "100%";
      lbl.style.textAlign    = EAlign::Near;
      lbl.style.marginBottom = 4.f;
    });

    mContent->add.gradientEditor([this](glint_gradient_editor& ge) {
      ge.stops = {
        { 0.f, glint_color(0,   100, 160, 255) },
        { 1.f, glint_color(255, 100, 160, 255) },
      };
      ge.style.width = "100%";
      ge.style.marginBottom = 16.f;
      ge.onChange = [this](const auto&) { scheduleRedraw(); };
    });
  }

  // Direction dial demo — starts at 90° (top→bottom)
  {
    mContent->add.div([](auto& lbl) {
      lbl.innerText          = "Direction dial  (drag the dial knob to change gradient angle)";
      lbl.style.color        = glint_demo_theme::muted;
      lbl.style.fontSize     = 12.f;
      lbl.style.width        = "100%";
      lbl.style.textAlign    = EAlign::Near;
      lbl.style.marginBottom = 4.f;
    });

    mContent->add.gradientEditor([this](glint_gradient_editor& ge) {
      ge.stops = {
        { 0.f,  glint_color(255, 255, 100, 60) },
        { 1.f,  glint_color(255,  60, 100, 255) },
      };
      ge.direction = 90.f;   // start top→bottom so the dial is visibly non-default
      ge.style.width = "100%";
      ge.style.marginBottom = 8.f;
      ge.onChange = [this](const auto&) { scheduleRedraw(); };
      ge.onDirectionChange = [this](float) { scheduleRedraw(); };
    });
  }
}
