#pragma once

inline void glint_demos_window::buildRadios()
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

  // ── Basic radio group ─────────────────────────────────────────────────────
  addHeading("Basic radio group");

  auto group1 = std::make_shared<glint_radio_group>();
  group1->selected = "b";

  const struct { const char* val; const char* lbl; bool init; } opts1[] = {
    { "a", "Option A", false },
    { "b", "Option B", true  },
    { "c", "Option C", false },
  };

  for (const auto& o : opts1)
  {
    glint_radio* r = nullptr;
    mContent->add.fromClass<glint_radio>([&, o](glint_radio& radio) {
      radio.text    = o.lbl;
      radio.value   = o.val;
      radio.checked = o.init;
      radio.size    = 16.f;
      radio.group   = group1;
      radio.style.marginBottom = 6.f;
    }, &r);
    if (r) group1->register_member(r);
  }

  auto* fb1 = mContent->add.div([](glint_component_style& fb) {
    fb.innerText      = "Selected: b";
    fb.style.color    = glint_demo_theme::muted;
    fb.style.fontSize = 12.f;
    fb.style.width    = "100%";
    fb.style.textAlign = EAlign::Near;
  });

  glint_element* fb1Ptr = fb1;
  group1->onChange = [fb1Ptr](const std::string& v) {
    fb1Ptr->innerText = std::string("Selected: ") + v;
    fb1Ptr->setDirty(false);
  };

  addSpacer(16.f);

  // ── Colour themes ─────────────────────────────────────────────────────────
  addHeading("Colour themes");

  auto group2 = std::make_shared<glint_radio_group>();
  group2->selected = "primary";

  const struct {
    const char* val;
    const char* lbl;
    const char* color;
    bool init;
  } themes[] = {
    { "primary", "Primary (blue)",   glint_demo_theme::accent,     true  },
    { "success", "Success (green)",  glint_demo_theme::successBg,  false },
    { "danger",  "Danger (red)",     glint_demo_theme::dangerBg,   false },
    { "slate",   "Slate",            glint_demo_theme::slate,      false },
  };

  for (const auto& t : themes)
  {
    glint_radio* r = nullptr;
    mContent->add.fromClass<glint_radio>([&, t](glint_radio& radio) {
      radio.text       = t.lbl;
      radio.value      = t.val;
      radio.checked    = t.init;
      radio.size       = 16.f;
      radio.checkedCol = t.color;
      radio.group      = group2;
      radio.style.marginBottom = 6.f;
    }, &r);
    if (r) group2->register_member(r);
  }

  addSpacer(16.f);

  // ── Size variants ─────────────────────────────────────────────────────────
  addHeading("Size variants");

  auto group3 = std::make_shared<glint_radio_group>();
  group3->selected = "16";

  const float sizes[] = { 12.f, 16.f, 20.f, 26.f };

  auto* sizeRow = mContent->add.div([](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems    = "center";
    row.style.gap           = 24.f;
    row.style.width         = "100%";
  });

  for (float sz : sizes)
  {
    const std::string val = std::to_string(static_cast<int>(sz));
    glint_radio* r = nullptr;
    sizeRow->add.fromClass<glint_radio>([sz, val, &group3](glint_radio& radio) {
      radio.text    = std::to_string(static_cast<int>(sz)) + "px";
      radio.value   = val;
      radio.checked = (sz == 16.f);
      radio.size    = sz;
      radio.group   = group3;
    }, &r);
    if (r) group3->register_member(r);
  }
}
