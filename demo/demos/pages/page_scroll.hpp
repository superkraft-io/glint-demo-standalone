#pragma once

inline void glint_demos_window::buildScroll()
{
  // ── 1. Vertical scroll (overflowY: auto) ───────────────────────────────────
  addHeading("Vertical scroll  (overflowY: auto)");

  mContent->add.div([](glint_component_style& sub) {
    sub.innerText          = "200px box, 30 rows. Mousewheel scrolls vertically. Scrollbar appears automatically.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 6.f;
  });

  mContent->add.div([](glint_component_style& box) {
    box.style.width           = "100%";
    box.style.height          = 200.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius    = 8.f;
    box.style.overflowY       = "auto";
    box.style.marginBottom    = 20.f;

    for (int i = 1; i <= 30; ++i)
    {
      box.add.div([i](glint_component_style& row) {
        row.style.width           = "100%";
        row.style.height          = 26.f;
        row.style.backgroundColor = (i % 2 == 0) ? glint_demo_theme::panelBg : glint_demo_theme::surfaceAlt;
        row.style.padding         = "0 10";
        row.style.display         = "flex";
        row.style.alignItems      = "center";

        row.add.div([i](glint_component_style& txt) {
          txt.innerText       = std::string("Scroll row ") + std::to_string(i);
          txt.style.color     = (i % 5 == 0) ? glint_demo_theme::heading : glint_demo_theme::text;
          txt.style.fontSize  = 12.f;
          txt.style.width     = "100%";
          txt.style.textAlign = EAlign::Near;
        });
      });
    }
  });

  // ── 2. Horizontal scroll (overflowX: auto) ─────────────────────────────────
  addHeading("Horizontal scroll  (overflowX: auto)");

  mContent->add.div([](glint_component_style& sub) {
    sub.innerText          = "Fixed-height box, content wider than container. "
                             "Horizontal trackpad gesture or Shift+mousewheel scrolls left/right.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 6.f;
  });

  mContent->add.div([](glint_component_style& box) {
    box.style.width           = "100%";
    box.style.height          = 80.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius    = 8.f;
    box.style.overflowX       = "auto";
    box.style.overflowY       = "hidden";
    box.style.display         = "flex";
    box.style.flexDirection   = "row";
    box.style.alignItems      = "center";
    box.style.gap             = 8.f;
    box.style.padding         = "0 10";
    box.style.marginBottom    = 4.f;

    for (int i = 1; i <= 20; ++i)
    {
      box.add.div([i](glint_component_style& col) {
        col.style.width           = 110.f;
        col.style.height          = 56.f;
        col.style.backgroundColor = (i % 3 == 0) ? glint_demo_theme::primary
                                  : (i % 3 == 1) ? glint_demo_theme::surface
                                  :                glint_demo_theme::surfaceHover;
        col.style.borderRadius    = 6.f;
        col.style.display         = "flex";
        col.style.alignItems      = "center";
        col.style.justifyContent  = "center";

        col.add.div([i](glint_component_style& lbl) {
          lbl.innerText      = std::string("Col ") + std::to_string(i);
          lbl.style.color    = glint_demo_theme::text;
          lbl.style.fontSize = 12.f;
        });
      });
    }
  });

  mContent->add.div([](glint_component_style& note) {
    note.innerText          = "Horizontal wheel events (deltaX) are routed to overflowX containers automatically. "
                              "On a standard vertical-only mouse, hold Shift while scrolling.";
    note.style.color        = glint_demo_theme::subtle;
    note.style.fontSize     = 11.f;
    note.style.width        = "100%";
    note.style.textAlign    = EAlign::Near;
    note.style.marginBottom = 20.f;
  });

  // ── 3. Two-axis scroll (overflowX + overflowY both auto) ───────────────────
  addHeading("Two-axis scroll  (overflow: auto on both axes)");

  mContent->add.div([](glint_component_style& sub) {
    sub.innerText          = "Scrollbars appear on both axes when content exceeds the box in either direction.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 6.f;
  });

  mContent->add.div([](glint_component_style& box) {
    box.style.width           = "100%";
    box.style.height          = 180.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius    = 8.f;
    box.style.overflowX       = "auto";
    box.style.overflowY       = "auto";
    box.style.marginBottom    = 20.f;

    for (int row = 0; row < 12; ++row)
    {
      box.add.div([row](glint_component_style& r) {
        r.style.display       = "flex";
        r.style.flexDirection = "row";
        r.style.height        = 28.f;
        r.style.width         = 1200.f;   // wider than the box

        for (int col = 0; col < 10; ++col)
        {
          r.add.div([row, col](glint_component_style& cell) {
            cell.style.width           = 120.f;
            cell.style.height          = 28.f;
            cell.style.backgroundColor = ((row + col) % 2 == 0)
                                       ? glint_demo_theme::panelBg
                                       : glint_demo_theme::surfaceAlt;
            cell.style.padding         = "0 6";
            cell.style.display         = "flex";
            cell.style.alignItems      = "center";

            cell.add.div([row, col](glint_component_style& lbl) {
              lbl.innerText      = std::string("r") + std::to_string(row + 1)
                                 + " c" + std::to_string(col + 1);
              lbl.style.color    = glint_demo_theme::muted;
              lbl.style.fontSize = 11.f;
            });
          });
        }
      });
    }
  });

  // ── 4. DOM scroll properties (scrollTop / scrollLeft / scrollWidth / scrollHeight) ─
  addHeading("DOM scroll properties  (scrollTop, scrollLeft, scrollWidth, scrollHeight)");

  mContent->add.div([](glint_component_style& sub) {
    sub.innerText          = "Live readout via the 'scroll' event. Buttons show programmatic scrollTop / scrollLeft assignment.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 4.f;
  });

  glint_element* domBoxPtr    = nullptr;
  glint_element* domReadoutPtr = nullptr;

  mContent->add.div([](glint_component_style& lbl) {
    lbl.innerText          = "scrollTop=0   scrollLeft=0   scrollWidth=0   scrollHeight=0";
    lbl.style.color        = glint_demo_theme::subtle;
    lbl.style.fontSize     = 11.f;
    lbl.style.width        = "100%";
    lbl.style.textAlign    = EAlign::Near;
    lbl.style.marginBottom = 4.f;
  }, &domReadoutPtr);

  mContent->add.div([](glint_component_style& box) {
    box.style.width           = "100%";
    box.style.height          = 150.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius    = 8.f;
    box.style.overflowY       = "auto";
    box.style.overflowX       = "auto";
    box.style.marginBottom    = 8.f;

    for (int i = 1; i <= 20; ++i)
    {
      box.add.div([i](glint_component_style& row) {
        row.style.width           = 600.f;   // intentionally wider than the box
        row.style.height          = 26.f;
        row.style.backgroundColor = (i % 2 == 0) ? glint_demo_theme::panelBg : glint_demo_theme::surfaceAlt;
        row.style.padding         = "0 10";
        row.style.display         = "flex";
        row.style.alignItems      = "center";

        row.add.div([i](glint_component_style& lbl) {
          lbl.innerText      = std::string("Row ") + std::to_string(i) + "  (content is wider than the box to enable X scroll)";
          lbl.style.color    = glint_demo_theme::text;
          lbl.style.fontSize = 12.f;
        });
      });
    }
  }, &domBoxPtr);

  // Wire the 'scroll' event after pointer is valid
  domBoxPtr->element.addEventListener("scroll", [domBoxPtr, domReadoutPtr](glint_event&) {
    const float st = domBoxPtr->scrollTop;
    const float sl = domBoxPtr->scrollLeft;
    const float sw = domBoxPtr->element.scrollWidth;
    const float sh = domBoxPtr->element.scrollHeight;
    char buf[160];
    std::snprintf(buf, sizeof(buf),
      "scrollTop=%.0f   scrollLeft=%.0f   scrollWidth=%.0f   scrollHeight=%.0f",
      st, sl, sw, sh);
    domReadoutPtr->innerText = buf;
    domReadoutPtr->setDirty(false);
  });

  // Programmatic scroll button row
  auto* domBtnRow = mContent->add.div([](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 8.f;
    row.style.width         = "100%";
    row.style.marginBottom  = 20.f;
  });

  struct BtnDef { const char* label; float topVal; float leftVal; };
  const BtnDef btns[] = {
    { "Scroll to top",    0.f,    -1.f },
    { "Scroll to bottom", 9999.f, -1.f },
    { "Scroll left",     -1.f,    0.f  },
    { "Scroll right",    -1.f,    9999.f },
  };
  for (const auto& bd : btns)
  {
    domBtnRow->add.template fromClass<glint_button>([bd, domBoxPtr](glint_button& btn) {
      btn.innerText             = bd.label;
      btn.style.height          = 28.f;
      btn.style.padding         = "0 10";
      btn.style.borderRadius    = 4.f;
      btn.style.fontSize        = 12.f;
      btn.style.backgroundColor = glint_demo_theme::surface;
      btn.style.color           = glint_demo_theme::text;
      btn.style.borderColor     = glint_demo_theme::border;
      btn.style.borderWidth     = 1.f;
      btn.hover.backgroundColor = glint_demo_theme::surfaceHover;
      btn.hover.color           = glint_demo_theme::text;
      btn.hover.borderColor     = glint_demo_theme::border;
      btn.hover.borderWidth     = 1.f;
      btn.hover.borderRadius    = 4.f;
      const float topV  = bd.topVal;
      const float leftV = bd.leftVal;
      btn.onClick = [domBoxPtr, topV, leftV] {
        if (topV  >= 0.f) domBoxPtr->scrollTop  = topV;
        if (leftV >= 0.f) domBoxPtr->scrollLeft = leftV;
        domBoxPtr->setDirty(false);
      };
    });
  }

  // ── 5. overflow: hidden ────────────────────────────────────────────────────
  addHeading("overflow: hidden  (content clipped, no scrollbar)");

  mContent->add.div([](glint_component_style& box) {
    box.style.width           = "100%";
    box.style.height          = 120.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius    = 8.f;
    box.style.overflow        = "hidden";
    box.style.marginBottom    = 20.f;

    for (int i = 1; i <= 10; ++i)
    {
      box.add.div([i](glint_component_style& lbl) {
        lbl.innerText       = std::string("Hidden row ") + std::to_string(i);
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 12.f;
        lbl.style.height    = 22.f;
        lbl.style.width     = "100%";
        lbl.style.textAlign = EAlign::Near;
        lbl.style.padding   = "0 10";
      });
    }
  });

  // ── 6. Custom scrollbar colours ────────────────────────────────────────────
  addHeading("Custom scrollbar colours  (scrollbarWidth, scrollbarThumbColor, scrollbarTrackColor)");

  mContent->add.div([](glint_component_style& box) {
    box.style.width                = "100%";
    box.style.height               = 130.f;
    box.style.backgroundColor      = glint_demo_theme::surfaceAlt;
    box.style.borderRadius         = 8.f;
    box.style.overflowY            = "scroll";
    box.style.scrollbarWidth       = 14.f;
    box.style.scrollbarTrackColor  = glint_demo_theme::panelBg;
    box.style.scrollbarThumbColor  = glint_demo_theme::primary;
    box.style.scrollbarButtonColor = glint_demo_theme::surfaceHover;

    for (int i = 1; i <= 20; ++i)
    {
      box.add.div([i](glint_component_style& lbl) {
        lbl.innerText       = std::string("Custom scrollbar row ") + std::to_string(i);
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 12.f;
        lbl.style.height    = 22.f;
        lbl.style.width     = "100%";
        lbl.style.textAlign = EAlign::Near;
        lbl.style.padding   = "0 10";
      });
    }
  });
}
