#pragma once

inline void glint_demos_window::buildStroke()
{
  // â”€â”€ Intro â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  mContent->add.div([](auto& sub) {
    sub.innerText          = "SVG stroke properties on box borders: stroke, stroke-width, stroke-dasharray, "
                             "stroke-dashoffset, stroke-linecap, stroke-linejoin, stroke-miterlimit, stroke-opacity.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 14.f;
  });

  // â”€â”€ Helper: labelled demo box â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  // Adds a flex-column card (box 90Ã—90 + label + desc) into parentRow.
  auto addBox = [&](glint_element* parentRow, const char* label, const char* desc,
                    std::function<void(glint_component_style&)> setup)
  {
    parentRow->add.div([label, desc, setup](auto& col) {
      col.style.display       = "flex";
      col.style.flexDirection = "column";
      col.style.alignItems    = "center";
      col.style.gap           = 6.f;
      col.style.width         = 130.f;
      col.style.marginBottom  = 16.f;

      col.add.div([setup](auto& box) {
        box.style.width           = 90.f;
        box.style.height          = 90.f;   // fixed-size demo swatch
        box.style.borderRadius    = 8.f;
        box.style.backgroundColor = glint_demo_theme::surfaceAlt;
        box.style.borderWidth     = 0.f;
        setup(box);
      });

      col.add.div([label](auto& lbl) {
        lbl.innerText       = label;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 11.f;
        lbl.style.width     = 130.f;
        lbl.style.textAlign = EAlign::Center;
      });

      col.add.div([desc](auto& dlbl) {
        dlbl.innerText       = desc;
        dlbl.style.color     = glint_demo_theme::subtle;
        dlbl.style.fontSize  = 10.f;
        dlbl.style.width     = 130.f;
        dlbl.style.textAlign = EAlign::Center;
      });
    });
  };

  // â”€â”€ 1. border-style â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("border-style  (solid / dashed / dotted)");
  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    addBox(row, "solid",        "borderStyle: solid",  [](glint_component_style& b) {
      b.style.borderStyle = "solid";
      b.style.borderWidth = 2.f;
      b.style.borderColor = "#89b4fa";
    });
    addBox(row, "dashed",       "borderStyle: dashed", [](glint_component_style& b) {
      b.style.borderStyle = "dashed";
      b.style.borderWidth = 2.f;
      b.style.borderColor = "#a6e3a1";
    });
    addBox(row, "dotted",       "borderStyle: dotted", [](glint_component_style& b) {
      b.style.borderStyle = "dotted";
      b.style.borderWidth = 2.f;
      b.style.borderColor = "#f38ba8";
    });
    addBox(row, "dashed thick", "borderWidth: 5",      [](glint_component_style& b) {
      b.style.borderStyle = "dashed";
      b.style.borderWidth = 5.f;
      b.style.borderColor = "#fab387";
    });
  }

  // â”€â”€ 2. stroke-dasharray â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("stroke-dasharray  (custom dash intervals)");
  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    addBox(row, "5 5",        "equal dash + gap",        [](glint_component_style& b) {
      b.style.borderWidth     = 2.f;
      b.style.borderColor     = "#89b4fa";
      b.style.strokeDasharray = "5 5";
    });
    addBox(row, "10 3",       "long dash, tiny gap",     [](glint_component_style& b) {
      b.style.borderWidth     = 2.f;
      b.style.borderColor     = "#a6e3a1";
      b.style.strokeDasharray = "10 3";
    });
    addBox(row, "2 6",        "short dash, big gap",     [](glint_component_style& b) {
      b.style.borderWidth     = 2.f;
      b.style.borderColor     = "#f38ba8";
      b.style.strokeDasharray = "2 6";
    });
    addBox(row, "8 3 2 3",    "dash-dot pattern",        [](glint_component_style& b) {
      b.style.borderWidth     = 2.f;
      b.style.borderColor     = "#cba6f7";
      b.style.strokeDasharray = "8 3 2 3";
    });
    addBox(row, "1 1 (odd)",  "odd count \xe2\x86\x92 duplicated", [](glint_component_style& b) {
      b.style.borderWidth     = 3.f;
      b.style.borderColor     = "#fab387";
      b.style.strokeDasharray = "1 1 1";
    });
  }

  // â”€â”€ 3. stroke-linecap â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("stroke-linecap  (butt / round / square)");
  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    addBox(row, "butt",   "strokeLinecap: butt",   [](glint_component_style& b) {
      b.style.borderStyle   = "dashed";
      b.style.borderWidth   = 6.f;
      b.style.borderColor   = "#89b4fa";
      b.style.strokeLinecap = "butt";
    });
    addBox(row, "round",  "strokeLinecap: round",  [](glint_component_style& b) {
      b.style.borderStyle   = "dashed";
      b.style.borderWidth   = 6.f;
      b.style.borderColor   = "#a6e3a1";
      b.style.strokeLinecap = "round";
    });
    addBox(row, "square", "strokeLinecap: square", [](glint_component_style& b) {
      b.style.borderStyle   = "dashed";
      b.style.borderWidth   = 6.f;
      b.style.borderColor   = "#f38ba8";
      b.style.strokeLinecap = "square";
    });
  }

  // â”€â”€ 4. stroke-linejoin â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("stroke-linejoin  (miter / round / bevel)");
  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    addBox(row, "miter", "strokeLinejoin: miter", [](glint_component_style& b) {
      b.style.borderRadius   = 0.f;
      b.style.borderStyle    = "solid";
      b.style.borderWidth    = 8.f;
      b.style.borderColor    = "#89b4fa";
      b.style.strokeLinejoin = "miter";
    });
    addBox(row, "round", "strokeLinejoin: round", [](glint_component_style& b) {
      b.style.borderRadius   = 0.f;
      b.style.borderStyle    = "solid";
      b.style.borderWidth    = 8.f;
      b.style.borderColor    = "#a6e3a1";
      b.style.strokeLinejoin = "round";
    });
    addBox(row, "bevel", "strokeLinejoin: bevel", [](glint_component_style& b) {
      b.style.borderRadius   = 0.f;
      b.style.borderStyle    = "solid";
      b.style.borderWidth    = 8.f;
      b.style.borderColor    = "#f38ba8";
      b.style.strokeLinejoin = "bevel";
    });
  }

  // â”€â”€ 5. stroke-opacity â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("stroke-opacity");
  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    static constexpr float kOpacities[] = { 1.f, 0.7f, 0.4f, 0.15f };
    static const char*     kOpLabels[]  = { "1.0", "0.7", "0.4", "0.15" };
    for (int i = 0; i < 4; ++i)
    {
      const float op = kOpacities[i];
      const char* lb = kOpLabels[i];
      addBox(row, lb, "strokeOpacity", [op](glint_component_style& b) {
        b.style.borderStyle   = "solid";
        b.style.borderWidth   = 4.f;
        b.style.borderColor   = "#89b4fa";
        b.style.strokeOpacity = op;
      });
    }
  }

  // â”€â”€ 6. stroke (color override) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("stroke  (color override)");
  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    struct ColEntry { const char* col; const char* lbl; };
    static const ColEntry kCols[] = {
      { "#f38ba8", "red"    }, { "#a6e3a1", "green"  },
      { "#fab387", "orange" }, { "#cba6f7", "mauve"  },
    };
    for (const auto& ce : kCols)
    {
      const char* col = ce.col;
      const char* lb  = ce.lbl;
      addBox(row, lb, "stroke override", [col](glint_component_style& b) {
        b.style.borderStyle = "solid";
        b.style.borderWidth = 3.f;
        b.style.borderColor = "#45475a";
        b.style.strokeColor = col;
      });
    }
  }

  // â”€â”€ 7. stroke-width override â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("stroke-width  (overrides border-width for paint)");
  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    static constexpr float kWidths[] = { 1.f, 3.f, 6.f, 10.f };
    static const char*     kWLabels[]= { "1px", "3px", "6px", "10px" };
    for (int i = 0; i < 4; ++i)
    {
      const float sw = kWidths[i];
      const char* lb = kWLabels[i];
      addBox(row, lb, "strokeWidth", [sw](glint_component_style& b) {
        b.style.borderStyle = "solid";
        b.style.borderColor = "#89b4fa";
        b.style.strokeWidth = sw;
      });
    }
  }

  addSpacer(12.f);

  // â”€â”€ 8. Continuous marching ants â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("Animated marching ants  (continuous loop)");

  mContent->add.div([](auto& desc) {
    desc.innerText          = "stroke-dashoffset: -1400 over 50 s linear = ~2 periods/s. Three dash patterns side by side.";
    desc.style.color        = glint_demo_theme::subtle;
    desc.style.fontSize     = 11.f;
    desc.style.width        = "100%";
    desc.style.textAlign    = EAlign::Near;
    desc.style.marginBottom = 12.f;
  });

  struct AntEntry { const char* dasharray; const char* color; const char* label; float period; };
  static const AntEntry kAnts[] = {
    { "8 6",  "#89b4fa", "8 6   (14px period)",  14.f },
    { "3 3",  "#a6e3a1", "3 3   (6px period)",    6.f },
    { "14 4", "#f38ba8", "14 4  (18px period)",  18.f },
  };

  auto antBoxesSP  = std::make_shared<std::vector<glint_element*>>();
  auto antPeriodsSP = std::make_shared<std::vector<float>>();

  for (const auto& ae : kAnts)
  {
    glint_element* boxPtr = nullptr;
    mContent->add.div([&ae](auto& box) {
      box.style.width            = "100%";
      box.style.height           = 40.f;   // fixed-height demo strip
      box.style.borderRadius     = 8.f;
      box.style.backgroundColor  = glint_demo_theme::surfaceAlt;
      box.style.borderWidth      = 2.f;
      box.style.borderColor      = ae.color;
      box.style.strokeDasharray  = ae.dasharray;
      box.style.strokeDashoffset = 0.f;
      box.style.transition       = "stroke-dashoffset 50s linear";
      box.style.marginBottom     = 8.f;
      box.innerText              = ae.label;
      box.style.color            = ae.color;
      box.style.fontSize         = 10.f;
      box.style.textAlign        = EAlign::Near;
      box.style.paddingLeft      = 8.f;
    }, &boxPtr);
    antBoxesSP->push_back(boxPtr);
    antPeriodsSP->push_back(ae.period);
  }

  auto running = std::make_shared<bool>(false);

  glint_button* marchBtnPtr = nullptr;
  mContent->add.fromClass<glint_button>([](auto& b) {
    b.innerText             = "\xe2\x96\xb6  Start";
    b.style.width           = 120.f;
    b.style.height          = 32.f;
    b.style.backgroundColor = glint_demo_theme::surface;
    b.style.borderColor     = glint_demo_theme::border;
    b.style.borderWidth     = 1.f;
    b.style.borderRadius    = 6.f;
    b.style.color           = glint_demo_theme::text;
    b.style.fontSize        = 12.f;
    b.style.marginBottom    = 20.f;
  }, &marchBtnPtr);

  marchBtnPtr->onClick = [running, antBoxesSP, antPeriodsSP, marchBtnPtr]() mutable
  {
    *running = !*running;
    marchBtnPtr->innerText = *running ? "\xe2\x96\xa0  Stop" : "\xe2\x96\xb6  Start";
    marchBtnPtr->setDirty(false);
    for (int i = 0; i < (int)antBoxesSP->size(); ++i)
    {
      auto* b = (*antBoxesSP)[i];
      if (*running)
      {
        b->style.strokeDashoffset = -100.f * (*antPeriodsSP)[i];
      }
      else
      {
        b->style.transition       = "";
        b->style.strokeDashoffset = 0.f;
        b->style.transition       = "stroke-dashoffset 50s linear";
      }
      b->setDirty(false);
    }
  };

  // â”€â”€ 9. Hover-animated stroke props â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("Hover-animated stroke properties");

  // Custom element: W-shaped zigzag path so the miter spike / bevel
  // transition is clearly visible at acute-angle peaks.
  struct glint_ml_hover_card : public glint_element
  {
    void DrawContentToCanvas(SkCanvas* canvas) override
    {
      if (!canvas) return;
      const glint_rect r = GetPaintRECT();
      const float l   = r.L + 8.f;
      const float ri  = r.R - 8.f;
      const float cy  = r.MH();
      const float amp = (r.H() - 16.f) * 0.5f;
      const float w4  = (ri - l) * 0.25f;

      SkPath path;
      path.moveTo(l,           cy + amp);
      path.lineTo(l + w4,      cy - amp);
      path.lineTo(l + 2.f*w4,  cy + amp);
      path.lineTo(l + 3.f*w4,  cy - amp);
      path.lineTo(ri,          cy + amp);

      SkPaint bp;
      bp.setAntiAlias(true);
      bp.setStyle(SkPaint::kStroke_Style);
      bp.setStrokeWidth(3.f);
      bp.setStrokeJoin(SkPaint::kMiter_Join);
      bp.setStrokeMiter(computedStyle.strokeMiterlimit);
      bp.setStrokeCap(SkPaint::kButt_Cap);

      glint_color col = computedStyle.strokeColor.isSet
                   ? computedStyle.strokeColor.value
                   : computedStyle.borderColor.value;
      col = ApplyOpacity(col, computedStyle.opacity);
      bp.setColor(skColor(col));
      canvas->drawPath(path, bp);
    }
  };

  struct StrokeCard {
    const char* title;
    const char* desc;
    const char* transition;
    std::function<void(glint_element*)> onEnter;
    std::function<void(glint_element*)> onLeave;
  };

  const StrokeCard scards[] =
  {
    {
      "stroke-width",
      "1px \xe2\x86\x92 8px  300ms",
      "stroke-width 300ms ease-out",
      [](glint_element* c) { c->style.strokeWidth = 8.f; },
      [](glint_element* c) { c->style.strokeWidth = 1.f; },
    },
    {
      "stroke-opacity",
      "1.0 \xe2\x86\x92 0.2  400ms",
      "stroke-opacity 400ms ease-in-out",
      [](glint_element* c) { c->style.strokeOpacity = 0.2f; },
      [](glint_element* c) { c->style.strokeOpacity = 1.0f; },
    },
    {
      "stroke (color)",
      "blue \xe2\x86\x92 pink  500ms",
      "stroke 500ms ease",
      [](glint_element* c) { c->style.strokeColor = "#f38ba8"; },
      [](glint_element* c) { c->style.strokeColor = "#89b4fa"; },
    },
    {
      "stroke-miterlimit",
      "10 \xe2\x86\x92 1  300ms",
      "stroke-miterlimit 300ms linear",
      [](glint_element* c) { c->style.strokeMiterlimit = 1.f; },
      [](glint_element* c) { c->style.strokeMiterlimit = 10.f; },
    },
    {
      "stroke-dashoffset",
      "0 \xe2\x86\x92 20  600ms",
      "stroke-dashoffset 600ms ease-in-out",
      [](glint_element* c) { c->style.strokeDashoffset = 20.f; },
      [](glint_element* c) { c->style.strokeDashoffset = 0.f;  },
    },
  };

  {
    auto* row = addRow(12.f);
    row->style.flexDirection = "row";
    row->style.alignItems    = "flex-start";

    for (const auto& sc : scards)
    {
      const bool isMiter = (std::string(sc.title) == "stroke-miterlimit");
      glint_element*       cardPtr  = nullptr;
      glint_ml_hover_card* miterPtr = nullptr;  // must be at loop scope, not inside the lambda

      row->add.div([&](auto& col) {
        col.style.display       = "flex";
        col.style.flexDirection = "column";
        col.style.alignItems    = "center";
        col.style.gap           = 4.f;
        col.style.width         = 130.f;
        col.style.marginBottom  = 16.f;

        if (isMiter)
        {
          // Pass &miterPtr — lives at loop scope so the deferred op can write to it.
          // Do NOT assign cardPtr = miterPtr here; miterPtr is null until ops run.
          col.add.template fromClass<glint_ml_hover_card>([&sc](auto& card) {
            card.style.width            = 130.f;
            card.style.height           = 90.f;
            card.style.backgroundColor  = "#181825";
            card.style.transition       = sc.transition;
            card.style.marginBottom     = 6.f;
            card.style.borderWidth      = 0.f;
            card.style.borderColor      = "#89b4fa";
            card.style.strokeMiterlimit = 10.f;
          }, &miterPtr);
        }
        else
        {
          col.add.div([&sc](auto& card) {
            card.style.width           = 130.f;
            card.style.height          = 90.f;
            card.style.backgroundColor = "#181825";
            card.style.transition      = sc.transition;
            card.style.marginBottom    = 6.f;
            card.style.borderRadius    = 10.f;
            card.style.borderWidth     = 2.f;
            card.style.borderColor     = "#89b4fa";
            card.style.strokeColor     = "#89b4fa";
            card.style.strokeWidth     = 1.f;
            card.style.strokeDasharray = "8 4";
            card.style.strokeOpacity   = 1.f;
            card.style.strokeMiterlimit = 4.f;
            card.style.strokeDashoffset = 0.f;
          }, &cardPtr);
        }

        col.add.div([&sc](auto& lbl) {
          lbl.innerText       = sc.title;
          lbl.style.color     = "#cdd6f4";
          lbl.style.fontSize  = 11.f;
          lbl.style.width     = 130.f;
          lbl.style.textAlign = EAlign::Center;
        });

        col.add.div([&sc](auto& dlbl) {
          dlbl.innerText       = sc.desc;
          dlbl.style.color     = "#6c7086";
          dlbl.style.fontSize  = 10.f;
          dlbl.style.width     = 130.f;
          dlbl.style.textAlign = EAlign::Center;
        });
      });

      // Ops have now run — miterPtr and cardPtr are valid.
      if (isMiter) cardPtr = miterPtr;

      const auto onEnter = sc.onEnter;
      const auto onLeave = sc.onLeave;
      cardPtr->element.addEventListener("mouseenter", [cardPtr, onEnter](glint_event&) {
        onEnter(cardPtr);
        cardPtr->setDirty(false);
      });
      cardPtr->element.addEventListener("mouseleave", [cardPtr, onLeave](glint_event&) {
        onLeave(cardPtr);
        cardPtr->setDirty(false);
      });
    }
  }

  addSpacer(20.f);

  // â”€â”€ 10. Easing comparison â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
  addHeading("Easing comparison  (5 stroke props x 5 easing functions)");
  {
    mContent->add.div([](auto& desc) {
      desc.innerText          = "Click Animate to interpolate all boxes simultaneously. Each column uses a different easing. Click Reset to return them.";
      desc.style.color        = glint_demo_theme::subtle;
      desc.style.fontSize     = 11.f;
      desc.style.width        = "100%";
      desc.style.textAlign    = EAlign::Near;
      desc.style.marginBottom = 14.f;
    });

    struct EasingDef { const char* name; const char* css; const char* color; };
    static const EasingDef kEasings[] = {
      { "linear",      "linear",      "#89b4fa" },
      { "ease",        "ease",        "#a6e3a1" },
      { "ease-in",     "ease-in",     "#f38ba8" },
      { "ease-out",    "ease-out",    "#fab387" },
      { "ease-in-out", "ease-in-out", "#cba6f7" },
    };
    const int NE = 5;

    struct PropDef { const char* label; float fromW; float toW; float fromO; float toO;
                     const char* fromStroke; const char* toStroke;
                     float fromDO; float toDO; float fromML; float toML; };
    static const PropDef kProps[] = {
      { "stroke-width",      1.f, 10.f, 1.f, 1.f,  nullptr,    nullptr,    0.f,  0.f, 4.f,  4.f },
      { "stroke-opacity",    2.f,  2.f, 1.f, 0.1f, nullptr,    nullptr,    0.f,  0.f, 4.f,  4.f },
      { "stroke color",      2.f,  2.f, 1.f, 1.f,  "#89b4fa",  "#f38ba8",  0.f,  0.f, 4.f,  4.f },
      { "stroke-dashoffset", 2.f,  2.f, 1.f, 1.f,  nullptr,    nullptr,    0.f, 28.f, 4.f,  4.f },
      { "stroke-miterlimit", 2.f,  2.f, 1.f, 1.f,  nullptr,    nullptr,    0.f,  0.f, 10.f, 1.f },
    };
    const int NP = 5;

    // Custom W-path cell for stroke-miterlimit row
    struct glint_ml_demo_cell : public glint_element
    {
      void DrawContentToCanvas(SkCanvas* canvas) override
      {
        if (!canvas) return;
        const glint_rect r = GetPaintRECT();
        const float l   = r.L + 5.f;
        const float ri  = r.R - 5.f;
        const float cy  = r.MH();
        const float amp = (r.H() - 10.f) * 0.5f;
        const float w4  = (ri - l) * 0.25f;

        SkPath path;
        path.moveTo(l,           cy + amp);
        path.lineTo(l + w4,      cy - amp);
        path.lineTo(l + 2.f*w4,  cy + amp);
        path.lineTo(l + 3.f*w4,  cy - amp);
        path.lineTo(ri,          cy + amp);

        SkPaint bp;
        bp.setAntiAlias(true);
        bp.setStyle(SkPaint::kStroke_Style);
        bp.setStrokeWidth(3.f);
        bp.setStrokeJoin(SkPaint::kMiter_Join);
        bp.setStrokeMiter(computedStyle.strokeMiterlimit);
        bp.setStrokeCap(SkPaint::kButt_Cap);

        glint_color col = computedStyle.strokeColor.isSet
                     ? computedStyle.strokeColor.value
                     : computedStyle.borderColor.value;
        col = ApplyOpacity(col, computedStyle.opacity);
        if (computedStyle.strokeOpacity < 1.f)
          col = glint_color(static_cast<int>(col.A * computedStyle.strokeOpacity),
                       col.R, col.G, col.B);
        bp.setColor(skColor(col));
        canvas->drawPath(path, bp);
      }
    };

    auto animatedSP = std::make_shared<bool>(false);
    auto easingBoxesSP = std::make_shared<std::array<std::array<glint_element*, 5>, 5>>();

    // Column headers row
    mContent->add.div([&](auto& headerRow) {
      headerRow.style.display       = "flex";
      headerRow.style.flexDirection = "row";
      headerRow.style.width         = "100%";
      headerRow.style.marginBottom  = 4.f;

      headerRow.add.div([](auto& spacer) {
        spacer.style.width = 120.f;
      });

      for (int e = 0; e < NE; ++e)
      {
        const auto& ez = kEasings[e];
        headerRow.add.div([ez](auto& hdr) {
          hdr.innerText       = ez.name;
          hdr.style.flexGrow  = 1.f;
          hdr.style.color     = ez.color;
          hdr.style.fontSize  = 9.f;
          hdr.style.textAlign = EAlign::Center;
        });
      }
    });

    // One row per stroke property
    for (int p = 0; p < NP; ++p)
    {
      const auto& pd = kProps[p];
      // Must live at loop-body scope — deferred ops write here after the setup lambda returns.
      std::array<glint_ml_demo_cell*, 5> mlCells = {};

      mContent->add.div([&](auto& row) {
        row.style.display       = "flex";
        row.style.flexDirection = "row";
        row.style.width         = "100%";
        row.style.alignItems    = "center";
        row.style.marginBottom  = 6.f;

        row.add.div([pd](auto& lbl) {
          lbl.innerText       = pd.label;
          lbl.style.width     = 120.f;
          lbl.style.color     = glint_demo_theme::text;
          lbl.style.fontSize  = 10.f;
          lbl.style.textAlign = EAlign::Near;
        });

        for (int e = 0; e < NE; ++e)
        {
          if (p == 4)
          {
            // Capture e and pd by value — e is a local of this setup lambda and will be
            // gone when the deferred op fires. Pass &mlCells[e] (loop-scope stable address)
            // instead of a lambda-local cellPtr.
            row.add.template fromClass<glint_ml_demo_cell>([e, pd](auto& box) {
              box.style.flexGrow         = 1.f;
              box.style.height           = 36.f;
              box.style.borderRadius     = 6.f;
              box.style.backgroundColor  = glint_demo_theme::surfaceAlt;
              box.style.borderWidth      = 0.f;
              box.style.borderColor      = "#89b4fa";
              box.style.strokeOpacity    = pd.fromO;
              box.style.strokeDashoffset = pd.fromDO;
              box.style.strokeMiterlimit = pd.fromML;
              box.style.marginLeft       = 4.f;
              box.style.transition       = std::string("stroke-width 1s ") + kEasings[e].css +
                                           ", stroke-opacity 1s " + kEasings[e].css +
                                           ", stroke 1s " + kEasings[e].css +
                                           ", stroke-dashoffset 1s " + kEasings[e].css +
                                           ", stroke-miterlimit 1s " + kEasings[e].css;
            }, &mlCells[e]);
          }
          else
          {
            // Capture e, pd, p by value — same reason.
            row.add.div([e, pd, p](auto& box) {
              box.style.flexGrow         = 1.f;
              box.style.height           = 36.f;
              box.style.borderRadius     = 6.f;
              box.style.backgroundColor  = glint_demo_theme::surfaceAlt;

              if (p == 0)
                box.style.strokeWidth  = pd.fromW;
              else
                box.style.borderWidth  = pd.fromW;

              box.style.borderColor      = glint_demo_theme::border;
              box.style.strokeOpacity    = pd.fromO;
              box.style.strokeDasharray  = "6 4";
              box.style.strokeDashoffset = pd.fromDO;
              box.style.strokeMiterlimit = pd.fromML;
              if (pd.fromStroke)
                box.style.strokeColor = pd.fromStroke;
              box.style.marginLeft       = 4.f;
              box.style.transition       = std::string("stroke-width 1s ") + kEasings[e].css +
                                           ", stroke-opacity 1s " + kEasings[e].css +
                                           ", stroke 1s " + kEasings[e].css +
                                           ", stroke-dashoffset 1s " + kEasings[e].css +
                                           ", stroke-miterlimit 1s " + kEasings[e].css;
            }, &(*easingBoxesSP)[p][e]);
          }
        }
      });
      // Ops have now run — mlCells[e] are valid. Copy into the shared store.
      if (p == 4)
        for (int e = 0; e < NE; ++e)
          (*easingBoxesSP)[4][e] = mlCells[e];
    }

    // Animate / Reset button
    glint_button* animBtnPtr = nullptr;
    mContent->add.fromClass<glint_button>([](auto& b) {
      b.innerText             = "Animate";
      b.style.width           = 120.f;
      b.style.height          = 32.f;
      b.style.backgroundColor = glint_demo_theme::surface;
      b.style.borderColor     = glint_demo_theme::border;
      b.style.borderWidth     = 1.f;
      b.style.borderRadius    = 6.f;
      b.style.color           = glint_demo_theme::text;
      b.style.fontSize        = 12.f;
      b.style.marginTop       = 8.f;
    }, &animBtnPtr);

    animBtnPtr->onClick = [animatedSP, animBtnPtr, easingBoxesSP, this]() mutable
    {
      *animatedSP = !*animatedSP;
      animBtnPtr->innerText = *animatedSP ? "Reset" : "Animate";
      animBtnPtr->setDirty(false);

      const float toW[]   = { 10.f, 2.f,  2.f,  2.f,  2.f };
      const float fromW[] = {  1.f, 2.f,  2.f,  2.f,  2.f };
      const float toO[]   = {  1.f, 0.1f, 1.f,  1.f,  1.f };
      const float fromO[] = {  1.f, 1.f,  1.f,  1.f,  1.f };
      const float toDO[]  = {  0.f, 0.f,  0.f, 28.f,  0.f };
      const float fromDO[]= {  0.f, 0.f,  0.f,  0.f,  0.f };
      const float toML[]  = {  4.f, 4.f,  4.f,  4.f,  1.f };
      const float fromML[]= {  4.f, 4.f,  4.f,  4.f, 10.f };

      for (int p = 0; p < 5; ++p)
      for (int e = 0; e < 5; ++e)
      {
        auto* box = (*easingBoxesSP)[p][e];
        if (*animatedSP)
        {
          if (p == 0) box->style.strokeWidth = toW[p];
          else        box->style.borderWidth  = toW[p];
          box->style.strokeOpacity    = toO[p];
          box->style.strokeDashoffset = toDO[p];
          box->style.strokeMiterlimit = toML[p];
          if (p == 2) box->style.strokeColor = "#f38ba8";
        }
        else
        {
          if (p == 0) box->style.strokeWidth = fromW[p];
          else        box->style.borderWidth  = fromW[p];
          box->style.strokeOpacity    = fromO[p];
          box->style.strokeDashoffset = fromDO[p];
          box->style.strokeMiterlimit = fromML[p];
          if (p == 2) box->style.strokeColor = "#89b4fa";
        }
        box->setDirty(false);
      }
      mContent->setDirty(false);
    };
  }

  addSpacer(24.f);
}
