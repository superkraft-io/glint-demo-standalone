#pragma once

inline void glint_demos_window::buildImages()
{
  static constexpr const char* kHeroImage    = "url(/img/demo.png)";
  static constexpr const char* kDemoSvgImage = "url(/img/demo.svg)";

  // -- Intro -----------------------------------------------------------------
  addHeading("Images");

  mContent->add.div([](auto& sub) {
    sub.innerText =
      "Images can be rendered via style.backgroundImage (the background-img / background CSS "
      "shorthand) applied to any div element. Key sub-properties: background-size (cover / contain "
      "/ 100%% 100%% / explicit px), background-position (top left / center / custom %), "
      "background-repeat (no-repeat / repeat / repeat-x / repeat-y), and opacity. "
      "SVG sources are rasterized to the element bounds automatically.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 16.f;
  });

  // -- Helper: labelled img card -------------------------------------------
  // Adds label + img-box into 'row' (a glint_component_adder context).
  auto makeImgCard = [](auto& row, const char* label, const char* bgImg,
                        const char* bgSize     = "cover",
                        const char* bgPosition = "center",
                        const char* bgRepeat   = "no-repeat",
                        float w = 150.f, float h = 110.f)
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
        box.style.width              = "100%";
        box.style.height             = h;
        box.style.borderRadius       = 6.f;
        box.style.backgroundColor    = glint_color(255, 30, 30, 46);
        box.style.backgroundImageProp = bgImg;
        box.style.backgroundSize     = bgSize;
        box.style.backgroundPosition = bgPosition;
        box.style.backgroundRepeat   = bgRepeat;
      });
    });
  };

  // -- Row 1: background-img on a <div> ------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "background-img: url(...) -  the basic technique";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });
  mContent->add.div([&](auto& row) {
    row.style.display    = "flex";
    row.style.flexDirection = "row";
    row.style.gap        = 14.f;
    row.style.width      = "100%";
    row.style.alignItems = "flex-start";
    row.style.marginBottom = 12.f;
    for (int k = 0; k < 5; ++k)
      makeImgCard(row, "url(/img/demo.png)", kHeroImage, "cover", "center", "no-repeat");
  });

  addSpacer(10.f);

  // -- Row 2: background-size ------------------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "background-size  (cover / contain / 100% 100% / explicit px)";
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
    makeImgCard(row, "cover",     kHeroImage, "cover",    "center", "no-repeat");
    makeImgCard(row, "contain",   kHeroImage, "contain",  "center", "no-repeat");
    makeImgCard(row, "100% 100%", kHeroImage, "100% 100%","center", "no-repeat");
    makeImgCard(row, "auto",      kHeroImage, "auto",     "center", "no-repeat");
    makeImgCard(row, "60px 60px", kHeroImage, "60px 60px","center", "no-repeat");
  });

  addSpacer(10.f);

  // -- Row 3: background-position --------------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "background-position  (center / corners / custom %)";
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
    makeImgCard(row, "center",       kHeroImage, "60px 60px", "center",      "no-repeat");
    makeImgCard(row, "top left",     kHeroImage, "60px 60px", "top left",    "no-repeat");
    makeImgCard(row, "top right",    kHeroImage, "60px 60px", "top right",   "no-repeat");
    makeImgCard(row, "bottom left",  kHeroImage, "60px 60px", "bottom left", "no-repeat");
    makeImgCard(row, "bottom right", kHeroImage, "60px 60px", "bottom right","no-repeat");
    makeImgCard(row, "25% 75%",      kHeroImage, "60px 60px", "25% 75%",     "no-repeat");
  });

  addSpacer(10.f);

  // -- Row 4: background-repeat ----------------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "background-repeat  (using a small pattern img)";
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
    makeImgCard(row, "no-repeat", kHeroImage, "80px 80px", "center", "no-repeat");
    makeImgCard(row, "repeat",    kHeroImage, "80px 80px", "center", "repeat");
    makeImgCard(row, "repeat-x",  kHeroImage, "80px 80px", "center", "repeat-x");
    makeImgCard(row, "repeat-y",  kHeroImage, "80px 80px", "center", "repeat-y");
  });

  addSpacer(10.f);

  // -- Row 5: background shorthand -------------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "style.background = \"url(...) position/size repeat\"  (shorthand)";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });

  auto addShorthandCard = [](auto& row, const char* label, const char* img, const char* position, const char* size, const char* repeat) {
    row.add.div([=](auto& wrap) {
      wrap.style.width = 155.f;

      wrap.add.div([=](auto& lbl) {
        lbl.innerText          = label;
        lbl.style.color        = glint_demo_theme::text;
        lbl.style.fontSize     = 10.f;
        lbl.style.height       = 28.f;
        lbl.style.width        = "100%";
        lbl.style.textAlign    = EAlign::Center;
        lbl.style.marginBottom = 4.f;
      });

      wrap.add.div([=](auto& box) {
        box.style.width              = "100%";
        box.style.height             = 110.f;
        box.style.borderRadius       = 6.f;
        box.style.backgroundColor    = glint_color(255, 30, 30, 46);
        box.style.backgroundImageProp = img;
        box.style.backgroundPosition = position;
        box.style.backgroundSize     = size;
        box.style.backgroundRepeat   = repeat;
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
    addShorthandCard(row, "center / cover",      kHeroImage, "center",   "cover",     "no-repeat");
    addShorthandCard(row, "top left / contain",  kHeroImage, "top left", "contain",   "no-repeat");
    addShorthandCard(row, "50% 50% / 80px 80px", kHeroImage, "50% 50%",  "80px 80px", "no-repeat");
    addShorthandCard(row, "center / 80px 80px\nrepeat", kHeroImage, "center", "80px 80px", "repeat");
  });

  addSpacer(10.f);

  // -- Row 6: opacity --------------------------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "opacity applied to img elements";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });

  static const float kOpacities[] = { 1.0f, 0.75f, 0.5f, 0.25f, 0.1f };
  mContent->add.div([&](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 14.f;
    row.style.width         = "100%";
    row.style.alignItems    = "flex-start";
    row.style.marginBottom  = 12.f;
    for (float op : kOpacities)
    {
      row.add.div([op](glint_component_style& wrap) {
        wrap.style.width = 120.f;

        char buf[32];
        snprintf(buf, sizeof(buf), "opacity: %.0f%%", op * 100.f);
        std::string label = buf;

        wrap.add.div([label](glint_component_style& lbl) {
          lbl.innerText          = label;
          lbl.style.color        = glint_demo_theme::text;
          lbl.style.fontSize     = 11.f;
          lbl.style.height       = 20.f;
          lbl.style.width        = "100%";
          lbl.style.textAlign    = EAlign::Center;
          lbl.style.marginBottom = 4.f;
        });

        wrap.add.div([op](glint_component_style& box) {
          box.style.width              = "100%";
          box.style.height             = 90.f;
          box.style.borderRadius       = 6.f;
          box.style.backgroundColor    = glint_color(255, 30, 30, 46);
          box.style.backgroundImageProp = kHeroImage;
          box.style.backgroundSize     = "cover";
          box.style.backgroundPosition = "center";
          box.style.backgroundRepeat   = "no-repeat";
          box.style.opacity            = op;
        });
      });
    }
  });

  addSpacer(10.f);

  // -- Row 7: border-radius shaping -----------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "border-radius shaping  (pill / circle / rounded rect)";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });

  auto addShapeCard = [](glint_component_style& row, const char* label, float w, float h, float radius) {
    row.add.div([=](glint_component_style& wrap) {
      wrap.style.width = w;

      wrap.add.div([=](glint_component_style& lbl) {
        lbl.innerText          = label;
        lbl.style.color        = glint_demo_theme::text;
        lbl.style.fontSize     = 11.f;
        lbl.style.height       = 28.f;
        lbl.style.width        = "100%";
        lbl.style.textAlign    = EAlign::Center;
        lbl.style.marginBottom = 4.f;
      });

      wrap.add.div([=](glint_component_style& box) {
        box.style.width              = "100%";
        box.style.height             = h;
        box.style.borderRadius       = radius;
        box.style.backgroundColor    = glint_color(255, 30, 30, 46);
        box.style.backgroundImageProp = kHeroImage;
        box.style.backgroundSize     = "cover";
        box.style.backgroundPosition = "center";
        box.style.backgroundRepeat   = "no-repeat";
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
    addShapeCard(row, "square\n(no radius)",         110.f, 110.f,  0.f);
    addShapeCard(row, "rounded rect\n(radius: 12)",  110.f, 110.f, 12.f);
    addShapeCard(row, "circle\n(radius: 55)",        110.f, 110.f, 55.f);
    addShapeCard(row, "pill / banner\n(radius: 40)", 200.f,  80.f, 40.f);
  });

  addSpacer(10.f);

  // -- Row 8: SVG background img -------------------------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "SVG as background-img  (rasterized to element bounds)";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });

  auto addSvgCard = [](auto& row, const char* label, const char* src, const char* size) {
    row.add.div([=](auto& wrap) {
      wrap.style.width = 130.f;

      wrap.add.div([=](auto& lbl) {
        lbl.innerText          = label;
        lbl.style.color        = glint_demo_theme::text;
        lbl.style.fontSize     = 10.f;
        lbl.style.height       = 28.f;
        lbl.style.width        = "100%";
        lbl.style.textAlign    = EAlign::Center;
        lbl.style.marginBottom = 4.f;
      });

      wrap.add.div([=](auto& box) {
        box.style.width              = "100%";
        box.style.height             = 110.f;
        box.style.borderRadius       = 6.f;
        box.style.backgroundColor    = glint_color(255, 30, 30, 46);
        box.style.backgroundImageProp = src;
        box.style.backgroundSize     = size;
        box.style.backgroundPosition = "center";
        box.style.backgroundRepeat   = "no-repeat";
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
    addSvgCard(row, "demo.svg / cover",     kDemoSvgImage, "cover");
    addSvgCard(row, "demo.svg / contain",   kDemoSvgImage, "contain");
    addSvgCard(row, "demo.svg / 80px 80px", kDemoSvgImage, "80px 80px");
    addSvgCard(row, "demo.svg / 150x80",    kDemoSvgImage, "150px 80px");
    addSvgCard(row, "demo.svg / auto",      kDemoSvgImage, "auto");
    addSvgCard(row, "demo.svg / 16px 16px", kDemoSvgImage, "16px 16px");
  });

  addSpacer(10.f);

  // -- Row 9: background-img + gradient overlay ----------------------------
  mContent->add.div([](auto& hdr) {
    hdr.innerText       = "background-img + gradient overlay  (pseudo-layering with children)";
    hdr.style.color     = glint_demo_theme::heading;
    hdr.style.fontSize  = 12.f;
    hdr.style.width     = "100%";
    hdr.style.marginBottom = 8.f;
  });

  auto addOverlayCard = [](auto& row, const char* label, const char* gradient) {
    row.add.div([=](auto& wrap) {
      wrap.style.width = 150.f;

      wrap.add.div([=](auto& lbl) {
        lbl.innerText          = label;
        lbl.style.color        = glint_demo_theme::text;
        lbl.style.fontSize     = 11.f;
        lbl.style.height       = 20.f;
        lbl.style.width        = "100%";
        lbl.style.textAlign    = EAlign::Center;
        lbl.style.marginBottom = 4.f;
      });

      wrap.add.div([=](auto& base) {
        base.style.width             = "100%";
        base.style.height            = 110.f;
        base.style.borderRadius      = 6.f;
        base.style.backgroundColor   = glint_color(255, 30, 30, 46);
        base.style.backgroundImageProp = kHeroImage;
        base.style.backgroundSize    = "cover";
        base.style.backgroundPosition = "center";
        base.style.backgroundRepeat  = "no-repeat";
        base.style.position          = "relative";
        base.style.overflow          = "hidden";

        base.add.div([=](auto& overlay) {
          overlay.style.position   = "absolute";
          overlay.style.top        = 0.f;
          overlay.style.left       = 0.f;
          overlay.style.width      = "100%";
          overlay.style.height     = "100%";
          overlay.style.background = gradient;
        });

        base.add.div([=](auto& cap) {
          cap.innerText        = label;
          cap.style.position   = "absolute";
          cap.style.bottom     = 6.f;
          cap.style.left       = 0.f;
          cap.style.width      = "100%";
          cap.style.color      = "#ffffff";
          cap.style.fontSize   = 10.f;
          cap.style.textAlign  = EAlign::Center;
        });
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
    addOverlayCard(row, "bottom fade",   "linear-gradient(to top, #000000cc 0%, transparent 60%)");
    addOverlayCard(row, "top fade",      "linear-gradient(to bottom, #000000cc 0%, transparent 60%)");
    addOverlayCard(row, "dark vignette", "radial-gradient(circle, transparent 40%, #00000099 100%)");
    addOverlayCard(row, "left edge dim", "linear-gradient(to right, #000000bb 0%, transparent 50%)");
  });
}
