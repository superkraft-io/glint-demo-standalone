#pragma once

inline void glint_demos_window::buildMasks()
{
  // â”€â”€ Intro â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("CSS mask  (style.mask)");

  mContent->add.div([](auto& sub) {
    sub.innerText =
      "style.mask accepts the same gradient / url() syntax as CSS mask-image. "
      "The alpha channel of the mask image controls visibility: white (alpha=255) = fully visible, "
      "black/transparent (alpha=0) = fully hidden. "
      "Each card has a solid-colour content box; the gradient is the mask applied on top.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 16.f;
  });

  // â”€â”€ Helper: labelled mask card â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  auto makeCard = [](auto& row, const char* label, glint_color bg, const char* maskStr,
                     float w = 150.f, float h = 100.f)
  {
    row.add.div([=](auto& wrap) {
      wrap.style.width = w;

      wrap.add.div([=](auto& lbl) {
        lbl.innerText          = label;
        lbl.style.color        = glint_demo_theme::text;
        lbl.style.fontSize     = 11.f;
        lbl.style.height       = 20.f;
        lbl.style.width        = "100%";
        lbl.style.textAlign    = EAlign::Center;
        lbl.style.marginBottom = 4.f;
      });

      wrap.add.div([=](auto& box) {
        box.style.width           = "100%";
        box.style.height          = h;
        box.style.borderRadius    = 6.f;
        box.style.backgroundColor = bg;
        box.style.mask            = maskStr;
      });
    });
  };

  // â”€â”€ Row 1: Linear-gradient masks â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "linear-gradient masks";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });
  mContent->add.div([&](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 14.f;
    row.style.width         = "100%";
    row.style.alignItems    = "flex-start";
    row.style.marginBottom  = 12.f;
    makeCard(row, "to top (default)",  glint_color(255, 98,  130, 255), "linear-gradient(to top, transparent 0%, white 100%)");
    makeCard(row, "to bottom",         glint_color(255, 250, 179, 135), "linear-gradient(to bottom, white 0%, transparent 100%)");
    makeCard(row, "to right",          glint_color(255, 166, 227, 161), "linear-gradient(to right, transparent 0%, white 100%)");
    makeCard(row, "to left",           glint_color(255, 243, 139, 168), "linear-gradient(to left, transparent 0%, white 100%)");
    makeCard(row, "centre fade",       glint_color(255, 137, 180, 250), "linear-gradient(to bottom, transparent 0%, white 30%, white 70%, transparent 100%)");
  });

  addSpacer(10.f);

  // â”€â”€ Row 2: Radial-gradient masks â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "radial-gradient masks";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });
  mContent->add.div([&](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 14.f;
    row.style.width         = "100%";
    row.style.alignItems    = "flex-start";
    row.style.marginBottom  = 12.f;
    makeCard(row, "spotlight",       glint_color(255, 203, 166, 247), "radial-gradient(circle, white 30%, transparent 80%)");
    makeCard(row, "vignette",        glint_color(255, 148, 226, 213), "radial-gradient(circle, white 40%, transparent 100%)");
    makeCard(row, "inverse vignette",glint_color(255, 250, 179, 135), "radial-gradient(circle, transparent 30%, white 80%)");
    makeCard(row, "ellipse H",       glint_color(255, 243, 139, 168), "radial-gradient(ellipse 80% 40% at center, white 50%, transparent 100%)");
  });

  addSpacer(10.f);

  // â”€â”€ Row 3: Diagonal & corner â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "diagonal & corner masks";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });
  mContent->add.div([&](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 14.f;
    row.style.width         = "100%";
    row.style.alignItems    = "flex-start";
    row.style.marginBottom  = 12.f;
    makeCard(row, "45Â° diagonal",      glint_color(255, 98,  130, 255), "linear-gradient(45deg, transparent 0%, white 100%)");
    makeCard(row, "135Â°",              glint_color(255, 250, 179, 135), "linear-gradient(135deg, transparent 0%, white 100%)");
    makeCard(row, "top-right corner",  glint_color(255, 166, 227, 161), "radial-gradient(circle at top right, white 0%, transparent 70%)");
    makeCard(row, "bottom-left corner",glint_color(255, 137, 180, 250), "radial-gradient(circle at bottom left, white 0%, transparent 70%)");
    makeCard(row, "soft sharp edge",   glint_color(255, 203, 166, 247), "linear-gradient(to right, white 60%, transparent 60%)");
  });

  addSpacer(10.f);

  // â”€â”€ Row 4: Partial alpha â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "partial alpha (semi-transparent mask stops)";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });
  mContent->add.div([&](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 14.f;
    row.style.width         = "100%";
    row.style.alignItems    = "flex-start";
    row.style.marginBottom  = 12.f;
    makeCard(row, "#ffffff47 â†’ white",   glint_color(255, 250, 179, 135), "linear-gradient(0deg, #ffffff47 0%, white 100%)");
    makeCard(row, "white â†’ #ffffff47",   glint_color(255, 243, 139, 168), "linear-gradient(0deg, white 0%, #ffffff47 100%)");
    makeCard(row, "50% solid â†’ fade",    glint_color(255, 98,  130, 255), "linear-gradient(to right, white 0%, white 50%, transparent 100%)");
    makeCard(row, "quarter alpha band",  glint_color(255, 148, 226, 213), "linear-gradient(to bottom, transparent 0%, #ffffff47 40%, #ffffff47 60%, transparent 100%)");
  });

  addSpacer(10.f);

  // â”€â”€ Row 5: mask applied to text content â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "mask applied to text content";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });
  auto makeTextCard = [](auto& row, const char* label, const char* text, glint_color color, const char* maskStr)
  {
    row.add.div([=](auto& wrap) {
      wrap.style.width = 160.f;

      wrap.add.div([=](auto& lbl) {
        lbl.innerText          = label;
        lbl.style.color        = glint_demo_theme::text;
        lbl.style.fontSize     = 11.f;
        lbl.style.height       = 20.f;
        lbl.style.width        = "100%";
        lbl.style.textAlign    = EAlign::Center;
        lbl.style.marginBottom = 4.f;
      });

      wrap.add.div([=](auto& box) {
        box.innerText          = text;
        box.style.width        = "100%";
        box.style.height       = 60.f;
        box.style.borderRadius = 6.f;
        box.style.color        = color;
        box.style.fontSize     = 22.f;
        box.style.textAlign    = EAlign::Center;
        box.style.mask         = maskStr;
      });
    });
  };

  mContent->add.div([&](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 14.f;
    row.style.width         = "100%";
    row.style.alignItems    = "flex-start";
    row.style.marginBottom  = 12.f;

    makeTextCard(row, "fade right", "Hello World", glint_color(255, 203, 166, 247), "linear-gradient(to right, white 0%, transparent 100%)");
    makeTextCard(row, "spotlight",  "Skia UI",     glint_color(255, 250, 179, 135), "radial-gradient(circle, white 30%, transparent 80%)");
    makeTextCard(row, "top fade",   "glint",       glint_color(255, 137, 180, 250), "linear-gradient(to bottom, transparent 0%, white 60%)");
  });
}

