#pragma once

/**
 * glint_switch.hpp
 * A toggle-switch component for use with glint.
 *
 * Visual: a pill-shaped track with a circular thumb that slides.
 *
 * Usage:
 *   auto* sw = new glint_switch();
 *   sw->size     = 20.f;        // thumb diameter; track scales from this
 *   sw->trackOff = "#333";
 *   sw->trackOn  = "#3399ff";
 *   sw->thumb    = "#ffffff";
 *   sw->SetToggleValue(true, false);
 *   sw->onChange = [](bool v) { ... };
 */

#include "glint/glint_element.hpp"
#include <functional>

class glint_switch : public glint_element
{
public:
  // -- Public fields (set before the element is first laid out) --------------
  float       size     = 20.f;       // thumb diameter
  const char* trackOff = "#444444";
  const char* trackOn  = "#3399ff";
  const char* thumb    = "#ffffff";

  std::function<void(bool)> onChange;

  glint_switch()
  {
    style.position = "relative";
    style.cursor   = "pointer";

    mTrack = new glint_element();
    mTrack->style.position     = "absolute";
    mTrack->style.left         = 0.f;
    mTrack->style.top          = 0.f;
    addChild(mTrack);

    mThumb = new glint_element();
    mThumb->style.position = "absolute";
    addChild(mThumb);

    element.addEventListener("click", [this](glint_event&) { _toggle(); });
  }

  bool GetToggleValue() const { return mOn; }

  void SetToggleValue(bool v, bool /*animate*/ = true)
  {
    mOn = v;
    _sync();
    setDirty(false);
  }

  const char* typeName() const override { return "switch"; }

  void Layout(glint_canvas* g) override
  {
    _sync();
    glint_element::Layout(g);
  }

private:
  bool          mOn    = false;
  glint_element* mTrack = nullptr;
  glint_element* mThumb = nullptr;

  void _toggle()
  {
    mOn = !mOn;
    _sync();
    setDirty(false);
    if (onChange) onChange(mOn);
  }

  void _sync()
  {
    const float trackH  = size * 1.1f;
    const float trackW  = size * 1.9f;
    const float padding = (trackH - size) * 0.5f;

    style.width  = trackW;
    style.height = trackH;

    if (mTrack)
    {
      mTrack->style.width           = trackW;
      mTrack->style.height          = trackH;
      mTrack->style.borderRadius    = trackH * 0.5f;
      mTrack->style.backgroundColor = mOn ? trackOn : trackOff;
    }

    if (mThumb)
    {
      const float thumbLeft = mOn ? trackW - padding - size : padding;
      mThumb->style.width           = size;
      mThumb->style.height          = size;
      mThumb->style.borderRadius    = size * 0.5f;
      mThumb->style.backgroundColor = thumb;
      mThumb->style.top             = padding;
      mThumb->style.left            = thumbLeft;
    }
  }
};

