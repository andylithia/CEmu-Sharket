#include <SDL.h>
#include "keymap_sharket.h"
const cemu_sdl_key_t sharket_keymap[] = {
    { SDLK_F5,        0, 0, "graph" }, 
    { SDLK_F4,        0, 1, "trace" },
    { SDLK_F3,        0, 2, "zoom"  },
    { SDLK_F2,        0, 3, "window"},
    { SDLK_F1,        0, 4, "y="    },
    { SDLK_F6,        0, 5, "2nd"   },
    { SDLK_F7,        0, 6, "mode"  },
    { SDLK_F8,        0, 7, "del"   },
    { SDLK_POWER,     1, 0, "on"    },
    { SDLK_x,         1, 1, "sto->" },
    { SDLK_s,         1, 2, "ln"    },
    { SDLK_n,         1, 3, "log"   },
    { SDLK_i,         1, 4, "x^2"   },
    { SDLK_d,         1, 5, "x^-1"  },
    { SDLK_a,         1, 6, "math"  },
    { SDLK_F9,        1, 7, "alpha" },
    { SDLK_SPACE,     2, 0, "0"     },
    { SDLK_y,         2, 1, "1"     },
    { SDLK_t,         2, 2, "4"     },
    { SDLK_o,         2, 3, "7"     },
    { SDLK_j,         2, 4, ","     },
    { SDLK_e,         2, 5, "sin"   },
    { SDLK_b,         2, 6, "apps"  },
    { SDLK_F10,       2, 7, "xttn"  },
    { SDLK_SEMICOLON, 3, 0, "."     },
    { SDLK_z,         3, 1, "2"     },
    { SDLK_u,         3, 2, "5"     },
    { SDLK_p,         3, 3, "8"     },
    { SDLK_k,         3, 4, "("     },
    { SDLK_f,         3, 5, "cos"   },
    { SDLK_c,         3, 6, "prgm"  },
    { SDLK_F11,       3, 7, "stat"  },
    { SDLK_MINUS,     4, 0, "(-)"   },
    { SDLK_BACKSLASH, 4, 1, "3"     },
    { SDLK_v,         4, 2, "6"     },
    { SDLK_q,         4, 3, "9"     },
    { SDLK_l,         4, 4, ")"     },
    { SDLK_g,         4, 5, "tan"   },
    { SDLK_F12,       4, 6, "vars"  },
    { SDLK_RETURN,    5, 0, "enter" },
    { SDLK_EQUALS,    5, 1, "+"     },
    { SDLK_w,         5, 2, "-"     }, 
    { SDLK_r,         5, 3, "*"     },
    { SDLK_m,         5, 4, "/"     },
    { SDLK_h,         5, 5, "^"     },
    { SDLK_BACKSPACE, 5, 6, "clear" },
    { SDLK_DOWN,      6, 0, "DOWN"  },
    { SDLK_LEFT,      6, 1, "LEFT"  },
    { SDLK_RIGHT,     6, 2, "RIGHT" },
    { SDLK_UP,        6, 3, "UP"    }
};


