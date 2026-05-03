#pragma once

inline void glint_demos_window::buildBackdropFilters()
{
  addHeading("CSS backdrop-filter  (style.backdropFilter)");

  mContent->add.div([](auto& sub) {
    sub.innerText       = "Applies a filter to content already drawn behind the element. Each card: top half is filtered by the glass, bottom half is unfiltered \xe2\x80\x94 compare side by side.";
    sub.style.color     = glint_demo_theme::muted;
    sub.style.fontSize  = 12.f;
    sub.style.width     = "100%";
    sub.style.textAlign = EAlign::Near;
    sub.style.marginBottom = 14.f;
  });

  // ── Swatch grid ──────────────────────────────────────────────────────────
  struct BdEntry { const char* filterStr; };
  const BdEntry entries[] = {
    { "blur(8px)"               },
    { "blur(16px)"              },
    { "brightness(0.3)"         },
    { "brightness(2.0)"         },
    { "grayscale(1)"            },
    { "sepia(1)"                },
    { "saturate(4.0)"           },
    { "hue-rotate(180deg)"      },
    { "blur(6px) contrast(1.6)" },
  };

  struct CircDef { float x, y, d; glint_color col; };
  const CircDef circs[] = {
    {  6.f,  4.f, 52.f, glint_color(255, 255, 110,  35) },
    { 56.f,  8.f, 40.f, glint_color(255,  35, 175, 255) },
    { 28.f, 34.f, 36.f, glint_color(255, 210,  40, 255) },
    {  0.f, 62.f, 32.f, glint_color(255,  40, 220, 100) },
    { 72.f, 44.f, 38.f, glint_color(255, 255, 210,  50) },
  };
  const int kNumCircs = static_cast<int>(sizeof(circs) / sizeof(circs[0]));

  const float kCardH  = 130.f;
  const float kGlassH = 65.f;
  constexpr int   kCols   = 3;
  const int n   = static_cast<int>(sizeof(entries) / sizeof(entries[0]));
  int       col = 0;

  for (int i = 0; i < n; ++i)
  {
    if (col == 0)
    {
      const int rowStart = i;
      const int rowEnd   = std::min(i + kCols, n);

      mContent->add.div([&, rowStart, rowEnd](auto& row) {
        row.style.display       = "flex";
        row.style.flexDirection = "row";
        row.style.gap           = 10.f;
        row.style.width         = "100%";
        row.style.marginBottom  = 10.f;

        for (int j = rowStart; j < rowEnd; ++j)
        {
          row.add.div([&, j](auto& card) {
            card.style.flexGrow        = 1.f;
            card.style.height          = kCardH;
            card.style.borderRadius    = 8.f;
            card.style.overflow        = "hidden";
            card.style.position        = "relative";
            card.style.backgroundColor = glint_color(255, 14, 14, 14);

            // 1. Coloured circles behind the glass
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

            // 2. "Aa" text behind the glass
            card.add.div([](auto& textLbl) {
              textLbl.innerText       = "Aa";
              textLbl.style.position  = "absolute";
              textLbl.style.left      = 0.f;
              textLbl.style.top       = 20.f;
              textLbl.style.width     = "100%";
              textLbl.style.height    = 50.f;
              textLbl.style.fontSize  = 38.f;
              textLbl.style.textAlign = EAlign::Center;
              textLbl.style.color     = glint_color(210, 255, 255, 255);
            });

            // 3. Glass overlay — top half only
            card.add.div([&entries, &kGlassH, j](auto& glass) {
              glass.style.position        = "absolute";
              glass.style.top             = 0.f;
              glass.style.left            = 0.f;
              glass.style.width           = "100%";
              glass.style.height          = kGlassH;
              glass.style.backdropFilter  = entries[j].filterStr;
              glass.style.backgroundColor = glint_color(15, 255, 255, 255);
            });

            // 4. Divider between halves
            card.add.div([&kGlassH](auto& divider) {
              divider.style.position        = "absolute";
              divider.style.top             = kGlassH;
              divider.style.left            = 0.f;
              divider.style.width           = "100%";
              divider.style.height          = 1.f;
              divider.style.backgroundColor = glint_color(90, 255, 255, 255);
            });

            // 5. Filter name label at bottom
            card.add.div([&entries, j](auto& filterLabel) {
              filterLabel.innerText       = entries[j].filterStr;
              filterLabel.style.position  = "absolute";
              filterLabel.style.bottom    = 5.f;
              filterLabel.style.left      = 0.f;
              filterLabel.style.width     = "100%";
              filterLabel.style.height    = 16.f;
              filterLabel.style.fontSize  = 10.f;
              filterLabel.style.textAlign = EAlign::Center;
              filterLabel.style.color     = glint_color(200, 210, 210, 210);
            });
          });
        }
      });

      i   = rowEnd - 1;
      col = 0;
      continue;
    }
  }

  addSpacer(16.f);

  addHeading("Animated backdrop-filter transition");

  mContent->add.div([](auto& sub2) {
    sub2.innerText       = "Hover anywhere inside the scene to animate blur(2px) \xe2\x86\x92 blur(14px) via style.transition.";
    sub2.style.color     = glint_demo_theme::muted;
    sub2.style.fontSize  = 12.f;
    sub2.style.width     = "100%";
    sub2.style.textAlign = EAlign::Near;
    sub2.style.marginBottom = 10.f;
  });

  // Background scene — circles + text drawn BEFORE the glass overlay.
  struct SceneCirc { float x, y, d; glint_color col; };
  const SceneCirc sCircs[] = {
    {  10.f,  10.f, 130.f, glint_color(255, 255,  80,  30) },
    { 160.f,  25.f,  90.f, glint_color(255,  30, 160, 255) },
    {  55.f,  80.f, 110.f, glint_color(255, 200,  40, 255) },
    { 280.f,  15.f,  85.f, glint_color(255,  40, 230, 110) },
    { 340.f,  85.f, 120.f, glint_color(255, 255, 210,  50) },
  };

  auto addSceneCircle = [](auto& scene, float x, float y, float d, glint_color col) {
    scene.add.div([=](auto& circle) {
      circle.style.position        = "absolute";
      circle.style.left            = x;
      circle.style.top             = y;
      circle.style.width           = d;
      circle.style.height          = d;
      circle.style.borderRadius    = 9999.f;
      circle.style.backgroundColor = col;
    });
  };

  // Out-ptrs at function scope — valid after mContent->add.div returns.
  glint_element* animGlassPtr = nullptr;
  glint_element* hintLblPtr   = nullptr;

  mContent->add.div([&](auto& scene) {
    scene.style.width           = "100%";
    scene.style.height          = 200.f;
    scene.style.borderRadius    = 10.f;
    scene.style.overflow        = "hidden";
    scene.style.position        = "relative";
    scene.style.backgroundColor = glint_color(255, 10, 10, 10);

    for (const auto& c : sCircs)
    {
      addSceneCircle(scene, c.x, c.y, c.d, c.col);
    }

    scene.add.div([](auto& sceneLbl) {
      sceneLbl.innerText       = "Content visible through the glass";
      sceneLbl.style.position  = "absolute";
      sceneLbl.style.left      = 0.f;
      sceneLbl.style.top       = 80.f;
      sceneLbl.style.width     = "100%";
      sceneLbl.style.height    = 40.f;
      sceneLbl.style.fontSize  = 22.f;
      sceneLbl.style.textAlign = EAlign::Center;
      sceneLbl.style.color     = glint_color(220, 255, 255, 255);
    });

    scene.add.div([&hintLblPtr](auto& ag) {
      ag.style.position        = "absolute";
      ag.style.top             = 0.f;
      ag.style.left            = 0.f;
      ag.style.width           = "100%";
      ag.style.height          = 200.f;
      ag.style.backdropFilter  = "blur(2px)";
      ag.style.backgroundColor = glint_color(30, 255, 255, 255);
      ag.style.transition      = "backdrop-filter 400ms ease-in-out";
      ag.style.display         = "flex";
      ag.style.alignItems      = "center";
      ag.style.justifyContent  = "center";

      ag.add.div([](auto& hint) {
        hint.innerText       = "Hover: blur(2px) \xe2\x86\x92 blur(14px)";
        hint.style.color     = glint_color(230, 255, 255, 255);
        hint.style.fontSize  = 14.f;
        hint.style.width     = "100%";
        hint.style.textAlign = EAlign::Center;
      }, &hintLblPtr);
    }, &animGlassPtr);
  });

  // Ops complete — animGlassPtr and hintLblPtr are valid.
  animGlassPtr->element.addEventListener("mouseenter", [animGlassPtr, hintLblPtr](glint_event&) {
    animGlassPtr->style.backdropFilter = "blur(14px)";
    hintLblPtr->innerText = "blur(14px) active \xe2\x80\x94 move out to reset";
    animGlassPtr->setDirty(false);
  });
  animGlassPtr->element.addEventListener("mouseleave", [animGlassPtr, hintLblPtr](glint_event&) {
    animGlassPtr->style.backdropFilter = "blur(2px)";
    hintLblPtr->innerText = "Hover: blur(2px) \xe2\x86\x92 blur(14px)";
    animGlassPtr->setDirty(false);
  });

  addSpacer(12.f);

  mContent->add.div([](auto& note) {
    note.innerText       = "Tip: open 'Inspect Element' (Ctrl+Shift+I) and add backdropFilter live to any element.";
    note.style.color     = glint_demo_theme::subtle;
    note.style.fontSize  = 11.f;
    note.style.width     = "100%";
    note.style.textAlign = EAlign::Near;
  });
}

