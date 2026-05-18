#pragma once

inline void glint_demos_window::buildFonts()
{
  static constexpr const char* kPangram = "The quick brown fox jumps over the lazy dog";
  static constexpr const char* kCharset = "ABCDE abcde 01234  Ag Bg Cg";

  struct FontSpec { const char* id; const char* label; const char* labelColor; };
  static const FontSpec kFonts[] = {
    { "Roboto-Regular",   "Roboto  \xc2\xb7  Regular",    glint_demo_theme::heading },
    { "Kanit-Thin",       "Kanit  \xc2\xb7  Thin",        glint_demo_theme::text },
    { "Kanit-ThinItalic", "Kanit  \xc2\xb7  Thin Italic", glint_demo_theme::muted },
    { "Kanit-Regular",    "Kanit  \xc2\xb7  Regular",     glint_demo_theme::text },
    { "Kanit-Medium",     "Kanit  \xc2\xb7  Medium",      glint_demo_theme::heading },
    { "Kanit-SemiBold",   "Kanit  \xc2\xb7  Semi Bold",   glint_demo_theme::muted },
    { "Monoton",          "Monoton  \xc2\xb7  Regular",   glint_demo_theme::text },
    { "PinyonScript",     "Pinyon Script  \xc2\xb7  Regular", glint_demo_theme::muted },
    { "ShadowsIntoLight", "Shadows Into Light  \xc2\xb7  Regular", glint_demo_theme::text },
  };

  // ── Per-font specimen cards ───────────────────────────────────────────────
  addHeading("Registered typefaces");

  for (const auto& f : kFonts)
  {
    const bool tfReady = glint_font_registry::isLoaded(f.id);

    mContent->add.div([=](glint_component_style& card) {
      card.style.width           = "100%";
      card.style.backgroundColor = glint_demo_theme::surfaceAlt;
      card.style.borderRadius    = 8.f;
      card.style.padding         = "12 16";
      card.style.marginBottom    = 12.f;

      // -- Header row: name pill + status badge ------------------------------
      card.add.div([=](glint_component_style& hdr) {
        hdr.style.display       = "flex";
        hdr.style.flexDirection = "row";
        hdr.style.alignItems    = "center";
        hdr.style.gap           = 8.f;
        hdr.style.marginBottom  = 10.f;

        hdr.add.div([=](glint_component_style& pill) {
          pill.innerText             = f.label;
          pill.style.color           = f.labelColor;
          pill.style.fontSize        = 11.f;
          pill.style.backgroundColor = glint_demo_theme::surface;
          pill.style.borderRadius    = 4.f;
          pill.style.padding         = "3 8";
          pill.style.textAlign       = EAlign::Near;
        });

        hdr.add.div([=](glint_component_style& idLbl) {
          idLbl.innerText       = std::string("\"") + f.id + "\"";
          idLbl.style.fontSize  = 10.f;
          idLbl.style.color     = glint_demo_theme::subtle;
          idLbl.style.textAlign = EAlign::Near;
        });

        hdr.add.div([=](glint_component_style& badge) {
          badge.innerText             = tfReady ? "  typeface OK  " : "  NO TYPEFACE  ";
          badge.style.fontSize        = 10.f;
          badge.style.color           = tfReady ? glint_demo_theme::success : glint_demo_theme::danger;
          badge.style.backgroundColor = tfReady ? glint_color(60, 100, 200, 100) : glint_color(60, 220, 60, 60);
          badge.style.borderRadius    = 4.f;
          badge.style.padding         = "2 6";
          badge.style.textAlign       = EAlign::Near;
        });
      });

      if (!tfReady)
      {
        card.add.div([](glint_component_style& warn) {
          warn.innerText       = "Typeface not in registry \xe2\x80\x94 _ensureFontsLoaded() may have failed. "
                                 "Text will render as the system fallback font.";
          warn.style.color     = glint_demo_theme::danger;
          warn.style.fontSize  = 11.f;
          warn.style.width     = "100%";
          warn.style.textAlign = EAlign::Near;
        });
        return;
      }

      // -- Specimens at 32 / 20 / 13 px --------------------------------------
      static constexpr float kSizes[] = { 32.f, 20.f, 13.f };
      for (float sz : kSizes)
      {
        card.add.div([=](glint_component_style& lbl) {
          lbl.innerText          = kPangram;
          lbl.style.fontFamily   = f.id;
          lbl.style.fontSize     = sz;
          lbl.style.color        = glint_demo_theme::text;
          lbl.style.width        = "100%";
          lbl.style.textAlign    = EAlign::Near;
          lbl.style.marginBottom = 4.f;
        });
      }

      // -- Character sampler -------------------------------------------------
      card.add.div([=](glint_component_style& cs) {
        cs.innerText       = kCharset;
        cs.style.fontFamily = f.id;
        cs.style.fontSize  = 14.f;
        cs.style.color     = glint_demo_theme::subtle;
        cs.style.width     = "100%";
        cs.style.textAlign = EAlign::Near;
      });
    });
  }

  addSpacer(8.f);

  // ── Side-by-side weight comparison (all Kanit at 28 px) ──────────────────
  addHeading("Kanit \xe2\x80\x94 weight comparison at 28 px");

  static const struct { int weight; const char* label; const char* style; } kWeights[] = {
    { 100, "Thin",        "normal" },
    { 100, "Thin Italic", "italic" },
    { 400, "Regular",     "normal" },
    { 500, "Medium",      "normal" },
    { 600, "Semi Bold",   "normal" },
  };

  for (const auto& w : kWeights)
  {
    mContent->add.div([=](glint_component_style& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 16.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 10.f;

      row.add.div([=](glint_component_style& tag) {
        tag.innerText       = w.label;
        tag.style.fontSize  = 10.f;
        tag.style.color     = glint_demo_theme::subtle;
        tag.style.width     = 72.f;
        tag.style.textAlign = EAlign::Far;
      });

      row.add.div([=](glint_component_style& lbl) {
        lbl.innerText       = "VoiceCleaner Ultra \xe2\x80\x94 glint";
        lbl.style.fontFamily = "Kanit";
        lbl.style.fontSize  = 28.f;
        lbl.style.fontWeight = static_cast<float>(w.weight);
        lbl.style.fontStyle = w.style;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.textAlign = EAlign::Near;
      });
    });
  }

  addSpacer(8.f);

  // ── Size scale (Roboto) ──────────────────────────────────────────────────
  addHeading("Size scale \xe2\x80\x94 Roboto-Regular");

  static constexpr float kScaleSizes[] = { 8.f, 10.f, 12.f, 14.f, 18.f, 24.f, 32.f, 48.f };
  for (float sz : kScaleSizes)
  {
    mContent->add.div([=](glint_component_style& row) {
      row.style.display       = "flex";
      row.style.flexDirection = "row";
      row.style.alignItems    = "center";
      row.style.gap           = 12.f;
      row.style.width         = "100%";
      row.style.marginBottom  = 6.f;

      row.add.div([=](glint_component_style& tag) {
        tag.innerText       = std::to_string(static_cast<int>(sz)) + " px";
        tag.style.fontSize  = 10.f;
        tag.style.color     = glint_demo_theme::subtle;
        tag.style.width     = 36.f;
        tag.style.textAlign = EAlign::Far;
      });

      row.add.div([=](glint_component_style& lbl) {
        lbl.innerText       = "The quick brown fox";
        lbl.style.fontSize  = sz;
        lbl.style.color     = glint_demo_theme::text;
        lbl.style.textAlign = EAlign::Near;
      });
    });
  }
}
