# Touch Events Lab

This document captures the touch-event demo that previously lived inside Glint Edit so it can be moved into the official Glint demo app.

## Purpose

The demo is a modal-style interaction lab for validating Glint's DOM-style trackpad input events. It is designed to answer two questions quickly:

1. Are wheel events reaching Glint elements with the expected deltas and phase metadata?
2. Are native gesture events such as pinch, rotate, swipe, and smart zoom arriving with usable payloads and phases?

The demo is not tied to cursor-editor behavior. It is a self-contained event visualizer with a single interactive stage, a transformable target tile, four metric cards, and a rolling event log.

## Original Source Files

The original implementation in this repo was split across these files:

- `demo/demos/cursor_editor/touch_demo_modal.hpp`
- `demo/demos/window.hpp`
- `demo/demos/cursor_editor/shortcut_modal.hpp`
- `glint_user_code/web/styles/main.css`

The modal implementation itself was entirely in `touch_demo_modal.hpp`. The other files only wired it into the Glint Edit app and supplied CSS.

## Glint Event Types Used

The demo relies on the Glint event types defined in `third_party/glint/events/glint_event.hpp`.

### `glint_wheel_event`

Relevant fields used by the demo:

- `deltaX`
- `deltaY`
- `hasPreciseDeltas`
- `phase`
- `momentumPhase`

Behavioral notes:

- The event is listened for via `addEventListener("wheel", ...)`.
- The handler calls `preventDefault()` so the stage consumes the interaction instead of allowing default scroll behavior.
- The demo interprets `deltaX` as horizontal drift and `deltaY` as vertical drift.

### `glint_gesture_event`

Relevant fields used by the demo:

- `kind`
- `phase`
- `deltaX`
- `deltaY`
- `magnification`
- `scale`
- `rotation`

Behavioral notes:

- The event is listened for via `addEventListener("gesture", ...)`.
- The handler also calls `preventDefault()`.
- The demo uses `kind` to branch between pinch, rotate, swipe, and smart zoom.

### Enum string mapping used in the UI

Input phases were rendered as:

- `may_begin` -> `may-begin`
- `began` -> `began`
- `changed` -> `changed`
- `ended` -> `ended`
- `cancelled` -> `cancelled`
- `none` -> `none`

Gesture kinds were rendered as:

- `pinch` -> `pinch`
- `rotate` -> `rotate`
- `swipe` -> `swipe`
- `smart_zoom` -> `smart-zoom`
- `none` -> `lifecycle`

## UI Structure

The demo was a full-window overlay modal with a centered card.

### Backdrop

- Absolute-positioned element covering the full window.
- Uses a translucent dark backdrop.
- Clicking the backdrop hides the modal.
- Hit testing returns `nullptr` when opacity is effectively zero so the underlying app remains interactive while the modal is hidden.

### Card layout

The modal card is split into three major areas:

1. Header
2. Main content row
3. Event log column

#### Header contents

- A small `F2` badge in the original app, indicating the keyboard shortcut used there.
- Title: `Touch Events Lab`
- Subtitle: `Trackpad wheel and gesture events routed through Glint's DOM-style event system.`
- A `Reset` button that clears counters, transforms, phases, and log entries.

In the official demo app, the `F2` badge can be changed or removed depending on how the demo is launched.

#### Main content row

The left side contains:

- Four metric cards: `Wheel`, `Gesture`, `Transform`, and `Counters`
- A large interaction stage

The right side contains:

- `Event Stream` title
- Small explanatory hint text
- A vertical log list with 10 rows

### Stage contents

The stage contains:

- A centered hint line: `Scroll to drift. Pinch to scale. Rotate to spin. Swipe to nudge. Smart zoom to snap.`
- A transformable tile labeled `Gesture target`
- A caption under the tile showing current gesture kind and event counters

The stage itself is the element that receives `wheel` and `gesture` listeners.

## Internal State Model

The demo tracked this state:

- `mWheelCount`
- `mGestureCount`
- `mWheelTotalX`
- `mWheelTotalY`
- `mScale`
- `mRotationDeg`
- `mOffsetX`
- `mOffsetY`
- `mLastWheelPhase`
- `mLastMomentumPhase`
- `mLastGestureKind`
- `mLastGesturePhase`
- `mLogs`

Important defaults:

- `mScale = 1.0f`
- `mRotationDeg = 0.0f`
- `mOffsetX = 0.0f`
- `mOffsetY = 0.0f`
- all phase and gesture labels start as `none`

The log buffer kept only the most recent 10 entries.

## Wheel Handling Logic

Each wheel event does the following:

1. Increment `mWheelCount`.
2. Add `deltaX` into `mWheelTotalX`.
3. Add `deltaY` into `mWheelTotalY`.
4. Convert `phase` and `momentumPhase` into display strings.
5. Translate the tile:
   - `mOffsetX += deltaX * 0.12f`
   - `mOffsetY -= deltaY * 0.12f`
6. Clamp both offsets into `[-140, 140]`.
7. Push a formatted log row.
8. Refresh metrics, tile transform, and log rows.

The wheel log line format was:

```text
wheel dx=%0.1f dy=%0.1f phase=%s momentum=%s precise=%s
```

