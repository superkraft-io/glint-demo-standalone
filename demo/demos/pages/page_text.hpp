#pragma once

inline void glint_demos_window::buildText()
{
  auto addHeading = [&](const char* text, float marginBottom = 8.f) {
    mContent->add.div([=](glint_component_style& d) {
      d.innerText          = text;
      d.style.color        = glint_demo_theme::heading;
      d.style.fontSize     = 15.f;
      d.style.width        = "100%";
      d.style.textAlign    = EAlign::Near;
      d.style.marginBottom = marginBottom;
    });
  };

  // ── Font sizes ──────────────────────────────────────────────────────────
  addHeading("Font sizes");

  const float sizes[] = { 10.f, 12.f, 14.f, 18.f, 24.f, 32.f };
  for (float sz : sizes)
  {
    mContent->add.div([sz](glint_component_style& d) {
      d.innerText       = std::string("Label — ") + std::to_string(static_cast<int>(sz)) + " px";
      d.style.color     = glint_demo_theme::text;
      d.style.fontSize  = sz;
      d.style.width     = "100%";
      d.style.textAlign = EAlign::Near;
    });
  }

  addSpacer(16.f);

  // ── Colours ─────────────────────────────────────────────────────────────
  addHeading("Text colours");

  const struct { const char* text; const char* color; } colorRows[] = {
    { "Primary text  #e6e6e6", glint_demo_theme::text    },
    { "Heading       #ffffff", glint_demo_theme::heading },
    { "Muted         #b8b8b8", glint_demo_theme::muted   },
    { "Subtle        #8a8a8a", glint_demo_theme::subtle  },
    { "Success       #7ec07e", glint_demo_theme::success },
    { "Warning       #d0b16f", glint_demo_theme::warning },
    { "Danger        #d07b7b", glint_demo_theme::danger  },
  };
  for (const auto& cr : colorRows)
  {
    mContent->add.div([cr](glint_component_style& d) {
      d.innerText       = cr.text;
      d.style.color     = cr.color;
      d.style.fontSize  = 13.f;
      d.style.width     = "100%";
      d.style.textAlign = EAlign::Near;
    });
  }

  addSpacer(16.f);

  // ── Font weight ─────────────────────────────────────────────────────────
  addHeading("Font weight");

  static const struct { int weight; } kWeightFaces[] = {
    { 100 },
    { 200 },
    { 300 },
    { 400 },
    { 500 },
    { 600 },
    { 700 },
    { 800 },
    { 900 },
  };
  for (const auto& wf : kWeightFaces)
  {
    mContent->add.div([wf](glint_component_style& d) {
      d.innerText            = std::string("Kanit weight ") + std::to_string(wf.weight);
      d.style.color          = glint_demo_theme::text;
      d.style.fontFamily     = "Kanit";
      d.style.fontSize       = 14.f;
      d.style.fontWeight     = wf.weight;
      d.style.width          = "100%";
      d.style.textAlign      = EAlign::Near;
      d.style.marginBottom   = 2.f;
    });
  }

  addSpacer(16.f);

  // ── Alignment ───────────────────────────────────────────────────────────
  addHeading("Text alignment");

  const struct { const char* text; EAlign align; } alignRows[] = {
    { "Near (left)",  EAlign::Near   },
    { "Center",       EAlign::Center },
    { "Far (right)",  EAlign::Far    },
  };
  for (const auto& ar : alignRows)
  {
    mContent->add.div([ar](glint_component_style& bg) {
      bg.align                    = "left middle";
      bg.style.width              = "100%";
      bg.style.height             = 28.f;
      bg.style.backgroundColor    = glint_demo_theme::surface;
      bg.style.borderRadius       = 4.f;
      bg.style.marginBottom       = 4.f;

      bg.add.div([ar](glint_component_style& lbl) {
        lbl.innerText       = ar.text;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 13.f;
        lbl.style.width     = "100%";
        lbl.style.textAlign = ar.align;
        lbl.style.padding   = "0 8";
      });
    });
  }

  addSpacer(16.f);

  // ── Vertical-align (inline) ───────────────────────────────────────────
  addHeading("Vertical-align — inline formatting");

  auto addInlineCase = [&](const char* title, const char* inlineValue) {
    mContent->add.div([=](glint_component_style& card) {
      card.style.width           = "100%";
      card.style.backgroundColor = "#1d1f22";
      card.style.borderRadius    = 6.f;
      card.style.padding         = "8";
      card.style.marginBottom    = 8.f;

      card.add.div([=](glint_component_style& heading) {
        heading.innerText          = title;
        heading.style.color        = glint_demo_theme::muted;
        heading.style.fontSize     = 12.f;
        heading.style.width        = "100%";
        heading.style.marginBottom = 6.f;
      });

      card.add.div([=](glint_component_style& line) {
        line.style.width           = "100%";
        line.style.backgroundColor = "#23262a";
        line.style.borderRadius    = 4.f;
        line.style.padding         = "8";

        line.add.div([=](glint_component_style& txtA) {
          txtA.style.display       = "inline";
          txtA.innerText           = "prefix";
          txtA.style.color         = glint_demo_theme::text;
          txtA.style.fontSize      = 14.f;
          txtA.style.marginRight   = 6.f;
          txtA.style.verticalAlign = inlineValue;
        });

        line.add.div([=](glint_component_style& sample) {
          sample.style.display         = "inline-block";
          sample.innerText             = "ALIGN";
          sample.style.color           = "#ffffff";
          sample.style.fontSize        = 24.f;
          sample.style.backgroundColor = "#4c647a";
          sample.style.padding         = "2 6";
          sample.style.verticalAlign   = inlineValue;
          sample.style.marginRight     = 6.f;
        });

        line.add.div([=](glint_component_style& box) {
          box.style.display         = "inline-block";
          box.style.width           = 32.f;
          box.style.height          = 22.f;
          box.style.backgroundColor = "#b06a3c";
          box.style.borderRadius    = 3.f;
          box.style.verticalAlign   = inlineValue;
          box.style.marginRight     = 6.f;
        });

        line.add.div([=](glint_component_style& txtB) {
          txtB.style.display       = "inline";
          txtB.innerText           = "suffix";
          txtB.style.color         = glint_demo_theme::text;
          txtB.style.fontSize      = 14.f;
          txtB.style.verticalAlign = inlineValue;
        });
      });
    });
  };

  addInlineCase("baseline",    "baseline");
  addInlineCase("middle",      "middle");
  addInlineCase("text-top",    "text-top");
  addInlineCase("text-bottom", "text-bottom");
  addInlineCase("sub",         "sub");
  addInlineCase("super",       "super");
  addInlineCase("12px",        "12px");
  addInlineCase("-25%",        "-25%");

  addSpacer(16.f);

  // ── Vertical-align (table cells) ───────────────────────────────────────
  addHeading("Vertical-align — table cells");

  mContent->add.div([](glint_component_style& table) {
    table.style.display         = "table";
    table.style.width           = "100%";
    table.style.backgroundColor = "#1d1f22";
    table.style.borderRadius    = 6.f;
    table.style.marginBottom    = 8.f;

    struct TableDemoRow { const char* label; const char* vAlign; };
    const TableDemoRow tableRows[] = {
      { "top",      "top"      },
      { "middle",   "middle"   },
      { "bottom",   "bottom"   },
      { "baseline", "baseline" },
    };

    for (const auto& rowInfo : tableRows)
    {
      table.add.div([rowInfo](glint_component_style& row) {
        row.style.display = "table-row";

        row.add.div([rowInfo](glint_component_style& name) {
          name.style.display         = "table-cell";
          name.style.width           = "120";
          name.style.padding         = "8";
          name.style.backgroundColor = "#23262a";
          name.style.verticalAlign   = rowInfo.vAlign;
          name.innerText             = rowInfo.label;
          name.style.color           = glint_demo_theme::muted;
          name.style.fontSize        = 13.f;
        });

        row.add.div([rowInfo](glint_component_style& sample) {
          sample.style.display         = "table-cell";
          sample.style.padding         = "8";
          sample.style.backgroundColor = "#2a2e33";
          sample.style.verticalAlign   = rowInfo.vAlign;

          sample.add.div([](glint_component_style& big) {
            big.style.display    = "inline";
            big.innerText        = "Big";
            big.style.fontSize   = 24.f;
            big.style.color      = "#ffffff";
            big.style.marginRight = 8.f;
          });

          sample.add.div([](glint_component_style& chip) {
            chip.style.display         = "inline-block";
            chip.style.width           = 28.f;
            chip.style.height          = 18.f;
            chip.style.backgroundColor = "#7a5a34";
            chip.style.marginRight     = 8.f;
          });

          sample.add.div([](glint_component_style& smallTxt) {
            smallTxt.style.display  = "inline";
            smallTxt.innerText      = "small text";
            smallTxt.style.fontSize = 13.f;
            smallTxt.style.color    = glint_demo_theme::text;
          });
        });
      });
    }
  });

  addSpacer(16.f);

  // ── text-decoration ───────────────────────────────────────────────────────
  addHeading("text-decoration");

  const struct { const char* label; const char* decoration; const char* color; } decRows[] = {
    { "underline",               "underline",              glint_demo_theme::text    },
    { "line-through",            "line-through",           glint_demo_theme::text    },
    { "underline line-through",  "underline line-through", glint_demo_theme::warning },
    { "none  (explicit reset)",  "none",                   glint_demo_theme::muted   },
  };
  for (const auto& dr : decRows)
  {
    mContent->add.div([dr](glint_component_style& d) {
      d.innerText            = std::string(dr.label) + "  \xe2\x80\x94  sample text with decoration applied";
      d.style.color          = dr.color;
      d.style.fontSize       = 14.f;
      d.style.width          = "100%";
      d.style.textAlign      = EAlign::Near;
      d.style.textDecoration = dr.decoration;
      d.style.marginBottom   = 6.f;
    });
  }
}

