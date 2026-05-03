#pragma once

#include "glint_shader_base.hpp"
#include "glint_shader_registry.hpp"

class glint_lens_refraction : public glint_shader_base
{
public:
  glint_lens_refraction() { animated = false; isBackdrop = true; }

  float sampleRadius() const override { return 80.f; }

  const char* sksl() const override
  {
    return R"(
      uniform shader src;
      uniform float2 resolution;
      uniform float2 origin;
      uniform float  thickness;
      uniform float  ior;
      uniform float  baseHeight;
      uniform float  colorMix;

      float sdfRoundedRect(float2 center, float2 halfSize, float2 p, float r) {
        float2 q = abs(p - center) - halfSize;
        return length(max(q, float2(0.0))) + min(max(q.x, q.y), 0.0) - r;
      }

      float3 lensNormal(float sd, float dx, float dy, float thick) {
        float n_cos = clamp((thick + sd) / thick, 0.0, 1.0);
        float n_sin = sqrt(max(1.0 - n_cos * n_cos, 0.0));
        return normalize(float3(dx * n_cos, dy * n_cos, n_sin));
      }

      float lensHeight(float sd, float thick) {
        if (sd >= 0.0)     return 0.0;
        if (sd < -thick)   return thick;
        float x = thick + sd;
        return sqrt(thick * thick - x * x);
      }

      half4 main(float2 coord) {
        float2 local  = coord - origin;
        float2 center = resolution * 0.5;

        float r  = min(resolution.x, resolution.y) * 0.4;
        float hx = max(resolution.x * 0.5 - r, 0.0);

        float sd = sdfRoundedRect(center, float2(hx, 0.0), local, r);

        half4 bgSample = src.eval(coord);
        float bgBlend  = smoothstep(-4.0, 0.0, sd);
        if (bgBlend >= 1.0) return bgSample;

        float eps = 1.0;
        float sdR = sdfRoundedRect(center, float2(hx, 0.0), local + float2(eps,  0.0), r);
        float sdD = sdfRoundedRect(center, float2(hx, 0.0), local + float2(0.0,  eps), r);
        float dx  = sdR - sd;
        float dy  = sdD - sd;

        float3 N = lensNormal(sd, dx, dy, thickness);
        float3 I = float3(0.0, 0.0, -1.0);

        float3 R    = refract(I, N, 1.0 / ior);
        float  h    = lensHeight(sd, thickness);
        float  tLen = (h + baseHeight) / max(-R.z, 0.001);
        half4  refractCol = src.eval(coord + R.xy * tLen);

        float3 Refl   = reflect(I, N);
        float  spec   = clamp(abs(Refl.x - Refl.y), 0.0, 1.0);
        half4  reflCol = half4(spec, spec, spec, 0.0);

        half4 lensCol = mix(
          mix(refractCol, reflCol, (1.0 - N.z) * 2.0),
          half4(1.0),
          colorMix
        );
        lensCol = clamp(lensCol, 0.0, 1.0);

        return mix(lensCol, bgSample, bgBlend);
      }
    )";
  }

  void setUniforms(SkRuntimeShaderBuilder& b, float w, float h, float) override
  {
    b.uniform("resolution") = SkV2{w, h};
    b.uniform("origin")     = SkV2{mCurrentRect.L, mCurrentRect.T};
    b.uniform("thickness")  = getFloat("thickness",   14.f);
    b.uniform("ior")        = getFloat("ior",          1.5f);
    b.uniform("baseHeight") = getFloat("baseHeight", 112.f);
    b.uniform("colorMix")   = getFloat("colorMix",    0.3f);
  }
};

static bool _sk_lens_refraction_reg = glint_shader_registry::add(
  "lens_refraction", [] { return std::make_unique<glint_lens_refraction>(); });
