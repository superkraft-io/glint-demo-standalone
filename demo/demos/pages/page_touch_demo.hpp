#pragma once

#include <algorithm>
#include <array>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace {
inline const char* glint_touch_demo_phase_label(glint_input_phase phase)
{
  switch (phase)
  {
  case glint_input_phase::may_begin: return "may-begin";
  case glint_input_phase::began:     return "began";
  case glint_input_phase::changed:   return "changed";
  case glint_input_phase::ended:     return "ended";
  case glint_input_phase::cancelled: return "cancelled";
  case glint_input_phase::none:
  default:                           return "none";
  }
}

inline const char* glint_touch_demo_gesture_label(glint_gesture_kind kind)
{
  switch (kind)
  {
  case glint_gesture_kind::pinch:      return "pinch";
  case glint_gesture_kind::rotate:     return "rotate";
  case glint_gesture_kind::swipe:      return "swipe";
  case glint_gesture_kind::smart_zoom: return "smart-zoom";
  case glint_gesture_kind::none:
  default:                             return "lifecycle";
  }
}

inline float glint_touch_demo_clamp(float value, float lo, float hi)
{
  return std::max(lo, std::min(value, hi));
}
}

inline void glint_demos_window::buildTouchDemo()
{
  struct TouchDemoState
  {
    int wheelCount = 0;
    int gestureCount = 0;
    float wheelTotalX = 0.f;
    float wheelTotalY = 0.f;
    float scale = 1.f;
    float rotationDeg = 0.f;
    float offsetX = 0.f;
    float offsetY = 0.f;
    std::string lastWheelPhase = "none";
    std::string lastMomentumPhase = "none";
    std::string lastGestureKind = "none";
    std::string lastGesturePhase = "none";
    std::vector<std::string> logs = {
      "Touch lab ready. Use two-finger scroll, pinch, rotate, or swipe on the stage."
    };
  };

  auto state = std::make_shared<TouchDemoState>();
  auto metricValues = std::make_shared<std::array<glint_element*, 4>>();
  auto logRows = std::make_shared<std::array<glint_element*, 10>>();
  metricValues->fill(nullptr);
  logRows->fill(nullptr);

  glint_button* resetBtn = nullptr;
  glint_element* tilePtr = nullptr;
  glint_element* captionPtr = nullptr;

  auto addMetricCard = [metricValues](glint_element* parent,
                                      int idx,
                                      const char* title,
                                      const char* initialValue,
                                      const char* accent) {
    parent->add.div([metricValues, idx, title, initialValue, accent](glint_component_style& card) {
      card.style.flexGrow = 1.f;
      card.style.height = 86.f;
      card.style.backgroundColor = glint_demo_theme::surfaceAlt;
      card.style.borderColor = glint_demo_theme::border;
      card.style.borderWidth = 1.f;
      card.style.borderRadius = 12.f;
      card.style.padding = "12 14";
      card.style.display = "flex";
      card.style.flexDirection = "column";
      card.style.justifyContent = "space-between";

      card.add.div([title, accent](glint_component_style& label) {
        label.innerText = title;
        label.style.color = accent;
        label.style.fontSize = 11.f;
        label.style.width = "100%";
        label.style.textAlign = EAlign::Near;
      });

      card.add.div([initialValue](glint_component_style& value) {
        value.innerText = initialValue;
        value.style.color = glint_demo_theme::text;
        value.style.fontSize = 12.f;
        value.style.width = "100%";
        value.style.textAlign = EAlign::Near;
      }, &(*metricValues)[idx]);
    });
  };

  auto* headerRow = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "center";
    row.style.gap = 16.f;
    row.style.width = "100%";
    row.style.marginBottom = 16.f;
  });

  headerRow->add.div([](glint_component_style& titleWrap) {
    titleWrap.style.flexGrow = 1.f;
    titleWrap.style.display = "flex";
    titleWrap.style.flexDirection = "column";
    titleWrap.style.gap = 4.f;

    titleWrap.add.div([](glint_component_style& title) {
      title.innerText = "Touch Events Lab";
      title.style.color = glint_demo_theme::heading;
      title.style.fontSize = 18.f;
      title.style.width = "100%";
      title.style.textAlign = EAlign::Near;
    });

    titleWrap.add.div([](glint_component_style& subtitle) {
      subtitle.innerText = "Trackpad wheel and gesture events routed through Glint's DOM-style event system.";
      subtitle.style.color = glint_demo_theme::muted;
      subtitle.style.fontSize = 12.f;
      subtitle.style.width = "100%";
      subtitle.style.textAlign = EAlign::Near;
    });
  });

  headerRow->add.fromClass<glint_button>([](glint_button& btn) {
    btn.innerText = "Reset";
    btn.style.width = 92.f;
    btn.style.height = 34.f;
    btn.style.backgroundColor = glint_demo_theme::accent;
    btn.style.borderColor = glint_demo_theme::accent;
    btn.style.borderWidth = 1.f;
    btn.style.borderRadius = 8.f;
    btn.style.color = glint_demo_theme::heading;
    btn.style.fontSize = 12.f;
    btn.hover.backgroundColor = glint_demo_theme::accentHover;
    btn.hover.borderColor = glint_demo_theme::accentHover;
    btn.hover.borderWidth = 1.f;
    btn.hover.borderRadius = 8.f;
    btn.hover.color = glint_demo_theme::heading;
    btn.pressed.backgroundColor = glint_demo_theme::accent;
    btn.pressed.borderColor = glint_demo_theme::accent;
    btn.pressed.borderWidth = 1.f;
    btn.pressed.borderRadius = 8.f;
    btn.pressed.color = glint_demo_theme::heading;
  }, &resetBtn);

  auto* mainRow = mContent->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems = "stretch";
    row.style.gap = 16.f;
    row.style.width = "100%";
    row.style.marginBottom = 12.f;
  });

  auto* stageCol = mainRow->add.div([](glint_component_style& col) {
    col.style.flexGrow = 1.f;
    col.style.display = "flex";
    col.style.flexDirection = "column";
    col.style.gap = 12.f;
  });

  auto* logCol = mainRow->add.div([](glint_component_style& col) {
    col.style.width = 290.f;
    col.style.display = "flex";
    col.style.flexDirection = "column";
    col.style.gap = 8.f;
  });

  auto* metricRow1 = stageCol->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.gap = 12.f;
    row.style.width = "100%";
  });
  addMetricCard(metricRow1, 0, "Wheel", "dx 0.0   dy 0.0   phase none / none", glint_demo_theme::accent);
  addMetricCard(metricRow1, 1, "Gesture", "none / none", glint_demo_theme::warning);

  auto* metricRow2 = stageCol->add.div([](glint_component_style& row) {
    row.style.display = "flex";
    row.style.flexDirection = "row";
    row.style.gap = 12.f;
    row.style.width = "100%";
  });
  addMetricCard(metricRow2, 2, "Transform", "scale 1.00   rotate 0.0   offset 0.0,0.0", glint_demo_theme::success);
  addMetricCard(metricRow2, 3, "Counters", "wheel 0   gesture 0", glint_demo_theme::muted);

  stageCol->add.div([](glint_component_style& sectionLabel) {
    sectionLabel.innerText = "Interaction stage";
    sectionLabel.style.color = glint_demo_theme::muted;
    sectionLabel.style.fontSize = 12.f;
    sectionLabel.style.width = "100%";
    sectionLabel.style.textAlign = EAlign::Near;
  });

  glint_element* stagePtr = stageCol->add.div([](glint_component_style& stage) {
    stage.style.width = "100%";
    stage.style.height = 336.f;
    stage.style.backgroundColor = "#142235";
    stage.style.borderColor = "#29496d";
    stage.style.borderWidth = 1.f;
    stage.style.borderRadius = 18.f;
    stage.style.padding = "18";
    stage.style.display = "flex";
    stage.style.flexDirection = "column";
    stage.style.alignItems = "center";
    stage.style.justifyContent = "center";
    stage.style.gap = 20.f;
    stage.style.overflow = "hidden";
  });

  stagePtr->add.div([](glint_component_style& hint) {
    hint.innerText = "Scroll to drift. Pinch to scale. Rotate to spin. Swipe to nudge. Smart zoom to snap.";
    hint.style.color = "#c9ddf8";
    hint.style.fontSize = 12.f;
    hint.style.width = "100%";
    hint.style.textAlign = EAlign::Center;
  });

  auto* tileWrap = stagePtr->add.div([](glint_component_style& wrap) {
    wrap.style.flexGrow = 1.f;
    wrap.style.width = "100%";
    wrap.style.display = "flex";
    wrap.style.alignItems = "center";
    wrap.style.justifyContent = "center";
  });

  tileWrap->add.div([](glint_component_style& tile) {
    tile.innerText = "Gesture target";
    tile.style.width = 176.f;
    tile.style.height = 176.f;
    tile.style.backgroundColor = "#d8ebff";
    tile.style.color = "#16314f";
    tile.style.borderColor = "#ffffff";
    tile.style.borderWidth = 1.f;
    tile.style.borderRadius = 28.f;
    tile.style.display = "flex";
    tile.style.alignItems = "center";
    tile.style.justifyContent = "center";
    tile.style.fontSize = 16.f;
    tile.style.transform = "translate(0px, 0px) rotate(0deg) scale(1.0)";
  }, &tilePtr);

  stagePtr->add.div([](glint_component_style& caption) {
    caption.innerText = "kind none | wheel 0 | gesture 0";
    caption.style.color = "#dbe8f7";
    caption.style.fontSize = 12.f;
    caption.style.width = "100%";
    caption.style.textAlign = EAlign::Center;
  }, &captionPtr);

  logCol->add.div([](glint_component_style& title) {
    title.innerText = "Event Stream";
    title.style.color = glint_demo_theme::heading;
    title.style.fontSize = 15.f;
    title.style.width = "100%";
    title.style.textAlign = EAlign::Near;
  });

  logCol->add.div([](glint_component_style& hint) {
    hint.innerText = "Newest events appear first. Use this to verify deltas, kinds, and phase lifecycles.";
    hint.style.color = glint_demo_theme::muted;
    hint.style.fontSize = 11.f;
    hint.style.width = "100%";
    hint.style.textAlign = EAlign::Near;
  });

  auto* logList = logCol->add.div([](glint_component_style& list) {
    list.style.width = "100%";
    list.style.backgroundColor = glint_demo_theme::surfaceAlt;
    list.style.borderColor = glint_demo_theme::border;
    list.style.borderWidth = 1.f;
    list.style.borderRadius = 12.f;
    list.style.padding = "8";
    list.style.display = "flex";
    list.style.flexDirection = "column";
    list.style.gap = 6.f;
  });

  for (int i = 0; i < 10; ++i)
  {
    logList->add.div([i](glint_component_style& row) {
      row.innerText = i == 0 ? "Touch lab ready. Use two-finger scroll, pinch, rotate, or swipe on the stage." : "";
      row.style.minHeight = 30.f;
      row.style.backgroundColor = i == 0 ? glint_demo_theme::panelBg : "#00000000";
      row.style.borderRadius = 8.f;
      row.style.padding = "7 8";
      row.style.color = i == 0 ? glint_demo_theme::text : glint_demo_theme::subtle;
      row.style.fontSize = 11.f;
      row.style.width = "100%";
      row.style.textAlign = EAlign::Near;
    }, &(*logRows)[i]);
  }

  auto refresh = std::make_shared<std::function<void()>>();
  auto pushLog = std::make_shared<std::function<void(const std::string&)>>();

  *refresh = [this, state, metricValues, logRows, tilePtr, captionPtr]() {
    char wheelBuf[192];
    char transformBuf[192];
    char counterBuf[64];
    char tileBuf[160];

    std::snprintf(wheelBuf,
                  sizeof(wheelBuf),
                  "dx %.1f   dy %.1f   phase %s / %s",
                  state->wheelTotalX,
                  state->wheelTotalY,
                  state->lastWheelPhase.c_str(),
                  state->lastMomentumPhase.c_str());

    std::snprintf(transformBuf,
                  sizeof(transformBuf),
                  "scale %.2f   rotate %.1f   offset %.1f,%.1f",
                  state->scale,
                  state->rotationDeg,
                  state->offsetX,
                  state->offsetY);

    std::snprintf(counterBuf,
                  sizeof(counterBuf),
                  "wheel %d   gesture %d",
                  state->wheelCount,
                  state->gestureCount);

    std::snprintf(tileBuf,
                  sizeof(tileBuf),
                  "translate(%.1fpx, %.1fpx) rotate(%.1fdeg) scale(%.3f)",
                  state->offsetX,
                  state->offsetY,
                  state->rotationDeg,
                  state->scale);

    (*metricValues)[0]->innerText = wheelBuf;
    (*metricValues)[1]->innerText = state->lastGestureKind + " / " + state->lastGesturePhase;
    (*metricValues)[2]->innerText = transformBuf;
    (*metricValues)[3]->innerText = counterBuf;

    for (glint_element* metric : *metricValues)
    {
      if (!metric) continue;
      metric->setDirty(false);
    }

    tilePtr->style.transform = tileBuf;
    tilePtr->setDirty(false);

    captionPtr->innerText = std::string("kind ") + state->lastGestureKind
                          + " | wheel " + std::to_string(state->wheelCount)
                          + " | gesture " + std::to_string(state->gestureCount);
    captionPtr->setDirty(false);

    for (int i = 0; i < static_cast<int>(logRows->size()); ++i)
    {
      glint_element* row = (*logRows)[i];
      if (!row) continue;

      const int revIdx = static_cast<int>(state->logs.size()) - 1 - i;
      if (revIdx >= 0)
      {
        row->innerText = state->logs[revIdx];
        row->style.color = (i == 0) ? glint_demo_theme::text : glint_demo_theme::muted;
        row->style.backgroundColor = (i == 0) ? glint_demo_theme::panelBg : "#00000000";
      }
      else
      {
        row->innerText = "";
        row->style.color = glint_demo_theme::subtle;
        row->style.backgroundColor = "#00000000";
      }
      row->setDirty(false);
    }

    mContent->setDirty(false);
  };

  *pushLog = [state, refresh](const std::string& message) {
    state->logs.push_back(message);
    while (state->logs.size() > 10)
      state->logs.erase(state->logs.begin());
    (*refresh)();
  };

  resetBtn->onClick = [state, pushLog]() {
    state->wheelCount = 0;
    state->gestureCount = 0;
    state->wheelTotalX = 0.f;
    state->wheelTotalY = 0.f;
    state->scale = 1.f;
    state->rotationDeg = 0.f;
    state->offsetX = 0.f;
    state->offsetY = 0.f;
    state->lastWheelPhase = "none";
    state->lastMomentumPhase = "none";
    state->lastGestureKind = "none";
    state->lastGesturePhase = "none";
    state->logs.clear();
    (*pushLog)("Touch lab reset.");
  };

  stagePtr->element.addEventListener("wheel", [state, pushLog](glint_event& e) {
    auto& we = static_cast<glint_wheel_event&>(e);
    we.preventDefault();

    state->wheelCount += 1;
    state->wheelTotalX += we.deltaX;
    state->wheelTotalY += we.deltaY;
    state->lastWheelPhase = glint_touch_demo_phase_label(we.phase);
    state->lastMomentumPhase = glint_touch_demo_phase_label(we.momentumPhase);
    state->offsetX = glint_touch_demo_clamp(state->offsetX + we.deltaX * 0.12f, -140.f, 140.f);
    state->offsetY = glint_touch_demo_clamp(state->offsetY - we.deltaY * 0.12f, -140.f, 140.f);

    char buf[256];
    std::snprintf(buf,
                  sizeof(buf),
                  "wheel dx=%.1f dy=%.1f phase=%s momentum=%s precise=%s",
                  we.deltaX,
                  we.deltaY,
                  state->lastWheelPhase.c_str(),
                  state->lastMomentumPhase.c_str(),
                  we.hasPreciseDeltas ? "yes" : "no");
    (*pushLog)(buf);
  });

  stagePtr->element.addEventListener("gesture", [state, pushLog](glint_event& e) {
    auto& ge = static_cast<glint_gesture_event&>(e);
    ge.preventDefault();

    state->gestureCount += 1;
    state->lastGestureKind = glint_touch_demo_gesture_label(ge.kind);
    state->lastGesturePhase = glint_touch_demo_phase_label(ge.phase);

    switch (ge.kind)
    {
    case glint_gesture_kind::pinch:
      if (ge.phase == glint_input_phase::changed)
      {
        state->scale = glint_touch_demo_clamp(
          state->scale * std::max(0.35f, ge.scale),
          0.45f,
          3.4f);
      }
      break;
    case glint_gesture_kind::rotate:
      if (ge.phase == glint_input_phase::changed)
        state->rotationDeg -= ge.rotation;
      break;
    case glint_gesture_kind::swipe:
      state->offsetX = glint_touch_demo_clamp(state->offsetX + ge.deltaX * 56.f, -140.f, 140.f);
      state->offsetY = glint_touch_demo_clamp(state->offsetY - ge.deltaY * 56.f, -140.f, 140.f);
      break;
    case glint_gesture_kind::smart_zoom:
      if (ge.phase == glint_input_phase::changed)
      {
        state->scale = state->scale > 1.4f ? 1.f : 2.f;
        state->rotationDeg = 0.f;
        state->offsetX = 0.f;
        state->offsetY = 0.f;
      }
      break;
    case glint_gesture_kind::none:
    default:
      break;
    }

    char buf[256];
    std::snprintf(buf,
                  sizeof(buf),
                  "gesture %s %s dx=%.2f dy=%.2f mag=%.3f rot=%.2f",
                  state->lastGestureKind.c_str(),
                  state->lastGesturePhase.c_str(),
                  ge.deltaX,
                  ge.deltaY,
                  ge.magnification,
                  ge.rotation);
    (*pushLog)(buf);
  });

  (*refresh)();
}