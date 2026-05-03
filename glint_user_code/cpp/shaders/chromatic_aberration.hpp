#pragma once

#include "glint_shader_base.hpp"
#include "glint_shader_registry.hpp"

class glint_chromatic_aberration : public glint_shader_base
{
public:
  glint_chromatic_aberration() { animated = false; isBackdrop = true; }
  float sampleRadius() const override { return getFloat("strength", 30.f) + 4.f; }

  const char* sksl() const override
  {
    return R"(
      uniform shader src;
      uniform float2 resolution;
      uniform float2 origin;
      uniform float  strength;

      half4 main(float2 coord) {
        float2 local  = coord - origin;
        float2 center = resolution * 0.5;
        float2 dir    = (local - center) / max(resolution.x, resolution.y);
        float  dist2  = dot(dir, dir);
        float2 offset = dir * dist2 * strength;
        float r = src.eval(coord + offset).r;
        float g = src.eval(coord).g;
        float b = src.eval(coord - offset).b;
        float a = src.eval(coord).a;
        return half4(r, g, b, a);
      }
    )";
  }

  void setUniforms(SkRuntimeShaderBuilder& b, float w, float h, float) override
  {
    b.uniform("resolution") = SkV2{w, h};
    b.uniform("origin")     = SkV2{mCurrentRect.L, mCurrentRect.T};
    b.uniform("strength")   = getFloat("strength", 30.f);
  }
};

static bool _sk_chroma_reg = glint_shader_registry::add(
  "chromatic_aberration", [] { return std::make_unique<glint_chromatic_aberration>(); });
