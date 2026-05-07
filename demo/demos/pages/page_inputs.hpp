#pragma once

inline void glint_demos_window::buildInputs()
{
  const struct {
    const char* heading;
    const char* type;
    const char* placeholder;
  } inputDefs[] = {
    { "Text input",     "text",     "Enter text here\xe2\x80\xa6"    },
    { "Number input",   "number",   "0.0"                 },
    { "Password input", "password", "Enter password\xe2\x80\xa6"     },
    { "Email input",    "email",    "user@example.com"    },
  };

  for (const auto& id : inputDefs)
  {
    mContent->add.div([&](glint_component_style& lbl) {
      lbl.innerText = id.heading;
      lbl.style.color = glint_demo_theme::heading;
      lbl.style.fontSize = 14.f;
      lbl.style.width = "100%";
      lbl.style.textAlign = EAlign::Near;
      lbl.style.marginBottom = 4.f;
    });

    mContent->add.input([&](glint_input& inp) {
      inp.type = id.type;
      inp.placeholder = id.placeholder;
      inp.style.width = "100%";
      inp.style.height = 36.f;
    });

    mContent->add.div([](glint_component_style& spacer) {
      spacer.style.height = 12.f;
      spacer.style.width = "100%";
    });
  }

  mContent->add.div([](glint_component_style& spacer) {
    spacer.style.height = 4.f;
    spacer.style.width = "100%";
  });

  // Range slider
  {
    mContent->add.div([](glint_component_style& lbl) {
      lbl.innerText = "Range slider (type=\"range\")";
      lbl.style.color = glint_demo_theme::heading;
      lbl.style.fontSize = 14.f;
      lbl.style.width = "100%";
      lbl.style.textAlign = EAlign::Near;
      lbl.style.marginBottom = 8.f;
    });

    auto* sliderFeedback = mContent->add.div([](glint_component_style& fb) {
      fb.innerText = "Value: 50";
      fb.style.color = glint_demo_theme::muted;
      fb.style.fontSize = 12.f;
      fb.style.width = "100%";
      fb.style.textAlign = EAlign::Near;
      fb.style.marginBottom = 6.f;
    });

    glint_element* fbPtr = sliderFeedback;
    auto* sl = mContent->add.input([fbPtr](glint_input& inp) {
      inp.type  = "range";
      inp.min   = 0.f;
      inp.max   = 100.f;
      inp.style.width  = "100%";
      inp.style.height = 24.f;
      inp.onChange = [fbPtr](const std::string& v) {
        fbPtr->innerText = std::string("Value: ") + v;
        fbPtr->setDirty(false);
      };
    });
    sl->setFloatValue(50.f);

    mContent->add.div([](glint_component_style& spacer) {
      spacer.style.height = 16.f;
      spacer.style.width = "100%";
    });
  }

  // onChange / onSubmit feedback
  {
    mContent->add.div([](glint_component_style& lbl) {
      lbl.innerText = "onChange / onSubmit callbacks";
      lbl.style.color = glint_demo_theme::heading;
      lbl.style.fontSize = 14.f;
      lbl.style.width = "100%";
      lbl.style.textAlign = EAlign::Near;
      lbl.style.marginBottom = 4.f;
    });

    auto* inp = mContent->add.input([](glint_input& inp) {
      inp.type = "text";
      inp.placeholder = "Type and press Enter\xe2\x80\xa6";
      inp.style.width = "100%";
      inp.style.height = 36.f;
    });

    mContent->add.div([](glint_component_style& spacer) {
      spacer.style.height = 6.f;
      spacer.style.width = "100%";
    });

    auto* feedback = mContent->add.div([](glint_component_style& feedback) {
      feedback.innerText = "Last submitted: (none)";
      feedback.style.color = glint_demo_theme::muted;
      feedback.style.fontSize = 12.f;
      feedback.style.width = "100%";
      feedback.style.textAlign = EAlign::Near;
    });

    glint_element* fbPtr = feedback;
    inp->onChange  = [fbPtr](const std::string& v) {
      fbPtr->innerText = std::string("Value: ") + v;
      fbPtr->setDirty(false);
    };
    inp->onSubmit  = [fbPtr](const std::string& v) {
      fbPtr->innerText = std::string("Submitted: ") + v;
      fbPtr->style.color = glint_demo_theme::success;
      fbPtr->setDirty(false);
    };
  }
}
