#pragma once

inline void glint_demos_window::buildTransitions()
{
  addHeading("CSS transition  (style.transition)");

  mContent->add.div([](auto& sub) {
    sub.innerText          = "Smoothly interpolates declared properties whenever they change. Set style.transition, then write the property.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 14.f;
  });

  // ── Easing comparison: animated width bars (% → %) ───────────────────────
  addHeading("Easing functions  (width: 10% \xe2\x86\x92 100%, 700 ms)");

  struct EasingEntry { const char* label; const char* easing; glint_color color; };
  const EasingEntry easings[] = {
    { "linear",       "linear",       glint_color(255, 128, 128, 128) },
    { "ease",         "ease",         glint_color(255, 148, 148, 148) },
    { "ease-in",      "ease-in",      glint_color(255, 168, 168, 168) },
    { "ease-out",     "ease-out",     glint_color(255, 188, 188, 188) },
    { "ease-in-out",  "ease-in-out",  glint_color(255, 208, 208, 208) },
  };

  auto bars    = std::make_shared<std::vector<glint_element*>>();
  auto toggled = std::make_shared<bool>(false);

  for (const auto& e : easings)
  {
    // barPtr lives at loop-body scope. The track setup lambda captures &barPtr —
    // safe because ComponentAdd::div is fully synchronous; all ops complete before
    // mContent->add.div returns.
    glint_element* barPtr = nullptr;
    mContent->add.div([e, &barPtr](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 10.f;
      row.style.width         = "100%";
      row.style.height        = 28.f;
      row.style.marginBottom  = 6.f;

      row.add.div([e](auto& lbl) {
        lbl.innerText       = e.label;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.fontSize  = 12.f;
        lbl.style.width     = 100.f;
        lbl.style.textAlign = EAlign::Near;
      });

      row.add.div([e, &barPtr](auto& track) {
        track.style.flexGrow        = 1.f;
        track.style.height          = 16.f;
        track.style.backgroundColor = glint_demo_theme::panelBg;
        track.style.borderColor     = glint_demo_theme::border;
        track.style.borderWidth     = 1.f;
        track.style.borderRadius    = 3.f;
        track.style.overflow        = "hidden";

        track.add.div([e](auto& bar) {
          bar.style.width           = "10%";
          bar.style.height          = "100%";
          bar.style.backgroundColor = e.color;
          bar.style.borderRadius    = 2.f;
          bar.style.transition      = std::string("width 700ms ") + e.easing;
        }, &barPtr);
      });
    });
    bars->push_back(barPtr);
  }

  // Trigger button — set onClick after fromClass returns (trigBtnPtr is then valid).
  glint_button* trigBtnPtr = nullptr;
  mContent->add.fromClass<glint_button>([](auto& btn) {
    btn.innerText             = ">  Animate  (10% \xe2\x86\x92 100%)";
    btn.style.width           = 220.f;
    btn.style.height          = 34.f;
    btn.style.backgroundColor = glint_demo_theme::surface;
    btn.style.borderColor     = glint_demo_theme::border;
    btn.style.borderWidth     = 1.f;
    btn.style.borderRadius    = 6.f;
    btn.style.color           = glint_demo_theme::text;
    btn.style.fontSize        = 12.f;
    btn.style.marginTop       = 10.f;
    btn.style.marginBottom    = 4.f;
  }, &trigBtnPtr);
  trigBtnPtr->onClick = [toggled, bars, trigBtnPtr]()
  {
    *toggled = !*toggled;
    trigBtnPtr->innerText = *toggled
      ? "<  Reset  (100% \xe2\x86\x92 10%)"
      : ">  Animate  (10% \xe2\x86\x92 100%)";
    for (auto* bar : *bars)
    {
      bar->style.width = *toggled ? "100%" : "10%";
      bar->setDirty(false);
    }
    trigBtnPtr->setDirty(false);
  };

  addSpacer(20.f);

  // ── Property showcase: hover-triggered cards ──────────────────────────
  addHeading("Property showcase  (hover to animate)");

  struct CardDef
  {
    const char*                         title;
    const char*                         desc;
    const char*                         transStr;
    std::function<void(glint_element*)> onEnter;
    std::function<void(glint_element*)> onLeave;
  };

  const CardDef cards[] = {
    {
      "background-color",
      "300 ms  ease-out",
      "background-color 300ms ease-out",
      [](glint_element* c) { c->style.backgroundColor = glint_color(255, 110, 110, 110); },
      [](glint_element* c) { c->style.backgroundColor = glint_color(255, 49, 50, 68);   },
    },
    {
      "opacity",
      "500 ms  linear",
      "opacity 500ms linear",
      [](glint_element* c) { c->style.opacity = 0.15f; },
      [](glint_element* c) { c->style.opacity = 1.f;   },
    },
    {
      "border-radius",
      "400 ms  ease-in-out",
      "border-radius 400ms ease-in-out",
      [](glint_element* c) { c->style.borderRadius = 44.f; },
      [](glint_element* c) { c->style.borderRadius = 6.f;  },
    },
    {
      "all  (bg-color + radius + border-width)",
      "400 ms  ease",
      "all 400ms ease",
      [](glint_element* c) {
        c->style.backgroundColor = glint_color(255, 128, 128, 128);
        c->style.borderRadius    = 44.f;
        c->style.borderColor     = glint_color(255, 192, 192, 192);
        c->style.borderWidth     = 3.f;
      },
      [](glint_element* c) {
        c->style.backgroundColor = glint_color(255, 49, 50, 68);
        c->style.borderRadius    = 6.f;
        c->style.borderColor     = glint_color(255, 69, 71, 90);
        c->style.borderWidth     = 1.f;
      },
    },
  };

  const int nCards = static_cast<int>(sizeof(cards) / sizeof(cards[0]));
  int idx = 0;
  while (idx < nCards)
  {
    // Collect card pointers at while-loop body scope. mContent->add.div is synchronous
    // so all ops (including out-ptr writes) complete before it returns. Register event
    // listeners afterwards when the pointers are valid.
    std::array<glint_element*, 2>  rowCardPtrs = {};
    std::array<const CardDef*, 2>  rowCardDefs = {};
    int rowCount = 0;

    mContent->add.div([&](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "stretch";
      row.style.gap           = 12.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 10.f;

      for (int col = 0; col < 2 && idx < nCards; ++col, ++idx)
      {
        const CardDef& cd = cards[idx];
        rowCardDefs[col] = &cd;
        rowCount = col + 1;

        row.add.div([cd](auto& card) {
          card.style.flexGrow        = 1.f;
          card.style.height          = 80.f;
          card.style.backgroundColor = glint_color(255, 49, 50, 68);
          card.style.borderColor     = glint_color(255, 69, 71, 90);
          card.style.borderWidth     = 1.f;
          card.style.borderRadius    = 6.f;
          card.style.display         = "flex";
          card.style.flexDirection   = "column";
          card.style.alignItems      = "flex-start";
          card.style.justifyContent  = "center";
          card.style.paddingLeft     = 16.f;
          card.style.transition      = cd.transStr;

          card.add.div([cd](auto& tlbl) {
            tlbl.innerText       = cd.title;
            tlbl.style.color     = glint_demo_theme::text;
            tlbl.style.fontSize  = 12.f;
            tlbl.style.width     = "100%";
            tlbl.style.textAlign = EAlign::Near;
          });

          card.add.div([cd](auto& dlbl) {
            dlbl.innerText       = cd.desc;
            dlbl.style.color     = glint_demo_theme::subtle;
            dlbl.style.fontSize  = 10.f;
            dlbl.style.width     = "100%";
            dlbl.style.textAlign = EAlign::Near;
          });
        }, &rowCardPtrs[col]);
      }
    });

    // Ops have now run — card pointers are valid. Register hover listeners.
    for (int i = 0; i < rowCount; ++i)
    {
      glint_element* cardEl = rowCardPtrs[i];
      auto enterFn = rowCardDefs[i]->onEnter;
      auto leaveFn = rowCardDefs[i]->onLeave;
      cardEl->element.addEventListener("mouseenter", [cardEl, enterFn](glint_event&) {
        enterFn(cardEl); cardEl->setDirty(false);
      });
      cardEl->element.addEventListener("mouseleave", [cardEl, leaveFn](glint_event&) {
        leaveFn(cardEl); cardEl->setDirty(false);
      });
    }
  }

  addSpacer(8.f);

  mContent->add.div([](auto& note) {
    note.innerText       = "Tip: Inspect any element (Ctrl+Shift+I) and edit the 'transition' property live.";
    note.style.color     = glint_demo_theme::subtle;
    note.style.fontSize  = 11.f;
    note.style.width     = "100%";
    note.style.textAlign = EAlign::Near;
  });
}

