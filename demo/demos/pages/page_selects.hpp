#pragma once

inline void glint_demos_window::buildSelects()
{
  const bool compactLayout = isCompactLayout();

  // ── Basic select ──────────────────────────────────────────────────────────
  mContent->add.div([](glint_component_style& heading) {
    heading.innerText = "Basic <select>";
    heading.style.color = glint_demo_theme::heading;
    heading.style.fontSize = 15.f;
    heading.style.width = "100%";
    heading.style.textAlign = EAlign::Near;
    heading.style.marginBottom = 6.f;
  });

  auto* sel1 = mContent->add.fromClass<glint_select>([compactLayout](glint_select& sel1) {
    sel1.options = { "Option A", "Option B", "Option C", "Option D" };
    sel1.selectedIndex = 0;
    if (compactLayout) sel1.style.width = "100%";
    else sel1.style.width = 220.f;
    sel1.style.height = 34.f;
    sel1.style.backgroundColor = glint_demo_theme::surface;
    sel1.style.color = glint_demo_theme::text;
    sel1.style.borderRadius = 4.f;
    sel1.style.borderWidth = 1.f;
    sel1.style.borderColor = glint_demo_theme::border;
    sel1.style.paddingLeft = 10.f;
    sel1.style.fontSize = 13.f;
  });

  mContent->add.div([](glint_component_style& spacer) {
    spacer.style.height = 6.f;
    spacer.style.width = "100%";
  });

  auto* feedback1 = mContent->add.div([](glint_component_style& feedback1) {
    feedback1.innerText = "Selected: Option A";
    feedback1.style.color = glint_demo_theme::muted;
    feedback1.style.fontSize = 12.f;
    feedback1.style.width = "100%";
    feedback1.style.textAlign = EAlign::Near;
  });

  glint_element* fb1Ptr = feedback1;
  sel1->onChange = [fb1Ptr](int /*idx*/, const std::string& val) {
    fb1Ptr->innerText = std::string("Selected: ") + val;
    fb1Ptr->setDirty(false);
  };

  mContent->add.div([](glint_component_style& spacer) {
    spacer.style.height = 20.f;
    spacer.style.width = "100%";
  });

  // ── With placeholder (no selection) ───────────────────────────────────────
  mContent->add.div([](glint_component_style& heading) {
    heading.innerText = "With placeholder (no initial selection)";
    heading.style.color = glint_demo_theme::heading;
    heading.style.fontSize = 15.f;
    heading.style.width = "100%";
    heading.style.textAlign = EAlign::Near;
    heading.style.marginBottom = 6.f;
  });

  auto* sel2 = mContent->add.fromClass<glint_select>([compactLayout](glint_select& sel2) {
    sel2.options = { "Mono", "Stereo", "Mid/Side" };
    sel2.selectedIndex = -1;
    sel2.placeholder = "Choose a mode\xe2\x80\xa6";
    if (compactLayout) sel2.style.width = "100%";
    else sel2.style.width = 220.f;
    sel2.style.height = 34.f;
    sel2.style.backgroundColor = glint_demo_theme::surface;
    sel2.style.color = glint_demo_theme::subtle;
    sel2.style.borderRadius = 4.f;
    sel2.style.borderWidth = 1.f;
    sel2.style.borderColor = glint_demo_theme::border;
    sel2.style.paddingLeft = 10.f;
    sel2.style.fontSize = 13.f;
  });

  mContent->add.div([](glint_component_style& spacer) {
    spacer.style.height = 6.f;
    spacer.style.width = "100%";
  });

  auto* feedback2 = mContent->add.div([](glint_component_style& feedback2) {
    feedback2.innerText = "Nothing chosen yet";
    feedback2.style.color = glint_demo_theme::muted;
    feedback2.style.fontSize = 12.f;
    feedback2.style.width = "100%";
    feedback2.style.textAlign = EAlign::Near;
  });

  glint_element* fb2Ptr = feedback2;
  auto* sel2Ptr = sel2;
  sel2->onChange = [fb2Ptr, sel2Ptr](int /*idx*/, const std::string& val) {
    fb2Ptr->innerText = std::string("Chosen: ") + val;
    fb2Ptr->setDirty(false);
    // Once something is selected, brighten the text colour.
    sel2Ptr->style.color = glint_demo_theme::text;
    sel2Ptr->setDirty(false);
  };

  mContent->add.div([](glint_component_style& spacer) {
    spacer.style.height = 20.f;
    spacer.style.width = "100%";
  });

  // ── Hover animation ───────────────────────────────────────────────────────
  mContent->add.div([](glint_component_style& heading) {
    heading.innerText = "With hover transition";
    heading.style.color = glint_demo_theme::heading;
    heading.style.fontSize = 15.f;
    heading.style.width = "100%";
    heading.style.textAlign = EAlign::Near;
    heading.style.marginBottom = 6.f;
  });

  mContent->add.fromClass<glint_select>([compactLayout](glint_select& sel3) {
    sel3.options = { "Attack", "Decay", "Sustain", "Release" };
    sel3.selectedIndex = 0;
    if (compactLayout) sel3.style.width = "100%";
    else sel3.style.width = 220.f;
    sel3.style.height = 34.f;
    sel3.style.backgroundColor = glint_demo_theme::panelBg;
    sel3.style.color = glint_demo_theme::text;
    sel3.style.borderRadius = 6.f;
    sel3.style.borderWidth = 1.f;
    sel3.style.borderColor = glint_demo_theme::border;
    sel3.style.paddingLeft = 10.f;
    sel3.style.fontSize = 13.f;
    sel3.style.transition = "background-color 120ms ease-out, border-color 120ms ease-out";
    sel3.hover.backgroundColor = glint_demo_theme::surface;
    sel3.hover.borderColor = glint_demo_theme::heading;
  });

  mContent->add.div([](glint_component_style& spacer) {
    spacer.style.height = 20.f;
    spacer.style.width = "100%";
  });

  // ── Multiple selects in a row ─────────────────────────────────────────────
  mContent->add.div([](glint_component_style& heading) {
    heading.innerText = "Multiple selects in a flex row";
    heading.style.color = glint_demo_theme::heading;
    heading.style.fontSize = 15.f;
    heading.style.width = "100%";
    heading.style.textAlign = EAlign::Near;
    heading.style.marginBottom = 6.f;
  });

  static const char* kNoteNames[] = { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" };
  static const char* kOctaves[]   = { "1","2","3","4","5","6","7","8" };

  auto makeSmall = [compactLayout](glint_element* parent, std::vector<std::string> opts, int sel) {
    return parent->add.fromClass<glint_select>([opts = std::move(opts), sel, compactLayout](glint_select& s) mutable {
      s.options = std::move(opts);
      s.selectedIndex = sel;
      if (compactLayout) s.style.width = "100%";
      else s.style.width = 90.f;
      s.style.height = 30.f;
      s.style.backgroundColor = glint_demo_theme::surface;
      s.style.color = glint_demo_theme::text;
      s.style.borderRadius = 4.f;
      s.style.borderWidth = 1.f;
      s.style.borderColor = glint_demo_theme::border;
      s.style.paddingLeft = 8.f;
      s.style.fontSize = 12.f;
      s.style.transition = "background-color 100ms ease-out";
      s.hover.backgroundColor = glint_demo_theme::surfaceHover;
    });
  };

  std::vector<std::string> noteOpts, octOpts;
  for (const auto* n : kNoteNames) noteOpts.push_back(n);
  for (const auto* o : kOctaves)   octOpts.push_back(o);

  auto* row = mContent->add.div([compactLayout](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = compactLayout ? "column" : "row";
    row.style.alignItems = compactLayout ? "stretch" : "center";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  makeSmall(row, noteOpts, 9);   // A
  makeSmall(row, octOpts,  3);   // 4
}
