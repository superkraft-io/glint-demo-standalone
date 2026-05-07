#pragma once

inline void glint_demos_window::buildShaders()
{
  addHeading("SkSL Runtime Shaders");

  mContent->add.div([](auto& sub) {
    sub.innerText = "Custom SkSL shaders applied via style.filter / style.backdropFilter using shader(id, name) tokens. Pre-set parameters via comp->shaders[\"id\"]->params[\"key\"] = value;";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 14.f;
  });

  // -- Procedural background shaders ----------------------------------------
  addHeading("Background \xe2\x80\x94 Procedural (style.filter)");

  // Out-ptrs set during the synchronous mContent->add.div call below.
  glint_element*  aurCardPtr    = nullptr;
  glint_checkbox* aurTogPtr     = nullptr;
  glint_element*  vigOverlayPtr = nullptr;
  glint_checkbox* vigTogPtr     = nullptr;

  mContent->add.div([&](auto& bgRow) {
    bgRow.style.display       = "flex";
    bgRow.style.flexDirection = "row";
    bgRow.style.gap           = 12.f;
    bgRow.style.width         = "100%";
    bgRow.style.height        = 180.f;
    bgRow.style.marginBottom  = 4.f;

    // -- Aurora --
    bgRow.add.div([&](auto& card) {
      card.style.flexGrow        = 1.f;
      card.style.height          = "100%";
      card.style.borderRadius    = 10.f;
      card.style.overflow        = "hidden";
      card.style.position        = "relative";
      card.style.backgroundColor = glint_color(0, 0, 0, 0);

      card.add.div([](auto& lbl) {
        lbl.innerText           = "aurora";
        lbl.style.color         = glint_color(200, 255, 255, 255);
        lbl.style.fontSize      = 10.f;
        lbl.style.position      = "absolute";
        lbl.style.left          = 8.f;
        lbl.style.bottom        = 6.f;
        lbl.style.pointerEvents = "none";
      });

      card.add.template fromClass<glint_checkbox>([](auto& tog) {
        tog.checked         = true;
        tog.size            = 14.f;
        tog.style.position  = "absolute";
        tog.style.left      = 6.f;
        tog.style.top       = 6.f;
      }, &aurTogPtr);
    }, &aurCardPtr);

    // -- Vignette (backdrop shader over a gradient card) --
    bgRow.add.div([&](auto& card) {
      card.style.flexGrow     = 1.f;
      card.style.height       = "100%";
      card.style.borderRadius = 10.f;
      card.style.overflow     = "hidden";
      card.style.position     = "relative";
      card.style.backgroundGradient = {
        {0.f,  glint_color(255,  20,  90, 200)},
        {0.5f, glint_color(255, 100,  25, 140)},
        {1.f,  glint_color(255, 190,  50,  70)},
      };
      card.style.backgroundGradientAngle = 135.f;

      card.add.div([](auto& vov) {
        vov.style.position        = "absolute";
        vov.style.left            = 0.f;
        vov.style.top             = 0.f;
        vov.style.width           = "100%";
        vov.style.height          = "100%";
        vov.style.pointerEvents   = "none";
      }, &vigOverlayPtr);

      card.add.template fromClass<glint_checkbox>([](auto& tog) {
        tog.checked        = true;
        tog.size           = 14.f;
        tog.style.position = "absolute";
        tog.style.left     = 6.f;
        tog.style.top      = 6.f;
      }, &vigTogPtr);

      card.add.div([](auto& lbl) {
        lbl.innerText           = "vignette";
        lbl.style.color         = glint_color(220, 255, 255, 255);
        lbl.style.fontSize      = 10.f;
        lbl.style.position      = "absolute";
        lbl.style.left          = 8.f;
        lbl.style.bottom        = 6.f;
        lbl.style.pointerEvents = "none";
      });
    });
  });

  // Set filter + shader params on real elements.
  aurCardPtr->style.filter = "shader(aur, aurora)";
  aurCardPtr->shaders["aur"]->params["speed"] = 1.2f;
  vigOverlayPtr->style.backdropFilter = "shader(vig, vignette)";
  vigOverlayPtr->shaders["vig"]->params["strength"] = 2.4f;

  // Set callbacks now that all pointers are valid.
  aurTogPtr->onChange = [aurCardPtr](bool on) {
    aurCardPtr->style.filter = on ? "shader(aur, aurora)" : "";
    aurCardPtr->setDirty(false);
  };
  vigTogPtr->onChange = [vigOverlayPtr](bool on) {
    vigOverlayPtr->style.backdropFilter = on ? "shader(vig, vignette)" : "";
    vigOverlayPtr->setDirty(false);
  };

  // Row sub-labels
  mContent->add.div([](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 12.f;
    row.style.width         = "100%";
    row.style.marginBottom  = 20.f;
    row.add.div([](auto& l) {
      l.innerText       = "style.filter = \"shader(aur, aurora)\"";
      l.style.flexGrow  = 1.f;
      l.style.color     = glint_demo_theme::subtle;
      l.style.fontSize  = 11.f;
      l.style.textAlign = EAlign::Center;
    });
    row.add.div([](auto& l) {
      l.innerText       = "style.backdropFilter = \"shader(vig, vignette)\"";
      l.style.flexGrow  = 1.f;
      l.style.color     = glint_demo_theme::subtle;
      l.style.fontSize  = 11.f;
      l.style.textAlign = EAlign::Center;
    });
  });

  // -- Backdrop shaders -----------------------------------------------------
  addHeading("Backdrop \xe2\x80\x94 Samples Canvas Below (style.backdropFilter)");

  mContent->add.div([](auto& note) {
    note.innerText       = "Circles are drawn first (behind the glass). The backdrop shader captures those pixels and transforms them. Top half: shader active. Bottom half: unaffected.";
    note.style.color     = glint_demo_theme::muted;
    note.style.fontSize  = 11.f;
    note.style.width     = "100%";
    note.style.textAlign = EAlign::Near;
    note.style.marginBottom = 10.f;
  });

  // Helper: add coloured circles + "Aa" text to a card builder context.
  auto addCircle = [](auto& card, float l, float t, float s, glint_color col) {
    card.add.div([=](auto& ci) {
      ci.style.position        = "absolute";
      ci.style.left            = l;
      ci.style.top             = t;
      ci.style.width           = s;
      ci.style.height          = s;
      ci.style.borderRadius    = 9999.f;
      ci.style.backgroundColor = col;
      ci.style.pointerEvents   = "none";
    });
  };
  auto addScene = [&addCircle](auto& card) {
    addCircle(card,   8.f,  55.f, 64.f, glint_color(255, 255, 110,  50));
    addCircle(card,  58.f,  32.f, 52.f, glint_color(255,  60, 140, 255));
    addCircle(card, 110.f,  70.f, 56.f, glint_color(255, 210,  50, 200));
    addCircle(card, 155.f,  24.f, 46.f, glint_color(255,  50, 210, 110));
    card.add.div([](auto& aa) {
      aa.innerText           = "Aa";
      aa.style.color         = glint_color(180, 255, 255, 255);
      aa.style.fontSize      = 28.f;
      aa.style.position      = "absolute";
      aa.style.left          = 80.f;
      aa.style.top           = 48.f;
      aa.style.pointerEvents = "none";
    });
  };

  const float kHalf = 80.f;

  glint_element*  glassPtr  = nullptr;
  glint_checkbox* lgTogPtr  = nullptr;
  glint_element*  caPtr     = nullptr;
  glint_checkbox* caTogPtr  = nullptr;
  glint_element*  ripPtr    = nullptr;
  glint_checkbox* ripTogPtr = nullptr;

  mContent->add.div([&](auto& bdRow) {
    bdRow.style.display       = "flex";
    bdRow.style.flexDirection = "row";
    bdRow.style.gap           = 12.f;
    bdRow.style.width         = "100%";
    bdRow.style.height        = 160.f;
    bdRow.style.marginBottom  = 4.f;

    // -- Liquid Glass --
    bdRow.add.div([&](auto& card) {
      card.style.flexGrow        = 1.f;
      card.style.height          = "100%";
      card.style.borderRadius    = 10.f;
      card.style.overflow        = "hidden";
      card.style.position        = "relative";
      card.style.backgroundColor = glint_color(255, 14, 14, 16);

      addScene(card);

      card.add.div([kHalf](auto& glass) {
        glass.style.position        = "absolute";
        glass.style.left            = 0.f;
        glass.style.top             = 0.f;
        glass.style.width           = "100%";
        glass.style.height          = kHalf;
        glass.style.backgroundColor = glint_color(25, 255, 255, 255);
      }, &glassPtr);

      card.add.template fromClass<glint_checkbox>([](auto& tog) {
        tog.checked        = true;
        tog.size           = 14.f;
        tog.style.position = "absolute";
        tog.style.left     = 6.f;
        tog.style.top      = 6.f;
      }, &lgTogPtr);

      card.add.div([kHalf](auto& divider) {
        divider.style.position        = "absolute";
        divider.style.left            = 0.f;
        divider.style.top             = kHalf;
        divider.style.width           = "100%";
        divider.style.height          = 1.f;
        divider.style.backgroundColor = glint_color(60, 255, 255, 255);
        divider.style.pointerEvents   = "none";
      });
    });

    // -- Chromatic Aberration --
    bdRow.add.div([&](auto& card) {
      card.style.flexGrow        = 1.f;
      card.style.height          = "100%";
      card.style.borderRadius    = 10.f;
      card.style.overflow        = "hidden";
      card.style.position        = "relative";
      card.style.backgroundColor = glint_color(255, 14, 14, 16);

      addScene(card);

      card.add.div([kHalf](auto& ca) {
        ca.style.position        = "absolute";
        ca.style.left            = 0.f;
        ca.style.top             = 0.f;
        ca.style.width           = "100%";
        ca.style.height          = kHalf;
        ca.style.backgroundColor = glint_color(0, 0, 0, 0);
      }, &caPtr);

      card.add.template fromClass<glint_checkbox>([](auto& tog) {
        tog.checked        = true;
        tog.size           = 14.f;
        tog.style.position = "absolute";
        tog.style.left     = 6.f;
        tog.style.top      = 6.f;
      }, &caTogPtr);

      card.add.div([kHalf](auto& divider) {
        divider.style.position        = "absolute";
        divider.style.left            = 0.f;
        divider.style.top             = kHalf;
        divider.style.width           = "100%";
        divider.style.height          = 1.f;
        divider.style.backgroundColor = glint_color(60, 255, 255, 255);
        divider.style.pointerEvents   = "none";
      });
    });

    // -- Ripple --
    bdRow.add.div([&](auto& card) {
      card.style.flexGrow        = 1.f;
      card.style.height          = "100%";
      card.style.borderRadius    = 10.f;
      card.style.overflow        = "hidden";
      card.style.position        = "relative";
      card.style.backgroundColor = glint_color(255, 14, 14, 16);

      addScene(card);

      card.add.div([](auto& rip) {
        rip.style.position        = "absolute";
        rip.style.left            = 0.f;
        rip.style.top             = 0.f;
        rip.style.width           = "100%";
        rip.style.height          = "100%";
        rip.style.backgroundColor = glint_color(0, 0, 0, 0);
      }, &ripPtr);

      card.add.template fromClass<glint_checkbox>([](auto& tog) {
        tog.checked        = true;
        tog.size           = 14.f;
        tog.style.position = "absolute";
        tog.style.left     = 6.f;
        tog.style.top      = 6.f;
      }, &ripTogPtr);

      card.add.div([](auto& hint) {
        hint.innerText           = "click to trigger";
        hint.style.color         = glint_color(130, 255, 255, 255);
        hint.style.fontSize      = 10.f;
        hint.style.position      = "absolute";
        hint.style.width         = "100%";
        hint.style.height        = 14.f;
        hint.style.bottom        = 6.f;
        hint.style.textAlign     = EAlign::Center;
        hint.style.pointerEvents = "none";
      });
    });
  });

  glassPtr->style.backdropFilter = "shader(gl, liquid_glass)";
  glassPtr->shaders["gl"]->params["strength"] = 15.f;
  caPtr->style.backdropFilter = "shader(ca, chromatic_aberration)";
  caPtr->shaders["ca"]->params["strength"] = 28.f;
  ripPtr->style.backdropFilter = "shader(rip, ripple)";
  ripPtr->shaders["rip"]->params["amplitude"]  = 14.f;
  ripPtr->shaders["rip"]->params["wavelength"] = 34.f;

  lgTogPtr->onChange = [glassPtr](bool on) {
    glassPtr->style.backdropFilter = on ? "shader(gl, liquid_glass)" : "";
    glassPtr->setDirty(false);
  };
  caTogPtr->onChange = [caPtr](bool on) {
    caPtr->style.backdropFilter = on ? "shader(ca, chromatic_aberration)" : "";
    caPtr->setDirty(false);
  };
  ripTogPtr->onChange = [ripPtr](bool on) {
    ripPtr->style.backdropFilter = on ? "shader(rip, ripple)" : "";
    ripPtr->setDirty(false);
  };

  // Row sub-labels
  mContent->add.div([](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 12.f;
    row.style.width         = "100%";
    row.style.marginBottom  = 16.f;
    row.add.div([](auto& l) {
      l.innerText       = "Liquid Glass  (liquid_glass)";
      l.style.flexGrow  = 1.f;
      l.style.color     = glint_demo_theme::subtle;
      l.style.fontSize  = 11.f;
      l.style.textAlign = EAlign::Center;
    });
    row.add.div([](auto& l) {
      l.innerText       = "Chromatic Aberration  (chromatic_aberration)";
      l.style.flexGrow  = 1.f;
      l.style.color     = glint_demo_theme::subtle;
      l.style.fontSize  = 11.f;
      l.style.textAlign = EAlign::Center;
    });
    row.add.div([](auto& l) {
      l.innerText       = "Ripple  (ripple)  \xe2\x80\x94 click to trigger";
      l.style.flexGrow  = 1.f;
      l.style.color     = glint_demo_theme::subtle;
      l.style.fontSize  = 11.f;
      l.style.textAlign = EAlign::Center;
    });
  });

  // -- Lens Refraction (draggable) ------------------------------------------
  addSpacer(20.f);
  addHeading("Lens Refraction  \xe2\x80\x94  Draggable  (lens_refraction)");

  mContent->add.div([](auto& desc) {
    desc.innerText       = "IQ rounded-rect glass lens. SDF dome + Snell's law refraction samples the backdrop at a displaced coordinate. Drag the lens to move it.";
    desc.style.color     = glint_demo_theme::muted;
    desc.style.fontSize  = 11.f;
    desc.style.width     = "100%";
    desc.style.textAlign = EAlign::Near;
    desc.style.marginBottom = 10.f;
  });

  struct LensCirc { float l, t, s; glint_color col; };
  static const LensCirc kLensCircles[] = {
    {  10.f,  20.f, 90.f, glint_color(255, 255,  80,  40) },
    {  80.f, 140.f, 70.f, glint_color(255,  40, 150, 255) },
    { 170.f,  30.f, 80.f, glint_color(255, 210,  50, 210) },
    { 260.f, 150.f, 60.f, glint_color(255,  50, 210, 110) },
    { 340.f,  40.f, 75.f, glint_color(255, 255, 180,  30) },
    { 430.f, 130.f, 65.f, glint_color(255,  90,  50, 240) },
    { 510.f,  50.f, 55.f, glint_color(255, 255,  60, 120) },
  };

  glint_element*  outerPtr   = nullptr;
  glint_element*  lensPtr    = nullptr;
  glint_checkbox* lensTogPtr = nullptr;

  mContent->add.div([&](auto& outer) {
    outer.style.width        = "100%";
    outer.style.height       = 300.f;
    outer.style.borderRadius = 10.f;
    outer.style.overflow     = "hidden";
    outer.style.position     = "relative";
    outer.style.marginBottom = 6.f;
    outer.style.backgroundGradient = {
      {0.f,  glint_color(255,  10,  12,  30)},
      {0.5f, glint_color(255,  20,  40,  80)},
      {1.f,  glint_color(255,  10,  20,  50)},
    };
    outer.style.backgroundGradientAngle = 135.f;

    for (const auto& c : kLensCircles) {
      addCircle(outer, c.l, c.t, c.s, c.col);
    }

    outer.add.div([](auto& lbl) {
      lbl.innerText           = "LENS";
      lbl.style.position      = "absolute";
      lbl.style.left          = 130.f;
      lbl.style.top           = 200.f;
      lbl.style.fontSize      = 40.f;
      lbl.style.color         = glint_color(180, 255, 255, 255);
      lbl.style.pointerEvents = "none";
    });
    outer.add.div([](auto& lbl) {
      lbl.innerText           = "REFRACT";
      lbl.style.position      = "absolute";
      lbl.style.left          = 310.f;
      lbl.style.top           = 80.f;
      lbl.style.fontSize      = 30.f;
      lbl.style.color         = glint_color(140, 200, 220, 255);
      lbl.style.pointerEvents = "none";
    });
    outer.add.div([](auto& lbl) {
      lbl.innerText           = "glint";
      lbl.style.position      = "absolute";
      lbl.style.left          = 30.f;
      lbl.style.top           = 240.f;
      lbl.style.fontSize      = 18.f;
      lbl.style.color         = glint_color(120, 255, 255, 255);
      lbl.style.pointerEvents = "none";
    });

    outer.add.div([](auto& lens) {
      lens.style.position        = "absolute";
      lens.style.left            = 120.f;
      lens.style.top             = 80.f;
      lens.style.width           = 220.f;
      lens.style.height          = 130.f;

      lens.align = "center middle";
      
      lens.add.div([](auto& hint) {
        hint.innerText           = "drag me";
        hint.style.color         = glint_color(140, 255, 255, 255);
        hint.style.fontSize      = 20.f;
        hint.style.position      = "absolute";
        hint.style.pointerEvents = "none";
      });
    }, &lensPtr);

    outer.add.template fromClass<glint_checkbox>([](auto& tog) {
      tog.checked        = true;
      tog.size           = 14.f;
      tog.style.position = "absolute";
      tog.style.left     = 6.f;
      tog.style.top      = 6.f;
    }, &lensTogPtr);

  }, &outerPtr);

  lensPtr->style.backdropFilter = "shader(lns, lens_refraction)";

  lensTogPtr->onChange = [lensPtr](bool on) {
    lensPtr->style.backdropFilter = on ? "shader(lns, lens_refraction)" : "";
    lensPtr->setDirty(false);
  };

  // Drag state and events for the lens.
  struct LensDrag {
    bool  active  = false;
    float startCX = 0.f, startCY = 0.f;
    float startL  = 0.f, startT  = 0.f;
  };
  auto drag = std::make_shared<LensDrag>();

  lensPtr->element.addEventListener("mousedown", [lensPtr, outerPtr, drag](glint_event& e) {
    auto& me      = static_cast<glint_mouse_event&>(e);
    drag->active  = true;
    drag->startCX = me.clientX;
    drag->startCY = me.clientY;
    drag->startL  = lensPtr->mPaintRECT.L - outerPtr->mPaintRECT.L;
    drag->startT  = lensPtr->mPaintRECT.T - outerPtr->mPaintRECT.T;
    e.stopPropagation();
  });
  lensPtr->element.addEventListener("mousemove", [lensPtr, outerPtr, drag](glint_event& e) {
    if (!drag->active) return;
    auto& me  = static_cast<glint_mouse_event&>(e);
    float newL = drag->startL + (me.clientX - drag->startCX);
    float newT = drag->startT + (me.clientY - drag->startCY);
    float maxL = outerPtr->mPaintRECT.W() - lensPtr->mPaintRECT.W();
    float maxT = outerPtr->mPaintRECT.H() - lensPtr->mPaintRECT.H();
    newL = std::max(0.f, std::min(newL, maxL));
    newT = std::max(0.f, std::min(newT, maxT));
    // Directly shift mRect/mPaintRECT for the lens and all its children so
    // that the new position takes effect this paint without a layout pass.
    // style.left/top are kept in sync so any future layout computes the same result.
    const float dL = (outerPtr->mPaintRECT.L + newL) - lensPtr->mPaintRECT.L;
    const float dT = (outerPtr->mPaintRECT.T + newT) - lensPtr->mPaintRECT.T;
    std::function<void(glint_element*)> shiftRects = [&](glint_element* el) {
      el->mRect.L += dL; el->mRect.R += dL;
      el->mRect.T += dT; el->mRect.B += dT;
      el->mPaintRECT.L += dL; el->mPaintRECT.R += dL;
      el->mPaintRECT.T += dT; el->mPaintRECT.B += dT;
      for (auto& child : el->mChildren) shiftRects(child.get());
    };
    shiftRects(lensPtr);
    lensPtr->style.left = newL;
    lensPtr->style.top  = newT;
    lensPtr->setPaintOnlyDirty();
  });
  lensPtr->element.addEventListener("mouseup", [drag](glint_event& e) {
    drag->active = false;
  });

  mContent->add.div([](auto& note) {
    note.innerText       = "Tip: Subclass glint_shader_base and override sksl() + setUniforms(). Set isBackdrop=false for fills (style.filter), isBackdrop=true for transforms (style.backdropFilter).";
    note.style.color     = glint_demo_theme::subtle;
    note.style.fontSize  = 11.f;
    note.style.width     = "100%";
    note.style.textAlign = EAlign::Near;
  });
}
