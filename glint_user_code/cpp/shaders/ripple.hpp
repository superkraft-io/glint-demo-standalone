#pragma once

#include "glint_shader_base.hpp"
#include "glint_shader_registry.hpp"

#include <chrono>

class glint_ripple : public glint_shader_base
{
public:
  glint_ripple() { animated = false; isBackdrop = true; }
  float sampleRadius() const override { return getFloat("amplitude", 14.f) + 2.f; }

  void trigger(float localX, float localY)
  {
    params["centerX"] = localX;
    params["centerY"] = localY;
    mRippleStart      = std::chrono::steady_clock::now();
    animated          = true;
  }
  void onMouseDown(float localX, float localY) override { trigger(localX, localY); }

  const char* sksl() const override
  {
    return R"(
      uniform shader src;
      uniform float2 resolution;
      uniform float  amplitude;
      uniform float  wavelength;
      uniform float  speed;
      uniform float  duration;
      uniform float  age;
      uniform float2 center;

      half4 main(float2 coord) {
        if (age <= 0.0) return src.eval(coord);

        float2 delta    = coord - center;
        float  dist     = length(delta);
        float  front    = age * speed;
        float  envelope = exp(-pow((dist - front) / (wavelength * 1.5), 2.0));
        float  wave     = sin((dist - front) / wavelength * 6.2832);
        float  fade     = smoothstep(duration, duration * 0.5, age);
        float  ripple   = wave * envelope * amplitude * fade;
        float2 dir      = dist > 0.001 ? delta / dist : float2(0.0, 0.0);
        return src.eval(coord + dir * ripple);
      }
    )";
  }

  void setUniforms(SkRuntimeShaderBuilder& b, float w, float h, float) override
  {
    using namespace std::chrono;
    float age = 0.f;
    if (animated) {
      age = duration_cast<duration<float>>(steady_clock::now() - mRippleStart).count();
      if (age >= getFloat("duration", 1.6f)) {
        animated = false;
        age      = getFloat("duration", 1.6f);
      }
    }
    b.uniform("resolution") = SkV2{w, h};
    b.uniform("amplitude")  = getFloat("amplitude",  14.f);
    b.uniform("wavelength") = getFloat("wavelength", 36.f);
    b.uniform("speed")      = getFloat("speed",      260.f);
    b.uniform("duration")   = getFloat("duration",   1.6f);
    b.uniform("age")        = age;
    b.uniform("center")     = SkV2{mCurrentRect.L + getFloat("centerX", w * 0.5f),
                                   mCurrentRect.T + getFloat("centerY", h * 0.5f)};
  }

private:
  std::chrono::steady_clock::time_point mRippleStart;
};

static bool _sk_ripple_reg = glint_shader_registry::add(
  "ripple", [] { return std::make_unique<glint_ripple>(); });
