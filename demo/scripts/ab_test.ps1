<#
.SYNOPSIS
  Before/after check of a change: screenshots and memory of every demo page,
  headless (no window shows), against a saved baseline build.

.DESCRIPTION
  ab_test.ps1 -Baseline
      Builds Release, keeps a copy of the exe as the baseline and runs the
      page scripts (tests\headless) with it. Run before changing code.
  ab_test.ps1
      Builds Release and runs the page scripts with it, comparing every
      screenshot with the baseline's, then prints the failures (screenshots
      that differ by more than 2/255, script errors) and a table of memory
      per page, baseline vs now.

  -Renderer cpu|gpu|both (default gpu), -Pages <filter> (e.g. 'masks|images')
  limits the scripts, -NoBuild uses the exe as it is.

  Screenshots come from a parallel run (seconds). Memory comes from a second
  run with one process at a time (~1 s per page): with many processes on the
  GPU at once each one's driver memory grows by ~9 MB, depending on what
  runs alongside; one at a time, a page reads the same within a few MB.

  Output: build\ab\baseline, build\ab\current (screenshots, *.diff.png for
  differences, report.txt per script) and *.mem (the sequential runs).
#>
param(
  [switch] $Baseline,
  [ValidateSet('gpu', 'cpu', 'both')][string] $Renderer = 'gpu',
  [string] $Pages = '',
  [switch] $NoBuild
)

$ErrorActionPreference = 'Stop'
$demo    = Split-Path -Parent $PSScriptRoot
$runner  = Join-Path $demo '..\third_party\glint\tools\glint_headless_run.ps1'
$exe     = Join-Path $demo 'build\windows-vs2022\Release\glint_demo.exe'
$abDir   = Join-Path $demo 'build\ab'
$baseExe = Join-Path $abDir 'baseline_exe\glint_demo.exe'
$baseOut = Join-Path $abDir 'baseline'
$curOut  = Join-Path $abDir 'current'

$scripts = Get-ChildItem (Join-Path $demo 'tests\headless') -Filter *.txt | Sort-Object Name
if ($Pages) { $scripts = $scripts | Where-Object { $_.BaseName -match $Pages } }
if (-not $scripts) { throw "no scripts match '$Pages'" }

if (-not $NoBuild) {
  $t = [Diagnostics.Stopwatch]::StartNew()
  $o = cmake --build (Join-Path $demo 'build\windows-vs2022') --config Release 2>&1
  if ($LASTEXITCODE -ne 0) { $o | Select-String ' error ' | Select-Object -First 10; throw 'build failed' }
  'build: {0:N0} s' -f $t.Elapsed.TotalSeconds
}

function Fresh($dir) { if (Test-Path -LiteralPath $dir) { Remove-Item -LiteralPath $dir -Recurse -Force }; New-Item -ItemType Directory -Force $dir | Out-Null }
function Run($exePath, $list, $out, [switch] $Sequential, [string] $Against = '') {
  $a = @{ Exe = $exePath; Scripts = $list.FullName; Out = $out; Renderer = $Renderer }
  if ($Sequential) { $a.Jobs = 1 }
  if ($Against) { $a.Compare = $Against }
  & $runner @a
}

if ($Baseline) {
  Fresh (Split-Path $baseExe)
  Copy-Item -LiteralPath $exe -Destination $baseExe
  Fresh $baseOut;       Run $baseExe $scripts $baseOut | Select-Object -Last 1
  Fresh "$baseOut.mem"; Run $baseExe $scripts "$baseOut.mem" -Sequential | Out-Null
  "baseline saved: $(git -C $demo log -1 --format='%h %s') (+ uncommitted changes if any)"
  exit 0
}

if (-not (Test-Path -LiteralPath $baseOut)) { throw 'no baseline: run ab_test.ps1 -Baseline first' }
# Scripts added since the baseline was saved: run them with the baseline now.
$missing = $scripts | Where-Object { -not (Test-Path -LiteralPath (Join-Path $baseOut $_.BaseName)) }
if ($missing) {
  Run $baseExe $missing $baseOut | Out-Null
  Run $baseExe $missing "$baseOut.mem" -Sequential | Out-Null
}

Fresh $curOut;       $summary = Run $exe $scripts $curOut -Against $baseOut
Fresh "$curOut.mem"; Run $exe $scripts "$curOut.mem" -Sequential | Out-Null
$failedLines = $summary | Where-Object { $_ -match 'result failed|\[error\]' }
$totals = $summary | Select-Object -Last 1

# Memory per page: the "mem" lines of each script's sequential run.
function MemOf($dir) {
  $m = @{}
  foreach ($report in Get-ChildItem $dir -Recurse -Filter report.txt) {
    $page = Split-Path (Split-Path $report.FullName) -Leaf
    foreach ($l in Get-Content $report.FullName) {
      if ($l -match ': mem (\S+)\s+-> private_mb=([\d.]+)') { $m["$page $($Matches[1])"] = [double]$Matches[2] }
    }
  }
  return $m
}
$before = MemOf "$baseOut.mem"
$after  = MemOf "$curOut.mem"
''
'{0,-34} {1,9} {2,9} {3,8}' -f 'memory (private MB)', 'baseline', 'now', 'change'
foreach ($k in $after.Keys | Sort-Object) {
  if ($k -match ' start$') { continue }
  $b = $before[$k]; $a = $after[$k]
  $d = if ($null -ne $b) { $a - $b } else { $null }
  $flag = if ($null -ne $d -and [Math]::Abs($d) -ge 5) { '  <' } else { '' }
  '{0,-34} {1,9} {2,9:N1} {3,8}{4}' -f $k, $(if ($null -ne $b) { '{0:N1}' -f $b } else { '-' }), $a, $(if ($null -ne $d) { '{0:+0.0;-0.0;0.0}' -f $d } else { '' }), $flag
}
''
if ($failedLines) { 'FAILURES:'; $failedLines } else { 'all screenshots match the baseline (within 2/255)' }
$totals
"diff images: $curOut\<page>\*.diff.png"
exit $(if ($failedLines) { 1 } else { 0 })
