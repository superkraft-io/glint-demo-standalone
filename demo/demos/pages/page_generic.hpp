#pragma once

inline void glint_demos_window::buildGeneric()
{
  addHeading("box-shadow");

  mContent->add.div([](glint_component_style& sub) {
    sub.innerText          = "Generic surface styling demos that do not fit the text or filter pages cleanly.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 12.f;
  });

  struct ShadowEntry { const char* label; const char* shadow; };
  const ShadowEntry shadows[] = {
    { "2px 4px 8px #00000099  (standard drop shadow)",     "2px 4px 8px #00000099"      },
    { "0 0 12px #4c8fbf88  (soft glow)",                   "0 0 12px #4c8fbf88"         },
    { "4px 4px 0px #ffffff33  (hard offset, no blur)",     "4px 4px 0px #ffffff33"      },
    { "-4px -4px 8px #000000aa  (negative / top-left)",    "-4px -4px 8px #000000aa"    },
    { "inset 0 2px 6px #00000088  (inset shadow)",         "inset 0 2px 6px #00000088"  },
  };

  for (const auto& se : shadows)
  {
    mContent->add.div([se](glint_component_style& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 20.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 14.f;

      row.add.div([se](glint_component_style& box) {
        box.style.width           = 68.f;
        box.style.height          = 42.f;
        box.style.backgroundColor = glint_demo_theme::surface;
        box.style.borderRadius    = 6.f;
        box.style.boxShadow       = se.shadow;
      });

      row.add.div([se](glint_component_style& lbl) {
        lbl.innerText       = se.label;
        lbl.style.color     = glint_demo_theme::muted;
        lbl.style.fontSize  = 12.f;
        lbl.style.textAlign = EAlign::Near;
      });
    });
  }
}