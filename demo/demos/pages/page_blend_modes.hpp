#pragma once

inline void glint_demos_window::buildBlendModes()
{
  // ─────────────────────────────────────────────────────────────────────────
  // SECTION 1 — mix-blend-mode
  // ─────────────────────────────────────────────────────────────────────────
  addHeading("mix-blend-mode");

  mContent->add.div([](auto& sub) {
    sub.innerText       = "The coloured rectangle blends with the scene behind it using each CSS blend mode. "
                          "Bottom half of every card is unblended for direct comparison.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 14.f;
  });

  // All 16 CSS Compositing Level 1 blend modes
  struct BlendEntry { const char* mode; };
  const BlendEntry blendModes[] = {
    { "normal"      },
    { "multiply"    },
    { "screen"      },
    { "overlay"     },
    { "darken"      },
    { "lighten"     },
    { "color-dodge" },
    { "color-burn"  },
    { "hard-light"  },
    { "soft-light"  },
    { "difference"  },
    { "exclusion"   },
    { "hue"         },
    { "saturation"  },
    { "color"       },
    { "luminosity"  },
  };

  // Scene circles drawn behind the blend source in every card
  struct CircDef { float x, y, d; glint_color col; };
  const CircDef circs[] = {
    {  4.f,  4.f, 56.f, glint_color(255, 255, 110,  35) },   // orange
    { 52.f,  8.f, 44.f, glint_color(255,  35, 175, 255) },   // blue
    { 24.f, 36.f, 40.f, glint_color(255, 210,  40, 255) },   // magenta
    {  0.f, 66.f, 36.f, glint_color(255,  40, 220, 100) },   // green
    { 74.f, 48.f, 42.f, glint_color(255, 255, 210,  50) },   // yellow
  };
  const int kNumCircs = static_cast<int>(sizeof(circs) / sizeof(circs[0]));

  const float kCardH  = 120.f;
  const float kBlendH = 60.f;
  constexpr int kCols = 4;
  const int n         = static_cast<int>(sizeof(blendModes) / sizeof(blendModes[0]));

  for (int i = 0; i < n; ++i)
  {
    const int rowStart = i;
    const int rowEnd   = std::min(i + kCols, n);

    mContent->add.div([&, rowStart, rowEnd](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.gap           = 8.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 8.f;

      for (int j = rowStart; j < rowEnd; ++j)
      {
        row.add.div([&, j](auto& card) {
          card.style.flexGrow        = 1.f;
          card.style.height          = kCardH;
          card.style.borderRadius    = 8.f;
          card.style.overflow        = "hidden";
          card.style.position        = "relative";
          card.style.backgroundColor = glint_color(255, 14, 14, 14);

          // 1. Scene circles — drawn first, sit "behind" the blend overlay
          for (int ci = 0; ci < kNumCircs; ++ci)
          {
            card.add.div([&circs, ci](auto& circle) {
              const auto& c          = circs[ci];
              circle.style.position        = "absolute";
              circle.style.left            = c.x;
              circle.style.top             = c.y;
              circle.style.width           = c.d;
              circle.style.height          = c.d;
              circle.style.borderRadius    = 9999.f;
              circle.style.backgroundColor = c.col;
            });
          }

          // 2. Blend source element (top half)
          card.add.div([&blendModes, j, kBlendH](auto& blendEl) {
            blendEl.style.position     = "absolute";
            blendEl.style.top          = 0.f;
            blendEl.style.left         = 0.f;
            blendEl.style.width        = "100%";
            blendEl.style.height       = kBlendH;
            blendEl.style.background   = "url(/img/demo.png)";
            blendEl.style.mixBlendMode = blendModes[j].mode;
          });

          // 3. Thin divider between blended and unblended halves
          card.add.div([kBlendH](auto& divider) {
            divider.style.position        = "absolute";
            divider.style.top             = kBlendH;
            divider.style.left            = 0.f;
            divider.style.width           = "100%";
            divider.style.height          = 1.f;
            divider.style.backgroundColor = glint_color(90, 255, 255, 255);
          });

          // 4. Mode label at bottom
          card.add.div([&blendModes, j](auto& lbl) {
            lbl.innerText          = blendModes[j].mode;
            lbl.style.position     = "absolute";
            lbl.style.bottom       = 4.f;
            lbl.style.left         = 0.f;
            lbl.style.width        = "100%";
            lbl.style.height       = 14.f;
            lbl.style.fontSize     = 9.5f;
            lbl.style.textAlign    = EAlign::Center;
            lbl.style.color        = glint_color(200, 220, 220, 220);
          });
        });
      }
    });

    i = rowEnd - 1;
  }

  addSpacer(20.f);

  // ─────────────────────────────────────────────────────────────────────────
  // SECTION 2 — background-blend-mode
  // ─────────────────────────────────────────────────────────────────────────
  addHeading("background-blend-mode");

  mContent->add.div([](auto& sub2) {
    sub2.innerText       = "Each box has a linear-gradient background composited over a solid backgroundColor. "
                           "background-blend-mode controls how they mix.";
    sub2.style.color        = glint_demo_theme::muted;
    sub2.style.fontSize     = 12.f;
    sub2.style.width        = "100%";
    sub2.style.textAlign    = EAlign::Near;
    sub2.style.marginBottom = 14.f;
  });

  // Show a representative subset — all 16 would make the cards tiny
  struct BgBlendEntry { const char* mode; };
  const BgBlendEntry bgBlendModes[] = {
    { "normal"      },
    { "multiply"    },
    { "screen"      },
    { "overlay"     },
    { "color-dodge" },
    { "color-burn"  },
    { "difference"  },
    { "exclusion"   },
    { "hue"         },
    { "saturation"  },
    { "color"       },
    { "luminosity"  },
  };

  const int bN        = static_cast<int>(sizeof(bgBlendModes) / sizeof(bgBlendModes[0]));
  constexpr int bCols = 4;

  for (int i = 0; i < bN; ++i)
  {
    const int rowStart = i;
    const int rowEnd   = std::min(i + bCols, bN);

    mContent->add.div([&, rowStart, rowEnd](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.gap           = 8.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 8.f;

      for (int j = rowStart; j < rowEnd; ++j)
      {
        row.add.div([&bgBlendModes, j](auto& box) {
          box.style.flexGrow            = 1.f;
          box.style.height              = 80.f;
          box.style.borderRadius        = 8.f;
          box.style.position            = "relative";
          box.style.background          = "linear-gradient(135deg, #ff4400 0%, #0060ff 100%)";
          box.style.backgroundColor     = glint_color(255, 20, 180, 180);
          box.style.backgroundBlendMode = bgBlendModes[j].mode;

          box.add.div([&bgBlendModes, j](auto& lbl2) {
            lbl2.innerText          = bgBlendModes[j].mode;
            lbl2.style.position     = "absolute";
            lbl2.style.bottom       = 6.f;
            lbl2.style.left         = 0.f;
            lbl2.style.width        = "100%";
            lbl2.style.height       = 14.f;
            lbl2.style.fontSize     = 9.5f;
            lbl2.style.textAlign    = EAlign::Center;
            lbl2.style.color        = glint_color(240, 255, 255, 255);
          });
        });
      }
    });

    i = rowEnd - 1;
  }

  addSpacer(20.f);

  // ─────────────────────────────────────────────────────────────────────────
  // SECTION 3 — isolation: isolate
  // ─────────────────────────────────────────────────────────────────────────
  addHeading("isolation: isolate");

  mContent->add.div([](auto& sub3) {
    sub3.innerText       = "Without isolation the child blend mode composites against the background behind the parent. "
                           "With isolation:isolate the parent forms its own stacking context — children blend only "
                           "within the group and the result is then composited opaquely.";
    sub3.style.color        = glint_demo_theme::muted;
    sub3.style.fontSize     = 12.f;
    sub3.style.width        = "100%";
    sub3.style.textAlign    = EAlign::Near;
    sub3.style.marginBottom = 14.f;
  });

  struct IsoCirc { float x, y, d; glint_color col; };
  const IsoCirc isoCircs[] = {
    {  10.f,  10.f, 80.f,  glint_color(255, 255,  80,  30) },
    { 100.f,  20.f, 70.f,  glint_color(255,  30, 160, 255) },
    {  50.f,  65.f, 90.f,  glint_color(255, 200,  40, 255) },
    { 195.f,  10.f, 65.f,  glint_color(255,  40, 230, 110) },
    { 240.f,  70.f, 75.f,  glint_color(255, 255, 210,  50) },
  };
  const int kIsoCircs = static_cast<int>(sizeof(isoCircs) / sizeof(isoCircs[0]));

  auto buildIsoPanel = [&](auto& row, const char* label, bool isolate)
  {
    row.add.div([&isoCircs, kIsoCircs, label, isolate](auto& panel) {
      panel.style.flexGrow        = 1.f;
      panel.style.height          = 170.f;
      panel.style.borderRadius    = 8.f;
      panel.style.overflow        = "hidden";
      panel.style.position        = "relative";
      panel.style.backgroundColor = glint_color(255, 14, 14, 14);

      for (int ci = 0; ci < kIsoCircs; ++ci)
      {
        panel.add.div([&isoCircs, ci](auto& circle) {
          const auto& c          = isoCircs[ci];
          circle.style.position        = "absolute";
          circle.style.left            = c.x;
          circle.style.top             = c.y;
          circle.style.width           = c.d;
          circle.style.height          = c.d;
          circle.style.borderRadius    = 9999.f;
          circle.style.backgroundColor = c.col;
        });
      }

      panel.add.div([isolate](auto& group) {
        group.style.position        = "absolute";
        group.style.left            = 30.f;
        group.style.top             = 40.f;
        group.style.width           = 120.f;
        group.style.height          = 90.f;
        group.style.borderRadius    = 6.f;
        group.style.backgroundColor = glint_color(255, 255, 255, 255);
        if (isolate) group.style.isolation = "isolate";

        group.add.div([](auto& child) {
          child.style.position        = "absolute";
          child.style.left            = 10.f;
          child.style.top             = 15.f;
          child.style.width           = 100.f;
          child.style.height          = 60.f;
          child.style.borderRadius    = 4.f;
          child.style.backgroundColor = glint_color(255, 255, 100, 30);
          child.style.mixBlendMode    = "multiply";
        });
      });

      panel.add.div([label](auto& panelLbl) {
        panelLbl.innerText          = label;
        panelLbl.style.position     = "absolute";
        panelLbl.style.bottom       = 6.f;
        panelLbl.style.left         = 0.f;
        panelLbl.style.width        = "100%";
        panelLbl.style.height       = 16.f;
        panelLbl.style.fontSize     = 11.f;
        panelLbl.style.textAlign    = EAlign::Center;
        panelLbl.style.color        = glint_color(220, 255, 255, 255);
      });
    });
  };

  mContent->add.div([&](auto& isoRow) {
    isoRow.style.display       = "flex";
    isoRow.style.flexDirection = "row";
    isoRow.style.gap           = 12.f;
    isoRow.style.width         = "100%";
    isoRow.style.marginBottom  = 8.f;

    buildIsoPanel(isoRow, "isolation: auto  (child blends through parent into scene)", false);
    buildIsoPanel(isoRow, "isolation: isolate  (child blends only within white group)", true);
  });

  addSpacer(8.f);
}
