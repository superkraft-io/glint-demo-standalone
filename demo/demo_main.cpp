#include "demos/window.hpp"

#if defined(_WIN32)
#  include <windows.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
  glint_demos_window::open();

  while (glint_demos_window::isOpen())
    ::Sleep(16);

  return 0;
}

#else  // macOS / other POSIX

// Implemented in demo_mac_main.mm (Objective-C++) to access Cocoa NSApp.
void glint_demo_run_mac();

int main()
{
  glint_demo_run_mac();
  return 0;
}

#endif