#pragma once

// sorry :{

const char* get_key( u8 key ) {
  switch( key ) {
    case 0x08: {
      return "BACKSPACE";
    } break;
    case 0x09: {
      return "TAB";
    } break;
    case 0x0A: case 0x0B: {
      return "IGNORED";
    } break;
    case 0x0C: {
      return "CLEAR"; // may not be needed
    } break;
    case 0x0D: {
      return "RETURN";
    } break;
    case 0x10: {
      return "SHIFT";
    } break;
    case 0x11: {
      return "CONTROL";
    } break;
    case 0x14: {
      return "CAPS_LOCK";
    } break;
    case 0x1B: {
      return "ESCAPE";
    } break;
    case 0x20: {
      return "SPACE";
    } break;
    case 0x21: {
      return "PAGE_UP";
    } break;
    case 0x22: {
      return "PAGE_DOWN";
    } break;
    case 0x23: {
      return "END";
    } break;
    case 0x24: {
      return "HOME";
    } break;
    case 0x25: {
      return "LEFT";
    } break;
    case 0x26: {
      return "UP";
    } break;
    case 0x27: {
      return "RIGHT";
    } break;
    case 0x28: {
      return "DOWN";
    } break;
    case 0x2D: {
      return "INSERT";
    } break;
    case 0x2E: {
      return "DEL";
    } break;
    case 0x30: {
      return "NUM_ZERO";
    } break;
    case 0x31: {
      return "NUM_ONE";
    } break;
    case 0x32: {
      return "NUM_TWO";
    } break;
    case 0x33: {
      return "NUM_THREE";
    } break;
    case 0x34: {
      return "NUM_FOUR";
    } break;
    case 0x35: {
      return "NUM_FIVE";
    } break;
    case 0x36: {
      return "NUM_SIX";
    } break;
    case 0x37: {
      return "NUM_SEVEN";
    } break;
    case 0x38: {
      return "NUM_EIGHT";
    } break;
    case 0x39: {
      return "NUM_NINE";
    } break;
    case 0x41: {
      return "KEY_A";
    } break;
    case 0x42: {
      return "KEY_B";
    } break;
    case 0x43: {
      return "KEY_C";
    } break;
    case 0x44: {
      return "KEY_D";
    } break;
    case 0x45: {
      return "KEY_E";
    } break;
    case 0x46: {
      return "KEY_F";
    } break;
    case 0x47: {
      return "KEY_G";
    } break;
    case 0x48: {
      return "KEY_H";
    } break;
    case 0x49: {
      return "KEY_I";
    } break;
    case 0x4A: {
      return "KEY_J";
    } break;
    case 0x4B: {
      return "KEY_K";
    } break;
    case 0x4C: {
      return "KEY_L";
    } break;
    case 0x4D: {
      return "KEY_M";
    } break;
    case 0x4E: {
      return "KEY_N";
    } break;
    case 0x4F: {
      return "KEY_O";
    } break;
    case 0x50: {
      return "KEY_P";
    } break;
    case 0x51: {
      return "KEY_Q";
    } break;
    case 0x52: {
      return "KEY_R";
    } break;
    case 0x53: {
      return "KEY_S";
    } break;
    case 0x54: {
      return "KEY_T";
    } break;
    case 0x55: {
      return "KEY_U";
    } break;
    case 0x56: {
      return "KEY_V";
    } break;
    case 0x57: {
      return "KEY_W";
    } break;
    case 0x58: {
      return "KEY_X";
    } break;
    case 0x59: {
      return "KEY_Y";
    } break;
    case 0x5A: {
      return "KEY_Z";
    } break;
    case 0x60: {
      return "NUM_ZERO";
    } break;
    case 0x61: {
      return "NUM_ONE";
    } break;
    case 0x62: {
      return "NUM_TWO";
    } break;
    case 0x63: {
      return "NUM_THREE";
    } break;
    case 0x64: {
      return "NUM_FOUR";
    } break;
    case 0x65: {
      return "NUM_FIVE";
    } break;
    case 0x66: {
      return "NUM_SIX";
    } break;
    case 0x67: {
      return "NUM_SEVEN";
    } break;
    case 0x68: {
      return "NUM_EIGHT";
    } break;
    case 0x69: {
      return "NUM_NINE";
    } break;
    case 0x6A: {
      return "MULTIPLY";
    } break;
    case 0x6B: {
      return "ADD";
    } break;
    case 0x6C: {
      return "VERT_LINE";
    } break;
    case 0x6D: {
      return "SUBTRACT";
    } break;
    case 0x6E: {
      return "PERIOD";
    } break;
    case 0x6F: {
      return "DIVIDE";
    } break;
    case 0xA0: {
      return "L_SHIFT";
    } break;
    case 0xA1: {
      return "R_SHIFT";
    } break;
    case 0xA2: {
      return "L_CTRL";
    } break;
    case 0xA3: {
      return "R_CTRL";
    } break;
    case 0xBA: {
      return "COLON";
    } break;
    case 0xBB: {
      return "PLUS";
    } break;
    case 0xBC: {
      return "COMMA";
    } break;
    case 0xBD: {
      return "MINUS";
    } break;
    case 0xBE: {
      return "PERIOD";
    } break;
    case 0xBF: {
      return "QUESTION";
    } break;
    case 0xC0: {
      return "TILDE";
    } break;
    case 0xDB: {
      return "L_BRACKET";
    } break;
    case 0xDC: {
      return "BACKSLASH";
    } break;
    case 0xDD: {
      return "R_BRACKET";
    } break;
    case 0xDE: {
      return "QUOTE";
    } break;
    case 0xE2: {
      return "GT_LT_KEY";
    } break;
    default: {
      return "IGNORED";
    } break;
  }
}