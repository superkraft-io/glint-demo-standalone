#pragma once

#include "glint_shader_base.hpp"
#include "glint_shader_registry.hpp"

class glint_aurora : public glint_shader_base
{
public:
  glint_aurora() { animated = true; isBackdrop = false; }

  const char* sksl() const override
  {
    return R"(
      uniform float2 resolution;
      uniform float  time;
      uniform float  speed;

      float hash(float2 p) {
        p = fract(p * float2(234.34, 435.345));
        p += dot(p, p + 34.23);
        return fract(p.x * p.y);
      }
      float noise(float2 p) {
        float2 i = floor(p), f = fract(p);
        float2 u = f * f * (3.0 - 2.0 * f);
        return mix(mix(hash(i),               hash(i + float2(1,0)), u.x),
                   mix(hash(i + float2(0,1)), hash(i + float2(1,1)), u.x), u.y);
      }
      half4 main(float2 coord) {
        float2 uv  = coord / resolution;
        float  t   = time * speed * 0.3;
        float band = smoothstep(0.1, 0.5, uv.y) * smoothstep(1.0, 0.6, uv.y);
        float n1   = noise(float2(uv.x * 3.0 + t,        uv.y * 2.0));
        float n2   = noise(float2(uv.x * 6.0 - t * 1.3,  uv.y * 3.0 + 0.5));
        float n3   = noise(float2(uv.x * 4.0 + t * 0.7,  uv.y * 5.0 + 1.0));
        float a    = band * (n1 * 0.6 + n2 * 0.3 + n3 * 0.1);
        half3 col  = half3(0.1, 0.8, 0.6) * n1 * band
                   + half3(0.6, 0.1, 0.9) * n2 * band
                   + half3(0.1, 0.9, 0.2) * n3 * band * 0.5;
        return half4(col * a, a * 0.9);
      }
    )";
  }

  void setUniforms(SkRuntimeShaderBuilder& b, float w, float h, float t) override
  {
    b.uniform("resolution") = SkV2{w, h};
    b.uniform("time")       = t;
    b.uniform("speed")      = getFloat("speed", 1.f);
  }
};

static bool _sk_aurora_reg = glint_shader_registry::add(
  "aurora", [] { return std::make_unique<glint_aurora>(); });
