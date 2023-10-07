/*
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <map>
#include <linux/input.h>
#include "wayland_keycode_trans.h"
#include "key_event.h"

using namespace OHOS::MMI;

namespace FT {
namespace Wayland {
const std::map<int32_t, int32_t> MAP_KEY_EVENT_VALUE_TRANSFORMATION = {
    { KeyEvent::KEYCODE_UNKNOWN                  , KEY_UNKNOWN                   },
    { KeyEvent::KEYCODE_HOME                     , KEY_HOMEPAGE                  },
    { KeyEvent::KEYCODE_BACK                     , KEY_BACK                      },
    { KeyEvent::KEYCODE_CALL                     , KEY_PHONE                     },
    { KeyEvent::KEYCODE_CLEAR                    , KEY_CLEAR                     },
    { KeyEvent::KEYCODE_HEADSETHOOK              , KEY_MEDIA                     },
    { KeyEvent::KEYCODE_FOCUS                    , KEY_CAMERA_FOCUS              },
    { KeyEvent::KEYCODE_SEARCH                   , KEY_SEARCH                    },
    { KeyEvent::KEYCODE_MEDIA_PLAY_PAUSE         , KEY_PLAYPAUSE                 },
    { KeyEvent::KEYCODE_MEDIA_STOP               , KEY_STOPCD                    },
    { KeyEvent::KEYCODE_MEDIA_NEXT               , KEY_NEXTSONG                  },
    { KeyEvent::KEYCODE_MEDIA_PREVIOUS           , KEY_PREVIOUSSONG              },
    { KeyEvent::KEYCODE_MEDIA_REWIND             , KEY_REWIND                    },
    { KeyEvent::KEYCODE_MEDIA_FAST_FORWARD       , KEY_FASTFORWARD               },
    { KeyEvent::KEYCODE_VOLUME_UP                , KEY_VOLUMEUP                  },
    { KeyEvent::KEYCODE_VOLUME_DOWN              , KEY_VOLUMEDOWN                },
    { KeyEvent::KEYCODE_POWER                    , KEY_POWER                     },
    { KeyEvent::KEYCODE_CAMERA                   , KEY_CAMERA                    },
    { KeyEvent::KEYCODE_VOICE_ASSISTANT          , KEY_VOICECOMMAND              },
    { KeyEvent::KEYCODE_VOLUME_MUTE              , KEY_MUTE                      },
    { KeyEvent::KEYCODE_MUTE                     , KEY_MICMUTE                   },
    { KeyEvent::KEYCODE_BRIGHTNESS_UP            , KEY_BRIGHTNESSUP              },
    { KeyEvent::KEYCODE_BRIGHTNESS_DOWN          , KEY_BRIGHTNESSDOWN            },
    { KeyEvent::KEYCODE_0                        , KEY_0                         },
    { KeyEvent::KEYCODE_1                        , KEY_1                         },
    { KeyEvent::KEYCODE_2                        , KEY_2                         },
    { KeyEvent::KEYCODE_3                        , KEY_3                         },
    { KeyEvent::KEYCODE_4                        , KEY_4                         },
    { KeyEvent::KEYCODE_5                        , KEY_5                         },
    { KeyEvent::KEYCODE_6                        , KEY_6                         },
    { KeyEvent::KEYCODE_7                        , KEY_7                         },
    { KeyEvent::KEYCODE_8                        , KEY_8                         },
    { KeyEvent::KEYCODE_9                        , KEY_9                         },
    { KeyEvent::KEYCODE_STAR                     , KEY_NUMERIC_STAR              },
    { KeyEvent::KEYCODE_POUND                    , KEY_NUMERIC_POUND             },
    { KeyEvent::KEYCODE_DPAD_UP                  , KEY_UP                        },
    { KeyEvent::KEYCODE_DPAD_DOWN                , KEY_DOWN                      },
    { KeyEvent::KEYCODE_DPAD_LEFT                , KEY_LEFT                      },
    { KeyEvent::KEYCODE_DPAD_RIGHT               , KEY_RIGHT                     },
    { KeyEvent::KEYCODE_DPAD_CENTER              , KEY_SELECT                    },
    { KeyEvent::KEYCODE_A                        , KEY_A                         },
    { KeyEvent::KEYCODE_B                        , KEY_B                         },
    { KeyEvent::KEYCODE_C                        , KEY_C                         },
    { KeyEvent::KEYCODE_D                        , KEY_D                         },
    { KeyEvent::KEYCODE_E                        , KEY_E                         },
    { KeyEvent::KEYCODE_F                        , KEY_F                         },
    { KeyEvent::KEYCODE_G                        , KEY_G                         },
    { KeyEvent::KEYCODE_H                        , KEY_H                         },
    { KeyEvent::KEYCODE_I                        , KEY_I                         },
    { KeyEvent::KEYCODE_J                        , KEY_J                         },
    { KeyEvent::KEYCODE_K                        , KEY_K                         },
    { KeyEvent::KEYCODE_L                        , KEY_L                         },
    { KeyEvent::KEYCODE_M                        , KEY_M                         },
    { KeyEvent::KEYCODE_N                        , KEY_N                         },
    { KeyEvent::KEYCODE_O                        , KEY_O                         },
    { KeyEvent::KEYCODE_P                        , KEY_P                         },
    { KeyEvent::KEYCODE_Q                        , KEY_Q                         },
    { KeyEvent::KEYCODE_R                        , KEY_R                         },
    { KeyEvent::KEYCODE_S                        , KEY_S                         },
    { KeyEvent::KEYCODE_T                        , KEY_T                         },
    { KeyEvent::KEYCODE_U                        , KEY_U                         },
    { KeyEvent::KEYCODE_V                        , KEY_V                         },
    { KeyEvent::KEYCODE_W                        , KEY_W                         },
    { KeyEvent::KEYCODE_X                        , KEY_X                         },
    { KeyEvent::KEYCODE_Y                        , KEY_Y                         },
    { KeyEvent::KEYCODE_Z                        , KEY_Z                         },
    { KeyEvent::KEYCODE_COMMA                    , KEY_COMMA                     },
    { KeyEvent::KEYCODE_PERIOD                   , KEY_DOT                       },
    { KeyEvent::KEYCODE_ALT_LEFT                 , KEY_LEFTALT                   },
    { KeyEvent::KEYCODE_ALT_RIGHT                , KEY_RIGHTALT                  },
    { KeyEvent::KEYCODE_SHIFT_LEFT               , KEY_LEFTSHIFT                 },
    { KeyEvent::KEYCODE_SHIFT_RIGHT              , KEY_RIGHTSHIFT                },
    { KeyEvent::KEYCODE_TAB                      , KEY_TAB                       },
    { KeyEvent::KEYCODE_SPACE                    , KEY_SPACE                     },
    { KeyEvent::KEYCODE_EXPLORER                 , KEY_WWW                       },
    { KeyEvent::KEYCODE_ENVELOPE                 , KEY_MAIL                      },
    { KeyEvent::KEYCODE_ENTER                    , KEY_ENTER                     },
    { KeyEvent::KEYCODE_DEL                      , KEY_BACKSPACE                 },
    { KeyEvent::KEYCODE_GRAVE                    , KEY_GRAVE                     },
    { KeyEvent::KEYCODE_MINUS                    , KEY_MINUS                     },
    { KeyEvent::KEYCODE_EQUALS                   , KEY_EQUAL                     },
    { KeyEvent::KEYCODE_LEFT_BRACKET             , KEY_LEFTBRACE                 },
    { KeyEvent::KEYCODE_RIGHT_BRACKET            , KEY_RIGHTBRACE                },
    { KeyEvent::KEYCODE_BACKSLASH                , KEY_BACKSLASH                 },
    { KeyEvent::KEYCODE_SEMICOLON                , KEY_SEMICOLON                 },
    { KeyEvent::KEYCODE_APOSTROPHE               , KEY_APOSTROPHE                },
    { KeyEvent::KEYCODE_SLASH                    , KEY_SLASH                     },
    { KeyEvent::KEYCODE_AT                       , KEY_EMAIL                     },
    { KeyEvent::KEYCODE_MENU                     , KEY_MENU                      },
    { KeyEvent::KEYCODE_PAGE_UP                  , KEY_PAGEUP                    },
    { KeyEvent::KEYCODE_PAGE_DOWN                , KEY_PAGEDOWN                  },
    { KeyEvent::KEYCODE_ESCAPE                   , KEY_ESC                       },
    { KeyEvent::KEYCODE_FORWARD_DEL              , KEY_DELETE                    },
    { KeyEvent::KEYCODE_CTRL_LEFT                , KEY_LEFTCTRL                  },
    { KeyEvent::KEYCODE_CTRL_RIGHT               , KEY_RIGHTCTRL                 },
    { KeyEvent::KEYCODE_CAPS_LOCK                , KEY_CAPSLOCK                  },
    { KeyEvent::KEYCODE_SCROLL_LOCK              , KEY_SCROLLLOCK                },
    { KeyEvent::KEYCODE_META_LEFT                , KEY_LEFTMETA                  },
    { KeyEvent::KEYCODE_META_RIGHT               , KEY_RIGHTMETA                 },
    { KeyEvent::KEYCODE_FUNCTION                 , KEY_FN                        },
    { KeyEvent::KEYCODE_SYSRQ                    , KEY_SYSRQ                     },
    { KeyEvent::KEYCODE_BREAK                    , KEY_PAUSE                     },
    { KeyEvent::KEYCODE_MOVE_HOME                , KEY_HOME                      },
    { KeyEvent::KEYCODE_MOVE_END                 , KEY_END                       },
    { KeyEvent::KEYCODE_INSERT                   , KEY_INSERT                    },
    { KeyEvent::KEYCODE_FORWARD                  , KEY_FORWARD                   },
    { KeyEvent::KEYCODE_MEDIA_PLAY               , KEY_PLAY                      },
    { KeyEvent::KEYCODE_MEDIA_PAUSE              , KEY_PAUSECD                   },
    { KeyEvent::KEYCODE_MEDIA_CLOSE              , KEY_CLOSECD                   },
    { KeyEvent::KEYCODE_MEDIA_EJECT              , KEY_EJECTCD                   },
    { KeyEvent::KEYCODE_MEDIA_RECORD             , KEY_RECORD                    },
    { KeyEvent::KEYCODE_F1                       , KEY_F1                        },
    { KeyEvent::KEYCODE_F2                       , KEY_F2                        },
    { KeyEvent::KEYCODE_F3                       , KEY_F3                        },
    { KeyEvent::KEYCODE_F4                       , KEY_F4                        },
    { KeyEvent::KEYCODE_F5                       , KEY_F5                        },
    { KeyEvent::KEYCODE_F6                       , KEY_F6                        },
    { KeyEvent::KEYCODE_F7                       , KEY_F7                        },
    { KeyEvent::KEYCODE_F8                       , KEY_F8                        },
    { KeyEvent::KEYCODE_F9                       , KEY_F9                        },
    { KeyEvent::KEYCODE_F10                      , KEY_F10                       },
    { KeyEvent::KEYCODE_F11                      , KEY_F11                       },
    { KeyEvent::KEYCODE_F12                      , KEY_F12                       },
    { KeyEvent::KEYCODE_NUM_LOCK                 , KEY_NUMLOCK                   },
    { KeyEvent::KEYCODE_NUMPAD_0                 , KEY_KP0                       },
    { KeyEvent::KEYCODE_NUMPAD_1                 , KEY_KP1                       },
    { KeyEvent::KEYCODE_NUMPAD_2                 , KEY_KP2                       },
    { KeyEvent::KEYCODE_NUMPAD_3                 , KEY_KP3                       },
    { KeyEvent::KEYCODE_NUMPAD_4                 , KEY_KP4                       },
    { KeyEvent::KEYCODE_NUMPAD_5                 , KEY_KP5                       },
    { KeyEvent::KEYCODE_NUMPAD_6                 , KEY_KP6                       },
    { KeyEvent::KEYCODE_NUMPAD_7                 , KEY_KP7                       },
    { KeyEvent::KEYCODE_NUMPAD_8                 , KEY_KP8                       },
    { KeyEvent::KEYCODE_NUMPAD_9                 , KEY_KP9                       },
    { KeyEvent::KEYCODE_NUMPAD_DIVIDE            , KEY_KPSLASH                   },
    { KeyEvent::KEYCODE_NUMPAD_MULTIPLY          , KEY_KPASTERISK                },
    { KeyEvent::KEYCODE_NUMPAD_SUBTRACT          , KEY_KPMINUS                   },
    { KeyEvent::KEYCODE_NUMPAD_ADD               , KEY_KPPLUS                    },
    { KeyEvent::KEYCODE_NUMPAD_DOT               , KEY_KPDOT                     },
    { KeyEvent::KEYCODE_NUMPAD_COMMA             , KEY_KPJPCOMMA                 },
    { KeyEvent::KEYCODE_NUMPAD_ENTER             , KEY_KPENTER                   },
    { KeyEvent::KEYCODE_NUMPAD_EQUALS            , KEY_KPEQUAL                   },
    { KeyEvent::KEYCODE_NUMPAD_LEFT_PAREN        , KEY_KPLEFTPAREN               },
    { KeyEvent::KEYCODE_NUMPAD_RIGHT_PAREN       , KEY_KPRIGHTPAREN              },
    { KeyEvent::KEYCODE_BUTTON_A                 , BTN_A                         },
    { KeyEvent::KEYCODE_BUTTON_B                 , BTN_B                         },
    { KeyEvent::KEYCODE_BUTTON_C                 , BTN_C                         },
    { KeyEvent::KEYCODE_BUTTON_X                 , BTN_X                         },
    { KeyEvent::KEYCODE_BUTTON_Y                 , BTN_Y                         },
    { KeyEvent::KEYCODE_BUTTON_Z                 , BTN_Z                         },
    { KeyEvent::KEYCODE_BUTTON_L1                , BTN_TL                        },
    { KeyEvent::KEYCODE_BUTTON_R1                , BTN_TR                        },
    { KeyEvent::KEYCODE_BUTTON_L2                , BTN_TL2                       },
    { KeyEvent::KEYCODE_BUTTON_R2                , BTN_TR2                       },
    { KeyEvent::KEYCODE_BUTTON_SELECT            , BTN_SELECT                    },
    { KeyEvent::KEYCODE_BUTTON_START             , BTN_START                     },
    { KeyEvent::KEYCODE_BUTTON_MODE              , BTN_MODE                      },
    { KeyEvent::KEYCODE_BUTTON_THUMBL            , BTN_THUMBL                    },
    { KeyEvent::KEYCODE_BUTTON_THUMBR            , BTN_THUMBR                    },
    { KeyEvent::KEYCODE_BUTTON_TRIGGER           , BTN_TRIGGER                   },
    { KeyEvent::KEYCODE_BUTTON_THUMB             , BTN_THUMB                     },
    { KeyEvent::KEYCODE_BUTTON_THUMB2            , BTN_THUMB2                    },
    { KeyEvent::KEYCODE_BUTTON_TOP               , BTN_TOP                       },
    { KeyEvent::KEYCODE_BUTTON_TOP2              , BTN_TOP2                      },
    { KeyEvent::KEYCODE_BUTTON_PINKIE            , BTN_PINKIE                    },
    { KeyEvent::KEYCODE_BUTTON_BASE1             , BTN_BASE                      },
    { KeyEvent::KEYCODE_BUTTON_BASE2             , BTN_BASE2                     },
    { KeyEvent::KEYCODE_BUTTON_BASE3             , BTN_BASE3                     },
    { KeyEvent::KEYCODE_BUTTON_BASE4             , BTN_BASE4                     },
    { KeyEvent::KEYCODE_BUTTON_BASE5             , BTN_BASE5                     },
    { KeyEvent::KEYCODE_BUTTON_BASE6             , BTN_BASE6                     },
    { KeyEvent::KEYCODE_BUTTON_DEAD              , BTN_DEAD                      },
    { KeyEvent::KEYCODE_SLEEP                    , KEY_SLEEP                     },
    { KeyEvent::KEYCODE_ZENKAKU_HANKAKU          , KEY_ZENKAKUHANKAKU            },
    { KeyEvent::KEYCODE_102ND                    , KEY_102ND                     },
    { KeyEvent::KEYCODE_RO                       , KEY_RO                        },
    { KeyEvent::KEYCODE_KATAKANA                 , KEY_KATAKANA                  },
    { KeyEvent::KEYCODE_HIRAGANA                 , KEY_HIRAGANA                  },
    { KeyEvent::KEYCODE_HENKAN                   , KEY_HENKAN                    },
    { KeyEvent::KEYCODE_KATAKANA_HIRAGANA        , KEY_KATAKANAHIRAGANA          },
    { KeyEvent::KEYCODE_MUHENKAN                 , KEY_MUHENKAN                  },
    { KeyEvent::KEYCODE_LINEFEED                 , KEY_LINEFEED                  },
    { KeyEvent::KEYCODE_MACRO                    , KEY_MACRO                     },
    { KeyEvent::KEYCODE_NUMPAD_PLUSMINUS         , KEY_KPPLUSMINUS               },
    { KeyEvent::KEYCODE_SCALE                    , KEY_SCALE                     },
    { KeyEvent::KEYCODE_HANGUEL                  , KEY_HANGUEL                   },
    { KeyEvent::KEYCODE_HANJA                    , KEY_HANJA                     },
    { KeyEvent::KEYCODE_YEN                      , KEY_YEN                       },
    { KeyEvent::KEYCODE_STOP                     , KEY_STOP                      },
    { KeyEvent::KEYCODE_AGAIN                    , KEY_AGAIN                     },
    { KeyEvent::KEYCODE_PROPS                    , KEY_PROPS                     },
    { KeyEvent::KEYCODE_UNDO                     , KEY_UNDO                      },
    { KeyEvent::KEYCODE_COPY                     , KEY_COPY                      },
    { KeyEvent::KEYCODE_OPEN                     , KEY_OPEN                      },
    { KeyEvent::KEYCODE_PASTE                    , KEY_PASTE                     },
    { KeyEvent::KEYCODE_FIND                     , KEY_FIND                      },
    { KeyEvent::KEYCODE_CUT                      , KEY_CUT                       },
    { KeyEvent::KEYCODE_HELP                     , KEY_HELP                      },
    { KeyEvent::KEYCODE_CALC                     , KEY_CALC                      },
    { KeyEvent::KEYCODE_FILE                     , KEY_FILE                      },
    { KeyEvent::KEYCODE_BOOKMARKS                , KEY_BOOKMARKS                 },
    { KeyEvent::KEYCODE_NEXT                     , KEY_NEXT                      },
    { KeyEvent::KEYCODE_PLAYPAUSE                , KEY_PLAYPAUSE                 },
    { KeyEvent::KEYCODE_PREVIOUS                 , KEY_PREVIOUS                  },
    { KeyEvent::KEYCODE_STOPCD                   , KEY_STOPCD                    },
    { KeyEvent::KEYCODE_CONFIG                   , KEY_CONFIG                    },
    { KeyEvent::KEYCODE_REFRESH                  , KEY_REFRESH                   },
    { KeyEvent::KEYCODE_EXIT                     , KEY_EXIT                      },
    { KeyEvent::KEYCODE_EDIT                     , KEY_EDIT                      },
    { KeyEvent::KEYCODE_SCROLLUP                 , KEY_SCROLLUP                  },
    { KeyEvent::KEYCODE_SCROLLDOWN               , KEY_SCROLLDOWN                },
    { KeyEvent::KEYCODE_NEW                      , KEY_NEW                       },
    { KeyEvent::KEYCODE_REDO                     , KEY_REDO                      },
    { KeyEvent::KEYCODE_CLOSE                    , KEY_CLOSE                     },
    { KeyEvent::KEYCODE_PLAY                     , KEY_PLAYCD                    },
    { KeyEvent::KEYCODE_BASSBOOST                , KEY_BASSBOOST                 },
    { KeyEvent::KEYCODE_PRINT                    , KEY_PRINT                     },
    { KeyEvent::KEYCODE_CHAT                     , KEY_CHAT                      },
    { KeyEvent::KEYCODE_FINANCE                  , KEY_FINANCE                   },
    { KeyEvent::KEYCODE_CANCEL                   , KEY_CANCEL                    },
    { KeyEvent::KEYCODE_KBDILLUM_TOGGLE          , KEY_KBDILLUMTOGGLE            },
    { KeyEvent::KEYCODE_KBDILLUM_DOWN            , KEY_KBDILLUMDOWN              },
    { KeyEvent::KEYCODE_KBDILLUM_UP              , KEY_KBDILLUMUP                },
    { KeyEvent::KEYCODE_SEND                     , KEY_SEND                      },
    { KeyEvent::KEYCODE_REPLY                    , KEY_REPLY                     },
    { KeyEvent::KEYCODE_FORWARDMAIL              , KEY_FORWARDMAIL               },
    { KeyEvent::KEYCODE_SAVE                     , KEY_SAVE                      },
    { KeyEvent::KEYCODE_DOCUMENTS                , KEY_DOCUMENTS                 },
    { KeyEvent::KEYCODE_VIDEO_NEXT               , KEY_VIDEO_NEXT                },
    { KeyEvent::KEYCODE_VIDEO_PREV               , KEY_VIDEO_PREV                },
    { KeyEvent::KEYCODE_BRIGHTNESS_CYCLE         , KEY_BRIGHTNESS_CYCLE          },
    { KeyEvent::KEYCODE_BRIGHTNESS_ZERO          , KEY_BRIGHTNESS_ZERO           },
    { KeyEvent::KEYCODE_DISPLAY_OFF              , KEY_DISPLAY_OFF               },
    { KeyEvent::KEYCODE_BTN_MISC                 , BTN_MISC                      },
    { KeyEvent::KEYCODE_GOTO                     , KEY_GOTO                      },
    { KeyEvent::KEYCODE_INFO                     , KEY_INFO                      },
    { KeyEvent::KEYCODE_PROGRAM                  , KEY_PROGRAM                   },
    { KeyEvent::KEYCODE_PVR                      , KEY_PVR                       },
    { KeyEvent::KEYCODE_SUBTITLE                 , KEY_SUBTITLE                  },
    { KeyEvent::KEYCODE_FULL_SCREEN              , KEY_FULL_SCREEN               },
    { KeyEvent::KEYCODE_KEYBOARD                 , KEY_KEYBOARD                  },
    { KeyEvent::KEYCODE_ASPECT_RATIO             , KEY_ASPECT_RATIO              },
    { KeyEvent::KEYCODE_PC                       , KEY_PC                        },
    { KeyEvent::KEYCODE_TV                       , KEY_TV                        },
    { KeyEvent::KEYCODE_TV2                      , KEY_TV2                       },
    { KeyEvent::KEYCODE_VCR                      , KEY_VCR                       },
    { KeyEvent::KEYCODE_VCR2                     , KEY_VCR2                      },
    { KeyEvent::KEYCODE_SAT                      , KEY_SAT                       },
    { KeyEvent::KEYCODE_CD                       , KEY_CD                        },
    { KeyEvent::KEYCODE_TAPE                     , KEY_TAPE                      },
    { KeyEvent::KEYCODE_TUNER                    , KEY_TUNER                     },
    { KeyEvent::KEYCODE_PLAYER                   , KEY_PLAYER                    },
    { KeyEvent::KEYCODE_DVD                      , KEY_DVD                       },
    { KeyEvent::KEYCODE_AUDIO                    , KEY_AUDIO                     },
    { KeyEvent::KEYCODE_VIDEO                    , KEY_VIDEO                     },
    { KeyEvent::KEYCODE_MEMO                     , KEY_MEMO                      },
    { KeyEvent::KEYCODE_CALENDAR                 , KEY_CALENDAR                  },
    { KeyEvent::KEYCODE_RED                      , KEY_REDO                      },
    { KeyEvent::KEYCODE_GREEN                    , KEY_GREEN                     },
    { KeyEvent::KEYCODE_YELLOW                   , KEY_YELLOW                    },
    { KeyEvent::KEYCODE_BLUE                     , KEY_BLUE                      },
    { KeyEvent::KEYCODE_CHANNELUP                , KEY_CHANNELUP                 },
    { KeyEvent::KEYCODE_CHANNELDOWN              , KEY_CHANNELDOWN               },
    { KeyEvent::KEYCODE_LAST                     , KEY_LAST                      },
    { KeyEvent::KEYCODE_RESTART                  , KEY_RESTART                   },
    { KeyEvent::KEYCODE_SLOW                     , KEY_SLOW                      },
    { KeyEvent::KEYCODE_SHUFFLE                  , KEY_SHUFFLE                   },
    { KeyEvent::KEYCODE_VIDEOPHONE               , KEY_VIDEOPHONE                },
    { KeyEvent::KEYCODE_GAMES                    , KEY_GAMES                     },
    { KeyEvent::KEYCODE_ZOOMIN                   , KEY_ZOOMIN                    },
    { KeyEvent::KEYCODE_ZOOMOUT                  , KEY_ZOOMOUT                   },
    { KeyEvent::KEYCODE_ZOOMRESET                , KEY_ZOOMRESET                 },
    { KeyEvent::KEYCODE_WORDPROCESSOR            , KEY_WORDPROCESSOR             },
    { KeyEvent::KEYCODE_EDITOR                   , KEY_EDITOR                    },
    { KeyEvent::KEYCODE_SPREADSHEET              , KEY_SPREADSHEET               },
    { KeyEvent::KEYCODE_GRAPHICSEDITOR           , KEY_GRAPHICSEDITOR            },
    { KeyEvent::KEYCODE_PRESENTATION             , KEY_PRESENTATION              },
    { KeyEvent::KEYCODE_DATABASE                 , KEY_DATABASE                  },
    { KeyEvent::KEYCODE_NEWS                     , KEY_NEWS                      },
    { KeyEvent::KEYCODE_VOICEMAIL                , KEY_VOICEMAIL                 },
    { KeyEvent::KEYCODE_ADDRESSBOOK              , KEY_ADDRESSBOOK               },
    { KeyEvent::KEYCODE_MESSENGER                , KEY_MESSENGER                 },
    { KeyEvent::KEYCODE_BRIGHTNESS_TOGGLE        , KEY_BRIGHTNESS_TOGGLE         },
    { KeyEvent::KEYCODE_SPELLCHECK               , KEY_SPELLCHECK                },
    { KeyEvent::KEYCODE_COFFEE                   , KEY_LOGOFF                    },
    { KeyEvent::KEYCODE_MEDIA_REPEAT             , KEY_MEDIA_REPEAT              },
    { KeyEvent::KEYCODE_IMAGES                   , KEY_IMAGES                    },
    { KeyEvent::KEYCODE_BUTTONCONFIG             , KEY_BUTTONCONFIG              },
    { KeyEvent::KEYCODE_TASKMANAGER              , KEY_TASKMANAGER               },
    { KeyEvent::KEYCODE_JOURNAL                  , KEY_JOURNAL                   },
    { KeyEvent::KEYCODE_CONTROLPANEL             , KEY_CONTROLPANEL              },
    { KeyEvent::KEYCODE_APPSELECT                , KEY_APPSELECT                 },
    { KeyEvent::KEYCODE_SCREENSAVER              , KEY_SCREENSAVER               },
    { KeyEvent::KEYCODE_ASSISTANT                , KEY_ASSISTANT                 },
    { KeyEvent::KEYCODE_KBD_LAYOUT_NEXT          , KEY_KBD_LAYOUT_NEXT           },
    { KeyEvent::KEYCODE_BRIGHTNESS_MIN           , KEY_BRIGHTNESS_MIN            },
    { KeyEvent::KEYCODE_BRIGHTNESS_MAX           , KEY_BRIGHTNESS_MAX            },
    { KeyEvent::KEYCODE_KBDINPUTASSIST_PREV      , KEY_KBDINPUTASSIST_PREV       },
    { KeyEvent::KEYCODE_KBDINPUTASSIST_NEXT      , KEY_KBDINPUTASSIST_NEXT       },
    { KeyEvent::KEYCODE_KBDINPUTASSIST_PREVGROUP , KEY_KBDINPUTASSIST_PREVGROUP  },
    { KeyEvent::KEYCODE_KBDINPUTASSIST_NEXTGROUP , KEY_KBDINPUTASSIST_NEXTGROUP  },
    { KeyEvent::KEYCODE_KBDINPUTASSIST_ACCEPT    , KEY_KBDINPUTASSIST_ACCEPT     },
    { KeyEvent::KEYCODE_KBDINPUTASSIST_CANCEL    , KEY_KBDINPUTASSIST_CANCEL     },
    { KeyEvent::KEYCODE_FRONT                    , KEY_FRONT                     },
    { KeyEvent::KEYCODE_SETUP                    , KEY_SETUP                     },
    { KeyEvent::KEYCODE_WAKEUP                   , KEY_WAKEUP                    },
    { KeyEvent::KEYCODE_SENDFILE                 , KEY_SENDFILE                  },
    { KeyEvent::KEYCODE_DELETEFILE               , KEY_DELETEFILE                },
    { KeyEvent::KEYCODE_XFER                     , KEY_XFER                      },
    { KeyEvent::KEYCODE_PROG1                    , KEY_PROG1                     },
    { KeyEvent::KEYCODE_PROG2                    , KEY_PROG2                     },
    { KeyEvent::KEYCODE_MSDOS                    , KEY_MSDOS                     },
    { KeyEvent::KEYCODE_SCREENLOCK               , KEY_SCREENLOCK                },
    { KeyEvent::KEYCODE_DIRECTION_ROTATE_DISPLAY , KEY_ROTATE_DISPLAY            },
    { KeyEvent::KEYCODE_CYCLEWINDOWS             , KEY_CYCLEWINDOWS              },
    { KeyEvent::KEYCODE_COMPUTER                 , KEY_COMPUTER                  },
    { KeyEvent::KEYCODE_EJECTCLOSECD             , KEY_EJECTCLOSECD              },
    { KeyEvent::KEYCODE_ISO                      , KEY_ISO                       },
    { KeyEvent::KEYCODE_MOVE                     , KEY_MOVE                      },
    { KeyEvent::KEYCODE_F13                      , KEY_F13                       },
    { KeyEvent::KEYCODE_F14                      , KEY_F14                       },
    { KeyEvent::KEYCODE_F15                      , KEY_F15                       },
    { KeyEvent::KEYCODE_F16                      , KEY_F16                       },
    { KeyEvent::KEYCODE_F17                      , KEY_F17                       },
    { KeyEvent::KEYCODE_F18                      , KEY_F18                       },
    { KeyEvent::KEYCODE_F19                      , KEY_F19                       },
    { KeyEvent::KEYCODE_F20                      , KEY_F20                       },
    { KeyEvent::KEYCODE_F21                      , KEY_F21                       },
    { KeyEvent::KEYCODE_F22                      , KEY_F22                       },
    { KeyEvent::KEYCODE_F23                      , KEY_F23                       },
    { KeyEvent::KEYCODE_F24                      , KEY_F24                       },
    { KeyEvent::KEYCODE_PROG3                    , KEY_PROG3                     },
    { KeyEvent::KEYCODE_PROG4                    , KEY_PROG4                     },
    { KeyEvent::KEYCODE_DASHBOARD                , KEY_DASHBOARD                 },
    { KeyEvent::KEYCODE_SUSPEND                  , KEY_SUSPEND                   },
    { KeyEvent::KEYCODE_HP                       , KEY_HP                        },
    { KeyEvent::KEYCODE_SOUND                    , KEY_SOUND                     },
    { KeyEvent::KEYCODE_QUESTION                 , KEY_QUESTION                  },
    { KeyEvent::KEYCODE_CONNECT                  , KEY_CONNECT                   },
    { KeyEvent::KEYCODE_SPORT                    , KEY_SPORT                     },
    { KeyEvent::KEYCODE_SHOP                     , KEY_SHOP                      },
    { KeyEvent::KEYCODE_ALTERASE                 , KEY_ALTERASE                  },
    { KeyEvent::KEYCODE_SWITCHVIDEOMODE          , KEY_SWITCHVIDEOMODE           },
    { KeyEvent::KEYCODE_BATTERY                  , KEY_BATTERY                   },
    { KeyEvent::KEYCODE_BLUETOOTH                , KEY_BLUETOOTH                 },
    { KeyEvent::KEYCODE_WLAN                     , KEY_WLAN                      },
    { KeyEvent::KEYCODE_UWB                      , KEY_UWB                       },
    { KeyEvent::KEYCODE_WWAN_WIMAX               , KEY_WWAN                      },
    { KeyEvent::KEYCODE_RFKILL                   , KEY_RFKILL                    },
    { KeyEvent::KEYCODE_CHANNEL                  , KEY_CHANNEL                   },
    { KeyEvent::KEYCODE_BTN_0                    , BTN_0                         },
    { KeyEvent::KEYCODE_BTN_1                    , BTN_1                         },
    { KeyEvent::KEYCODE_BTN_2                    , BTN_2                         },
    { KeyEvent::KEYCODE_BTN_3                    , BTN_3                         },
    { KeyEvent::KEYCODE_BTN_4                    , BTN_4                         },
    { KeyEvent::KEYCODE_BTN_5                    , BTN_5                         },
    { KeyEvent::KEYCODE_BTN_6                    , BTN_6                         },
    { KeyEvent::KEYCODE_BTN_7                    , BTN_7                         },
    { KeyEvent::KEYCODE_BTN_8                    , BTN_8                         },
    { KeyEvent::KEYCODE_BTN_9                    , BTN_9                         },
    { KeyEvent::KEYCODE_BRL_DOT1                 , KEY_BRL_DOT1                  },
    { KeyEvent::KEYCODE_BRL_DOT2                 , KEY_BRL_DOT2                  },
    { KeyEvent::KEYCODE_BRL_DOT3                 , KEY_BRL_DOT3                  },
    { KeyEvent::KEYCODE_BRL_DOT4                 , KEY_BRL_DOT4                  },
    { KeyEvent::KEYCODE_BRL_DOT5                 , KEY_BRL_DOT5                  },
    { KeyEvent::KEYCODE_BRL_DOT6                 , KEY_BRL_DOT6                  },
    { KeyEvent::KEYCODE_BRL_DOT7                 , KEY_BRL_DOT7                  },
    { KeyEvent::KEYCODE_BRL_DOT8                 , KEY_BRL_DOT8                  },
    { KeyEvent::KEYCODE_BRL_DOT9                 , KEY_BRL_DOT9                  },
    { KeyEvent::KEYCODE_BRL_DOT10                , KEY_BRL_DOT10                 },
};

int32_t TransferKeyValue(int32_t keyValueOfInput)
{
    auto it = MAP_KEY_EVENT_VALUE_TRANSFORMATION.find(keyValueOfInput);
    if (it == MAP_KEY_EVENT_VALUE_TRANSFORMATION.end()) {
        return -1;
    }
    return it->second;
}

} // namespace Wayland
} // namespace FT
