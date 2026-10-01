#pragma once

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <variant>

inline void glint_demos_window::buildLiquidGlassShader()
{
  addHeading("Liquid Glass Shader");

  mContent->add.div([](auto& sub) {
    sub.innerText = "Dedicated demo page for the liquid_glass pill lens. This shader uses rounded-rect refraction, center magnification, edge highlights, tint, and chromatic aberration.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 14.f;
  });

  mContent->add.div([](auto& note) {
    note.innerText       = "The pill is rendered as a separate backdrop shader. Sampling and optics can be tuned independently so the lens can preview loupe behavior before it is connected to text caret geometry.";
    note.style.color     = glint_demo_theme::muted;
    note.style.fontSize  = 11.f;
    note.style.width     = "100%";
    note.style.textAlign = EAlign::Near;
    note.style.marginBottom = 10.f;
  });

  auto addCircle = [](auto& card, float l, float t, float s, glint_color col) {
    card.add.div([=](auto& ci) {
      ci.style.position        = "absolute";
      ci.style.left            = l;
      ci.style.top             = t;
      ci.style.width           = s;
      ci.style.height          = s;
      ci.style.borderRadius    = 9999.f;
      ci.style.backgroundColor = col;
      ci.style.pointerEvents   = "none";
    });
  };

  auto addScene = [&addCircle](auto& card) {
    addCircle(card,   8.f,  55.f, 64.f, glint_color(255, 255, 110,  50));
    addCircle(card,  58.f,  32.f, 52.f, glint_color(255,  60, 140, 255));
    addCircle(card, 110.f,  70.f, 56.f, glint_color(255, 210,  50, 200));
    addCircle(card, 155.f,  24.f, 46.f, glint_color(255,  50, 210, 110));
    addCircle(card,  36.f, 190.f, 88.f, glint_color(255,  70, 170, 255));
    addCircle(card, 180.f, 230.f, 96.f, glint_color(255, 255, 120,  70));
    addCircle(card, 350.f, 150.f, 72.f, glint_color(255, 130, 255, 180));
    addCircle(card, 470.f, 250.f, 120.f, glint_color(255,  60, 220, 130));
    addCircle(card, 120.f, 390.f, 110.f, glint_color(255, 255, 170,  80));
    addCircle(card, 300.f, 430.f, 82.f, glint_color(255,  80, 150, 255));
    addCircle(card, 500.f, 380.f, 92.f, glint_color(255, 220,  90, 210));
    card.add.div([](auto& aa) {
      aa.innerText           = "Aa";
      aa.style.color         = glint_color(180, 255, 255, 255);
      aa.style.fontSize      = 28.f;
      aa.style.position      = "absolute";
      aa.style.left          = 80.f;
      aa.style.top           = 48.f;
      aa.style.pointerEvents = "none";
    });
    card.add.div([](auto& text) {
      text.innerText           = "Liquid";
      text.style.color         = glint_color(145, 255, 255, 255);
      text.style.fontSize      = 34.f;
      text.style.position      = "absolute";
      text.style.left          = 250.f;
      text.style.top           = 82.f;
      text.style.pointerEvents = "none";
    });
    card.add.div([](auto& text) {
      text.innerText           = "Glass";
      text.style.color         = glint_color(150, 255, 255, 255);
      text.style.fontSize      = 54.f;
      text.style.position      = "absolute";
      text.style.left          = 60.f;
      text.style.top           = 300.f;
      text.style.pointerEvents = "none";
    });
    card.add.div([](auto& text) {
      text.innerText           = "Drag the pill across the stage";
      text.style.color         = glint_color(110, 255, 255, 255);
      text.style.fontSize      = 22.f;
      text.style.position      = "absolute";
      text.style.left          = 250.f;
      text.style.top           = 500.f;
      text.style.pointerEvents = "none";
    });
  };

  auto formatFloat = [](float value, int decimals) {
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%.*f", decimals, value);
    return std::string(buffer);
  };

  struct glint_liquid_surface_button : public glint_button
  {
    int surfaceType = 0;

    void _drawSurfaceIcon(SkCanvas* canvas, const glint_style& s, const glint_rect& r)
    {
      if (!canvas) return;
      const float inset = 8.f;
      const float width = std::max(1.f, r.W() - inset * 2.f);
      const float height = std::max(1.f, r.H() - inset * 2.f);

      auto mapX = [&](float x) { return r.L + inset + (x / 40.f) * width; };
      auto mapY = [&](float y) { return r.T + inset + (y / 40.f) * height; };

      SkPath path;
      switch (surfaceType)
      {
      case 0:
        path.moveTo(mapX(5.f), mapY(35.f));
        path.quadTo(mapX(5.f), mapY(5.f), mapX(35.f), mapY(5.f));
        break;
      case 1:
        path.moveTo(mapX(5.f), mapY(35.f));
        path.quadTo(mapX(5.f), mapY(20.f), mapX(20.f), mapY(10.f));
        path.quadTo(mapX(30.f), mapY(5.f), mapX(35.f), mapY(5.f));
        break;
      case 2:
        path.moveTo(mapX(5.f), mapY(5.f));
        path.quadTo(mapX(5.f), mapY(35.f), mapX(35.f), mapY(35.f));
        break;
      default:
        path.moveTo(mapX(5.f), mapY(25.f));
        path.quadTo(mapX(10.f), mapY(5.f), mapX(20.f), mapY(15.f));
        path.quadTo(mapX(30.f), mapY(25.f), mapX(35.f), mapY(20.f));
        break;
      }

      SkPaint stroke;
      stroke.setAntiAlias(true);
      stroke.setStyle(SkPaint::kStroke_Style);
      stroke.setStrokeWidth(2.f);
      stroke.setStrokeCap(SkPaint::kRound_Cap);
      stroke.setStrokeJoin(SkPaint::kRound_Join);
      stroke.setColor(skColor(ApplyOpacity(s.color.value, s.opacity)));
      canvas->drawPath(path, stroke);
    }

    void drawContent(glint_canvas& g) override
    {
      SkCanvas* canvas = static_cast<SkCanvas*>(g.GetDrawContext());
      const glint_style& s = mActiveStyle ? *mActiveStyle : style;
      _drawSurfaceIcon(canvas, s, getContent());
    }

    void DrawContentToCanvas(SkCanvas* canvas) override
    {
      const glint_style& s = mActiveStyle ? *mActiveStyle : style;
      _drawSurfaceIcon(canvas, s, getContent());
    }
  };

  struct SerializedParam {
    const char* key;
    int         decimals;
  };
  static constexpr SerializedParam kSerializedParams[] = {
    { "sampleOffsetX", 0 },
    { "sampleOffsetY", 0 },
    { "bezelWidth", 1 },
    { "glassThickness", 0 },
    { "refractiveIndex", 2 },
    { "magnification", 2 },
    { "surfaceType", 0 },
    { "cornerRadius", 0 },
    { "maxDisplacementScale", 2 },
    { "tintOpacity", 2 },
    { "tintR", 2 },
    { "tintG", 2 },
    { "tintB", 2 },
    { "specularOpacity", 2 },
    { "specularAngle", 2 },
    { "specularWidth", 1 },
    { "shadowOpacity", 2 },
    { "shadowWidth", 1 },
    { "chromaticStrength", 3 },
    { "chromaticBase", 2 },
  };

  glint_element*     stageCardPtr = nullptr;
  glint_element*     glassPillPtr = nullptr;
  glint_checkbox*    glassPillTogPtr = nullptr;
  glint_element*     controlSidebarPtr = nullptr;
  glint_textarea*    serializedParamsPtr = nullptr;
  // Shared holder: the Glass Theme select callback outlives this function and
  // the picker is only created further down, so capture the holder by value.
  auto glassColorPickerPtr = std::make_shared<glint_colorpicker*>(nullptr);

  mContent->add.div([&](auto& glassRow) {
    glassRow.style.display       = "flex";
    glassRow.style.flexDirection = "row";
    glassRow.style.gap           = 16.f;
    glassRow.style.width         = "100%";
    glassRow.style.height        = 600.f;
    glassRow.style.marginBottom  = 14.f;

    glassRow.add.div([&](auto& stageCol) {
      stageCol.style.flexGrow = 1.f;
      stageCol.style.minWidth = 0.f;
      stageCol.style.height   = "100%";

      stageCol.add.div([&](auto& card) {
        card.style.width           = "100%";
        card.style.height          = "100%";
        card.style.borderRadius    = 12.f;
        card.style.overflow        = "hidden";
        card.style.position        = "relative";
        card.style.backgroundColor = glint_color(255, 14, 14, 16);

        addScene(card);

        card.add.div([](auto& glass) {
          glass.style.position        = "absolute";
          glass.style.left            = 96.f;
          glass.style.top             = 72.f;
          glass.style.width           = 187.f;
          glass.style.height          = 91.f;
          glass.style.borderRadius    = 38.f;
          glass.style.backgroundColor = glint_color(18, 255, 255, 255);
          glass.style.cursor          = "move";
          glass.align                 = "center middle";

          glass.add.div([](auto& hint) {
            hint.innerText           = "drag me";
            hint.style.color         = glint_color(150, 255, 255, 255);
            hint.style.fontSize      = 18.f;
            hint.style.pointerEvents = "none";
          });
        }, &glassPillPtr);

        card.add.template fromClass<glint_checkbox>([](auto& tog) {
          tog.checked        = true;
          tog.size           = 14.f;
          tog.style.position = "absolute";
          tog.style.left     = 8.f;
          tog.style.top      = 8.f;
        }, &glassPillTogPtr);

        card.add.div([](auto& caption) {
          caption.innerText           = "liquid_glass pill prototype";
          caption.style.color         = glint_color(130, 255, 255, 255);
          caption.style.fontSize      = 10.f;
          caption.style.position      = "absolute";
          caption.style.width         = "100%";
          caption.style.height        = 14.f;
          caption.style.bottom        = 6.f;
          caption.style.textAlign     = EAlign::Center;
          caption.style.pointerEvents = "none";
        });
      }, &stageCardPtr);
    });

    glassRow.add.div([&](auto& sidebar) {
      sidebar.style.width           = 420.f;
      sidebar.style.minWidth        = 420.f;
      sidebar.style.height          = "100%";
      sidebar.style.overflowY       = "auto";
      sidebar.style.borderRadius    = 12.f;
      sidebar.style.backgroundColor = glint_color(255, 16, 16, 22);
      sidebar.style.padding         = 12.f;
    }, &controlSidebarPtr);
  });

  glassPillPtr->style.backdropFilter = "shader(gls, liquid_glass)";
  glassPillPtr->shaders["gls"]->params["sampleOffsetX"] = 0.f;
  glassPillPtr->shaders["gls"]->params["sampleOffsetY"] = 0.f;
  glassPillPtr->shaders["gls"]->params["bezelWidth"] = 48.f;
  glassPillPtr->shaders["gls"]->params["glassThickness"] = 50.f;
  glassPillPtr->shaders["gls"]->params["refractiveIndex"] = 1.5f;
  glassPillPtr->shaders["gls"]->params["magnification"] = -0.f;
  glassPillPtr->shaders["gls"]->params["surfaceType"] = 0.f;
  glassPillPtr->shaders["gls"]->params["cornerRadius"] = 80.f;
  glassPillPtr->shaders["gls"]->params["maxDisplacementScale"] = 1.f;
  glassPillPtr->shaders["gls"]->params["tintOpacity"] = 0.06f;
  glassPillPtr->shaders["gls"]->params["tintR"] = 1.f;
  glassPillPtr->shaders["gls"]->params["tintG"] = 1.f;
  glassPillPtr->shaders["gls"]->params["tintB"] = 1.f;
  glassPillPtr->shaders["gls"]->params["specularOpacity"] = 0.48f;
  glassPillPtr->shaders["gls"]->params["specularAngle"] = -0.85f;
  glassPillPtr->shaders["gls"]->params["specularWidth"] = 3.f;
  glassPillPtr->shaders["gls"]->params["shadowOpacity"] = 0.08f;
  glassPillPtr->shaders["gls"]->params["shadowWidth"] = 1.f;
  glassPillPtr->shaders["gls"]->params["chromaticStrength"] = 0.35f;
  glassPillPtr->shaders["gls"]->params["chromaticBase"] = 0.65f;

  auto refreshSerializedParams = std::make_shared<std::function<void()>>();

  struct LiquidGlassCssState {
    float       rectWidth              = 187.f;
    float       rectHeight             = 91.f;
    float       rectRadius             = 38.f;
    float       backgroundBrightness   = 1.f;
    float       blurAmount             = 0.f;
    float       progressiveBlur        = 0.f;
    int         progressiveBlurMode    = 0;
    bool        chromaticAberrationEnabled = true;
    float       chromaticStrength      = 0.35f;
    float       chromaticBase          = 0.65f;
    int         glassTheme             = 0;
    glint_color glassColor             = glint_color(255, 255, 255, 255);
    float       glassBackgroundOpacity = 0.07f;
    float       pressedGlassOpacity    = 0.16f;
    bool        liquidInteractionEnabled = false;
    float       pressScale             = 1.02f;
    float       baseRefractiveIndex    = 1.5f;
    float       pressRefraction        = 1.28f;
    float       speedSeconds           = 0.18f;
    float       clickSquash            = 1.02f;
    float       dragSquash             = 1.06f;
    float       releaseSquash          = 1.1f;
    float       shadowOpacity          = 0.22f;
    float       shadowBlur             = 28.f;
    float       shadowOffsetX          = 0.f;
    float       shadowOffsetY          = 12.f;
  };

  enum class GlassInteractionMode {
    Idle,
    Pressed,
    Dragging,
  };

  auto cssState = std::make_shared<LiquidGlassCssState>();
  auto interactionMode = std::make_shared<GlassInteractionMode>(GlassInteractionMode::Idle);

  auto glassThemeColor = [](int themeIndex) {
    switch (themeIndex)
    {
    case 1: return glint_color(255, 196, 232, 255);
    case 2: return glint_color(255, 255, 232, 204);
    default: return glint_color(255, 255, 255, 255);
    }
  };
  auto glassThemeName = [](int themeIndex) -> const char* {
    switch (themeIndex)
    {
    case 1: return "Cool";
    case 2: return "Warm";
    default: return "System";
    }
  };

  auto clampUnit = [](float value) {
    return std::max(0.f, std::min(1.f, value));
  };
  auto withAlpha = [clampUnit](glint_color color, float opacity) {
    const int alpha = static_cast<int>(std::round(clampUnit(opacity) * 255.f));
    return glint_color(alpha, color.R, color.G, color.B);
  };
  auto formatColor = [](glint_color color) {
    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "#%02x%02x%02x%02x", color.R, color.G, color.B, color.A);
    return std::string(buffer);
  };
  auto buildShadowString = [formatFloat, withAlpha, formatColor](const LiquidGlassCssState& state) {
    const auto shadowColor = withAlpha(glint_color(255, 0, 0, 0), state.shadowOpacity);
    return formatFloat(state.shadowOffsetX, 0) + "px "
      + formatFloat(state.shadowOffsetY, 0) + "px "
      + formatFloat(state.shadowBlur, 0) + "px "
      + formatColor(shadowColor);
  };
  auto buildBackdropFilter = [formatFloat](const LiquidGlassCssState& state, bool shaderEnabled) {
    std::string filter;
    if (state.blurAmount > 0.01f)
      filter += "blur(" + formatFloat(state.blurAmount, 0) + "px)";
    if (std::fabs(state.backgroundBrightness - 1.f) > 0.01f)
    {
      if (!filter.empty()) filter += " ";
      filter += "brightness(" + formatFloat(state.backgroundBrightness, 2) + ")";
    }
    if (shaderEnabled)
    {
      if (!filter.empty()) filter += " ";
      filter += "shader(gls, liquid_glass)";
    }
    return filter;
  };

  auto applyPillTransition = std::make_shared<std::function<void(bool)>>();
  auto applyInteractionVisual = std::make_shared<std::function<void(GlassInteractionMode)>>();
  auto applyCssState = std::make_shared<std::function<void()>>();
  auto applyChromaticState = std::make_shared<std::function<void()>>();

  *applyPillTransition = [glassPillPtr, cssState, formatFloat](bool releasing) {
    if (!glassPillPtr) return;
    const float baseMs = std::max(40.f, cssState->speedSeconds * 1000.f);
    const float releaseScale = releasing ? std::max(1.f, cssState->releaseSquash) : 1.f;
    const auto duration = formatFloat(baseMs * releaseScale, 0);
    glassPillPtr->style.transition = "transform " + duration + "ms ease-out, background-color "
      + duration + "ms ease-out, backdrop-filter " + duration + "ms ease-out";
  };

  *applyInteractionVisual = [glassPillPtr, cssState, interactionMode, withAlpha, applyPillTransition, formatFloat](GlassInteractionMode mode) {
    if (!glassPillPtr) return;

    *interactionMode = mode;

    float scale = 1.f;
    float squash = 1.f;
    float backgroundOpacity = cssState->glassBackgroundOpacity;
    bool releasing = false;
    const bool interactionActive = cssState->liquidInteractionEnabled && mode != GlassInteractionMode::Idle;

    switch (mode)
    {
    case GlassInteractionMode::Pressed:
      if (interactionActive)
      {
        scale = cssState->pressScale;
        squash = cssState->clickSquash;
        backgroundOpacity = cssState->pressedGlassOpacity;
      }
      break;
    case GlassInteractionMode::Dragging:
      if (interactionActive)
      {
        scale = cssState->pressScale;
        squash = cssState->dragSquash;
        backgroundOpacity = cssState->pressedGlassOpacity;
      }
      break;
    case GlassInteractionMode::Idle:
    default:
      releasing = true;
      break;
    }

    (*applyPillTransition)(releasing);
    if (!interactionActive)
      glassPillPtr->style.transform = "none";
    else
      glassPillPtr->style.transform = "scale(" + formatFloat(scale, 2) + ") scaleX(" + formatFloat(squash, 2)
        + ") scaleY(" + formatFloat(1.f / std::max(0.01f, squash), 2) + ")";

    if (auto* shader = glassPillPtr->shaders["gls"].get())
      shader->params["refractiveIndex"] = interactionActive ? cssState->pressRefraction : cssState->baseRefractiveIndex;

    glassPillPtr->style.backgroundColor = withAlpha(cssState->glassColor, backgroundOpacity);
    glassPillPtr->setDirty(false);
  };

  *applyCssState = [stageCardPtr,
                    glassPillPtr,
                    glassPillTogPtr,
                    cssState,
                    buildBackdropFilter,
                    buildShadowString,
                    withAlpha,
                    applyPillTransition,
                    applyInteractionVisual] {
    if (!stageCardPtr || !glassPillPtr) return;

    glassPillPtr->style.width        = cssState->rectWidth;
    glassPillPtr->style.height       = cssState->rectHeight;
    glassPillPtr->style.borderRadius = cssState->rectRadius;
    glassPillPtr->style.boxShadow    = buildShadowString(*cssState);
    glassPillPtr->style.backdropFilter = buildBackdropFilter(*cssState, glassPillTogPtr ? glassPillTogPtr->checked : true);
    glassPillPtr->style.backgroundColor = withAlpha(cssState->glassColor, cssState->glassBackgroundOpacity);
    (*applyPillTransition)(false);
    (*applyInteractionVisual)(GlassInteractionMode::Idle);

    stageCardPtr->setDirty(false);
    glassPillPtr->setDirty(false);
  };

  *applyChromaticState = [glassPillPtr, cssState] {
    if (!glassPillPtr) return;
    auto* shader = glassPillPtr->shaders["gls"].get();
    if (!shader) return;

    shader->params["chromaticStrength"] = cssState->chromaticAberrationEnabled ? cssState->chromaticStrength : 0.f;
    shader->params["chromaticBase"] = cssState->chromaticAberrationEnabled ? cssState->chromaticBase : 0.f;
    glassPillPtr->setDirty(false);
  };

  glassPillTogPtr->onChange = [applyCssState](bool) {
    if (*applyCssState) (*applyCssState)();
  };

  struct GlassPillDrag {
    bool  active  = false;
    float startCX = 0.f, startCY = 0.f;
    float startL  = 0.f, startT  = 0.f;
  };
  auto drag = std::make_shared<GlassPillDrag>();

  glint_element* stagePtr = glassPillPtr ? glassPillPtr->mParent : nullptr;

  glassPillPtr->element.addEventListener("mousedown", [glassPillPtr, stagePtr, drag, applyInteractionVisual](glint_event& e) {
    if (!stagePtr) return;
    auto& me      = static_cast<glint_mouse_event&>(e);
    drag->active  = true;
    drag->startCX = me.clientX;
    drag->startCY = me.clientY;
    drag->startL  = glassPillPtr->mPaintRECT.L - stagePtr->mPaintRECT.L;
    drag->startT  = glassPillPtr->mPaintRECT.T - stagePtr->mPaintRECT.T;
    if (*applyInteractionVisual) (*applyInteractionVisual)(GlassInteractionMode::Pressed);
    e.stopPropagation();
  });
  glassPillPtr->element.addEventListener("mousemove", [glassPillPtr, stagePtr, drag, applyInteractionVisual](glint_event& e) {
    if (!drag->active || !stagePtr) return;
    auto& me = static_cast<glint_mouse_event&>(e);
    float newL = drag->startL + (me.clientX - drag->startCX);
    float newT = drag->startT + (me.clientY - drag->startCY);
    float maxL = stagePtr->mPaintRECT.W() - glassPillPtr->mPaintRECT.W();
    float maxT = stagePtr->mPaintRECT.H() - glassPillPtr->mPaintRECT.H();
    newL = std::max(0.f, std::min(newL, maxL));
    newT = std::max(0.f, std::min(newT, maxT));

    const float dL = (stagePtr->mPaintRECT.L + newL) - glassPillPtr->mPaintRECT.L;
    const float dT = (stagePtr->mPaintRECT.T + newT) - glassPillPtr->mPaintRECT.T;
    std::function<void(glint_element*)> shiftRects = [&](glint_element* el) {
      el->mRect.L += dL; el->mRect.R += dL;
      el->mRect.T += dT; el->mRect.B += dT;
      el->mPaintRECT.L += dL; el->mPaintRECT.R += dL;
      el->mPaintRECT.T += dT; el->mPaintRECT.B += dT;
      for (auto& child : el->mChildren) shiftRects(child.get());
    };
    shiftRects(glassPillPtr);
    glassPillPtr->style.left = newL;
    glassPillPtr->style.top  = newT;
    if (*applyInteractionVisual) (*applyInteractionVisual)(GlassInteractionMode::Dragging);
    glassPillPtr->setPaintOnlyDirty();
  });
  glassPillPtr->element.addEventListener("mouseup", [drag, applyInteractionVisual](glint_event&) {
    drag->active = false;
    if (*applyInteractionVisual) (*applyInteractionVisual)(GlassInteractionMode::Idle);
  });
  glassPillPtr->element.addEventListener("mouseleave", [drag, applyInteractionVisual](glint_event&) {
    if (!drag->active) return;
    drag->active = false;
    if (*applyInteractionVisual) (*applyInteractionVisual)(GlassInteractionMode::Idle);
  });

  auto addControlGroup = [&](auto& host, const char* title, auto buildGroup) {
    host.add.div([titleText = std::string(title), buildGroup](auto& group) {
      group.style.width           = "100%";
      group.style.borderRadius    = 12.f;
      group.style.backgroundColor = glint_color(255, 18, 18, 24);
      group.style.padding         = 12.f;
      group.style.marginBottom    = 12.f;

      group.add.div([titleText](auto& heading) {
        heading.innerText          = titleText;
        heading.style.color        = glint_demo_theme::heading;
        heading.style.fontSize     = 13.f;
        heading.style.width        = "100%";
        heading.style.textAlign    = EAlign::Near;
        heading.style.marginBottom = 8.f;
      });

      buildGroup(group);
    });
  };

  auto addControlGroupBox = [&](glint_ctx& host, const char* title) {
    return host.add.div([titleText = std::string(title)](auto& group) {
      group.style.width           = "100%";
      group.style.borderRadius    = 12.f;
      group.style.backgroundColor = glint_color(255, 18, 18, 24);
      group.style.padding         = 12.f;
      group.style.marginBottom    = 12.f;

      group.add.div([titleText](auto& heading) {
        heading.innerText          = titleText;
        heading.style.color        = glint_demo_theme::heading;
        heading.style.fontSize     = 13.f;
        heading.style.width        = "100%";
        heading.style.textAlign    = EAlign::Near;
        heading.style.marginBottom = 8.f;
      });
    });
  };

  auto addSliderControl = [glassPillPtr, formatFloat](auto& parent,
                                                      const char* label,
                                                      const char* key,
                                                      float initialValue,
                                                      float minValue,
                                                      float maxValue,
                                                      float stepValue,
                                                      int decimals,
                                                      std::function<std::string(float)> valueFormatter,
                                                      std::shared_ptr<std::function<void()>> refreshSerializedParams) {
    auto labelText = std::string(label);
    auto keyText = std::string(key);
    auto formatter = std::make_shared<std::function<std::string(float)>>(std::move(valueFormatter));
    auto valueHolder = std::make_shared<glint_element*>(nullptr);
    auto initialText = (*formatter) ? (*formatter)(initialValue) : formatFloat(initialValue, decimals);

    parent.add.div([glassPillPtr,
                    formatFloat,
                    labelText,
                    keyText,
                    initialValue,
                    minValue,
                    maxValue,
                    stepValue,
                    decimals,
                    formatter,
                    valueHolder,
                    refreshSerializedParams,
                    initialText](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.width         = "100%";
      row.style.gap           = 12.f;
      row.style.marginBottom  = 8.f;

      row.add.div([labelText](auto& name) {
        name.innerText       = labelText;
        name.style.width     = 100.f;
        name.style.color     = glint_demo_theme::muted;
        name.style.fontSize  = 11.f;
        name.style.textAlign = EAlign::Near;
      });

      row.add.input([glassPillPtr,
                     formatFloat,
                     keyText,
                     initialValue,
                     minValue,
                     maxValue,
                     stepValue,
                     decimals,
                     formatter,
                     valueHolder,
                     refreshSerializedParams](glint_input& inp) {
        inp.type         = "range";
        inp.min          = minValue;
        inp.max          = maxValue;
        inp.step         = stepValue;
        inp.style.flexGrow = 1.f;
        inp.style.height = 24.f;
        inp.setFloatValue(initialValue);
        inp.onChange = [glassPillPtr,
                        formatFloat,
                        keyText,
                        decimals,
                        formatter,
                        valueHolder,
                        refreshSerializedParams](const std::string& v) {
          try {
            float nextValue = std::stof(v);
            glassPillPtr->shaders["gls"]->params[keyText] = nextValue;
            glassPillPtr->setDirty(false);
            if (*valueHolder) {
              (*valueHolder)->innerText = (*formatter) ? (*formatter)(nextValue) : formatFloat(nextValue, decimals);
              (*valueHolder)->setDirty(false);
            }
            if (*refreshSerializedParams) {
              (*refreshSerializedParams)();
            }
          } catch (...) {}
        };
      });

      row.add.div([initialText](auto& value) {
        value.innerText       = initialText;
        value.style.width     = 42.f;
        value.style.color     = glint_demo_theme::subtle;
        value.style.fontSize  = 11.f;
        value.style.textAlign = EAlign::Far;
      }, valueHolder.get());
    });
  };

  auto addLiveSliderControl = [formatFloat](auto& parent,
                                            const char* label,
                                            float initialValue,
                                            float minValue,
                                            float maxValue,
                                            float stepValue,
                                            int decimals,
                                            std::function<std::string(float)> valueFormatter,
                                            std::function<void(float)> onValue) {
    auto labelText = std::string(label);
    auto formatter = std::make_shared<std::function<std::string(float)>>(std::move(valueFormatter));
    auto callback = std::make_shared<std::function<void(float)>>(std::move(onValue));
    auto valueHolder = std::make_shared<glint_element*>(nullptr);
    auto initialText = (*formatter) ? (*formatter)(initialValue) : formatFloat(initialValue, decimals);

    parent.add.div([labelText,
                    initialValue,
                    minValue,
                    maxValue,
                    stepValue,
                    decimals,
                    formatFloat,
                    formatter,
                    callback,
                    valueHolder,
                    initialText](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.width         = "100%";
      row.style.gap           = 12.f;
      row.style.marginBottom  = 8.f;

      row.add.div([labelText](auto& name) {
        name.innerText       = labelText;
        name.style.width     = 100.f;
        name.style.color     = glint_demo_theme::muted;
        name.style.fontSize  = 11.f;
        name.style.textAlign = EAlign::Near;
      });

      row.add.input([initialValue,
                     minValue,
                     maxValue,
                     stepValue,
                     decimals,
                     formatFloat,
                     formatter,
                     callback,
                     valueHolder](glint_input& inp) {
        inp.type           = "range";
        inp.min            = minValue;
        inp.max            = maxValue;
        inp.step           = stepValue;
        inp.style.flexGrow = 1.f;
        inp.style.height   = 24.f;
        inp.setFloatValue(initialValue);
        inp.onChange = [decimals,
                        formatFloat,
                        formatter,
                        callback,
                        valueHolder](const std::string& v) {
          try {
            const float nextValue = std::stof(v);
            if (*callback) (*callback)(nextValue);
            if (*valueHolder) {
              (*valueHolder)->innerText = (*formatter) ? (*formatter)(nextValue) : formatFloat(nextValue, decimals);
              (*valueHolder)->setDirty(false);
            }
          } catch (...) {}
        };
      });

      row.add.div([initialText](auto& value) {
        value.innerText       = initialText;
        value.style.width     = 42.f;
        value.style.color     = glint_demo_theme::subtle;
        value.style.fontSize  = 11.f;
        value.style.textAlign = EAlign::Far;
      }, valueHolder.get());
    });
  };

  auto addColorControl = [](auto& parent,
                            const char* label,
                            glint_color initialColor,
                            glint_colorpicker** pickerOut,
                            std::function<void(glint_color)> onColor) {
    auto labelText = std::string(label);
    auto callback = std::make_shared<std::function<void(glint_color)>>(std::move(onColor));

    parent.add.div([labelText, initialColor, callback, pickerOut](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "flex-start";
      row.style.gap           = 12.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 10.f;

      row.add.div([labelText](auto& name) {
        name.innerText       = labelText;
        name.style.width     = 100.f;
        name.style.color     = glint_demo_theme::muted;
        name.style.fontSize  = 11.f;
        name.style.textAlign = EAlign::Near;
      });

      row.add.template fromClass<glint_colorpicker>([initialColor](glint_colorpicker& picker) {
        picker.value       = initialColor;
        picker.style.width = 220.f;
      }, pickerOut);
    });

    if (pickerOut && *pickerOut)
      (*pickerOut)->onChange = [callback](glint_color nextColor) {
        if (*callback) (*callback)(nextColor);
      };
  };

  auto addSelectControl = [](auto& parent,
                             const char* label,
                             std::vector<std::string> options,
                             int selectedIndex,
                             std::function<void(int, const std::string&)> onSelect) {
    auto labelText = std::string(label);
    auto callback = std::make_shared<std::function<void(int, const std::string&)>>(std::move(onSelect));

    parent.add.div([labelText, options = std::move(options), selectedIndex, callback](auto& row) mutable {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 12.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 8.f;

      row.add.div([labelText](auto& name) {
        name.innerText       = labelText;
        name.style.width     = 100.f;
        name.style.color     = glint_demo_theme::muted;
        name.style.fontSize  = 11.f;
        name.style.textAlign = EAlign::Near;
      });

      row.add.template fromClass<glint_select>([options = std::move(options), selectedIndex, callback](glint_select& sel) mutable {
        sel.options               = std::move(options);
        sel.selectedIndex         = selectedIndex;
        sel.style.flexGrow        = 1.f;
        sel.style.height          = 28.f;
        sel.style.backgroundColor = glint_color(255, 14, 14, 18);
        sel.style.color           = glint_demo_theme::text;
        sel.style.borderRadius    = 8.f;
        sel.style.borderWidth     = 1.f;
        sel.style.borderColor     = glint_demo_theme::border;
        sel.style.paddingLeft     = 10.f;
        sel.style.fontSize        = 11.f;
        sel.style.transition      = "background-color 120ms ease-out, border-color 120ms ease-out";
        sel.hover.backgroundColor = glint_color(255, 20, 20, 24);
        sel.hover.borderColor     = glint_demo_theme::heading;
        sel.onChange = [callback](int idx, const std::string& value) {
          if (*callback) (*callback)(idx, value);
        };
      });
    });
  };

  auto addCheckboxControl = [](auto& parent,
                               const char* label,
                               bool initialValue,
                               std::function<void(bool)> onToggle) {
    auto labelText = std::string(label);
    auto callback = std::make_shared<std::function<void(bool)>>(std::move(onToggle));

    parent.add.div([labelText, initialValue, callback](auto& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 12.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 8.f;

      row.add.div([labelText](auto& name) {
        name.innerText       = labelText;
        name.style.width     = 100.f;
        name.style.color     = glint_demo_theme::muted;
        name.style.fontSize  = 11.f;
        name.style.textAlign = EAlign::Near;
      });

      row.add.template fromClass<glint_checkbox>([initialValue, callback](glint_checkbox& cb) {
        cb.checked = initialValue;
        cb.size    = 14.f;
        cb.style.width = "fit-content";
        cb.onChange = [callback](bool checked) {
          if (*callback) (*callback)(checked);
        };
      });
    });
  };

  glint_ctx sidebarCtx(controlSidebarPtr);

  sidebarCtx.add.div([](auto& sub) {
    sub.innerText = "Live controls for the active liquid_glass shader instance.";
    sub.style.color        = glint_demo_theme::muted;
    sub.style.fontSize     = 12.f;
    sub.style.width        = "100%";
    sub.style.textAlign    = EAlign::Near;
    sub.style.marginBottom = 10.f;
  });

  auto* samplingGroupPtr = addControlGroupBox(sidebarCtx, "Sampling and Lens Shape");
  samplingGroupPtr->add.div([](auto& label) {
    label.innerText          = "Surface Type";
    label.style.color        = glint_demo_theme::muted;
    label.style.fontSize     = 11.f;
    label.style.width        = "100%";
    label.style.textAlign    = EAlign::Near;
    label.style.marginBottom = 6.f;
  });

  auto* surfaceRowPtr = samplingGroupPtr->add.div([](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 8.f;
    row.style.width         = "100%";
    row.style.marginBottom  = 10.f;
  });

  struct SurfaceButtonSpec {
    int         surfaceType;
    const char* title;
  };
  static constexpr SurfaceButtonSpec kSurfaceButtons[] = {
    { 0, "Convex Circle" },
    { 1, "Convex Squircle" },
    { 2, "Concave" },
    { 3, "Lip" },
  };

  auto surfaceButtons = std::make_shared<std::array<glint_liquid_surface_button*, 4>>();
  auto syncSurfaceButtons = std::make_shared<std::function<void(int)>>();

  for (int i = 0; i < 4; ++i)
  {
    const auto spec = kSurfaceButtons[i];
    surfaceRowPtr->add.fromClass<glint_liquid_surface_button>([glassPillPtr, refreshSerializedParams, syncSurfaceButtons, spec](glint_liquid_surface_button& btn) {
      btn.surfaceType            = spec.surfaceType;
      btn.innerText              = "";
      btn.style.width            = 76.f;
      btn.style.height           = 48.f;
      btn.style.borderRadius     = 10.f;
      btn.style.borderWidth      = 1.f;
      btn.style.transition       = "background-color 120ms ease-out, border-color 120ms ease-out, color 120ms ease-out";
      btn.hover.borderRadius     = 10.f;
      btn.hover.borderWidth      = 1.f;
      btn.pressed.borderRadius   = 10.f;
      btn.pressed.borderWidth    = 1.f;
      btn.onClick = [glassPillPtr, refreshSerializedParams, syncSurfaceButtons, spec] {
        glassPillPtr->shaders["gls"]->params["surfaceType"] = static_cast<float>(spec.surfaceType);
        glassPillPtr->setDirty(false);
        if (*refreshSerializedParams) (*refreshSerializedParams)();
        if (*syncSurfaceButtons) (*syncSurfaceButtons)(spec.surfaceType);
      };
    }, &(*surfaceButtons)[i]);
  }

  *syncSurfaceButtons = [surfaceButtons](int activeSurfaceType) {
    for (int i = 0; i < 4; ++i)
    {
      auto* btn = (*surfaceButtons)[i];
      if (!btn) continue;

      const bool active = (i == activeSurfaceType);
      btn->style.backgroundColor  = active ? glint_color(255, 38, 92, 170) : glint_color(255, 14, 14, 18);
      btn->style.borderColor      = active ? glint_color(255, 90, 220, 255) : glint_color(70, 255, 255, 255);
      btn->style.color            = active ? glint_color(255, 255, 255, 255) : glint_color(180, 255, 255, 255);
      btn->hover.backgroundColor  = active ? glint_color(255, 48, 108, 190) : glint_color(255, 22, 22, 30);
      btn->hover.borderColor      = active ? glint_color(255, 110, 235, 255) : glint_color(110, 255, 255, 255);
      btn->hover.color            = glint_color(255, 255, 255, 255);
      btn->pressed.backgroundColor = active ? glint_color(255, 34, 82, 155) : glint_color(255, 28, 28, 36);
      btn->pressed.borderColor     = active ? glint_color(255, 90, 220, 255) : glint_color(130, 255, 255, 255);
      btn->pressed.color           = glint_color(255, 255, 255, 255);
      btn->setDirty(false);
    }
  };
  (*syncSurfaceButtons)(1);

  samplingGroupPtr->add.div([](auto& label) {
    label.innerText          = "Geometry";
    label.style.color        = glint_demo_theme::muted;
    label.style.fontSize     = 11.f;
    label.style.width        = "100%";
    label.style.textAlign    = EAlign::Near;
    label.style.marginBottom = 6.f;
  });

  glint_ctx samplingCtx(samplingGroupPtr);
  addSliderControl(samplingCtx, "Bezel Width", "bezelWidth", 48.f, 1.f, 48.f, 0.5f, 1, {}, refreshSerializedParams);
  addSliderControl(samplingCtx, "Glass Thickness", "glassThickness", 50.f, 0.f, 96.f, 1.f, 0, {}, refreshSerializedParams);
  addLiveSliderControl(samplingCtx, "Refractive Index", cssState->baseRefractiveIndex, 1.01f, 2.f, 0.01f, 2, {}, [cssState, glassPillPtr, interactionMode, refreshSerializedParams](float value) {
    cssState->baseRefractiveIndex = value;
    if (*interactionMode == GlassInteractionMode::Idle || !cssState->liquidInteractionEnabled)
      glassPillPtr->shaders["gls"]->params["refractiveIndex"] = value;
    glassPillPtr->setDirty(false);
    if (*refreshSerializedParams) (*refreshSerializedParams)();
  });
  addSliderControl(samplingCtx, "Refraction Level", "maxDisplacementScale", 1.f, 0.f, 4.f, 0.05f, 2, {}, refreshSerializedParams);
  addSliderControl(samplingCtx, "Magnifying Scale", "magnification", -0.f, -0.35f, 0.5f, 0.01f, 2, {}, refreshSerializedParams);

  samplingGroupPtr->add.div([](auto& label) {
    label.innerText          = "Sampling";
    label.style.color        = glint_demo_theme::muted;
    label.style.fontSize     = 11.f;
    label.style.width        = "100%";
    label.style.textAlign    = EAlign::Near;
    label.style.marginTop    = 4.f;
    label.style.marginBottom = 6.f;
  });

  addSliderControl(samplingCtx, "Sample Offset X", "sampleOffsetX", 0.f, -160.f, 160.f, 1.f, 0, {}, refreshSerializedParams);
  addSliderControl(samplingCtx, "Sample Offset Y", "sampleOffsetY", 0.f, -160.f, 160.f, 1.f, 0, {}, refreshSerializedParams);
  addSliderControl(samplingCtx, "Corner Radius", "cornerRadius", 80.f, 0.f, 80.f, 1.f, 0, {}, refreshSerializedParams);

  addControlGroup(sidebarCtx, "Optics", [addSelectControl, addLiveSliderControl, cssState, applyCssState, applyChromaticState, refreshSerializedParams, glassThemeColor, glassColorPickerPtr](auto& group) {
    addSelectControl(group, "Blur Type", { "Gaussian" }, 0, [](int, const std::string&) {});
    addLiveSliderControl(group, "Blur", cssState->blurAmount, 0.f, 48.f, 1.f, 0, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->blurAmount = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Progressive Blur", cssState->progressiveBlur, 0.f, 1.f, 0.05f, 2, {}, [cssState, refreshSerializedParams](float value) {
      cssState->progressiveBlur = value;
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addSelectControl(group, "Prog Blur Mode", { "Edge Falloff" }, cssState->progressiveBlurMode, [cssState, refreshSerializedParams](int idx, const std::string&) {
      cssState->progressiveBlurMode = idx;
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Chromatic Aberr", cssState->chromaticStrength, 0.f, 0.35f, 0.005f, 3, {}, [cssState, applyChromaticState, refreshSerializedParams](float value) {
      cssState->chromaticStrength = value;
      cssState->chromaticAberrationEnabled = value > 0.f;
      if (*applyChromaticState) (*applyChromaticState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addSelectControl(group, "Glass Theme", { "System", "Cool", "Warm" }, cssState->glassTheme, [cssState, applyCssState, refreshSerializedParams, glassThemeColor, glassColorPickerPtr](int idx, const std::string&) {
      cssState->glassTheme = idx;
      cssState->glassColor = glassThemeColor(idx);
      if (*glassColorPickerPtr) (*glassColorPickerPtr)->setValue(cssState->glassColor);
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Glass Bg Op", cssState->glassBackgroundOpacity, 0.f, 1.f, 0.01f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->glassBackgroundOpacity = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Pressed Bg Op", cssState->pressedGlassOpacity, 0.f, 1.f, 0.01f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->pressedGlassOpacity = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
  });

  addControlGroup(sidebarCtx, "CSS Layout and Backdrop", [addLiveSliderControl, cssState, applyCssState, refreshSerializedParams](auto& group) {
    addLiveSliderControl(group, "Rect Width", cssState->rectWidth, 80.f, 320.f, 1.f, 0, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->rectWidth = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Rect Height", cssState->rectHeight, 44.f, 160.f, 1.f, 0, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->rectHeight = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Rect Radius", cssState->rectRadius, 8.f, 120.f, 1.f, 0, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->rectRadius = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Bg Brightness", cssState->backgroundBrightness, 0.4f, 1.6f, 0.01f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->backgroundBrightness = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
  });

  auto* cssAppearanceGroup = addControlGroupBox(sidebarCtx, "CSS Glass Appearance");
  addColorControl(*cssAppearanceGroup, "Glass Color", cssState->glassColor, glassColorPickerPtr.get(), [cssState, applyCssState, refreshSerializedParams](glint_color nextColor) {
    cssState->glassColor = nextColor;
    if (*applyCssState) (*applyCssState)();
    if (*refreshSerializedParams) (*refreshSerializedParams)();
  });

  addControlGroup(sidebarCtx, "Liquid Interaction", [addLiveSliderControl, addCheckboxControl, cssState, applyCssState, refreshSerializedParams, formatFloat](auto& group) {
    addCheckboxControl(group, "Enabled", cssState->liquidInteractionEnabled, [cssState, applyCssState, refreshSerializedParams](bool checked) {
      cssState->liquidInteractionEnabled = checked;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Press Scale", cssState->pressScale, 1.f, 1.4f, 0.01f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->pressScale = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Press Refraction", cssState->pressRefraction, 1.f, 2.5f, 0.01f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->pressRefraction = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Speed", cssState->speedSeconds, 0.25f, 2.f, 0.05f, 2, [formatFloat](float value) {
      return formatFloat(value * 1000.f, 0) + "ms";
    }, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->speedSeconds = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Click Squash", cssState->clickSquash, 0.f, 1.5f, 0.05f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->clickSquash = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Drag Squash", cssState->dragSquash, 0.f, 2.f, 0.05f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->dragSquash = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Release Squash", cssState->releaseSquash, 0.f, 1.5f, 0.05f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->releaseSquash = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    group.add.div([](auto& note) {
      note.innerText          = "Release squash currently biases the return timing; true bounce-back needs a timed second transform step.";
      note.style.color        = glint_demo_theme::subtle;
      note.style.fontSize     = 10.f;
      note.style.width        = "100%";
      note.style.textAlign    = EAlign::Near;
      note.style.marginBottom = 10.f;
    });
  });

  addControlGroup(sidebarCtx, "Shadow", [addLiveSliderControl, cssState, applyCssState, refreshSerializedParams](auto& group) {
    addLiveSliderControl(group, "Shadow Opacity", cssState->shadowOpacity, 0.f, 1.f, 0.01f, 2, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->shadowOpacity = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Shadow Blur", cssState->shadowBlur, 0.f, 80.f, 1.f, 0, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->shadowBlur = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Shadow X", cssState->shadowOffsetX, -40.f, 40.f, 1.f, 0, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->shadowOffsetX = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
    addLiveSliderControl(group, "Shadow Y", cssState->shadowOffsetY, -40.f, 40.f, 1.f, 0, {}, [cssState, applyCssState, refreshSerializedParams](float value) {
      cssState->shadowOffsetY = value;
      if (*applyCssState) (*applyCssState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
  });

  if (*applyCssState) (*applyCssState)();
  if (*applyChromaticState) (*applyChromaticState)();

  addControlGroup(sidebarCtx, "Tint and Lighting", [addSliderControl, refreshSerializedParams](auto& group) {
    addSliderControl(group, "Tint Opacity", "tintOpacity", 0.06f, 0.f, 0.5f, 0.01f, 2, {}, refreshSerializedParams);
    addSliderControl(group, "Tint Red", "tintR", 1.f, 0.f, 1.f, 0.01f, 2, {}, refreshSerializedParams);
    addSliderControl(group, "Tint Green", "tintG", 1.f, 0.f, 1.f, 0.01f, 2, {}, refreshSerializedParams);
    addSliderControl(group, "Tint Blue", "tintB", 1.f, 0.f, 1.f, 0.01f, 2, {}, refreshSerializedParams);
    addSliderControl(group, "Specular Opacity", "specularOpacity", 0.48f, 0.f, 1.f, 0.01f, 2, {}, refreshSerializedParams);
    addSliderControl(group, "Specular Angle", "specularAngle", -0.85f, -3.14f, 3.14f, 0.01f, 2, {}, refreshSerializedParams);
    addSliderControl(group, "Specular Width", "specularWidth", 3.f, 1.f, 16.f, 0.1f, 1, {}, refreshSerializedParams);
    addSliderControl(group, "Shadow Opacity", "shadowOpacity", 0.08f, 0.f, 1.f, 0.01f, 2, {}, refreshSerializedParams);
    addSliderControl(group, "Shadow Width", "shadowWidth", 1.f, 0.f, 8.f, 0.1f, 1, {}, refreshSerializedParams);
  });

  addControlGroup(sidebarCtx, "Chromatic Aberration", [addLiveSliderControl, cssState, applyChromaticState, refreshSerializedParams](auto& group) {
    addLiveSliderControl(group, "Chromatic Base", cssState->chromaticBase, 0.f, 2.f, 0.01f, 2, {}, [cssState, applyChromaticState, refreshSerializedParams](float value) {
      cssState->chromaticBase = value;
      if (*applyChromaticState) (*applyChromaticState)();
      if (*refreshSerializedParams) (*refreshSerializedParams)();
    });
  });

  sidebarCtx.add.div([](auto& footer) {
    footer.innerText          = "Shader id: liquid_glass";
    footer.style.color        = glint_demo_theme::subtle;
    footer.style.fontSize     = 11.f;
    footer.style.width        = "100%";
    footer.style.textAlign    = EAlign::Near;
    footer.style.marginTop    = 4.f;
    footer.style.marginBottom = 2.f;
  });

  sidebarCtx.add.div([](auto& label) {
    label.innerText          = "Serialized Params";
    label.style.color        = glint_demo_theme::muted;
    label.style.fontSize     = 11.f;
    label.style.width        = "100%";
    label.style.textAlign    = EAlign::Near;
    label.style.marginTop    = 8.f;
    label.style.marginBottom = 6.f;
  });

  serializedParamsPtr = sidebarCtx.add.fromClass<glint_textarea>([](glint_textarea& ta) {
    ta.readonly              = true;
    ta.style.width           = "100%";
    ta.style.height          = 220.f;
    ta.style.backgroundColor = glint_color(255, 12, 12, 16);
    ta.style.color           = glint_demo_theme::text;
    ta.style.borderWidth     = 1.f;
    ta.style.borderColor     = glint_demo_theme::border;
    ta.style.borderRadius    = 8.f;
    ta.style.padding         = 8.f;
    ta.style.fontSize        = 11.f;
  });

  *refreshSerializedParams = [glassPillPtr, serializedParamsPtr, cssState, formatFloat, formatColor, withAlpha, glassThemeName] {
    if (!serializedParamsPtr) return;
    std::string serialized;
    auto* shader = glassPillPtr->shaders["gls"].get();
    if (!shader) return;

    serialized += "rectWidth:" + formatFloat(cssState->rectWidth, 0);
    serialized += "\nrectHeight:" + formatFloat(cssState->rectHeight, 0);
    serialized += "\nrectRadius:" + formatFloat(cssState->rectRadius, 0);
    serialized += "\nbackgroundBrightness:" + formatFloat(cssState->backgroundBrightness, 2);
    serialized += "\nblurType:gaussian";
    serialized += "\nblur:" + formatFloat(cssState->blurAmount, 0);
    serialized += "\nprogressiveBlur:" + formatFloat(cssState->progressiveBlur, 2);
    serialized += "\nprogressiveBlurMode:edge-falloff";
    serialized += "\nchromaticAberration:" + std::string(cssState->chromaticAberrationEnabled ? "true" : "false");
    serialized += "\nglassTheme:" + std::string(glassThemeName(cssState->glassTheme));
    serialized += "\nglassColor:" + formatColor(withAlpha(cssState->glassColor, cssState->glassBackgroundOpacity));
    serialized += "\nglassBackgroundOpacity:" + formatFloat(cssState->glassBackgroundOpacity, 2);
    serialized += "\npressedGlassOpacity:" + formatFloat(cssState->pressedGlassOpacity, 2);
    serialized += "\nliquidInteractionEnabled:" + std::string(cssState->liquidInteractionEnabled ? "true" : "false");
    serialized += "\npressScale:" + formatFloat(cssState->pressScale, 2);
    serialized += "\npressRefraction:" + formatFloat(cssState->pressRefraction, 2);
    serialized += "\nspeed:" + formatFloat(cssState->speedSeconds, 2);
    serialized += "\nclickSquash:" + formatFloat(cssState->clickSquash, 2);
    serialized += "\ndragSquash:" + formatFloat(cssState->dragSquash, 2);
    serialized += "\nreleaseSquash:" + formatFloat(cssState->releaseSquash, 2);
    serialized += "\nshadowOpacity:" + formatFloat(cssState->shadowOpacity, 2);
    serialized += "\nshadowBlur:" + formatFloat(cssState->shadowBlur, 0);
    serialized += "\nshadowOffsetX:" + formatFloat(cssState->shadowOffsetX, 0);
    serialized += "\nshadowOffsetY:" + formatFloat(cssState->shadowOffsetY, 0);

    for (const auto& spec : kSerializedParams) {
      auto it = shader->params.find(spec.key);
      if (it == shader->params.end()) continue;
      if (const auto* value = std::get_if<float>(&it->second)) {
        if (!serialized.empty()) serialized += "\n";
        serialized += spec.key;
        serialized += ":";
        serialized += formatFloat(*value, spec.decimals);
      }
    }
    serializedParamsPtr->setValue(serialized);
    serializedParamsPtr->setDirty(false);
  };
  (*refreshSerializedParams)();

  controlSidebarPtr->setDirty(false);
}