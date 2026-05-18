#pragma once

inline void glint_demos_window::buildTooltips()
{
  addMobileUselessPageNote();

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

  // ── Basic tooltips ────────────────────────────────────────────────────────
  addHeading("Hover over each button to see its tooltip");

  auto* row1 = mContent->add.div([](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems    = "flex-end";
    row.style.gap           = 12.f;
    row.style.width         = "100%";
    row.style.height        = 60.f;
    row.style.paddingTop    = 20.f;
  });

  const struct { const char* btn; const char* tip; } basicTips[] = {
    { "Save",   "Save your changes (Ctrl+S)"         },
    { "Delete", "Permanently delete this item"       },
    { "Copy",   "Copy to clipboard (Ctrl+C)"         },
    { "Share",  "Share with others"                  },
  };

  for (const auto& bt : basicTips)
  {
    row1->add.fromClass<glint_tooltip>([&, bt](glint_tooltip& t) {
      t.text              = bt.tip;
      t.style.display     = "inline-flex";
      t.style.alignItems  = "center";
      t.style.justifyContent = "center";

      t.add.button([bt](glint_button& btn) {
        btn.innerText              = bt.btn;
        btn.style.height           = 32.f;
        btn.style.padding          = "0 14";
        btn.style.backgroundColor  = glint_demo_theme::surface;
        btn.style.color            = glint_demo_theme::text;
        btn.style.borderRadius     = 4.f;
        btn.style.borderWidth      = 1.f;
        btn.style.borderColor      = glint_demo_theme::border;
        btn.style.fontSize         = 13.f;
        btn.hover.backgroundColor  = glint_demo_theme::surfaceHover;
        btn.hover.borderColor      = glint_demo_theme::slate;
        btn.hover.borderWidth      = 1.f;
        btn.hover.borderRadius     = 4.f;
        btn.hover.color            = glint_demo_theme::heading;
        btn.pressed.backgroundColor = glint_demo_theme::primary;
        btn.pressed.borderColor    = glint_demo_theme::slate;
        btn.pressed.borderWidth    = 1.f;
        btn.pressed.borderRadius   = 4.f;
        btn.pressed.color          = glint_demo_theme::heading;
      });
    });
  }

  addSpacer(16.f);

  // ── Long tooltip ──────────────────────────────────────────────────────────
  addHeading("Long tooltip with wrapping");

  mContent->add.div([](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems    = "flex-end";
    row.style.width         = "100%";
    row.style.height        = 60.f;
    row.style.paddingTop    = 20.f;

    row.add.fromClass<glint_tooltip>([](glint_tooltip& t) {
      t.text      = "This is a longer tooltip that may wrap across multiple lines when it exceeds the maximum width.";
      t.style.display    = "inline-flex";
      t.style.alignItems = "center";

      t.add.button([](glint_button& btn) {
        btn.innerText             = "Hover for long tip";
        btn.style.height          = 32.f;
        btn.style.padding         = "0 14";
        btn.style.backgroundColor = glint_demo_theme::accent;
        btn.style.color           = glint_demo_theme::heading;
        btn.style.borderRadius    = 4.f;
        btn.style.fontSize        = 13.f;
        btn.hover.backgroundColor = glint_demo_theme::accentHover;
        btn.hover.color           = glint_demo_theme::heading;
        btn.hover.borderRadius    = 4.f;
        btn.pressed.backgroundColor = glint_demo_theme::accent;
        btn.pressed.color         = glint_demo_theme::heading;
        btn.pressed.borderRadius  = 4.f;
      });
    });
  });

  addSpacer(16.f);

  // ── Tooltip on a text label ───────────────────────────────────────────────
  addHeading("Tooltip on a text element");

  mContent->add.div([](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems    = "flex-end";
    row.style.width         = "100%";
    row.style.height        = 50.f;
    row.style.paddingTop    = 16.f;
    row.style.gap           = 20.f;

    const struct { const char* lbl; const char* tip; } items[] = {
      { "Username",  "Your unique display name"           },
      { "API Key",   "Keep this secret \xe2\x80\x94 never share it" },
      { "Timeout",   "Maximum wait in seconds"            },
    };

    for (const auto& item : items)
    {
      row.add.fromClass<glint_tooltip>([item](glint_tooltip& t) {
        t.text             = item.tip;
        t.style.display    = "inline-flex";
        t.style.alignItems = "center";
        t.style.cursor     = "help";

        t.add.div([item](glint_component_style& lbl) {
          lbl.innerText            = item.lbl;
          lbl.style.color          = glint_demo_theme::text;
          lbl.style.fontSize       = 13.f;
          lbl.style.borderBottom   = "1px dotted";
          lbl.style.borderColor    = glint_demo_theme::subtle;
          lbl.style.paddingBottom  = 2.f;
          lbl.style.cursor         = "help";
        });
      });
    }
  });
}
