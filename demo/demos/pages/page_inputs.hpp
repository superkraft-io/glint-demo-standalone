#pragma once

inline void glint_demos_window::buildInputs()
{
  auto addHeading = [&](const char* text, float marginBottom = 8.f) {
    mContent->add.div([=](glint_component_style& lbl) {
      lbl.innerText = text;
      lbl.style.color = glint_demo_theme::heading;
      lbl.style.fontSize = 15.f;
      lbl.style.width = "100%";
      lbl.style.textAlign = EAlign::Near;
      lbl.style.marginBottom = marginBottom;
    });
  };

  auto addMeta = [&](std::string text) {
    mContent->add.div([text = std::move(text)](glint_component_style& meta) {
      meta.innerText = text;
      meta.style.color = glint_demo_theme::subtle;
      meta.style.fontSize = 12.f;
      meta.style.width = "100%";
      meta.style.textAlign = EAlign::Near;
      meta.style.marginBottom = 6.f;
    });
  };

  auto addNote = [&](const char* text) {
    mContent->add.div([=](glint_component_style& note) {
      note.innerText = text;
      note.style.color = glint_demo_theme::muted;
      note.style.fontSize = 12.f;
      note.style.width = "100%";
      note.style.textAlign = EAlign::Near;
      note.style.marginTop = 6.f;
    });
  };

  auto addSpacer = [&](float height) {
    mContent->add.div([=](glint_component_style& spacer) {
      spacer.style.height = height;
      spacer.style.width = "100%";
    });
  };

  auto addInputDemo = [&](const char* heading,
                          const char* type,
                          const char* inputmode,
                          const char* placeholder,
                          const char* note) {
    addHeading(heading, 4.f);

    std::string meta = std::string("type=\"") + type + "\"";
    if (inputmode && inputmode[0] != '\0')
      meta += std::string("  |  inputmode=\"") + inputmode + "\"";
    addMeta(meta);

    mContent->add.input([=](glint_input& inp) {
      inp.type = type;
      inp.inputmode = inputmode ? inputmode : "";
      inp.placeholder = placeholder;
      inp.style.width = "100%";
      inp.style.height = 36.f;
    });

    addNote(note);
    addSpacer(14.f);
  };

  addHeading("Supported input types");

  addInputDemo(
    "Text input",
    "text",
    "text",
    "Enter text here\xe2\x80\xa6",
    "Baseline single-line text editing.");

  addInputDemo(
    "Number input",
    "number",
    "decimal",
    "0.0",
    "Numeric parsing and clamping come from type=\"number\"; the keyboard hint is secondary.");

  addInputDemo(
    "Password input",
    "password",
    "text",
    "Enter password\xe2\x80\xa6",
    "Secure-entry masking stays tied to the password type.");

  addInputDemo(
    "Email input",
    "email",
    "email",
    "user@example.com",
    "Email semantics are controlled by the type, not by inputmode.");

  addSpacer(6.f);

  addHeading("Type and inputmode mapping");

  addInputDemo(
    "Text with numeric keyboard hint",
    "text",
    "numeric",
    "12345",
    "Shows text semantics with a numeric keyboard hint on mobile.");

  addInputDemo(
    "Text with decimal keyboard hint",
    "text",
    "decimal",
    "12.34",
    "Useful for soft-keyboard tuning without switching to number parsing rules.");

  addInputDemo(
    "Text with search keyboard hint",
    "text",
    "search",
    "Search terms",
    "Still a plain text field; inputmode only adjusts the virtual keyboard hint.");

  addInputDemo(
    "Email type with conflicting inputmode",
    "email",
    "numeric",
    "user@example.com",
    "Demonstrates email validation semantics with an inputmode=\"numeric\" number pad keyboard hint.");

  addInputDemo(
    "Number type with conflicting inputmode",
    "number",
    "text",
    "42",
    "Demonstrates number parsing semantics with an inputmode=\"text\" standard text keyboard hint.");

  addInputDemo(
    "Text with no software keyboard",
    "text",
    "none",
    "Hardware keyboard / custom input only",
    "On iOS this suppresses the software keyboard while preserving focus.");

  addSpacer(6.f);

  addHeading("Fallback type showcases");

  addInputDemo(
    "Search type fallback",
    "search",
    "search",
    "Search fallback",
    "Currently uses the text delegate, with iOS keyboard traits derived from the type/inputmode hint.");

  addInputDemo(
    "Telephone type fallback",
    "tel",
    "tel",
    "+1 555 123 4567",
    "Currently uses the text delegate, with a phone-style keyboard hint on iOS.");

  addInputDemo(
    "URL type fallback",
    "url",
    "url",
    "https://superkraft.io",
    "Currently uses the text delegate, with a URL-style keyboard hint on iOS.");

  addSpacer(4.f);

  // Range slider
  {
    addHeading("Range slider (type=\"range\")");
    addMeta("type=\"range\"  |  inputmode is not applicable");

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

    addNote("Range keeps its slider behavior instead of acting like a text-editable control.");
    addSpacer(16.f);
  }

  // onChange / onSubmit feedback
  {
    addHeading("onChange / onSubmit callbacks", 4.f);
    addMeta("type=\"text\"  |  inputmode=\"search\"");

    auto* inp = mContent->add.input([](glint_input& inp) {
      inp.type = "text";
      inp.inputmode = "search";
      inp.placeholder = "Type and press Enter\xe2\x80\xa6";
      inp.style.width = "100%";
      inp.style.height = 36.f;
    });

    addSpacer(6.f);

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

    addNote("Use this field to test change vs submit behavior with a non-default keyboard hint.");
  }
}
