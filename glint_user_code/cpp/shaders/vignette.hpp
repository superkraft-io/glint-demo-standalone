#pragma once

#include "glint_shader_base.hpp"
#include "glint_shader_registry.hpp"

class glint_vignette : public glint_shader_base
{
public:
  glint_vignette() { animated = false; isBackdrop = true; }

  float sampleRadius() const override { return 0.f; }

  const char* sksl() const override
  {
    return R"(
      uniform shader src;
      uniform float2 resolution;
      uniform float2 origin;
      uniform float  strength;
      uniform float4 tintColor;

      half4 main(float2 coord) {
        half4  col    = src.eval(coord);
        float2 local  = coord - origin;
        float2 uv     = local / resolution;
        float  aspect = resolution.x / resolution.y;
        uv.x  *= aspect;
        float2 center = float2(aspect * 0.5, 0.5);
        float  d  = distance(uv, center);
        float  v  = clamp(d * d * strength, 0.0, 1.0);
        half4  tint = half4(half3(tintColor.rgb) * half(tintColor.a), half(tintColor.a));
        return mix(col, tint, half(v));
      }
    )";
  }

  void setUniforms(SkRuntimeShaderBuilder& b, float w, float h, float) override
  {
    b.uniform("resolution") = SkV2{w, h};
    b.uniform("origin")     = SkV2{mCurrentRect.L, mCurrentRect.T};
    b.uniform("strength")   = getFloat("strength", 1.6f);
    glint_color c = getColor("tintColor", glint_color(255, 0, 0, 0));
    b.uniform("tintColor")  = SkV4{c.R / 255.f, c.G / 255.f, c.B / 255.f, c.A / 255.f};
  }
};

static bool _sk_vignette_reg = glint_shader_registry::add(
  "vignette", [] { return std::make_unique<glint_vignette>(); });
