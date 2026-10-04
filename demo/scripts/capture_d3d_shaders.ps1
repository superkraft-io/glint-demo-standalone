# Captures the Direct3D shaders the demo uses into demo/shaders/glint_d3d_shaders.bin,
# which the build embeds in glint_demo.exe (glint_embed_d3d_shaders in CMakeLists.txt).
# With it, pages open without compiling shaders, even on a user's first launch.
#
# Run again after updating Skia or changing what pages draw: shaders the pack
# doesn't hold still work, they are just compiled (and disk-cached) at first use.
#
# Steps: build Release + Debug, run each one headless over every demo page
# (tests\headless: opened, hovered, scrolled) into one pack, then rebuild both
# so they embed it. No window shows. Most shaders are
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
  # Headless (no window): every page of tests\headless, opened, hovered over a
  # grid of points (hover states use their own shaders), scrolled, hovered
  # again. One process at a time: each adds its shaders to the pack file.
  $work = Join-Path ([IO.Path]::GetTempPath()) ("glint_shader_capture_scripts_" + [guid]::NewGuid().ToString('N'))
  New-Item -ItemType Directory -Force $work | Out-Null
  $moves = foreach ($y in (0..15 | ForEach-Object { 40 + $_ * 60 })) {
    foreach ($x in (0..17 | ForEach-Object { 200 + $_ * 80 })) { "move $x $y"; 'frames 1' }
  }
  foreach ($page in Get-ChildItem (Join-Path $demo 'tests\headless') -Filter *.txt) {
    $name = (Get-Content -LiteralPath $page.FullName | Where-Object { $_ -match '^page ' } | Select-Object -First 1)
    if (-not $name) { continue }
    $lines = @('size 1600 1000 1', 'wait', $name, 'wait') + $moves + @('scroll 900 500 800', 'wait') + $moves + @('wait')
    Set-Content -LiteralPath (Join-Path $work $page.Name) -Value $lines -Encoding ascii
  }
  $runner = Join-Path $demo '..\third_party\glint\tools\glint_headless_run.ps1'
  try {
    foreach ($config in 'Release', 'Debug') {
      Write-Host "Capturing shaders ($config), headless..."
      $exe = Join-Path $build "$config\glint_demo.exe"
      & $runner -Exe $exe -Scripts $work -Out (Join-Path $work "out_$config") -Jobs 1 -TimeoutSec 300 | Select-Object -Last 1 | Write-Host
    }
  }
  finally {
    if (Test-Path -LiteralPath $work) { Remove-Item -LiteralPath $work -Recurse -Force }
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
