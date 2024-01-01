#pragma once
#include "../../wincludes.h"

#include "wnd_key_hook.h"
#include "wnd_mouse_hook.h"

void wnd_init_hooks() {
  mouse_hook = SetWindowsHookExA(
    WH_MOUSE_LL, mouse_hook_proc,
    GetModuleHandleA( NULL ), 0
  );

  key_hook = SetWindowsHookExA(
    WH_KEYBOARD_LL, key_hook_proc,
    GetModuleHandleA( NULL ), 0
  );
}