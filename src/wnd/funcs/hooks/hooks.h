#pragma once
#include "../../wincludes.h"

#include "key_hook.h"
#include "mouse_hook.h"

inline void init_hooks() {
  mouse_hook = SetWindowsHookExA(
    WH_MOUSE_LL, mouse_hook_proc,
    GetModuleHandleA( NULL ), 0
  );

  key_hook = SetWindowsHookExA(
    WH_KEYBOARD_LL, key_hook_proc,
    GetModuleHandleA( NULL ), 0
  );
}

inline void unhook() {
  UnhookWindowsHookEx( key_hook );
  UnhookWindowsHookEx( mouse_hook );
}