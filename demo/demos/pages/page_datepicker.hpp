#pragma once

inline void glint_demos_window::buildDatePicker()
{
  auto addHeading = [&](const char* text, float mb = 8.f) {
    mContent->add.div([=](glint_component_style& d) {
      d.innerText          = text;
      d.style.color        = glint_demo_theme::heading;
      d.style.fontSize     = 15.f;
      d.style.width        = "100%";
      d.style.textAlign    = EAlign::Near;
      d.style.marginBottom = mb;
    });
  };

  auto addSpacer = [&](float h) {
    mContent->add.div([=](glint_component_style& s) {
      s.style.height = h;
      s.style.width  = "100%";
    });
  };

  // -- 1. Inline calendar (glint_datepicker) ---------------------------------
  addHeading("Inline calendar- glint_datepicker");

  auto* cal1 = mContent->add.fromClass<glint_datepicker>([](glint_datepicker& dp) {
    dp.setDate(2024, 6, 15);
    dp.style.marginBottom = 6.f;
  });

  auto* fb0 = mContent->add.div([](glint_component_style& fb) {
    fb.innerText       = "Selected: 2024-06-15";
    fb.style.color     = glint_demo_theme::muted;
    fb.style.fontSize  = 12.f;
    fb.style.width     = "100%";
    fb.style.textAlign = EAlign::Near;
    fb.style.marginBottom = 4.f;
  });
  glint_element* fb0Ptr = fb0;
  cal1->onChange = [fb0Ptr](int y, int m, int d) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "Selected: %04d-%02d-%02d", y, m, d);
    fb0Ptr->innerText = buf;
    fb0Ptr->setDirty(false);
  };

  addSpacer(20.f);

  // -- 2. Text-input spinner (glint_date_input) ------------------------------
  addHeading("<input type=\"date\"> spinner- glint_date_input");

  auto* dp1 = mContent->add.fromClass<glint_date_input>([](glint_date_input& dp) {
    dp.setDate(2024, 6, 15);
    dp.style.marginBottom = 6.f;
  });

  auto* fb1 = mContent->add.div([](glint_component_style& fb) {
    fb.innerText       = "Date: 2024-06-15";
    fb.style.color     = glint_demo_theme::muted;
    fb.style.fontSize  = 12.f;
    fb.style.width     = "100%";
    fb.style.textAlign = EAlign::Near;
    fb.style.marginBottom = 4.f;
  });
  glint_element* fb1Ptr = fb1;
  dp1->onChange = [fb1Ptr](int y, int m, int d) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "Date: %04d-%02d-%02d", y, m, d);
    fb1Ptr->innerText = buf;
    fb1Ptr->setDirty(false);
  };

  addSpacer(20.f);

  // -- 3. Date range (two text inputs) --------------------------------------
  addHeading("Date range (Start -- End)");

  mContent->add.div([&](glint_component_style& row) {
    row.style.display       = "flex";
    row.style.flexDirection = "row";
    row.style.alignItems    = "center";
    row.style.gap           = 10.f;
    row.style.width         = "100%";
    row.style.marginBottom  = 6.f;

    row.add.div([](glint_component_style& lbl) {
      lbl.innerText      = "Start";
      lbl.style.color    = glint_demo_theme::muted;
      lbl.style.fontSize = 12.f;
      lbl.style.width    = 36.f;
    });
    row.add.fromClass<glint_date_input>([](glint_date_input& dp) {
      dp.setDate(2024, 1, 1);
    });
    row.add.div([](glint_component_style& sep) {
      sep.innerText      = "\xe2\x86\x92";
      sep.style.color    = glint_demo_theme::subtle;
      sep.style.fontSize = 14.f;
    });
    row.add.fromClass<glint_date_input>([](glint_date_input& dp) {
      dp.setDate(2024, 12, 31);
    });
  });

  addSpacer(20.f);

  // -- 4. Popup window (glint_datepicker_window) -----------------------------
  addHeading("Popup window- glint_datepicker_window");

  static glint_datepicker_window* sDpWin = nullptr;
  static int sWinY = 2025, sWinM = 3, sWinD = 14;
  // Pre-warm: create hidden so first click goes straight to reopen()
  if (!sDpWin) sDpWin = glint_datepicker_window::open(sWinY, sWinM, sWinD, {}, nullptr, nullptr);

  auto* fb2 = mContent->add.div([](glint_component_style& fb) {
    fb.innerText       = "Picked: 2025-03-14";
    fb.style.color     = glint_demo_theme::muted;
    fb.style.fontSize  = 12.f;
    fb.style.width     = "100%";
    fb.style.textAlign = EAlign::Near;
    fb.style.marginBottom = 6.f;
  });
  glint_element* fb2Ptr = fb2;

  auto* btn = mContent->add.button([&](glint_button& b) {
    b.innerText            = "Pick a date\xe2\x80\xa6";
    b.style.width          = 140.f;
    b.style.height         = 32.f;
    b.style.backgroundColor = glint_color(255, 40, 40, 40);
    b.style.color          = glint_color(255, 210, 210, 210);
    b.style.borderRadius   = 6.f;
    b.style.borderWidth    = 1.f;
    b.style.borderColor    = glint_color(255, 70, 70, 70);
    b.style.fontSize       = 13.f;
    b.style.marginBottom   = 4.f;
  });

  btn->onClick = [btn, fb2Ptr]()
  {
    // Compute screen-space anchor rect from the button's mRect,
    // subtracting ancestor scroll offsets for any scrollable parents.
    float cl = btn->mRect.L, ct = btn->mRect.T;
    for (glint_element* p = btn->mParent; p; p = p->mParent) {
      cl -= p->mScrollLeft;
      ct -= p->mScrollTop;
    }
    const float bW = btn->mRect.W(), bH = btn->mRect.H();

#if defined(_WIN32) || defined(OS_WIN)
    POINT bottomLeft{ (LONG)cl, (LONG)(ct + bH) };
    if (HWND hwnd = btn->mRoot ? btn->mRoot->hwnd : nullptr)
      ::ClientToScreen(hwnd, &bottomLeft);
    RECT anchor{ bottomLeft.x, bottomLeft.y - (LONG)bH,
                 bottomLeft.x + (LONG)bW,  bottomLeft.y };
#elif defined(__linux__)
    RECT anchor = (btn->mRoot && btn->mRoot->linuxWindow)
      ? btn->mRoot->linuxWindow->contentRectToScreen(cl, ct, bW, bH)
      : RECT{};
#else
    RECT anchor = (btn->mRoot && btn->mRoot->macWindow)
      ? btn->mRoot->macWindow->contentRectToScreen(cl, ct, bW, bH)
      : RECT{};
#endif

    auto onChanged = [fb2Ptr](int y, int m, int d) {
      sWinY = y; sWinM = m; sWinD = d;
      char buf[32];
      std::snprintf(buf, sizeof(buf), "Picked: %04d-%02d-%02d", y, m, d);
      fb2Ptr->innerText = buf;
      fb2Ptr->setDirty(false);
    };

    if (sDpWin->isVisible()) {
      sDpWin->hide();
    } else {
      sDpWin->reopen(sWinY, sWinM, sWinD, anchor, onChanged, nullptr,
                     btn->mRoot ? &btn->mRoot->mCanvas : nullptr);
    }
  };
}
