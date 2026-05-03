#pragma once

#include "glint_shader_base.hpp"
#include "glint_shader_registry.hpp"

class glint_liquid_glass : public glint_shader_base
{
public:
  glint_liquid_glass() { animated = true; isBackdrop = true; }
  float sampleRadius() const override { return getFloat("strength", 18.f) + 4.f; }

  const char* sksl() const override
  {
    return R"(
      uniform shader src;
      uniform float2 resolution;
      uniform float2 origin;
      uniform float  strength;
      uniform float  time;
      uniform float  speed;

      half4 main(float2 coord) {
        float2 local = coord - origin;
        float2 uv = local / resolution;
        float  t  = time * speed;
        float2 d;
        d.x = sin(uv.y * 6.2832 * 2.0 + t) * cos(uv.x * 3.1416 + t * 0.7);
        d.y = cos(uv.x * 6.2832 * 2.0 + t * 1.3) * sin(uv.y * 3.1416 + t * 0.5);
        half4 c    = src.eval(coord + d * strength);
        float spec = pow(max(0.0, sin(uv.y * 8.0 + t * 1.5)), 6.0) * 0.25;
        return c + half4(spec, spec, spec, 0.0);
      }
    )";
  }

  void setUniforms(SkRuntimeShaderBuilder& b, float w, float h, float t) override
  {
    b.uniform("resolution") = SkV2{w, h};
    b.uniform("origin")     = SkV2{mCurrentRect.L, mCurrentRect.T};
    b.uniform("strength")   = getFloat("strength", 18.f);
    b.uniform("time")       = t;
    b.uniform("speed")      = getFloat("speed", 1.f);
  }
};

static bool _sk_liquid_glass_reg = glint_shader_registry::add(
  "liquid_glass", [] { return std::make_unique<glint_liquid_glass>(); });
