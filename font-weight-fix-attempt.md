# Attempt 1

## Problem

Font weight appeared to be broken even though the CSS and style system already carried `font-weight` values correctly.

The real issue was in the lower text rendering path used by `DrawText()`-based controls. During `@font-face` loading, multiple weighted faces in a family such as `Kanit` were being registered into the graphics layer under the same family-level font ID. Later, `DrawText()` would resolve text by font name and fall back to a normal-weight CoreText family lookup, which collapsed the selected weight back to a default face.

That meant controls rendered through the `DrawText()` path could ignore the requested weight even though the higher-level font registry knew about the correct weighted variants.

## Fix

I changed the font registration and draw path so weighted variants stay distinct all the way to rendering:

- In `third_party/glint/glint_document.hpp`, each `@font-face` variant now gets a concrete graphics font ID derived from the font file name, instead of reusing the shared family name for every weight/style variant.
- The axis registry now stores that concrete per-variant graphics font ID, so resolving `(family, weight, style)` returns the actual loaded face for the requested variant.
- In `third_party/glint/glint_graphics.hpp`, `DrawText()` now first checks Glint's registered `SkTypeface` map for the resolved font ID before falling back to platform family lookup.
- I also updated the font demos to actually test `fontWeight` on the unified `Kanit` family, instead of only swapping between separate family names such as `Kanit-Bold` and `Kanit-Light`.

## Result

Weighted custom fonts now resolve correctly in `DrawText()`-based controls, and the demo pages actually exercise the fixed `fontWeight` behavior.

## Validation

- `cmake --build --preset ios-sim-debug`

# Attempt 2

## What Attempt 1 Missed

Attempt 1 fixed a real issue in the `DrawText()` path, but that was not the reason the iOS demo still looked broken.

The remaining bug was a startup ordering problem combined with overly aggressive typeface caching:

- On iOS, the demo built the document shell first and then deferred `completeDocumentSetup()` onto the main queue.
- That meant the first render pass could ask for `Kanit` before `@font-face` registration had run.
- When that happened, `skFont()` fell back to a system face, which on the simulator ended up as Helvetica.
- `skFont()` then cached that substituted face for `(family="Kanit", weight=..., style=...)`.
- Even after the stylesheet finally loaded and the real Kanit faces were registered, later lookups reused the cached fallback.

So the actual failure was not "font-weight math is wrong" or "DrawText is the only broken path".
It was "the first text resolution happens before fonts are registered, and the fallback result gets pinned".

## Actual Fix

The final fix had two parts.

### 1. Load the stylesheet synchronously during iOS document creation

The deferred iOS setup was changed so stylesheet loading happens immediately in `onDocumentCreated`, instead of being pushed to a later main-queue turn.

Before, iOS used:

```mm
controller->_demo->completeDocumentSetup(true);
```

After the fix, it uses:

```mm
options.onDocumentCreated = [controller](glint_document& document) {
	controller->_demo->attachToDocument(
		document,
		[controller]() {
			if (controller->_glintView)
				controller->_glintView->requestRedraw();
		},
		[](std::function<void()> fn) {
			dispatch_async(dispatch_get_main_queue(), ^{
				fn();
			});
		});
	[controller syncDemoMetricsForBounds:controller.view.bounds];
	controller->_demo->completeDocumentSetup(false);
};
```

That change ensures `loadStylesheet("/styles/main.css")` and `@font-face` registration happen before the first meaningful render of the text demo.

### 2. Do not cache substituted fallback faces for unresolved custom families

Even with the ordering fix, `skFont()` also needed to stop permanently caching a substituted system fallback face for a custom family that had not resolved yet.

The relevant code in `third_party/glint/element/glint_element_render.hpp` now behaves like this:

```cpp
static std::unordered_map<_TfKey, sk_sp<SkTypeface>, _TfKeyHash> sTfCache;

sk_sp<SkTypeface> tf;
const bool hasFamily = family && family[0];
if (hasFamily)
{
	_TfKey key{ std::string(family), weight, std::string(style ? style : "") };
	auto it = sTfCache.find(key);
	if (it != sTfCache.end()) {
		tf = it->second;
	} else {
		bool shouldCache = false;

		tf = glint_font_registry::getTypefaceByAxes(family, weight, style);
		if (tf)
		{
			shouldCache = true;
		}
		else
		{
			tf = glint_font_registry::getTypefaceWeighted(family, weight);
			if (tf)
				shouldCache = true;
		}

		if (!tf)
		{
			tf = glint_font_registry::getSystemTypefaceByAxes(family, weight, style);
			if (tf)
			{
				SkString matchedFamily;
				tf->getFamilyName(&matchedFamily);
				shouldCache = matchedFamily.equals(family);
			}
		}

		if (shouldCache)
			sTfCache.emplace(std::move(key), tf);
	}
}
if (!tf) tf = *sFallback;
```

The important part is this:

```cpp
if (tf)
{
	SkString matchedFamily;
	tf->getFamilyName(&matchedFamily);
	shouldCache = matchedFamily.equals(family);
}
```

That prevents a substituted family such as Helvetica from being cached under `Kanit`.

## Why This Actually Fixed It

After these two changes:

- the Kanit faces are registered before the demo text first asks for them, and
- even if an early fallback ever happens, it does not get cached as if it were the real `Kanit` family.

That combination is what made the font weights visibly start working on iOS.

Attempt 1 improved the lower rendering path, but Attempt 2 fixed the actual runtime behavior that was making every weight look the same.

## Validation

- `cd demo && cmake --build --preset ios-sim-debug`
- install the freshly built app bundle onto the simulator:

```sh
xcrun simctl install booted build/ios-sim/Debug-iphonesimulator/glint_demo.app
```

- relaunch the simulator app:

```sh
xcrun simctl terminate booted io.superkraft.glintdemo || true
xcrun simctl launch booted io.superkraft.glintdemo
```

- final visual verification on the simulator showed the Kanit weight differences correctly

## Final Conclusion

Attempt 2 was the real fix.

The working solution was not another change to CSS parsing or another tweak to `DrawText()`.
It was:

1. removing the iOS deferred stylesheet/font-loading race, and
2. preventing `skFont()` from caching substituted fallback faces for unresolved custom families.
