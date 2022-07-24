#pragma once

#include "rune/defines.hpp"

namespace Rune
{
    namespace Input
    {
        constexpr u32 HAT_CENTERED = 0;
        constexpr u32 HAT_UP = 1;
        constexpr u32 HAT_RIGHT = 2;
        constexpr u32 HAT_DOWN = 4;
        constexpr u32 HAT_LEFT = 8;
        constexpr u32 HAT_RIGHT_UP = (HAT_RIGHT | HAT_UP);
        constexpr u32 HAT_RIGHT_DOWN = (HAT_RIGHT | HAT_DOWN);
        constexpr u32 HAT_LEFT_UP = (HAT_LEFT | HAT_UP);
        constexpr u32 HAT_LEFT_DOWN = (HAT_LEFT | HAT_DOWN);

        constexpr u32 KEY_UNKNOWN = -1;

        constexpr u32 KEY_SPACE = 32;
        constexpr u32 KEY_APOSTROPHE = 39; /* ' */
        constexpr u32 KEY_COMMA = 44;      /* , */
        constexpr u32 KEY_MINUS = 45;      /* - */
        constexpr u32 KEY_PERIOD = 46;     /* . */
        constexpr u32 KEY_SLASH = 47;      /* / */
        constexpr u32 KEY_0 = 48;
        constexpr u32 KEY_1 = 49;
        constexpr u32 KEY_2 = 50;
        constexpr u32 KEY_3 = 51;
        constexpr u32 KEY_4 = 52;
        constexpr u32 KEY_5 = 53;
        constexpr u32 KEY_6 = 54;
        constexpr u32 KEY_7 = 55;
        constexpr u32 KEY_8 = 56;
        constexpr u32 KEY_9 = 57;
        constexpr u32 KEY_SEMICOLON = 59; /* ; */
        constexpr u32 KEY_EQUAL = 61;     /* = */
        constexpr u32 KEY_A = 65;
        constexpr u32 KEY_B = 66;
        constexpr u32 KEY_C = 67;
        constexpr u32 KEY_D = 68;
        constexpr u32 KEY_E = 69;
        constexpr u32 KEY_F = 70;
        constexpr u32 KEY_G = 71;
        constexpr u32 KEY_H = 72;
        constexpr u32 KEY_I = 73;
        constexpr u32 KEY_J = 74;
        constexpr u32 KEY_K = 75;
        constexpr u32 KEY_L = 76;
        constexpr u32 KEY_M = 77;
        constexpr u32 KEY_N = 78;
        constexpr u32 KEY_O = 79;
        constexpr u32 KEY_P = 80;
        constexpr u32 KEY_Q = 81;
        constexpr u32 KEY_R = 82;
        constexpr u32 KEY_S = 83;
        constexpr u32 KEY_T = 84;
        constexpr u32 KEY_U = 85;
        constexpr u32 KEY_V = 86;
        constexpr u32 KEY_W = 87;
        constexpr u32 KEY_X = 88;
        constexpr u32 KEY_Y = 89;
        constexpr u32 KEY_Z = 90;
        constexpr u32 KEY_LEFT_BRACKET = 91;  /* [ */
        constexpr u32 KEY_BACKSLASH = 92;     /* \ */
        constexpr u32 KEY_RIGHT_BRACKET = 93; /* ] */
        constexpr u32 KEY_GRAVE_ACCENT = 96;  /* ` */
        constexpr u32 KEY_WORLD_1 = 161;      /* non-US #1 */
        constexpr u32 KEY_WORLD_2 = 162;      /* non-US #2 */

