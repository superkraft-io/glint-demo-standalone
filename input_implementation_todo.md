# Input Implementation Todo

Track Glint `<input>` support against the current HTML standard and mobile behavior requirements.

Notes:
- `datetime` is deprecated and should not be implemented as a modern HTML input type.
- `datetime-local` is the current standard type.
- `inputmode` is only a virtual keyboard hint; it does not change validation semantics.
- Current proven Glint support in code: `text`, `email`, `password`, `number`, `range`, `checkbox`, and `radio`.
- Unsupported `type` values currently fall through the text delegate unless they have a dedicated Glint implementation elsewhere.
- `inputmode` is now plumbed through `glint_input` and `glint_textarea` into iOS keyboard traits, including `inputmode="none"` software-keyboard suppression.
- iOS long-press edit actions now route Cut, Copy, Paste, and Select All through the focused Glint text editor, but the mobile behavior items stay unchecked until runtime verification on the simulator/device.

## Core Mobile Behavior

- [ ] Typing inserts text reliably for focused text controls
- [ ] Backspace deletes reliably for focused text controls
- [ ] Return key behavior matches control semantics
- [x] Tapping outside a text control dismisses the software keyboard
- [x] Switching between focused controls updates keyboard type immediately
- [ ] Selection and caret behavior remain correct on iOS
- [ ] Copy, paste, cut, and select-all behavior works on mobile

## Input Types

- [ ] `hidden`
- [x] `text`
- [ ] `search`
- [ ] `tel`
- [ ] `url`
- [x] `email`
- [x] `password`
- [ ] `date`
- [ ] `month`
- [ ] `week`
- [ ] `time`
- [ ] `datetime-local`
- [x] `number`
- [x] `range`
- [ ] `color`
- [x] `checkbox`
- [x] `radio`
- [ ] `file`
- [ ] `submit`
- [ ] `image`
- [ ] `reset`
- [ ] `button`

## Input Modes

- [x] `none`
- [x] `text`
- [x] `decimal`
- [x] `numeric`
- [x] `tel`
- [x] `search`
- [x] `email`
- [x] `url`

## Type vs Inputmode Mapping

- [ ] `type` controls validation and submitted value semantics
- [x] `inputmode` only affects virtual keyboard choice
- [x] `type="text" inputmode="numeric"` behaves as text with numeric keyboard hint
- [x] `type="email"` preserves email semantics even if `inputmode` is absent or different
- [x] `type="number"` behavior is distinguished from `inputmode="numeric"`

## Related Attributes Worth Tracking

- [ ] `enterkeyhint`
- [ ] `autocomplete`
- [ ] `autocapitalize`
- [ ] `spellcheck`
- [ ] `maxlength`
- [ ] `minlength`
- [x] `min`
- [x] `max`
- [x] `step`
- [ ] `pattern`
- [x] `placeholder`
- [x] `readonly`
- [ ] `required`
- [ ] `multiple`
- [ ] `list`

## Validation Matrix

- [ ] Text-like types sanitize and validate correctly
- [ ] Numeric types enforce numeric parsing rules correctly
- [ ] Date and time types enforce normalized value formats correctly
- [ ] File inputs reflect picker results and selected files correctly
- [ ] Button-like types do not pretend to be text-editable controls

## Platform Parity

- [ ] iOS virtual keyboard mapping is correct per `type` and `inputmode`
- [ ] iOS action button label is configurable via `enterkeyhint`
- [ ] Desktop behavior remains consistent after mobile-specific changes
- [ ] Unsupported types degrade predictably to a safe text-like fallback when required