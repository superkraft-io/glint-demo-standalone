#pragma once

inline void glint_demos_window::buildCursors()
{
  addMobileUselessPageNote();

  auto addHeading = [&](const char* text) {
    mContent->add.div([=](glint_component_style& d) {
      d.innerText          = text;
      d.style.color        = glint_demo_theme::heading;
      d.style.fontSize     = 14.f;
      d.style.width        = "100%";
      d.style.textAlign    = EAlign::Near;
      d.style.marginBottom = 8.f;
    });
  };

  auto addSpacer = [&](float h = 16.f) {
    mContent->add.div([=](glint_component_style& s) {
      s.style.height = h;
      s.style.width  = "100%";
    });
  };

  // Returns a flex row container for cursor tiles.
  auto makeRow = [&]() {
    return mContent->add.div([](glint_component_style& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.gap           = 8.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 6.f;
    });
  };

  // Color cycle: grey → green → red → yellow → grey …
  static constexpr const char* kTileColors[4] = {
    glint_demo_theme::surface,  // 0 – default grey
    "#1a4a1a",                  // 1 – green
    "#4a1a1a",                  // 2 – red
    "#4a3d00",                  // 3 – yellow
  };
  static constexpr const char* kTileTextColors[4] = {
    glint_demo_theme::muted,
    "#6ddd6d",
    "#dd6d6d",
    "#ddcc00",
  };

  // ── Shared color-bucket state ─────────────────────────────────────────────
  struct TileColorState {
    std::vector<std::string> names[4]; // [1]=green  [2]=red  [3]=yellow
  };
  auto* colorState = new TileColorState();

  // Adds a single cursor tile (button) to a parent row.
  // Clicking cycles the background colour: grey → green → red → yellow → …
  auto addTile = [colorState](glint_element* parent, const char* cursorName) {
    auto* colorIdx = new int(0);  // heap-allocated per-tile state; lives for the lifetime of the page

    glint_button* tile = parent->add.button([cursorName](glint_button& btn) {
      btn.style.cursor          = cursorName;
      btn.style.userSelect      = "none";
      btn.style.width           = 96.f;
      btn.style.height          = 50.f;
      btn.style.backgroundColor = glint_demo_theme::surface;
      btn.style.borderWidth     = 1.f;
      btn.style.borderColor     = glint_demo_theme::border;
      btn.style.borderRadius    = 6.f;
      btn.style.display         = "flex";
      btn.style.alignItems      = "center";
      btn.style.justifyContent  = "center";

      btn.add.div([cursorName](glint_component_style& lbl) {
        lbl.innerText        = cursorName;
        lbl.style.fontSize   = 11.f;
        lbl.style.color      = glint_demo_theme::muted;
        lbl.style.textAlign  = EAlign::Center;
        lbl.style.userSelect = "none";
      });
    });

    tile->onClick = [tile, colorIdx, colorState, name = std::string(cursorName)]() {
      int oldIdx = *colorIdx;
      // remove from old color bucket
      if (oldIdx != 0) {
        auto& v = colorState->names[oldIdx];
        for (auto it = v.begin(); it != v.end(); )
          it = (*it == name) ? v.erase(it) : std::next(it);
      }
      // advance color index
      *colorIdx = (oldIdx + 1) % 4;
      // add to new color bucket
      if (*colorIdx != 0)
        colorState->names[*colorIdx].push_back(name);
      // update tile visuals
      tile->style.backgroundColor = kTileColors[*colorIdx];
      tile->style.borderColor     = kTileTextColors[*colorIdx];
      if (!tile->mChildren.empty())
        tile->mChildren[0]->style.color = kTileTextColors[*colorIdx];
      tile->setDirty(false);
    };
  };

  // ── Intro ─────────────────────────────────────────────────────────────────
  mContent->add.div([](glint_component_style& d) {
    d.innerText          = "Hover each tile to preview the cursor. Click to colorize.";
    d.style.color        = glint_demo_theme::muted;
    d.style.fontSize     = 12.f;
    d.style.width        = "100%";
    d.style.textAlign    = EAlign::Near;
    d.style.marginBottom = 12.f;
  });

  // ── General ───────────────────────────────────────────────────────────────
  addHeading("General");
  {
    auto* row = makeRow();
    addTile(row, "default");
    addTile(row, "auto");
    addTile(row, "none");
    addTile(row, "pointer");
    addTile(row, "help");
    addTile(row, "wait");
  }
  {
    auto* row = makeRow();
    addTile(row, "progress");
    addTile(row, "context-menu");
  }

  // ── Text ─────────────────────────────────────────────────────────────────
  addSpacer(8.f);
  addHeading("Text");
  {
    auto* row = makeRow();
    addTile(row, "text");
    addTile(row, "vertical-text");
  }

  // ── Drag & Select ────────────────────────────────────────────────────────
  addSpacer(8.f);
  addHeading("Drag & Select");
  {
    auto* row = makeRow();
    addTile(row, "crosshair");
    addTile(row, "cell");
    addTile(row, "grab");
    addTile(row, "grabbing");
    addTile(row, "move");
    addTile(row, "all-scroll");
  }
  {
    auto* row = makeRow();
    addTile(row, "copy");
    addTile(row, "alias");
  }

  // ── Not Allowed ──────────────────────────────────────────────────────────
  addSpacer(8.f);
  addHeading("Not Allowed");
  {
    auto* row = makeRow();
    addTile(row, "not-allowed");
    addTile(row, "no-drop");
  }

  // ── Zoom ─────────────────────────────────────────────────────────────────
  addSpacer(8.f);
  addHeading("Zoom");
  {
    auto* row = makeRow();
    addTile(row, "zoom-in");
    addTile(row, "zoom-out");
  }

  // ── Resize: Cardinal ─────────────────────────────────────────────────────
  addSpacer(8.f);
  addHeading("Resize \xe2\x80\x94 Cardinal");
  {
    auto* row = makeRow();
    addTile(row, "n-resize");
    addTile(row, "s-resize");
    addTile(row, "e-resize");
    addTile(row, "w-resize");
    addTile(row, "ns-resize");
    addTile(row, "ew-resize");
  }
  {
    auto* row = makeRow();
    addTile(row, "row-resize");
    addTile(row, "col-resize");
  }

  // ── Resize: Diagonal ─────────────────────────────────────────────────────
  addSpacer(8.f);
  addHeading("Resize \xe2\x80\x94 Diagonal");
  {
    auto* row = makeRow();
    addTile(row, "ne-resize");
    addTile(row, "nw-resize");
    addTile(row, "se-resize");
    addTile(row, "sw-resize");
    addTile(row, "nesw-resize");
    addTile(row, "nwse-resize");
  }
}