        constexpr u32 KEY_ESCAPE = 256;
        constexpr u32 KEY_ENTER = 257;
        constexpr u32 KEY_TAB = 258;
        constexpr u32 KEY_BACKSPACE = 259;
        constexpr u32 KEY_INSERT = 260;
        constexpr u32 KEY_DELETE = 261;
        constexpr u32 KEY_RIGHT = 262;
        constexpr u32 KEY_LEFT = 263;
        constexpr u32 KEY_DOWN = 264;
        constexpr u32 KEY_UP = 265;
        constexpr u32 KEY_PAGE_UP = 266;
        constexpr u32 KEY_PAGE_DOWN = 267;
        constexpr u32 KEY_HOME = 268;
        constexpr u32 KEY_END = 269;
        constexpr u32 KEY_CAPS_LOCK = 280;
        constexpr u32 KEY_SCROLL_LOCK = 281;
        constexpr u32 KEY_NUM_LOCK = 282;
        constexpr u32 KEY_PRINT_SCREEN = 283;
        constexpr u32 KEY_PAUSE = 284;
        constexpr u32 KEY_F1 = 290;
        constexpr u32 KEY_F2 = 291;
        constexpr u32 KEY_F3 = 292;
        constexpr u32 KEY_F4 = 293;
        constexpr u32 KEY_F5 = 294;
        constexpr u32 KEY_F6 = 295;
        constexpr u32 KEY_F7 = 296;
        constexpr u32 KEY_F8 = 297;
        constexpr u32 KEY_F9 = 298;
        constexpr u32 KEY_F10 = 299;
        constexpr u32 KEY_F11 = 300;
        constexpr u32 KEY_F12 = 301;
        constexpr u32 KEY_F13 = 302;
        constexpr u32 KEY_F14 = 303;
        constexpr u32 KEY_F15 = 304;
        constexpr u32 KEY_F16 = 305;
        constexpr u32 KEY_F17 = 306;
        constexpr u32 KEY_F18 = 307;
        constexpr u32 KEY_F19 = 308;
        constexpr u32 KEY_F20 = 309;
        constexpr u32 KEY_F21 = 310;
        constexpr u32 KEY_F22 = 311;
        constexpr u32 KEY_F23 = 312;
        constexpr u32 KEY_F24 = 313;
        constexpr u32 KEY_F25 = 314;
        constexpr u32 KEY_KP_0 = 320;
        constexpr u32 KEY_KP_1 = 321;
        constexpr u32 KEY_KP_2 = 322;
        constexpr u32 KEY_KP_3 = 323;
        constexpr u32 KEY_KP_4 = 324;
        constexpr u32 KEY_KP_5 = 325;
        constexpr u32 KEY_KP_6 = 326;
        constexpr u32 KEY_KP_7 = 327;
        constexpr u32 KEY_KP_8 = 328;
        constexpr u32 KEY_KP_9 = 329;
        constexpr u32 KEY_KP_DECIMAL = 330;
        constexpr u32 KEY_KP_DIVIDE = 331;
        constexpr u32 KEY_KP_MULTIPLY = 332;
        constexpr u32 KEY_KP_SUBTRACT = 333;
        constexpr u32 KEY_KP_ADD = 334;
        constexpr u32 KEY_KP_ENTER = 335;
        constexpr u32 KEY_KP_EQUAL = 336;
        constexpr u32 KEY_LEFT_SHIFT = 340;
        constexpr u32 KEY_LEFT_CONTROL = 341;
        constexpr u32 KEY_LEFT_ALT = 342;
        constexpr u32 KEY_LEFT_SUPER = 343;
        constexpr u32 KEY_RIGHT_SHIFT = 344;
        constexpr u32 KEY_RIGHT_CONTROL = 345;
        constexpr u32 KEY_RIGHT_ALT = 346;
        constexpr u32 KEY_RIGHT_SUPER = 347;
        constexpr u32 KEY_MENU = 348;

        constexpr u32 KEY_LAST = KEY_MENU;

        constexpr u32 KEY_MOD_SHIFT = 0x0001;
        constexpr u32 KEY_MOD_CONTROL = 0x0002;
        constexpr u32 KEY_MOD_ALT = 0x0004;
        constexpr u32 KEY_MOD_SUPER = 0x0008;
        constexpr u32 KEY_MOD_CAPS_LOCK = 0x0010;
        constexpr u32 KEY_MOD_NUM_LOCK = 0x0020;

