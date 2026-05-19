#pragma once

inline void glint_demos_window::buildFilters()
{
  const bool compactLayout = isCompactLayout();

  addHeading("CSS filter  (style.filter)");

  mContent->add.div([](auto& sub) {
    sub.innerText          = "Skia backend only \xe2\x80\x94 applied as a SkImageFilter save-layer. Inspect any swatch to edit live.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 14.f;
  });

  // ── Swatch grid ───────────────────────────────────────────────────────
  struct FilterEntry { const char* filterStr; const char* desc; };

  const FilterEntry entries[] = {
    { "blur(3px)",                           "Gaussian blur"           },
    { "blur(8px)",                           "Heavy blur"              },
    { "brightness(1.6)",                     "Brighter"                },
    { "brightness(0.4)",                     "Dimmer"                  },
    { "contrast(2.0)",                       "High contrast"           },
    { "saturate(3.0)",                       "Hyper-saturated"         },
    { "saturate(0)",                         "Desaturate"              },
    { "grayscale(1)",                        "Greyscale"               },
    { "sepia(1)",                            "Sepia tone"              },
    { "invert(1)",                           "Invert colours"          },
    { "opacity(0.3)",                        "Low opacity"             },
    { "hue-rotate(120deg)",                  "Hue-rotate 120\xc2\xb0"  },
    { "hue-rotate(240deg)",                  "Hue-rotate 240\xc2\xb0"  },
    { "drop-shadow(4px 4px 6px #000000)",    "Drop shadow"             },
    { "contrast(1.5) saturate(2.0)",         "Contrast + saturate"     },
    { "saturate(0) brightness(0.8)",         "Desaturate + dim"        },
    { "invert(0.5) hue-rotate(180deg)",      "Invert + hue-rotate"     },
    { "brightness(1.3) contrast(1.2) saturate(1.8)", "Vivid boost"     },
  };

  const int     kCols = compactLayout ? 1 : 3;
  const int     n     = static_cast<int>(sizeof(entries) / sizeof(entries[0]));
  int           col   = 0;

  for (int i = 0; i < n; ++i)
  {
    if (col == 0)
    {
      // Start a new row — the loop body below will immediately add cards to it.
      // We use a nested adder pattern: accumulate cards inside the row callback.
      // Because this is a regular for-loop we need to add all columns that belong
      // to this row inside one add.div call.
      const int rowStart = i;
      const int rowEnd   = std::min(i + kCols, n);

      mContent->add.div([&entries, rowStart, rowEnd, compactLayout](auto& row) {
        row.style.display       = "flex";
        row.style.flexDirection = compactLayout ? "column" : "row";
        row.style.alignItems    = "stretch";
        row.style.gap           = 10.f;
        row.style.width         = "100%";
        row.style.marginBottom  = 10.f;

        for (int j = rowStart; j < rowEnd; ++j)
        {
          row.add.div([j, &entries, compactLayout](auto& card) {
            card.style.flexGrow        = compactLayout ? 0.f : 1.f;
            if (compactLayout) card.style.width = "100%";
            else card.style.width = 0.f;
            card.style.height          = 138.f;
            card.style.display         = "flex";
            card.style.flexDirection   = "column";
            card.style.alignItems      = "center";
            card.style.gap             = 6.f;
            card.style.padding         = "8";
            card.style.backgroundColor = glint_demo_theme::panelBg;
            card.style.borderColor     = glint_demo_theme::border;
            card.style.borderWidth     = 1.f;
            card.style.borderRadius    = 6.f;

            // Gradient swatch with filter applied
            card.add.div([j, &entries](auto& swatch) {
              swatch.style.width           = "100%";
              swatch.style.height          = 76.f;
              swatch.style.borderRadius    = 4.f;
              swatch.style.filter          = entries[j].filterStr;
              swatch.style.backgroundGradient = {
                { 0.f,   glint_color(255, 220,  80,  80) },
                { 0.5f,  glint_color(255,  80, 140, 255) },
                { 1.f,   glint_color(255,  60, 200, 110) },
              };
              swatch.style.backgroundGradientAngle = 135.f;
            });

            // Filter string label
            card.add.div([j, &entries](auto& filterLbl) {
              filterLbl.innerText       = entries[j].filterStr;
              filterLbl.style.color     = glint_demo_theme::text;
              filterLbl.style.fontSize  = 10.f;
              filterLbl.style.height    = 24.f;
              filterLbl.style.width     = "100%";
              filterLbl.style.textAlign = EAlign::Center;
            });

            // Description label
            card.add.div([j, &entries](auto& descLbl) {
              descLbl.innerText       = entries[j].desc;
              descLbl.style.color     = glint_demo_theme::subtle;
              descLbl.style.fontSize  = 10.f;
              descLbl.style.height    = 14.f;
              descLbl.style.width     = "100%";
              descLbl.style.textAlign = EAlign::Center;
            });
          });
        }
      });

      // Advance i to the last column of this row; the loop increment will step past it.
      i = rowEnd - 1;
      col = 0;
      continue;
    }
    col = (col + 1) % kCols;
  }

  addSpacer(20.f);

  // ── Compound / text filter strip ──────────────────────────────────────
  addHeading("Compound filters on text + background");

  struct TextEntry { const char* filter; const char* label; };
  const TextEntry textEntries[] = {
    { "blur(2px)",                            "blur(2px) \xe2\x80\x94 text goes soft"                    },
    { "brightness(1.5) saturate(2.0)",        "brightness(1.5) saturate(2.0)"                  },
    { "invert(1) hue-rotate(90deg)",          "invert(1) hue-rotate(90deg)"                    },
    { "drop-shadow(2px 4px 8px #6c3891)",     "drop-shadow(2px 4px 8px #6c3891)"               },
    { "grayscale(1) brightness(1.3)",         "grayscale(1) brightness(1.3) \xe2\x80\x94 faded chrome"    },
  };

  for (const auto& te : textEntries)
  {
    mContent->add.div([te, compactLayout](auto& row2) {
      row2.style.display       = "flex";
      row2.style.flexDirection = compactLayout ? "column" : "row";
      row2.style.alignItems    = compactLayout ? "stretch" : "center";
      row2.style.gap           = 14.f;
      row2.style.width         = "100%";
      if (compactLayout) row2.style.height = "auto";
      else row2.style.height = 44.f;
      row2.style.marginBottom  = 8.f;

      // Code label (left, no filter)
      row2.add.div([te, compactLayout](auto& codeLbl) {
        codeLbl.innerText       = te.filter;
        codeLbl.style.color     = glint_demo_theme::muted;
        codeLbl.style.fontSize  = 10.f;
        if (compactLayout) codeLbl.style.width = "100%";
        else codeLbl.style.width = 240.f;
        codeLbl.style.textAlign = EAlign::Near;
      });

      // Filtered pill (right)
      row2.add.div([te, compactLayout](auto& pill) {
        pill.style.flexGrow        = compactLayout ? 0.f : 1.f;
        if (compactLayout) pill.style.width = "100%";
        pill.style.height          = 40.f;
        pill.style.backgroundColor = glint_demo_theme::surface;
        pill.style.borderRadius    = 6.f;
        pill.style.display         = "flex";
        pill.style.alignItems      = "center";
        pill.style.padding         = "0 12";
        pill.style.filter          = te.filter;

        pill.add.div([te](auto& inner) {
          inner.innerText       = te.label;
          inner.style.color     = glint_demo_theme::text;
          inner.style.fontSize  = 12.f;
          inner.style.width     = "100%";
          inner.style.textAlign = EAlign::Near;
        });
      });
    });
  }

  addSpacer(8.f);

  mContent->add.div([](auto& note) {
    note.innerText       = "Tip: open 'Inspect Element' (Ctrl+Shift+I) and edit the filter property live on any swatch.";
    note.style.color     = glint_demo_theme::subtle;
    note.style.fontSize  = 11.f;
    note.style.width     = "100%";
    note.style.textAlign = EAlign::Near;
  });
}

