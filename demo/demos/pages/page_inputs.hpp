#pragma once

#include <limits>
#include <memory>

inline void glint_demos_window::buildInputs()
{
  const bool compactLayout = isCompactLayout();

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

  auto normalizeSelectValue = [](const std::string& value) {
    return value == "None" ? std::string() : value;
  };

  auto displayAttrValue = [](const std::string& value, const char* fallback = nullptr) {
    if (!value.empty()) return value;
    if (fallback) return std::string("None (defaults to ") + fallback + ")";
    return std::string("None");
  };

  auto isButtonLikeType = [](const std::string& type) {
    return type == "button" || type == "submit" || type == "reset";
  };

  auto placeholderForType = [](const std::string& type) {
    if (type == "button" || type == "submit" || type == "reset" || type == "hidden") return std::string();
    if (type == "email") return std::string("user@example.com");
    if (type == "password") return std::string("Enter password\xe2\x80\xa6");
    if (type == "number") return std::string("42");
    if (type == "search") return std::string("Search query");
    if (type == "tel") return std::string("+1 555 123 4567");
    if (type == "url") return std::string("https://superkraft.io");
    return std::string("Try different combinations\xe2\x80\xa6");
  };

  auto makeOptions = [](std::initializer_list<const char*> values) {
    std::vector<std::string> options;
    options.reserve(values.size());
    for (const char* value : values)
      options.emplace_back(value);
    return options;
  };

  auto describeFormValues = [](const std::vector<glint_form_value>& values) {
    if (values.empty()) return std::string("(empty)");

    std::string summary;
    for (size_t i = 0; i < values.size(); ++i)
    {
      if (i) summary += " | ";
      summary += values[i].name;
      summary += '=';
      summary += values[i].value.empty() ? std::string("(empty)") : values[i].value;
    }
    return summary;
  };

  addHeading("Configurable input playground");
  addMeta("One input controlled by type and keyboard selects plus a few extra attribute toggles for quick verification.");

  auto* playgroundInput = mContent->add.input([](glint_input& inp) {
    inp.type = "text";
    inp.placeholder = "Try different combinations\xe2\x80\xa6";
    inp.style.width = "100%";
    inp.style.height = 36.f;
  });

  addNote("None on type behaves like an omitted HTML type attribute, which defaults to text; hidden removes the control from layout; button-like types reuse the same playground and route clicks through the action feedback below.");
  addSpacer(12.f);

  auto* selectorsRow = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "flex-start";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto* attributeRow = mContent->add.div([compactLayout](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "center";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto* traitRow = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "flex-start";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto* lengthRow = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "flex-start";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto* patternRow = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "column";
    row.style.alignItems = "stretch";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto* numberRuleRow = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "flex-start";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto currentType = std::make_shared<std::string>();
  auto currentInputmode = std::make_shared<std::string>();
  auto currentEnterkeyhint = std::make_shared<std::string>();
  auto currentAutocomplete = std::make_shared<std::string>();
  auto currentAutocapitalize = std::make_shared<std::string>();
  auto currentSpellcheck = std::make_shared<std::string>();
  auto currentMaxlength = std::make_shared<int>(-1);
  auto currentMinlength = std::make_shared<int>(-1);
  auto currentPattern = std::make_shared<std::string>();
  auto currentMin = std::make_shared<std::string>();
  auto currentMax = std::make_shared<std::string>();
  auto currentStep = std::make_shared<std::string>();
  auto currentRequired = std::make_shared<bool>(false);
  auto currentMultiple = std::make_shared<bool>(false);
  auto currentReadonly = std::make_shared<bool>(false);
  auto currentDisabled = std::make_shared<bool>(false);

  auto* configFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Current: type=None (defaults to text) | inputmode=None | enterkeyhint=None | autocomplete=None | autocapitalize=None | spellcheck=None | maxlength=None | minlength=None | pattern=None | min=None | max=None | step=None | required=false | multiple=false | readonly=false | disabled=false";
    feedback.style.color = glint_demo_theme::muted;
    feedback.style.fontSize = 12.f;
    feedback.style.width = "100%";
    feedback.style.textAlign = EAlign::Near;
    feedback.style.marginBottom = 6.f;
  });

  auto* valueFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Value: (empty)";
    feedback.style.color = glint_demo_theme::muted;
    feedback.style.fontSize = 12.f;
    feedback.style.width = "100%";
    feedback.style.textAlign = EAlign::Near;
    feedback.style.marginBottom = 6.f;
  });

  auto* submitFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Last submitted: (none)";
    feedback.style.color = glint_demo_theme::muted;
    feedback.style.fontSize = 12.f;
    feedback.style.width = "100%";
    feedback.style.textAlign = EAlign::Near;
  });

  auto* constraintFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Constraints: valid";
    feedback.style.color = glint_demo_theme::success;
    feedback.style.fontSize = 12.f;
    feedback.style.width = "100%";
    feedback.style.textAlign = EAlign::Near;
    feedback.style.marginTop = 6.f;
  });

  glint_element* configFeedbackPtr = configFeedback;
  glint_element* valueFeedbackPtr = valueFeedback;
  glint_element* submitFeedbackPtr = submitFeedback;
  glint_element* constraintFeedbackPtr = constraintFeedback;

  auto refreshValueFeedback = [=]() {
    const std::string value = playgroundInput->getValue();
    if (isButtonLikeType(playgroundInput->type))
      valueFeedbackPtr->innerText = value.empty() ? "Label: (empty)" : std::string("Label: ") + value;
    else
      valueFeedbackPtr->innerText = value.empty() ? "Value: (empty)" : std::string("Value: ") + value;
    valueFeedbackPtr->style.color = glint_demo_theme::muted;
    valueFeedbackPtr->setDirty(false);
  };

  auto refreshConstraintFeedback = [=]() {
    std::string message = "Constraints: valid";
    const char* color = glint_demo_theme::success;

    if (isButtonLikeType(playgroundInput->type))
    {
      message = "Constraints: not applicable to button-like types";
      color = glint_demo_theme::muted;
      constraintFeedbackPtr->innerText = std::move(message);
      constraintFeedbackPtr->style.color = color;
      constraintFeedbackPtr->setDirty(false);
      return;
    }

    if (playgroundInput->type == "number")
    {
      if (!playgroundInput->satisfiesRequired())
      {
        message = "Constraints: required value missing";
        color = glint_demo_theme::warning;
      }
      else if (!playgroundInput->hasValidNumberValue())
      {
        message = "Constraints: invalid number";
        color = glint_demo_theme::warning;
      }
      else if (!playgroundInput->satisfiesMinValue())
      {
        message = "Constraints: below min";
        color = glint_demo_theme::warning;
      }
      else if (!playgroundInput->satisfiesMaxValue())
      {
        message = "Constraints: above max";
        color = glint_demo_theme::warning;
      }
      else if (!playgroundInput->satisfiesStepValue())
      {
        message = "Constraints: step mismatch";
        color = glint_demo_theme::warning;
      }
    }
    else if (!playgroundInput->satisfiesRequired())
    {
      message = "Constraints: required value missing";
      color = glint_demo_theme::warning;
    }
    else if (playgroundInput->type == "email" && !playgroundInput->satisfiesEmailValue())
    {
      message = "Constraints: invalid email";
      color = glint_demo_theme::warning;
    }
    else if (playgroundInput->type == "url" && !playgroundInput->satisfiesUrlValue())
    {
      message = "Constraints: invalid url";
      color = glint_demo_theme::warning;
    }
    else if (!playgroundInput->satisfiesMinLength())
    {
      message = "Constraints: minlength not reached";
      color = glint_demo_theme::warning;
    }
    else if (!playgroundInput->satisfiesPattern())
    {
      message = "Constraints: pattern mismatch";
      color = glint_demo_theme::warning;
    }

    constraintFeedbackPtr->innerText = std::move(message);
    constraintFeedbackPtr->style.color = color;
    constraintFeedbackPtr->setDirty(false);
  };

  playgroundInput->onChange = [valueFeedbackPtr, refreshConstraintFeedback](const std::string& value) {
    valueFeedbackPtr->innerText = value.empty() ? "Value: (empty)" : std::string("Value: ") + value;
    valueFeedbackPtr->style.color = glint_demo_theme::muted;
    valueFeedbackPtr->setDirty(false);
    refreshConstraintFeedback();
  };

  playgroundInput->onClick = [playgroundInput, submitFeedbackPtr](const std::string& value) {
    if (playgroundInput->type == "submit") return;

    const bool isResetType = (playgroundInput->type == "reset");
    submitFeedbackPtr->innerText = value.empty()
      ? std::string(isResetType ? "Last clicked reset: (empty)" : "Last clicked button: (empty)")
      : std::string(isResetType ? "Last clicked reset: " : "Last clicked button: ") + value;
    submitFeedbackPtr->style.color = isResetType ? glint_demo_theme::warning : glint_demo_theme::success;
    submitFeedbackPtr->setDirty(false);
  };

  playgroundInput->onSubmit = [submitFeedbackPtr](const std::string& value) {
    submitFeedbackPtr->innerText = value.empty() ? "Last submitted: (empty)" : std::string("Last submitted: ") + value;
    submitFeedbackPtr->style.color = glint_demo_theme::success;
    submitFeedbackPtr->setDirty(false);
  };

  auto applyConfig = [=]() {
    const std::string resolvedType = currentType->empty() ? std::string("text") : *currentType;
    auto parseOptionalFloat = [](const std::string& value, float unsetValue) {
      if (value.empty() || value == "-" || value == "." || value == "-.") return unsetValue;
      try { return std::stof(value); } catch (...) { return unsetValue; }
    };

    playgroundInput->type = resolvedType;
    playgroundInput->inputmode = *currentInputmode;
    playgroundInput->enterkeyhint = *currentEnterkeyhint;
    playgroundInput->autocomplete = *currentAutocomplete;
    playgroundInput->autocapitalize = *currentAutocapitalize;
    playgroundInput->spellcheck = *currentSpellcheck;
    playgroundInput->maxlength = *currentMaxlength;
    playgroundInput->minlength = *currentMinlength;
    playgroundInput->pattern = *currentPattern;
    playgroundInput->min = parseOptionalFloat(*currentMin, std::numeric_limits<float>::lowest());
    playgroundInput->max = parseOptionalFloat(*currentMax, std::numeric_limits<float>::max());
    playgroundInput->step = parseOptionalFloat(*currentStep, 0.f);
    playgroundInput->required = *currentRequired;
    playgroundInput->multiple = *currentMultiple;
    playgroundInput->readonly = *currentReadonly;
    playgroundInput->disabled = *currentDisabled;
    playgroundInput->placeholder = placeholderForType(resolvedType);

    configFeedbackPtr->innerText = std::string("Current: type=")
                                + displayAttrValue(*currentType, "text")
                                + " | inputmode=" + displayAttrValue(*currentInputmode)
                                + " | enterkeyhint=" + displayAttrValue(*currentEnterkeyhint)
                                + " | autocomplete=" + displayAttrValue(*currentAutocomplete)
                                + " | autocapitalize=" + displayAttrValue(*currentAutocapitalize)
                                + " | spellcheck=" + displayAttrValue(*currentSpellcheck)
                                + " | maxlength=" + (*currentMaxlength >= 0 ? std::to_string(*currentMaxlength) : std::string("None"))
                                + " | minlength=" + (*currentMinlength >= 0 ? std::to_string(*currentMinlength) : std::string("None"))
                                + " | pattern=" + displayAttrValue(*currentPattern)
                                + " | min=" + displayAttrValue(*currentMin)
                                + " | max=" + displayAttrValue(*currentMax)
                                + " | step=" + displayAttrValue(*currentStep)
                                + " | required=" + (*currentRequired ? "true" : "false")
                                + " | multiple=" + (*currentMultiple ? "true" : "false")
                                + " | readonly=" + (*currentReadonly ? "true" : "false")
                                + " | disabled=" + (*currentDisabled ? "true" : "false");
    playgroundInput->setDirty(false);
    configFeedbackPtr->setDirty(false);
    refreshValueFeedback();
    refreshConstraintFeedback();
  };

  auto addLabeledSelect = [&](glint_element* rowTarget,
                              const char* label,
                              std::vector<std::string> options,
                              int selectedIndex,
                              std::function<void(const std::string&)> onValue) {
    auto* group = rowTarget->add.div([](glint_component_style& group) {
      group.style.display = "flex";
      group.style.flexDirection = "column";
      group.style.flexGrow = 1.f;
      group.style.minWidth = 0.f;
    });

    group->add.div([label](glint_component_style& heading) {
      heading.innerText = label;
      heading.style.color = glint_demo_theme::heading;
      heading.style.fontSize = 13.f;
      heading.style.width = "100%";
      heading.style.textAlign = EAlign::Near;
      heading.style.marginBottom = 6.f;
    });

    auto* select = group->add.fromClass<glint_select>([compactLayout, options = std::move(options), selectedIndex](glint_select& sel) mutable {
      sel.options = std::move(options);
      sel.selectedIndex = selectedIndex;
      sel.style.width = "100%";
      if (!compactLayout) sel.style.minWidth = 0.f;
      sel.style.height = 34.f;
      sel.style.backgroundColor = glint_demo_theme::surface;
      sel.style.color = glint_demo_theme::text;
      sel.style.borderRadius = 4.f;
      sel.style.borderWidth = 1.f;
      sel.style.borderColor = glint_demo_theme::border;
      sel.style.paddingLeft = 10.f;
      sel.style.fontSize = 13.f;
    });

    select->onChange = [onValue = std::move(onValue)](int /*idx*/, const std::string& value) {
      onValue(value);
    };
  };

  auto addLabeledNumberInput = [&](glint_element* rowTarget,
                                   const char* label,
                                   const char* placeholder,
                                   std::shared_ptr<int> currentValue) {
    auto* group = rowTarget->add.div([](glint_component_style& group) {
      group.style.display = "flex";
      group.style.flexDirection = "column";
      group.style.flexGrow = 1.f;
      group.style.minWidth = 0.f;
    });

    group->add.div([label](glint_component_style& heading) {
      heading.innerText = label;
      heading.style.color = glint_demo_theme::heading;
      heading.style.fontSize = 13.f;
      heading.style.width = "100%";
      heading.style.textAlign = EAlign::Near;
      heading.style.marginBottom = 6.f;
    });

    auto* input = group->add.input([=](glint_input& inp) {
      inp.type = "number";
      inp.placeholder = placeholder;
      inp.min = 0.f;
      inp.style.width = "100%";
      inp.style.height = 34.f;
      inp.style.backgroundColor = glint_demo_theme::surface;
      inp.style.color = glint_demo_theme::text;
      inp.style.borderRadius = 4.f;
      inp.style.borderWidth = 1.f;
      inp.style.borderColor = glint_demo_theme::border;
      inp.style.paddingLeft = 10.f;
      inp.style.fontSize = 13.f;
    });

    if (*currentValue >= 0)
      input->setValue(std::to_string(*currentValue));

    input->onChange = [currentValue, applyConfig](const std::string& value) {
      if (value.empty() || value == "-" || value == ".")
      {
        *currentValue = -1;
        applyConfig();
        return;
      }

      try
      {
        *currentValue = std::stoi(value);
        applyConfig();
      }
      catch (...)
      {
      }
    };
  };

  auto addBooleanCheckbox = [&](glint_element* rowTarget,
                                const char* label,
                                std::shared_ptr<bool> currentValue) {
    auto* checkbox = rowTarget->add.fromClass<glint_checkbox>([=](glint_checkbox& cb) {
      cb.text = label;
      cb.checked = *currentValue;
      cb.size = 16.f;
      cb.textCol = glint_demo_theme::text;
      cb.borderCol = glint_demo_theme::border;
      cb.boxBg = glint_demo_theme::surface;
      cb.checkedBg = glint_demo_theme::successBg;
      cb.checkmarkCol = glint_demo_theme::heading;
      cb.style.width = "fit-content";
    });

    checkbox->onChange = [currentValue, applyConfig](bool checked) {
      *currentValue = checked;
      applyConfig();
    };
  };

  auto addLabeledTextInput = [&](glint_element* rowTarget,
                                 const char* label,
                                 const char* placeholder,
                                 std::shared_ptr<std::string> currentValue) {
    auto* group = rowTarget->add.div([](glint_component_style& group) {
      group.style.display = "flex";
      group.style.flexDirection = "column";
      group.style.flexGrow = 1.f;
      group.style.minWidth = 0.f;
    });

    group->add.div([label](glint_component_style& heading) {
      heading.innerText = label;
      heading.style.color = glint_demo_theme::heading;
      heading.style.fontSize = 13.f;
      heading.style.width = "100%";
      heading.style.textAlign = EAlign::Near;
      heading.style.marginBottom = 6.f;
    });

    auto* input = group->add.input([=](glint_input& inp) {
      inp.type = "text";
      inp.placeholder = placeholder;
      inp.style.width = "100%";
      inp.style.height = 34.f;
      inp.style.backgroundColor = glint_demo_theme::surface;
      inp.style.color = glint_demo_theme::text;
      inp.style.borderRadius = 4.f;
      inp.style.borderWidth = 1.f;
      inp.style.borderColor = glint_demo_theme::border;
      inp.style.paddingLeft = 10.f;
      inp.style.fontSize = 13.f;
    });

    if (!currentValue->empty())
      input->setValue(*currentValue);

    input->onChange = [currentValue, applyConfig](const std::string& value) {
      *currentValue = value;
      applyConfig();
    };
  };

  auto addLabeledDecimalInput = [&](glint_element* rowTarget,
                                    const char* label,
                                    const char* placeholder,
                                    std::shared_ptr<std::string> currentValue) {
    auto* group = rowTarget->add.div([](glint_component_style& group) {
      group.style.display = "flex";
      group.style.flexDirection = "column";
      group.style.flexGrow = 1.f;
      group.style.minWidth = 0.f;
    });

    group->add.div([label](glint_component_style& heading) {
      heading.innerText = label;
      heading.style.color = glint_demo_theme::heading;
      heading.style.fontSize = 13.f;
      heading.style.width = "100%";
      heading.style.textAlign = EAlign::Near;
      heading.style.marginBottom = 6.f;
    });

    auto* input = group->add.input([=](glint_input& inp) {
      inp.type = "number";
      inp.placeholder = placeholder;
      inp.style.width = "100%";
      inp.style.height = 34.f;
      inp.style.backgroundColor = glint_demo_theme::surface;
      inp.style.color = glint_demo_theme::text;
      inp.style.borderRadius = 4.f;
      inp.style.borderWidth = 1.f;
      inp.style.borderColor = glint_demo_theme::border;
      inp.style.paddingLeft = 10.f;
      inp.style.fontSize = 13.f;
    });

    if (!currentValue->empty())
      input->setValue(*currentValue);

    input->onChange = [currentValue, applyConfig](const std::string& value) {
      *currentValue = value;
      applyConfig();
    };
  };

  addLabeledSelect(
    selectorsRow,
    "Type",
    makeOptions({ "None", "text", "email", "password", "number", "search", "tel", "url", "hidden", "button", "submit", "reset" }),
    0,
    [currentType, applyConfig, normalizeSelectValue](const std::string& value) {
      *currentType = normalizeSelectValue(value);
      applyConfig();
    });

  addLabeledSelect(
    selectorsRow,
    "Inputmode",
    makeOptions({ "None", "text", "decimal", "numeric", "tel", "search", "email", "url", "none" }),
    0,
    [currentInputmode, applyConfig, normalizeSelectValue](const std::string& value) {
      *currentInputmode = normalizeSelectValue(value);
      applyConfig();
    });

  addLabeledSelect(
    selectorsRow,
    "Enterkeyhint",
    makeOptions({ "None", "enter", "done", "go", "next", "search", "send" }),
    0,
    [currentEnterkeyhint, applyConfig, normalizeSelectValue](const std::string& value) {
      *currentEnterkeyhint = normalizeSelectValue(value);
      applyConfig();
    });

  addLabeledSelect(
    traitRow,
    "Autocapitalize",
    makeOptions({ "Unset", "none", "sentences", "words", "characters", "off", "on" }),
    0,
    [currentAutocapitalize, applyConfig](const std::string& value) {
      *currentAutocapitalize = value == "Unset" ? std::string() : value;
      applyConfig();
    });

  addLabeledSelect(
    traitRow,
    "Spellcheck",
    makeOptions({ "Unset", "true", "false" }),
    0,
    [currentSpellcheck, applyConfig](const std::string& value) {
      *currentSpellcheck = value == "Unset" ? std::string() : value;
      applyConfig();
    });

  addLabeledNumberInput(lengthRow, "Maxlength", "None", currentMaxlength);
  addLabeledNumberInput(lengthRow, "Minlength", "None", currentMinlength);
  addLabeledTextInput(patternRow, "Pattern", "e.g. [a-z]{3,8}", currentPattern);
  addLabeledTextInput(patternRow, "Autocomplete", "off / email / username / one-time-code", currentAutocomplete);
  addLabeledDecimalInput(numberRuleRow, "Min", "None", currentMin);
  addLabeledDecimalInput(numberRuleRow, "Max", "None", currentMax);
  addLabeledDecimalInput(numberRuleRow, "Step", "None", currentStep);

  addBooleanCheckbox(attributeRow, "Required", currentRequired);
  addBooleanCheckbox(attributeRow, "Multiple", currentMultiple);
  addBooleanCheckbox(attributeRow, "Readonly", currentReadonly);
  addBooleanCheckbox(attributeRow, "Disabled", currentDisabled);

  addSpacer(8.f);
  addNote("Use the controls to compare type and keyboard combinations, then layer on autocomplete, autocapitalize, spellcheck, maxlength, minlength, pattern, min, max, step, required, multiple, readonly, or disabled without duplicating the page.");

  addSpacer(24.f);
  addHeading("Form submit and reset");
  addMeta("This form uses a real form owner: pressing Enter in the first field submits through the form, submit serializes named controls, and reset restores the original default state.");

  auto* formStateFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Form state: valid defaults loaded";
    feedback.style.color = glint_demo_theme::success;
    feedback.style.fontSize = 12.f;
    feedback.style.width = "100%";
    feedback.style.textAlign = EAlign::Near;
    feedback.style.marginBottom = 6.f;
  });

  auto* formSubmitFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Form data: (none)";
    feedback.style.color = glint_demo_theme::muted;
    feedback.style.fontSize = 12.f;
    feedback.style.width = "100%";
    feedback.style.textAlign = EAlign::Near;
    feedback.style.marginBottom = 6.f;
  });

  glint_element* formStateFeedbackPtr = formStateFeedback;
  glint_element* formSubmitFeedbackPtr = formSubmitFeedback;
  glint_input* formNameInput = nullptr;

  mContent->add.custom<glint_form>([&](glint_form& form) {
    form.style.display = "flex";
    form.style.flexDirection = "column";
    form.style.gap = 10.f;
    form.style.width = "100%";
    form.style.padding = 12.f;
    form.style.backgroundColor = glint_demo_theme::surfaceHover;
    form.style.borderRadius = 8.f;
    form.style.borderWidth = 1.f;
    form.style.borderColor = glint_demo_theme::border;

    form.onSubmit = [formStateFeedbackPtr, formSubmitFeedbackPtr, describeFormValues](const std::vector<glint_form_value>& values,
                                                                                       glint_element* submitter) {
      std::string source = "Enter";
      if (auto* input = dynamic_cast<glint_input*>(submitter))
      {
        if (input->type == "submit")
          source = input->getValue().empty() ? std::string("submit") : input->getValue();
      }

      formStateFeedbackPtr->innerText = std::string("Form state: submitted via ") + source;
      formStateFeedbackPtr->style.color = glint_demo_theme::success;
      formStateFeedbackPtr->setDirty(false);

      formSubmitFeedbackPtr->innerText = std::string("Form data: ") + describeFormValues(values);
      formSubmitFeedbackPtr->style.color = values.empty() ? glint_demo_theme::muted : glint_demo_theme::success;
      formSubmitFeedbackPtr->setDirty(false);
      return true;
    };

    form.onReset = [formStateFeedbackPtr, formSubmitFeedbackPtr]() {
      formStateFeedbackPtr->innerText = "Form state: defaults restored";
      formStateFeedbackPtr->style.color = glint_demo_theme::warning;
      formStateFeedbackPtr->setDirty(false);

      formSubmitFeedbackPtr->innerText = "Form data: (reset)";
      formSubmitFeedbackPtr->style.color = glint_demo_theme::muted;
      formSubmitFeedbackPtr->setDirty(false);
    };

    form.add.div([](glint_component_style& heading) {
      heading.innerText = "Named fields";
      heading.style.color = glint_demo_theme::heading;
      heading.style.fontSize = 13.f;
      heading.style.width = "100%";
      heading.style.textAlign = EAlign::Near;
    });

    auto* fieldRow = form.add.div([compactLayout](glint_component_style& row) {
      row.style.display = "flex";
      row.style.flexDirection = compactLayout ? std::string("column") : std::string("row");
      row.style.alignItems = "stretch";
      row.style.gap = 10.f;
      row.style.width = "100%";
    });

    auto* nameGroup = fieldRow->add.div([](glint_component_style& group) {
      group.style.display = "flex";
      group.style.flexDirection = "column";
      group.style.flexGrow = 1.f;
      group.style.minWidth = 0.f;
    });

    nameGroup->add.div([](glint_component_style& label) {
      label.innerText = "Name (required)";
      label.style.color = glint_demo_theme::heading;
      label.style.fontSize = 12.f;
      label.style.width = "100%";
      label.style.textAlign = EAlign::Near;
      label.style.marginBottom = 6.f;
    });

    nameGroup->add.input([&](glint_input& inp) {
      formNameInput = &inp;
      inp.name = "name";
      inp.type = "text";
      inp.required = true;
      inp.placeholder = "Ada Lovelace";
      inp.style.width = "100%";
      inp.style.height = 34.f;
      inp.style.backgroundColor = glint_demo_theme::surface;
      inp.style.color = glint_demo_theme::text;
      inp.style.borderRadius = 4.f;
      inp.style.borderWidth = 1.f;
      inp.style.borderColor = glint_demo_theme::border;
      inp.style.paddingLeft = 10.f;
      inp.style.fontSize = 13.f;
      inp.setValue("Ada");
    });

    auto* roleGroup = fieldRow->add.div([](glint_component_style& group) {
      group.style.display = "flex";
      group.style.flexDirection = "column";
      group.style.flexGrow = 1.f;
      group.style.minWidth = 0.f;
    });

    roleGroup->add.div([](glint_component_style& label) {
      label.innerText = "Role";
      label.style.color = glint_demo_theme::heading;
      label.style.fontSize = 12.f;
      label.style.width = "100%";
      label.style.textAlign = EAlign::Near;
      label.style.marginBottom = 6.f;
    });

    roleGroup->add.fromClass<glint_select>([](glint_select& sel) {
      sel.name = "role";
      sel.options = { "Designer", "Engineer", "Producer" };
      sel.selectedIndex = 1;
      sel.style.width = "100%";
      sel.style.height = 34.f;
      sel.style.backgroundColor = glint_demo_theme::surface;
      sel.style.color = glint_demo_theme::text;
      sel.style.borderRadius = 4.f;
      sel.style.borderWidth = 1.f;
      sel.style.borderColor = glint_demo_theme::border;
      sel.style.paddingLeft = 10.f;
      sel.style.fontSize = 13.f;
    });

    auto* notesGroup = form.add.div([](glint_component_style& group) {
      group.style.display = "flex";
      group.style.flexDirection = "column";
      group.style.width = "100%";
    });

    notesGroup->add.div([](glint_component_style& label) {
      label.innerText = "Notes";
      label.style.color = glint_demo_theme::heading;
      label.style.fontSize = 12.f;
      label.style.width = "100%";
      label.style.textAlign = EAlign::Near;
      label.style.marginBottom = 6.f;
    });

    notesGroup->add.fromClass<glint_textarea>([](glint_textarea& ta) {
      ta.name = "notes";
      ta.placeholder = "Add context for the submission";
      ta.style.width = "100%";
      ta.style.height = 88.f;
      ta.style.backgroundColor = glint_demo_theme::surface;
      ta.style.color = glint_demo_theme::text;
      ta.style.borderRadius = 4.f;
      ta.style.borderWidth = 1.f;
      ta.style.borderColor = glint_demo_theme::border;
      ta.style.padding = 10.f;
      ta.style.fontSize = 13.f;
      ta.setValue("Ships Friday.");
    });

    form.add.input([](glint_input& inp) {
      inp.name = "updates";
      inp.type = "checkbox";
      inp.text = "Email me release notes";
      inp.value = "yes";
      inp.checked = true;
      inp.style.width = "fit-content";
      inp.style.height = 22.f;
    });

    auto* actionRow = form.add.div([compactLayout](glint_component_style& row) {
      row.style.display = "flex";
      row.style.flexDirection = compactLayout ? std::string("column") : std::string("row");
      row.style.alignItems = "stretch";
      row.style.gap = 10.f;
      row.style.width = "100%";
    });

    actionRow->add.input([](glint_input& inp) {
      inp.name = "submit-action";
      inp.type = "submit";
      inp.style.width = "fit-content";
      inp.style.height = 34.f;
      inp.setValue("Submit form");
    });

    actionRow->add.input([](glint_input& inp) {
      inp.type = "reset";
      inp.style.width = "fit-content";
      inp.style.height = 34.f;
      inp.setValue("Reset defaults");
    });

    form.add.div([](glint_component_style& note) {
      note.innerText = "Clear the required name field to block submission, press Enter in that field to submit through the form owner, or use reset to restore the original defaults.";
      note.style.color = glint_demo_theme::muted;
      note.style.fontSize = 12.f;
      note.style.width = "100%";
      note.style.textAlign = EAlign::Near;
    });
  });

  auto refreshFormState = [formNameInput, formStateFeedbackPtr]() {
    if (!formNameInput) return;
    if (formNameInput->satisfiesConstraints())
    {
      formStateFeedbackPtr->innerText = "Form state: ready to submit";
      formStateFeedbackPtr->style.color = glint_demo_theme::success;
    }
    else
    {
      formStateFeedbackPtr->innerText = "Form state: required name missing";
      formStateFeedbackPtr->style.color = glint_demo_theme::warning;
    }
    formStateFeedbackPtr->setDirty(false);
  };

  if (formNameInput)
    formNameInput->onChange = [refreshFormState](const std::string&) { refreshFormState(); };

  refreshFormState();

  applyConfig();
}