`precise` was rendered as `yes` or `no` from `hasPreciseDeltas`.

## Gesture Handling Logic

Every gesture event increments `mGestureCount`, records the current gesture kind and phase strings, then applies behavior based on `kind`.

### Pinch

- Only updates on `phase == changed`
- Multiplies the current scale by `max(0.35f, ge.scale)`
- Clamps final scale into `[0.45, 3.4]`

### Rotate

- Only updates on `phase == changed`
- Applies `mRotationDeg -= ge.rotation`

### Swipe

- Applies immediately regardless of phase
- Uses gesture deltas as a larger nudge than wheel movement:
  - `mOffsetX += ge.deltaX * 56.f`
  - `mOffsetY -= ge.deltaY * 56.f`
- Clamps both offsets into `[-140, 140]`

### Smart zoom

- Only updates on `phase == changed`
- Toggles between a normal and zoomed-in state:
  - if current scale is greater than `1.4f`, snap back to `1.0f`
  - otherwise snap to `2.0f`
- Also resets translation and rotation to zero

### None / lifecycle-only event

- No transform is applied
- The event is still logged and the gesture/phase cards still update

The gesture log line format was:

```text
gesture %s %s dx=%0.2f dy=%0.2f mag=%0.3f rot=%0.2f
```

The logged values are:

- gesture kind string
- phase string
- `deltaX`
- `deltaY`
- `magnification`
- `rotation`

## Metrics Display

The four metric cards showed:

### Wheel

```text
dx <totalX>   dy <totalY>   phase <phase> / <momentumPhase>
```

This card accumulates total wheel travel and shows both the direct phase and momentum phase.

### Gesture

```text
<gestureKind> / <gesturePhase>
```

This card reports only the most recent gesture kind and phase.

### Transform

```text
scale <scale>  rotate <rotationDeg>  offset <offsetX>,<offsetY>
```

This card reports the effective transform applied to the tile.

### Counters

```text
wheel <count>   gesture <count>
```

This card is useful for confirming whether a platform emits wheel, gesture, or both during a trackpad interaction.

## Tile Transform

The tile is updated by setting `style.transform` with this composition order:

```text
translate(<offsetX>px, <offsetY>px) rotate(<rotationDeg>deg) scale(<scale>)
```

That order should be preserved if visual parity matters.

The tile caption format was:

```text
kind <lastGestureKind> | wheel <wheelCount> | gesture <gestureCount>
```

## Log Behavior

- The log stores up to 10 rows.
- Newest events appear at the top of the UI.
- Storage is FIFO internally, but rendering reverses the order so the most recent row is first.
- Reset clears the buffer and inserts one new message: `Touch lab reset.`

Initial startup also inserts one message:

```text
Touch lab ready. Use two-finger scroll, pinch, rotate, or swipe on the stage.
```

## Original Styling

The original CSS used these classes:

- `.touch-demo-backdrop`
- `.touch-demo-card`
- `.touch-demo-header`
- `.touch-demo-badge`
- `.touch-demo-title-wrap`
- `.touch-demo-title`
- `.touch-demo-subtitle`
- `.touch-demo-divider`
- `.touch-demo-content`
- `.touch-demo-stage-col`
- `.touch-demo-metric-grid`
- `.touch-demo-metric`
- `.touch-demo-metric-title`
- `.touch-demo-metric-value`
- `.touch-demo-stage`
- `.touch-demo-stage-hint`
- `.touch-demo-tile`
- `.touch-demo-tile-title`
- `.touch-demo-tile-caption`
- `.touch-demo-log-col`
- `.touch-demo-log-title`
- `.touch-demo-log-hint`
- `.touch-demo-log-list`
- `.touch-demo-log-row`

Visually, the design was a dark modal with a blue-tinted stage, a glassy tile, and a narrow fixed-width log column.

## Integration Notes For The Official Demo App

To move this into the official Glint demo app, the cleanest split is:

1. Keep the interaction logic as its own `glint_element` subclass.
2. Let the host demo app decide how it is launched.
3. Replace the Glint Edit-specific `F2` affordance with the host app's normal navigation or demo picker.

Recommended host responsibilities:

- Create and own the demo element.
- Decide whether it lives in a modal, a dedicated demo panel, or a tab.
- Supply CSS or inline style equivalents for the listed class names.

Recommended element responsibilities:

- Build the card
- Register `wheel` and `gesture` listeners on the stage
- Maintain counters, transforms, and logs
- Refresh metric and log views after each event
- Provide `show`, `hide`, and `toggle` only if the host still wants modal behavior

## App-Specific Wiring That Was Removed Here

The Glint Edit app had two app-level entry points:

- `F2` toggled the demo
- The shortcuts overlay exposed a `Touch Lab` button

Those hooks were editor-specific conveniences and are not required by the demo logic itself.

## Minimal Port Checklist

If rebuilding this in the official Glint demo app, preserve these pieces first:

1. One stage element listening for `wheel` and `gesture`
2. The state fields for counts, totals, phases, scale, rotation, offsets, and logs
3. The per-gesture transform rules and clamps
4. The metric cards and newest-first event log
5. The tile transform string order: translate, then rotate, then scale

Everything else, especially modal launch mechanics and styling details, can be adapted to the destination app.