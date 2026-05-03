#pragma once

inline void glint_demos_window::buildLists()
{
  glint_element* statusPtr  = nullptr;
  glint_list*    listPtr    = nullptr;
  glint_button*  addBtnPtr  = nullptr;
  glint_button*  clrBtnPtr  = nullptr;
  int*           extraCount = new int(0);  // window-lifetime

  mContent->add.div([](auto& h) {
    h.innerText          = "glint_list (scrollable, selectable)";
    h.style.color        = glint_demo_theme::heading;
    h.style.fontSize     = 15.f;
    h.style.width        = "100%";
    h.style.textAlign    = EAlign::Near;
    h.style.marginBottom = 10.f;
  });

  mContent->add.fromClass<glint_list>([](auto& l) {
    l.style.width           = "100%";
    l.style.height          = 260.f;
    l.style.backgroundColor = glint_demo_theme::surfaceAlt;
    l.style.borderRadius    = 8.f;
    l.highlightOnSelect     = true;
  }, &listPtr);

  static const struct { const char* label; const char* id; } kItems[] = {
    { "First item",   "item_1"  },
    { "Second item",  "item_2"  },
    { "Third item",   "item_3"  },
    { "Fourth item",  "item_4"  },
    { "Fifth item",   "item_5"  },
    { "Sixth item",   "item_6"  },
    { "Seventh item", "item_7"  },
    { "Eighth item",  "item_8"  },
    { "Ninth item",   "item_9"  },
    { "Tenth item",   "item_10" },
  };
  for (const auto& it : kItems)
  {
    listPtr->items.add([it](glint_list_item& li) {
      li.innerText                     = it.label;
      li.id                            = it.id;
      li.style.height                  = 34.f;
      li.style.fontSize                = 13.f;
      li.style.paddingLeft             = 12.f;
      li.style.color                   = glint_demo_theme::text;
      li.selectedStyle.backgroundColor = glint_demo_theme::surfaceHover;
      li.selectedStyle.color           = glint_demo_theme::heading;
      li.selectedStyle.fontSize        = 13.f;
      li.selectedStyle.height          = 34.f;
      li.selectedStyle.paddingLeft     = 12.f;
    });
  }

  addSpacer(8.f);

  mContent->add.div([](auto& s) {
    s.innerText       = "Selected: (none)";
    s.style.color     = glint_demo_theme::muted;
    s.style.fontSize  = 12.f;
    s.style.width     = "100%";
    s.style.textAlign = EAlign::Near;
  }, &statusPtr);

  listPtr->onItemSelected = [statusPtr](glint_list_item* item) {
    if (!item) return;
    statusPtr->innerText = std::string("Selected: ") + item->innerText + "  (id=" + item->id + ")";
    statusPtr->setDirty(false);
  };

  addSpacer(20.f);

  mContent->add.div([](auto& h) {
    h.innerText          = "API: add / clear";
    h.style.color        = glint_demo_theme::heading;
    h.style.fontSize     = 14.f;
    h.style.width        = "100%";
    h.style.textAlign    = EAlign::Near;
    h.style.marginBottom = 8.f;
  });

  mContent->add.div([&](auto& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.gap           = 10.f;
    row.style.width         = "100%";

    row.add.template fromClass<glint_button>([](auto& b) {
      b.innerText             = "Add item";
      b.style.height          = 34.f;
      b.style.width           = 120.f;
      b.style.backgroundColor = glint_demo_theme::surface;
      b.style.color           = glint_demo_theme::text;
      b.style.borderRadius    = 6.f;
      b.style.fontSize        = 13.f;
      b.hover.backgroundColor = glint_demo_theme::surfaceHover;
      b.hover.color           = glint_demo_theme::text;
      b.hover.borderRadius    = 6.f;
      b.hover.fontSize        = 13.f;
      b.pressed               = b.hover;
    }, &addBtnPtr);

    row.add.template fromClass<glint_button>([](auto& b) {
      b.innerText             = "Clear all";
      b.style.height          = 34.f;
      b.style.width           = 120.f;
      b.style.backgroundColor = glint_demo_theme::dangerBg;
      b.style.color           = glint_demo_theme::text;
      b.style.borderRadius    = 6.f;
      b.style.fontSize        = 13.f;
      b.hover.backgroundColor = "#714040";
      b.hover.color           = glint_demo_theme::text;
      b.hover.borderRadius    = 6.f;
      b.hover.fontSize        = 13.f;
      b.pressed               = b.hover;
    }, &clrBtnPtr);
  });

  addBtnPtr->onClick = [listPtr, extraCount] {
    (*extraCount)++;
    const int n = *extraCount;
    listPtr->items.add([n](glint_list_item& li) {
      li.innerText                     = std::string("Extra item ") + std::to_string(n);
      li.id                            = std::string("extra_") + std::to_string(n);
      li.style.height                  = 34.f;
      li.style.fontSize                = 13.f;
      li.style.paddingLeft             = 12.f;
      li.style.color                   = glint_demo_theme::warning;
      li.selectedStyle.backgroundColor = glint_demo_theme::surfaceHover;
      li.selectedStyle.color           = glint_demo_theme::heading;
      li.selectedStyle.fontSize        = 13.f;
      li.selectedStyle.height          = 34.f;
      li.selectedStyle.paddingLeft     = 12.f;
    });
  };

  clrBtnPtr->onClick = [listPtr, statusPtr] {
    listPtr->clear();
    statusPtr->innerText = "Selected: (none)";
    statusPtr->setDirty(false);
  };
}
