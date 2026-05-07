#pragma once

inline void glint_demos_window::buildTextarea()
{
  auto addHeading = [&](const char* text, float mb = 8.f) {
    mContent->add.div([=](glint_component_style& d) {
      d.innerText          = text;
      d.style.color        = glint_demo_theme::heading;
      d.style.fontSize     = 15.f;
      d.style.width        = "100%";
      d.style.textAlign    = EAlign::Near;
      d.style.marginBottom = mb;
    });
  };

  auto addSpacer = [&](float h) {
    mContent->add.div([=](glint_component_style& s) {
      s.style.height = h;
      s.style.width  = "100%";
    });
  };

  // ── Basic textarea ────────────────────────────────────────────────────────
  addHeading("Basic <textarea>");

  mContent->add.fromClass<glint_textarea>([](glint_textarea& ta) {
    ta.placeholder = "Enter multi-line text here\xe2\x80\xa6";
    ta.style.width           = "100%";
    ta.style.height          = 90.f;
    ta.style.backgroundColor = "#2d2d2d";
    ta.style.color           = glint_demo_theme::text;
    ta.style.borderWidth     = 1.f;
    ta.style.borderColor     = glint_demo_theme::border;
    ta.style.borderRadius    = 4.f;
    ta.style.padding         = 8.f;
    ta.style.fontSize        = 13.f;
  });

  addSpacer(16.f);

  // ── onChange callback feedback ────────────────────────────────────────────
  addHeading("onChange callback");

  auto* feedback = mContent->add.div([](glint_component_style& fb) {
    fb.innerText       = "Characters: 0  |  Lines: 1";
    fb.style.color     = glint_demo_theme::muted;
    fb.style.fontSize  = 12.f;
    fb.style.width     = "100%";
    fb.style.textAlign = EAlign::Near;
    fb.style.marginBottom = 6.f;
  });

  glint_element* fbPtr = feedback;
  mContent->add.fromClass<glint_textarea>([fbPtr](glint_textarea& ta) {
    ta.placeholder = "Type here to see stats update\xe2\x80\xa6";
    ta.style.width           = "100%";
    ta.style.height          = 80.f;
    ta.style.backgroundColor = "#2d2d2d";
    ta.style.color           = glint_demo_theme::text;
    ta.style.borderWidth     = 1.f;
    ta.style.borderColor     = glint_demo_theme::border;
    ta.style.borderRadius    = 4.f;
    ta.style.padding         = 8.f;
    ta.style.fontSize        = 13.f;
    ta.onChange = [fbPtr](const std::string& v) {
      int lines = 1;
      for (char c : v) if (c == '\n') ++lines;
      fbPtr->innerText = std::string("Characters: ") + std::to_string(v.size())
                       + "  |  Lines: " + std::to_string(lines);
      fbPtr->setDirty(false);
    };
  });

  addSpacer(16.f);

  // ── Read-only ─────────────────────────────────────────────────────────────
  addHeading("Read-only");

  auto* roTa = mContent->add.fromClass<glint_textarea>([](glint_textarea& ta) {
    ta.style.width           = "100%";
    ta.style.height          = 60.f;
    ta.style.backgroundColor = "#1e1e1e";
    ta.style.color           = glint_demo_theme::muted;
    ta.style.borderWidth     = 1.f;
    ta.style.borderColor     = glint_demo_theme::border;
    ta.style.borderRadius    = 4.f;
    ta.style.padding         = 8.f;
    ta.style.fontSize        = 13.f;
    ta.readonly              = true;
  });
  roTa->setValue("This is read-only content.\nYou can select and copy it,\nbut not edit it.");

  addSpacer(16.f);

  // ── Tall textarea ─────────────────────────────────────────────────────────
  addHeading("Tall textarea (scroll)");

  auto* bigTa = mContent->add.fromClass<glint_textarea>([](glint_textarea& ta) {
    ta.style.width           = "100%";
    ta.style.height          = 120.f;
    ta.style.backgroundColor = "#2d2d2d";
    ta.style.color           = glint_demo_theme::text;
    ta.style.borderWidth     = 1.f;
    ta.style.borderColor     = glint_demo_theme::border;
    ta.style.borderRadius    = 4.f;
    ta.style.padding         = 8.f;
    ta.style.fontSize        = 13.f;
  });
  bigTa->setValue(
    "Line 1: The quick brown fox jumps over the lazy dog.\n"
    "Line 2: Pack my box with five dozen liquor jugs.\n"
    "Line 3: How vexingly quick daft zebras jump!\n"
    "Line 4: The five boxing wizards jump quickly.\n"
    "Line 5: Sphinx of black quartz, judge my vow.\n"
    "Line 6: Waltz, bad nymph, for quick jigs vex.\n"
  );
}
