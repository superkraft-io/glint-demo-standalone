#pragma once

inline void glint_demos_window::buildTransforms()
{
  addHeading("CSS transform  (style.transform)");

  mContent->add.div([](auto& sub) {
    sub.innerText          = "Visual-only \xe2\x80\x94 layout rect unchanged. Inspect each box to confirm mRECT stays fixed.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 10.f;
  });

  // Helper: adds one demo row — label left, preview box right.
  // 'transform' is applied to a child box; the wrapper shows the layout slot.
  auto addDemo = [this](const char* caption, const char* xformStr)
  {
    mContent->add.div([caption, xformStr](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 16.f;
      row.style.width         = "100%";
      row.style.height        = 120.f;
      row.style.marginBottom  = 12.f;

      // Description
      row.add.div([caption](auto& lbl) {
        lbl.innerText       = caption;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 12.f;
        lbl.style.width     = 200.f;
        lbl.style.textAlign = EAlign::Near;
        lbl.style.overflow  = "hidden";
      });

      // Preview zone — flex-fills remaining width and clips so transforms
      // can't bleed leftward onto the label column.
      row.add.div([xformStr](auto& zone) {
        zone.style.flexGrow       = 1.f;
        zone.style.height         = 120.f;
        zone.style.overflow       = "hidden";
        zone.style.display        = "flex";
        zone.style.alignItems     = "center";
        zone.style.justifyContent = "center";

        // Preview slot (dark, bordered = "layout rect")
        zone.add.div([xformStr](auto& slot) {
          slot.style.width           = 80.f;
          slot.style.height          = 80.f;
          slot.style.backgroundColor = glint_demo_theme::panelBg;
          slot.style.borderColor     = glint_demo_theme::border;
          slot.style.borderWidth     = 1.f;
          slot.style.borderRadius    = 4.f;
          slot.style.overflow        = "visible";

          // The transformed child box (fills the slot, then visually moves)
          slot.add.div([xformStr](auto& box) {
            box.style.width           = "100%";
            box.style.height          = "100%";
            box.style.backgroundColor = glint_demo_theme::primary;
            box.style.borderRadius    = 4.f;
            box.style.transform       = xformStr;
          });
        });
      });
    });
  };

  addDemo(
    "translateX(-30%)\n\nBox shifts left by 30 % of its own width",
    "translateX(-30%)"
  );

  addDemo(
    "translate(20px, -20px)\n\nBox shifts right 20 px / up 20 px",
    "translate(20px, -20px)"
  );

  addDemo(
    "scale(1.4)\n\nBox scales up 1.4\xc3\x97 around its center (layout slot unchanged)",
    "scale(1.4)"
  );

  addDemo(
    "scale(1.0, 0.5)\n\nVertical squash to 50 % height",
    "scale(1.0, 0.5)"
  );

  addDemo(
    "rotate(30deg)\n\nClockwise 30\xc2\xb0 around center\n(positive = clockwise, CSS Y-down)",
    "rotate(30deg)"
  );

  addDemo(
    "rotate(-45deg)\n\nCounter-clockwise 45\xc2\xb0",
    "rotate(-45deg)"
  );

  addDemo(
    "translate(10px, 0) rotate(20deg)\n\nLeft-to-right chaining: translate first, then rotate",
    "translate(10px, 0) rotate(20deg)"
  );

  addDemo(
    "scale(1.2) rotate(15deg)\n\nScale then rotate",
    "scale(1.2) rotate(15deg)"
  );

  addDemo(
    "matrix(1, 0, 0, 1, 20, -10)\n\nRaw 2\xc3\x97" "3 affine matrix\nmatrix(a,b,c,d,e,f)",
    "matrix(1, 0, 0, 1, 20, -10)"
  );

  addSpacer(4.f);

  mContent->add.div([](auto& sub) {
    sub.innerText          = "rotateZ \xe2\x80\x94 Z-axis rotation (same as rotate)";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 10.f;
  });

  addDemo(
    "rotateZ(30deg)\n\nSame as rotate(30deg) \xe2\x80\x94 rotation around the screen-normal Z axis",
    "rotateZ(30deg)"
  );

  addDemo(
    "rotateZ(-60deg)\n\nCounter-clockwise 60\xc2\xb0 around Z",
    "rotateZ(-60deg)"
  );

  addSpacer(4.f);
  mContent->add.div([](auto& sub) {
    sub.innerText          = "3-D axis rotations WITHOUT perspective \xe2\x80\x94 orthographic (no depth cue)";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 10.f;
  });

  addDemo(
    "rotateX(45deg)\n\nX-axis rotation, no perspective\n\xe2\x86\x92 pure 4x4 rotation matrix applied",
    "rotateX(45deg)"
  );

  addDemo(
    "rotateY(45deg)\n\nY-axis rotation, no perspective",
    "rotateY(45deg)"
  );

  addSpacer(4.f);
  mContent->add.div([](auto& sub) {
    sub.innerText          = "perspective(d) + 3-D rotations \xe2\x80\x94 true depth cue (Chrome 1:1)";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 10.f;
  });

  addDemo(
    "perspective(400px) rotateX(45deg)\n\nTop edge recedes, bottom comes forward\nNear edge appears wider \xe2\x80\x94 trapezoid shape",
    "perspective(400px) rotateX(45deg)"
  );

  addDemo(
    "perspective(400px) rotateX(-45deg)\n\nNegative angle: top comes forward",
    "perspective(400px) rotateX(-45deg)"
  );

  addDemo(
    "perspective(400px) rotateX(60deg)\n\nDeeper X tilt at d=400",
    "perspective(400px) rotateX(60deg)"
  );

  addDemo(
    "perspective(200px) rotateX(45deg)\n\nShort perspective (200px) \xe2\x80\x94 stronger distortion",
    "perspective(200px) rotateX(45deg)"
  );

  addDemo(
    "perspective(400px) rotateY(45deg)\n\nRight edge recedes, left comes forward",
    "perspective(400px) rotateY(45deg)"
  );

  addDemo(
    "perspective(400px) rotateY(-45deg)\n\nLeft edge recedes",
    "perspective(400px) rotateY(-45deg)"
  );

  addDemo(
    "perspective(400px) rotateY(60deg)\n\nDeep Y tilt at d=400",
    "perspective(400px) rotateY(60deg)"
  );

  addDemo(
    "perspective(400px) rotateZ(30deg)\n\nPerspective + Z spin (Z spin is unaffected by perspective)",
    "perspective(400px) rotateZ(30deg)"
  );

  addDemo(
    "perspective(400px) rotateX(30deg) rotateY(30deg)\n\nChained 3-D rotations with perspective",
    "perspective(400px) rotateX(30deg) rotateY(30deg)"
  );

  addDemo(
    "perspective(400px) rotateY(30deg) rotateX(30deg)\n\nOrder matters: Y first, then X",
    "perspective(400px) rotateY(30deg) rotateX(30deg)"
  );

  addDemo(
    "perspective(300px) rotate3d(1, 1, 0, 45deg)\n\nrotate3d(x,y,z,angle) \xe2\x80\x94 arbitrary axis",
    "perspective(300px) rotate3d(1, 1, 0, 45deg)"
  );

  addSpacer(8.f);

  mContent->add.div([](auto& note) {
    note.innerText      = "Tip: open 'Inspect Element' (Ctrl+Shift+I) and edit the transform property live on any box above.";
    note.style.color    = glint_demo_theme::subtle;
    note.style.fontSize = 11.f;
    note.style.width    = "100%";
    note.style.textAlign = EAlign::Near;
  });

  addSpacer(20.f);

  // ── CSS position: absolute / relative / z-index ───────────────────────────
  addHeading("CSS positioning  (position: relative / absolute, z-index)");

  mContent->add.div([](auto& sub) {
    sub.innerText          = "A position:relative container acts as the containing block. "
                             "position:absolute children are placed via top/right/bottom/left offsets. "
                             "z-index controls paint order among positioned siblings.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 10.f;
  });

  // Containing block  —  position: relative
  mContent->add.div([](auto& container) {
    container.style.position        = "relative";
    container.style.width           = "100%";
    container.style.height          = 180.f;
    container.style.backgroundColor = glint_demo_theme::panelBg;
    container.style.borderColor     = glint_demo_theme::border;
    container.style.borderWidth     = 1.f;
    container.style.borderRadius    = 8.f;
    container.style.marginBottom    = 12.f;

    // Centre label (in normal flow so it occupies the "background")
    container.add.div([](auto& label) {
      label.innerText       = "position: relative  (containing block)";
      label.style.color     = glint_demo_theme::subtle;
      label.style.fontSize  = 11.f;
      label.style.width     = "100%";
      label.style.textAlign = EAlign::Center;
      label.style.padding   = "8";
    });

    // Top-left pin
    container.add.div([](auto& pin) {
      pin.style.position        = "absolute";
      pin.style.top             = 10.f;
      pin.style.left            = 10.f;
      pin.style.width           = 110.f;
      pin.style.height          = 32.f;
      pin.style.backgroundColor = glint_demo_theme::accent;
      pin.style.borderRadius    = 4.f;
      pin.style.display         = "flex";
      pin.style.alignItems      = "center";
      pin.style.justifyContent  = "center";
      pin.style.zIndex          = 2;
      pin.add.div([](auto& lbl) {
        lbl.innerText      = "top:10 left:10";
        lbl.style.color    = "#ffffff";
        lbl.style.fontSize = 11.f;
      });
    });

    // Top-right pin
    container.add.div([](auto& pin) {
      pin.style.position        = "absolute";
      pin.style.top             = 10.f;
      pin.style.right           = 10.f;
      pin.style.width           = 110.f;
      pin.style.height          = 32.f;
      pin.style.backgroundColor = glint_demo_theme::success;
      pin.style.backgroundColor = glint_color(255, 60, 140, 100);
      pin.style.borderRadius    = 4.f;
      pin.style.display         = "flex";
      pin.style.alignItems      = "center";
      pin.style.justifyContent  = "center";
      pin.style.zIndex          = 2;
      pin.add.div([](auto& lbl) {
        lbl.innerText      = "top:10 right:10";
        lbl.style.color    = "#ffffff";
        lbl.style.fontSize = 11.f;
      });
    });

    // Bottom-left pin
    container.add.div([](auto& pin) {
      pin.style.position        = "absolute";
      pin.style.bottom          = 10.f;
      pin.style.left            = 10.f;
      pin.style.width           = 110.f;
      pin.style.height          = 32.f;
      pin.style.backgroundColor = glint_color(255, 160, 100, 50);
      pin.style.borderRadius    = 4.f;
      pin.style.display         = "flex";
      pin.style.alignItems      = "center";
      pin.style.justifyContent  = "center";
      pin.style.zIndex          = 2;
      pin.add.div([](auto& lbl) {
        lbl.innerText      = "bottom:10 left:10";
        lbl.style.color    = "#ffffff";
        lbl.style.fontSize = 11.f;
      });
    });

    // Bottom-right pin
    container.add.div([](auto& pin) {
      pin.style.position        = "absolute";
      pin.style.bottom          = 10.f;
      pin.style.right           = 10.f;
      pin.style.width           = 110.f;
      pin.style.height          = 32.f;
      pin.style.backgroundColor = glint_color(255, 150, 60, 150);
      pin.style.borderRadius    = 4.f;
      pin.style.display         = "flex";
      pin.style.alignItems      = "center";
      pin.style.justifyContent  = "center";
      pin.style.zIndex          = 2;
      pin.add.div([](auto& lbl) {
        lbl.innerText      = "bottom:10 right:10";
        lbl.style.color    = "#ffffff";
        lbl.style.fontSize = 11.f;
      });
    });

    // Centre absolute — sits behind the corner pins via z-index
    container.add.div([](auto& pin) {
      pin.style.position        = "absolute";
      pin.style.top             = "50%";
      pin.style.left            = "50%";
      pin.style.width           = 130.f;
      pin.style.height          = 36.f;
      pin.style.backgroundColor = glint_demo_theme::surface;
      pin.style.borderColor     = glint_demo_theme::border;
      pin.style.borderWidth     = 1.f;
      pin.style.borderRadius    = 6.f;
      pin.style.display         = "flex";
      pin.style.alignItems      = "center";
      pin.style.justifyContent  = "center";
      pin.style.zIndex          = 1;
      pin.add.div([](auto& lbl) {
        lbl.innerText      = "center  z-index:1";
        lbl.style.color    = glint_demo_theme::muted;
        lbl.style.fontSize = 11.f;
      });
    });
  });

  mContent->add.div([](auto& note) {
    note.innerText       = "Note: translate(-50%, -50%) is not applied here because the transform "
                           "public surface is currently the translate subset only. "
                           "The centre box is offset by its own width/height manually via top:50% left:50%.";
    note.style.color     = glint_demo_theme::subtle;
    note.style.fontSize  = 11.f;
    note.style.width     = "100%";
    note.style.textAlign = EAlign::Near;
  });
}
