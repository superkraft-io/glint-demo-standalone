#pragma once

inline void glint_demos_window::buildScroll()
{
  mContent->add.div([](glint_component_style& _c) {
    _c.innerText = "Overflow / Scroll";
    _c.style.color = glint_demo_theme::heading;
    _c.style.fontSize = 15.f;
    _c.style.width = "100%";
    _c.style.textAlign = EAlign::Near;
    _c.style.marginBottom = 10.f;
  });

  // overflowY: auto  — scrollbar appears when needed
  mContent->add.div([](glint_component_style& hdr) {
    hdr.innerText = "overflowY: auto  (200px box, 800px content)";
    hdr.style.color = glint_demo_theme::muted;
    hdr.style.fontSize = 13.f;
    hdr.style.width = "100%";
    hdr.style.textAlign = EAlign::Near;
    hdr.style.marginBottom = 6.f;
  });

  mContent->add.div([](glint_component_style& box) {
    box.style.width = "100%";
    box.style.height = 200.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius = 8.f;
    box.style.overflowY = "auto";
    box.style.marginBottom = 16.f;

    for (int i = 1; i <= 30; ++i)
    {
      box.add.div([i](glint_component_style& row) {
        row.style.width = "100%";
        row.style.height = 26.f;
        row.style.backgroundColor = (i % 2 == 0) ? glint_demo_theme::panelBg : glint_demo_theme::surfaceAlt;
        row.style.padding = "0 10";
        row.style.display = "flex";
        row.style.alignItems = "center";

        row.add.div([i](glint_component_style& txt) {
          txt.innerText = std::string("Scroll row ") + std::to_string(i);
          txt.style.color = (i % 5 == 0) ? glint_demo_theme::heading : glint_demo_theme::text;
          txt.style.fontSize = 12.f;
          txt.style.width = "100%";
          txt.style.textAlign = EAlign::Near;
        });
      });
    }
  });

  // overflowY: hidden — no scrollbar, content clipped
  mContent->add.div([](glint_component_style& hdr) {
    hdr.innerText = "overflow: hidden  (120px box, content clipped)";
    hdr.style.color = glint_demo_theme::muted;
    hdr.style.fontSize = 13.f;
    hdr.style.width = "100%";
    hdr.style.textAlign = EAlign::Near;
    hdr.style.marginBottom = 6.f;
  });

  mContent->add.div([](glint_component_style& box) {
    box.style.width = "100%";
    box.style.height = 120.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius = 8.f;
    box.style.overflow = "hidden";
    box.style.marginBottom = 16.f;

    for (int i = 1; i <= 10; ++i)
    {
      box.add.div([i](glint_component_style& lbl) {
        lbl.innerText = std::string("Hidden row ") + std::to_string(i);
        lbl.style.color = glint_demo_theme::text;
        lbl.style.fontSize = 12.f;
        lbl.style.height = 22.f;
        lbl.style.width = "100%";
        lbl.style.textAlign = EAlign::Near;
        lbl.style.padding = "0 10";
      });
    }
  });

  // Scrollbar style customisation
  mContent->add.div([](glint_component_style& hdr) {
    hdr.innerText = "Custom scrollbar colours";
    hdr.style.color = glint_demo_theme::muted;
    hdr.style.fontSize = 13.f;
    hdr.style.width = "100%";
    hdr.style.textAlign = EAlign::Near;
    hdr.style.marginBottom = 6.f;
  });

  mContent->add.div([](glint_component_style& box) {
    box.style.width = "100%";
    box.style.height = 130.f;
    box.style.backgroundColor = glint_demo_theme::surfaceAlt;
    box.style.borderRadius = 8.f;
    box.style.overflowY = "scroll";
    box.style.scrollbarWidth = 14.f;
    box.style.scrollbarTrackColor = glint_demo_theme::panelBg;
    box.style.scrollbarThumbColor = glint_demo_theme::primary;
    box.style.scrollbarButtonColor = glint_demo_theme::surfaceHover;

    for (int i = 1; i <= 20; ++i)
    {
      box.add.div([i](glint_component_style& lbl) {
        lbl.innerText = std::string("Custom scrollbar row ") + std::to_string(i);
        lbl.style.color = glint_demo_theme::text;
        lbl.style.fontSize = 12.f;
        lbl.style.height = 22.f;
        lbl.style.width = "100%";
        lbl.style.textAlign = EAlign::Near;
        lbl.style.padding = "0 10";
      });
    }
  });
}
