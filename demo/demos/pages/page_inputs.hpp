#pragma once

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
  addMeta("One input controlled by three selects. Each menu starts with None to represent an omitted attribute.");

  auto* playgroundInput = mContent->add.input([](glint_input& inp) {
    inp.type = "text";
    inp.placeholder = "Try different combinations\xe2\x80\xa6";
    inp.style.width = "100%";
    inp.style.height = 36.f;
  });

  addNote("This playground is for text-editable input combinations. None on type behaves like an omitted HTML type attribute, which defaults to text.");
  addSpacer(12.f);

  auto* selectorsRow = mContent->add.div([compactLayout](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = compactLayout ? "column" : "row";
    row.style.alignItems = compactLayout ? "stretch" : "flex-start";
    row.style.gap = 12.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto currentType = std::make_shared<std::string>();
  auto currentInputmode = std::make_shared<std::string>();
  auto currentEnterkeyhint = std::make_shared<std::string>();

  auto* configFeedback = mContent->add.div([](glint_component_style& feedback) {
    feedback.innerText = "Current: type=None (defaults to text) | inputmode=None | enterkeyhint=None";
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

  glint_element* configFeedbackPtr = configFeedback;
  glint_element* valueFeedbackPtr = valueFeedback;
  glint_element* submitFeedbackPtr = submitFeedback;

  playgroundInput->onChange = [valueFeedbackPtr](const std::string& value) {
    valueFeedbackPtr->innerText = value.empty() ? "Value: (empty)" : std::string("Value: ") + value;
    valueFeedbackPtr->style.color = glint_demo_theme::muted;
    valueFeedbackPtr->setDirty(false);
  };

  playgroundInput->onSubmit = [submitFeedbackPtr](const std::string& value) {
    submitFeedbackPtr->innerText = value.empty() ? "Last submitted: (empty)" : std::string("Last submitted: ") + value;
    submitFeedbackPtr->style.color = glint_demo_theme::success;
    submitFeedbackPtr->setDirty(false);
  };

  auto applyConfig = [=]() {
    const std::string resolvedType = currentType->empty() ? std::string("text") : *currentType;
    playgroundInput->type = resolvedType;
    playgroundInput->inputmode = *currentInputmode;
    playgroundInput->enterkeyhint = *currentEnterkeyhint;
    playgroundInput->placeholder = placeholderForType(resolvedType);

    configFeedbackPtr->innerText = std::string("Current: type=")
                                + displayAttrValue(*currentType, "text")
                                + " | inputmode=" + displayAttrValue(*currentInputmode)
                                + " | enterkeyhint=" + displayAttrValue(*currentEnterkeyhint);
    playgroundInput->setDirty(false);
    configFeedbackPtr->setDirty(false);
  };

  auto addLabeledSelect = [&](const char* label,
                              std::vector<std::string> options,
                              int selectedIndex,
                              std::function<void(const std::string&)> onValue) {
    auto* group = selectorsRow->add.div([](glint_component_style& group) {
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

  addLabeledSelect(
    "Type",
    makeOptions({ "None", "text", "email", "password", "number", "search", "tel", "url" }),
    0,
    [currentType, applyConfig, normalizeSelectValue](const std::string& value) {
      *currentType = normalizeSelectValue(value);
      applyConfig();
    });

  addLabeledSelect(
    "Inputmode",
    makeOptions({ "None", "text", "decimal", "numeric", "tel", "search", "email", "url", "none" }),
    0,
    [currentInputmode, applyConfig, normalizeSelectValue](const std::string& value) {
      *currentInputmode = normalizeSelectValue(value);
      applyConfig();
    });

  addLabeledSelect(
    "Enterkeyhint",
    makeOptions({ "None", "enter", "done", "go", "next", "search", "send" }),
    0,
    [currentEnterkeyhint, applyConfig, normalizeSelectValue](const std::string& value) {
      *currentEnterkeyhint = normalizeSelectValue(value);
      applyConfig();
    });

  addSpacer(8.f);
  addNote("Use the three menus to compare combinations like type=None + enterkeyhint=search, or type=text + inputmode=search + enterkeyhint=search, without duplicating the page.");

  applyConfig();
}