        constexpr u32 MOUSE_BUTTON_1 = 0;
        constexpr u32 MOUSE_BUTTON_2 = 1;
        constexpr u32 MOUSE_BUTTON_3 = 2;
        constexpr u32 MOUSE_BUTTON_4 = 3;
        constexpr u32 MOUSE_BUTTON_5 = 4;
        constexpr u32 MOUSE_BUTTON_6 = 5;
        constexpr u32 MOUSE_BUTTON_7 = 6;
        constexpr u32 MOUSE_BUTTON_8 = 7;
        constexpr u32 MOUSE_BUTTON_LAST = MOUSE_BUTTON_8;
        constexpr u32 MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1;
        constexpr u32 MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2;
        constexpr u32 MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3;

        constexpr u32 JOYSTICK_1 = 0;
        constexpr u32 JOYSTICK_2 = 1;
        constexpr u32 JOYSTICK_3 = 2;
        constexpr u32 JOYSTICK_4 = 3;
        constexpr u32 JOYSTICK_5 = 4;
        constexpr u32 JOYSTICK_6 = 5;
        constexpr u32 JOYSTICK_7 = 6;
        constexpr u32 JOYSTICK_8 = 7;
        constexpr u32 JOYSTICK_9 = 8;
        constexpr u32 JOYSTICK_10 = 9;
        constexpr u32 JOYSTICK_11 = 10;
        constexpr u32 JOYSTICK_12 = 11;
        constexpr u32 JOYSTICK_13 = 12;
        constexpr u32 JOYSTICK_14 = 13;
        constexpr u32 JOYSTICK_15 = 14;
        constexpr u32 JOYSTICK_16 = 15;
        constexpr u32 JOYSTICK_LAST = JOYSTICK_16;

        constexpr u32 GAMEPAD_BUTTON_A = 0;
        constexpr u32 GAMEPAD_BUTTON_B = 1;
        constexpr u32 GAMEPAD_BUTTON_X = 2;
        constexpr u32 GAMEPAD_BUTTON_Y = 3;
        constexpr u32 GAMEPAD_BUTTON_LEFT_BUMPER = 4;
        constexpr u32 GAMEPAD_BUTTON_RIGHT_BUMPER = 5;
        constexpr u32 GAMEPAD_BUTTON_BACK = 6;
        constexpr u32 GAMEPAD_BUTTON_START = 7;
        constexpr u32 GAMEPAD_BUTTON_GUIDE = 8;
        constexpr u32 GAMEPAD_BUTTON_LEFT_THUMB = 9;
        constexpr u32 GAMEPAD_BUTTON_RIGHT_THUMB = 10;
        constexpr u32 GAMEPAD_BUTTON_DPAD_UP = 11;
        constexpr u32 GAMEPAD_BUTTON_DPAD_RIGHT = 12;
        constexpr u32 GAMEPAD_BUTTON_DPAD_DOWN = 13;
        constexpr u32 GAMEPAD_BUTTON_DPAD_LEFT = 14;
        constexpr u32 GAMEPAD_BUTTON_LAST = GAMEPAD_BUTTON_DPAD_LEFT;

        constexpr u32 GAMEPAD_BUTTON_CROSS = GAMEPAD_BUTTON_A;
        constexpr u32 GAMEPAD_BUTTON_CIRCLE = GAMEPAD_BUTTON_B;
        constexpr u32 GAMEPAD_BUTTON_SQUARE = GAMEPAD_BUTTON_X;
        constexpr u32 GAMEPAD_BUTTON_TRIANGLE = GAMEPAD_BUTTON_Y;

        constexpr u32 GAMEPAD_AXIS_LEFT_X = 0;
        constexpr u32 GAMEPAD_AXIS_LEFT_Y = 1;
        constexpr u32 GAMEPAD_AXIS_RIGHT_X = 2;
        constexpr u32 GAMEPAD_AXIS_RIGHT_Y = 3;
        constexpr u32 GAMEPAD_AXIS_LEFT_TRIGGER = 4;
        constexpr u32 GAMEPAD_AXIS_RIGHT_TRIGGER = 5;
        constexpr u32 GAMEPAD_AXIS_LAST = GAMEPAD_AXIS_RIGHT_TRIGGER;
    }
}