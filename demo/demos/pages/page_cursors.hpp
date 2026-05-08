#pragma once

inline void glint_demos_window::buildCursors()
{
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

  // Adds a single cursor tile to a parent row.
  // The text label is placed in a child div so that the parent's flex
  // centering (alignItems + justifyContent) actually centres it both ways.
  auto addTile = [](glint_element* parent, const char* cursorName) {
    parent->add.div([cursorName](glint_component_style& d) {
      d.style.cursor           = cursorName;
      d.style.userSelect       = "none";
      d.style.width            = 96.f;
      d.style.height           = 50.f;
      d.style.backgroundColor  = glint_demo_theme::surface;
      d.style.borderWidth      = 1.f;
      d.style.borderColor      = glint_demo_theme::border;
      d.style.borderRadius     = 6.f;
      d.style.display          = "flex";
      d.style.alignItems       = "center";
      d.style.justifyContent   = "center";

      d.add.div([cursorName](glint_component_style& lbl) {
        lbl.innerText        = cursorName;
        lbl.style.fontSize   = 11.f;
        lbl.style.color      = glint_demo_theme::muted;
        lbl.style.textAlign  = EAlign::Center;
        lbl.style.userSelect = "none";
      });
    });
  };

  // ── Intro ─────────────────────────────────────────────────────────────────
  mContent->add.div([](glint_component_style& d) {
    d.innerText          = "Hover each tile to preview the cursor.";
    d.style.color        = glint_demo_theme::muted;
    d.style.fontSize     = 12.f;
    d.style.width        = "100%";
    d.style.textAlign    = EAlign::Near;
    d.style.marginBottom = 16.f;
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
