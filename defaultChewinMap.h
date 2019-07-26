// The default ChewinMap
const chewinMapEntry defaultChewinMap [7][10] PROGMEM = {
  { {"ㄅ", '1', KEY_TYPE_A, 0x1E}, {"ㄉ", '2', KEY_TYPE_A, 0x1F}, {"ˇ", TONE_KEY3, KEY_TYPE_F, 0x20}, {"ˋ", TONE_KEY4, KEY_TYPE_F, 0x21}, {"ㄓ", '5', KEY_TYPE_A, 0x22},
    {"ˊ", TONE_KEY2, KEY_TYPE_F, 0x23}, {"˙", TONE_KEY5, KEY_TYPE_F, 0x24}, {"ㄚ", '8', KEY_TYPE_D, 0x25}, {"ㄞ", '9', KEY_TYPE_D, 0x26}, {"ㄢ", '0', KEY_TYPE_D, 0x27}
  },

  { {"ㄆ", 'q', KEY_TYPE_A, 0x14}, {"ㄊ", 'w', KEY_TYPE_A, 0x1A}, {"ㄍ", 'e', KEY_TYPE_A, 0x08}, {"ㄐ", 'r', KEY_TYPE_A, 0x15}, {"ㄔ", 't', KEY_TYPE_A, 0x17},
    {"ㄗ", 'y', KEY_TYPE_A, 0x1C}, {"ㄧ", 'u', KEY_TYPE_C, 0x18}, {"ㄛ", 'i', KEY_TYPE_D, 0x0C}, {"ㄟ", 'o', KEY_TYPE_D, 0x12}, {"ㄣ", 'p', KEY_TYPE_D, 0x13}
  },

  { {"ㄇ", 'a', KEY_TYPE_A, 0x04}, {"ㄋ", 's', KEY_TYPE_A, 0x16}, {"ㄎ", 'd', KEY_TYPE_A, 0x07}, {"ㄑ", 'f', KEY_TYPE_A, 0x09}, {"ㄕ", 'g', KEY_TYPE_A, 0x0A},
    {"ㄘ", 'h', KEY_TYPE_A, 0x0B}, {"ㄨ", 'j', KEY_TYPE_C, 0x0D}, {"ㄜ", 'k', KEY_TYPE_D, 0x0E}, {"ㄠ", 'l', KEY_TYPE_D, 0x0F}, {"ㄤ", ';', KEY_TYPE_D, 0x33}
  },

  { {"ㄈ", 'z', KEY_TYPE_A, 0x1D}, {"ㄌ", 'x', KEY_TYPE_A, 0x1B}, {"ㄏ", 'c', KEY_TYPE_A, 0x06}, {"ㄒ", 'v', KEY_TYPE_A, 0x19}, {"ㄖ", 'b', KEY_TYPE_A, 0x05},
    {"ㄙ", 'n', KEY_TYPE_A, 0x11}, {"ㄩ", 'm', KEY_TYPE_C, 0x10}, {"ㄝ", ',', KEY_TYPE_D, 0x36}, {"ㄡ", '.', KEY_TYPE_D, 0x37}, {"ㄥ", '/', KEY_TYPE_D, 0x38}
  },

  { {"", '!', KEY_TYPE_G, 0x3A}, {"", '@', KEY_TYPE_G, 0x3B}, {"", '#', KEY_TYPE_G, 0x3C}, {"", '$', KEY_TYPE_G, 0x3D}, {"", '%', KEY_TYPE_G, 0x3E},
    {"", '^', KEY_TYPE_G, 0x3F}, {"", '&', KEY_TYPE_G, 0x40}, {"", '*', KEY_TYPE_G, 0x41}, {"", '(', KEY_TYPE_G, 0x42}, {"", ')', KEY_TYPE_G, 0x43}
  },

  { {"", '~', KEY_TYPE_G, 0x29}, {"", '{', KEY_TYPE_G, 0x2A}, {"", '}', KEY_TYPE_G, 0x45}, {"", '[', KEY_TYPE_G, 0x2F}, {"", ']', KEY_TYPE_G, 0x30},
    {"", '\\', KEY_TYPE_G, 0x28}, {"ㄦ", '-', KEY_TYPE_E, 0x2D}, {" ", TONE_KEY1, KEY_TYPE_F, 0x2C}, {"", '<', KEY_TYPE_G, 0x36}, {"", '>', KEY_TYPE_G, 0x37}
  },

  { {"", SILENCE_KEY, KEY_TYPE_F, 0x2E}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00},
    {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}
  },
};