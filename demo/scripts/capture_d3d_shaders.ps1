# Captures the Direct3D shaders the demo uses into demo/shaders/glint_d3d_shaders.bin,
# which the build embeds in glint_demo.exe (glint_embed_d3d_shaders in CMakeLists.txt).
# With it, pages open without compiling shaders, even on a user's first launch.
#
# Run again after updating Skia or changing what pages draw: shaders the pack
# doesn't hold still work, they are just compiled (and disk-cached) at first use.
#
# Steps: build Release + Debug, run each one's capture tour (every page, scrolled
# through) into one pack, then rebuild both so they embed it. Most shaders are
# shared (glint_d3d_shader_cache.hpp drops Debug Skia's debug compile flags); a few
# differ because Debug Skia generates slightly different HLSL.

$ErrorActionPreference = 'Stop'
$demo  = Split-Path -Parent $PSScriptRoot
$build = Join-Path $demo 'build\windows-vs2022'
$pack  = Join-Path $demo 'shaders\glint_d3d_shaders.bin'

Push-Location $demo
try {
  cmake --preset windows-vs2022 | Out-Null
  foreach ($config in 'Release', 'Debug') {
    cmake --build $build --config $config
    if ($LASTEXITCODE -ne 0) { throw "build $config failed" }
  }

  New-Item -ItemType Directory -Force (Split-Path $pack) | Out-Null
  if (Test-Path -LiteralPath $pack) { Remove-Item -LiteralPath $pack -Force }

  $env:GLINT_D3D_SHADER_CAPTURE = $pack
  # A private, empty disk cache: every shader goes through the capture.
  $env:GLINT_D3D_SHADER_CACHE_DIR = Join-Path ([IO.Path]::GetTempPath()) ("glint_shader_capture_" + [guid]::NewGuid().ToString('N'))
  try {
    foreach ($config in 'Release', 'Debug') {
      Write-Host "Capturing shaders ($config): the demo opens every page and closes by itself..."
      $exeDir = Join-Path $build $config
      $p = Start-Process (Join-Path $exeDir 'glint_demo.exe') -WorkingDirectory $exeDir -PassThru
      if (-not $p.WaitForExit(600000)) { Stop-Process -Id $p.Id -Force; throw "capture ($config) timed out" }
    }
  }
  finally {
    Remove-Item Env:GLINT_D3D_SHADER_CAPTURE
    if (Test-Path -LiteralPath $env:GLINT_D3D_SHADER_CACHE_DIR) { Remove-Item -LiteralPath $env:GLINT_D3D_SHADER_CACHE_DIR -Recurse -Force }
    Remove-Item Env:GLINT_D3D_SHADER_CACHE_DIR
  }

  $bytes = [IO.File]::ReadAllBytes($pack)
  Write-Host ("Captured {0} shaders ({1:N0} KB) into {2}" -f [BitConverter]::ToUInt32($bytes, 8), ($bytes.Length / 1KB), $pack)

  # Re-configure (the pack may be new) and rebuild so the exes embed it.
  cmake --preset windows-vs2022 | Out-Null
  foreach ($config in 'Release', 'Debug') {
    cmake --build $build --config $config
    if ($LASTEXITCODE -ne 0) { throw "build $config failed" }
  }
}
finally {
  Pop-Location
}
