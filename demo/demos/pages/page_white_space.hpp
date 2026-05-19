#pragma once

inline void glint_demos_window::buildWhiteSpace()
{
  const bool compactLayout = isCompactLayout();

  mContent->add.div([](glint_component_style& _c) {
    _c.innerText = "white-space";
    _c.style.color = glint_demo_theme::heading;
    _c.style.fontSize = 15.f;
    _c.style.width = "100%";
    _c.style.textAlign = EAlign::Near;
    _c.style.marginBottom = 6.f;
  });

  mContent->add.div([](glint_component_style& _c) {
    _c.innerText = "Chrome behavior reference: normal/nowrap/pre-line/pre-wrap/break-spaces";
    _c.style.color = glint_demo_theme::text;
    _c.style.fontSize = 13.f;
    _c.style.width = "100%";
    _c.style.textAlign = EAlign::Near;
    _c.style.marginBottom = 8.f;
  });

  const std::string sample =
    "Date: 15    03    2026\n"
    "Row 2: A    B    C\n"
    "Wrap test: this    sentence    contains    repeated    spaces.";

  const struct DemoRow {
    const char* label;
    const char* mode;
  } rows[] = {
    { "normal", "normal" },
    { "nowrap", "nowrap" },
    { "pre-line", "pre-line" },
    { "pre-wrap", "pre-wrap" },
    { "break-spaces", "break-spaces" },
  };

  for (const auto& row : rows)
  {
    mContent->add.div([&](glint_component_style& card) {
      card.style.width = "100%";
      card.style.display = "flex";
      card.style.flexDirection = "column";
      card.style.gap = 6.f;
      card.style.padding = "10";
      card.style.marginBottom = 10.f;
      card.style.backgroundColor = glint_demo_theme::surface;
      card.style.borderRadius = "6px";

      card.add.div([&](glint_component_style& title) {
        title.innerText = std::string("white-space: ") + row.label;
        title.style.color = glint_demo_theme::heading;
        title.style.fontSize = 13.f;
        title.style.width = "100%";
        title.style.textAlign = EAlign::Near;
      });

      card.add.div([&, compactLayout](glint_component_style& viewport) {
        if (compactLayout) viewport.style.width = "100%";
        else viewport.style.width = 340.f;
        viewport.style.padding = "8";
        viewport.style.backgroundColor = glint_demo_theme::panelBg;
        viewport.style.borderColor = glint_demo_theme::border;
        viewport.style.borderWidth = "1px";
        viewport.style.borderRadius = "4px";

        viewport.add.div([&](glint_component_style& txt) {
          txt.innerText = sample;
          txt.style.width = "100%";
          txt.style.color = glint_demo_theme::text;
          txt.style.fontSize = 13.f;
          txt.style.textAlign = EAlign::Near;
          txt.style.whiteSpace = row.mode;
        });
      });
    });
  }
}
