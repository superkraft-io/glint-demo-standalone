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

  auto placeholderForType = [](const std::string& type) {
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

  addHeading("Configurable input playground");
  addMeta("One input controlled by type and keyboard selects plus a few extra attribute toggles for quick verification.");

  auto* playgroundInput = mContent->add.input([](glint_input& inp) {
    inp.type = "text";
    inp.placeholder = "Try different combinations\xe2\x80\xa6";
    inp.style.width = "100%";
    inp.style.height = 36.f;
  });

  addNote("This playground is for text-editable input combinations. None on type behaves like an omitted HTML type attribute, which defaults to text.");
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
  auto currentMaxlength = std::make_shared<int>(-1);
  auto currentMinlength = std::make_shared<int>(-1);
  auto currentPattern = std::make_shared<std::string>();
  auto currentMin = std::make_shared<std::string>();
  auto currentMax = std::make_shared<std::string>();
  auto currentStep = std::make_shared<std::string>();
  auto currentRequired = std::make_shared<bool>(false);
  auto currentReadonly = std::make_shared<bool>(false);
  auto currentDisabled = std::make_shared<bool>(false);

  auto* configFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Current: type=None (defaults to text) | inputmode=None | enterkeyhint=None | maxlength=None | minlength=None | pattern=None | min=None | max=None | step=None | required=false | readonly=false | disabled=false";
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

  auto refreshConstraintFeedback = [=]() {
    std::string message = "Constraints: valid";
    const char* color = glint_demo_theme::success;

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
    playgroundInput->maxlength = *currentMaxlength;
    playgroundInput->minlength = *currentMinlength;
    playgroundInput->pattern = *currentPattern;
    playgroundInput->min = parseOptionalFloat(*currentMin, std::numeric_limits<float>::lowest());
    playgroundInput->max = parseOptionalFloat(*currentMax, std::numeric_limits<float>::max());
    playgroundInput->step = parseOptionalFloat(*currentStep, 0.f);
    playgroundInput->required = *currentRequired;
    playgroundInput->readonly = *currentReadonly;
    playgroundInput->disabled = *currentDisabled;
    playgroundInput->placeholder = placeholderForType(resolvedType);

    configFeedbackPtr->innerText = std::string("Current: type=")
                                + displayAttrValue(*currentType, "text")
                                + " | inputmode=" + displayAttrValue(*currentInputmode)
                                + " | enterkeyhint=" + displayAttrValue(*currentEnterkeyhint)
                                + " | maxlength=" + (*currentMaxlength >= 0 ? std::to_string(*currentMaxlength) : std::string("None"))
                                + " | minlength=" + (*currentMinlength >= 0 ? std::to_string(*currentMinlength) : std::string("None"))
                                + " | pattern=" + displayAttrValue(*currentPattern)
                                + " | min=" + displayAttrValue(*currentMin)
                                + " | max=" + displayAttrValue(*currentMax)
                                + " | step=" + displayAttrValue(*currentStep)
                                + " | required=" + (*currentRequired ? "true" : "false")
                                + " | readonly=" + (*currentReadonly ? "true" : "false")
                                + " | disabled=" + (*currentDisabled ? "true" : "false");
    playgroundInput->setDirty(false);
    configFeedbackPtr->setDirty(false);
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
    makeOptions({ "None", "text", "email", "password", "number", "search", "tel", "url" }),
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

  addLabeledNumberInput(lengthRow, "Maxlength", "None", currentMaxlength);
  addLabeledNumberInput(lengthRow, "Minlength", "None", currentMinlength);
  addLabeledTextInput(patternRow, "Pattern", "e.g. [a-z]{3,8}", currentPattern);
  addLabeledDecimalInput(numberRuleRow, "Min", "None", currentMin);
  addLabeledDecimalInput(numberRuleRow, "Max", "None", currentMax);
  addLabeledDecimalInput(numberRuleRow, "Step", "None", currentStep);

  addBooleanCheckbox(attributeRow, "Required", currentRequired);
  addBooleanCheckbox(attributeRow, "Readonly", currentReadonly);
  addBooleanCheckbox(attributeRow, "Disabled", currentDisabled);

  addSpacer(8.f);
  addNote("Use the controls to compare type and keyboard combinations, then layer on maxlength, minlength, pattern, min, max, step, required, readonly, or disabled without duplicating the page.");

  applyConfig();
}
