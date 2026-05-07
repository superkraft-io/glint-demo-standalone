#pragma once

inline void glint_demos_window::buildProgress()
{
  auto addHeading = [&](const char* text, float mb = 8.f) {
    mContent->add.div([=](glint_component_style& d) {
      d.innerText          = text;
      d.style.color        = glint_demo_theme::heading;
      d.style.fontSize     = 15.f;
      d.style.width        = "100%";
      d.style.textAlign    = EAlign::Near;
      d.style.marginBottom = mb;
    });
  };

  auto addSpacer = [&](float h) {
    mContent->add.div([=](glint_component_style& s) {
      s.style.height = h;
      s.style.width  = "100%";
    });
  };

  auto addLabel = [&](const char* text) {
    mContent->add.div([=](glint_component_style& d) {
      d.innerText       = text;
      d.style.color     = glint_demo_theme::muted;
      d.style.fontSize  = 12.f;
      d.style.width     = "100%";
      d.style.textAlign = EAlign::Near;
      d.style.marginBottom = 4.f;
    });
  };

  // ── Progress bar ──────────────────────────────────────────────────────────
  addHeading("<progress> — determinate");

  const struct { const char* lbl; float val; } progDefs[] = {
    { "25%",  0.25f },
    { "50%",  0.50f },
    { "75%",  0.75f },
    { "100%", 1.00f },
  };

  for (const auto& pd : progDefs)
  {
    addLabel(pd.lbl);
    mContent->add.fromClass<glint_progress>([&, pd](glint_progress& p) {
      p.value      = pd.val;
      p.style.width  = "100%";
      p.style.height = 8.f;
      p.style.marginBottom = 8.f;
    });
  }

  addSpacer(4.f);
  addHeading("Colour variants");

  // Colour variants — customised via fillClassName + CSS rules in main.css
  const struct { const char* lbl; const char* fillClass; } colorDefs[] = {
    { "Blue (default)",    "glint_progress_fill"         },
    { "Green",             "glint_progress_fill--green"  },
    { "Red",               "glint_progress_fill--red"    },
    { "Amber",             "glint_progress_fill--amber"  },
    { "Slate",             "glint_progress_fill--slate"  },
  };

  for (const auto& cd : colorDefs)
  {
    addLabel(cd.lbl);
    mContent->add.fromClass<glint_progress>([&, cd](glint_progress& p) {
      p.value         = 0.6f;
      p.fillClassName = cd.fillClass;
      p.style.width   = "100%";
      p.style.height  = 8.f;
      p.style.marginBottom = 8.f;
    });
  }

  addSpacer(4.f);

  // ── Interactive progress ──────────────────────────────────────────────────
  addHeading("Interactive (slider drives progress)");

  auto* slFeedback = mContent->add.div([](glint_component_style& fb) {
    fb.innerText      = "Value: 40%";
    fb.style.color    = glint_demo_theme::muted;
    fb.style.fontSize = 12.f;
    fb.style.width    = "100%";
    fb.style.textAlign = EAlign::Near;
    fb.style.marginBottom = 4.f;
  });

  glint_progress* interProg = nullptr;
  glint_element* slFbPtr = slFeedback;

  mContent->add.fromClass<glint_progress>([&](glint_progress& p) {
    p.value      = 0.4f;
    p.style.width  = "100%";
    p.style.height = 10.f;
    p.style.marginBottom = 8.f;
  }, &interProg);

  auto* sl = mContent->add.input([interProg, slFbPtr](glint_input& inp) {
    inp.type  = "range";
    inp.min   = 0.f;
    inp.max   = 100.f;
    inp.style.width  = "100%";
    inp.style.height = 24.f;
    inp.onChange = [interProg, slFbPtr](const std::string& v) {
      try {
        const float pct = std::stof(v) / 100.f;
        interProg->setValue(pct);
        slFbPtr->innerText = std::string("Value: ") + v + "%";
        slFbPtr->setDirty(false);
      } catch (...) {}
    };
  });
  sl->setFloatValue(40.f);

  addSpacer(16.f);

  // ── Indeterminate ─────────────────────────────────────────────────────────
  addHeading("<progress> — indeterminate (value = -1)");
  addLabel("No value attribute — sliding thumb animation, Chrome parity");
  mContent->add.fromClass<glint_progress>([&](glint_progress& p) {
    p.value      = -1.f;   // indeterminate
    p.style.width  = "100%";
    p.style.height = 10.f;
    p.style.marginBottom = 8.f;
  });

  addSpacer(16.f);

  // ── <meter> ───────────────────────────────────────────────────────────────
  addHeading("<meter> — value zones (green / yellow / red)");

  const struct {
    const char* lbl;
    float value;
    float low;
    float high;
    float optimum;
  } meterDefs[] = {
    { "Optimal   (value=0.85, optimum near high)", 0.85f, 0.25f, 0.75f, 0.90f },
    { "Suboptimal (value=0.50, optimum near high)", 0.50f, 0.25f, 0.75f, 0.90f },
    { "Bad        (value=0.10, optimum near high)", 0.10f, 0.25f, 0.75f, 0.90f },
    { "Optimal   (value=0.10, optimum near low)",   0.10f, 0.25f, 0.75f, 0.05f },
  };

  for (const auto& md : meterDefs)
  {
    addLabel(md.lbl);
    mContent->add.fromClass<glint_meter>([&, md](glint_meter& m) {
      m.value   = md.value;
      m.low     = md.low;
      m.high    = md.high;
      m.optimum = md.optimum;
      m.trackColor = glint_demo_theme::surface;
      m.style.width  = "100%";
      m.style.height = 10.f;
      m.style.marginBottom = 8.f;
    });
  }
}
