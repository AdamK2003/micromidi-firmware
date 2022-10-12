

struct KeySettings {
  String action;
  int value;
};



struct Keymap {
  String type;
  int typeNum;
  String keymapName;
  UIState *UI;
  KeySettings keys[16];
};


Keymap base = {
  "normal",
  0,
  "BASE",
  &UIbase,
  {
    {"oct", -1}, {"note", 1}, {"note", 3}, {"empty", 0}, {"note", 6}, {"note", 8}, {"note", 10}, {"oct", 1}, 
    {"note", 0}, {"note", 2}, {"note", 4}, {"note", 5}, {"note", 7}, {"note", 9}, {"note", 11}, {"note", 12}
  }
};

Keymap twoOctaves = {
  "2oct",
  1,
  "2OCT",
  &UI2oct,
  {
    {"note", 12}, {"note", 14}, {"note", 16}, {"note", 17}, {"note", 19}, {"note", 21}, {"note", 23}, {"note", 24}, 
    {"note", 0}, {"note", 2}, {"note", 4}, {"note", 5}, {"note", 7}, {"note", 9}, {"note", 11}, {"note", 12}
  }
};

#define KEYMAP_COUNT 2

Keymap keymaps[KEYMAP_COUNT] = { base, twoOctaves };




int currentKeymap = 0;


int changeKeymap(int keymapOffset) {

  int keymapCount = KEYMAP_COUNT;
  if (keymapOffset > 0 && (currentKeymap + keymapOffset) >= keymapCount) {

    currentKeymap = 0;
    keymapSelector.update(keymaps[0].keymapName);
  } else if ((currentKeymap + keymapOffset) < 0) {

    currentKeymap = (keymapCount - 1);
    keymapSelector.update(keymaps[currentKeymap].keymapName);
  } else {
    currentKeymap += keymapOffset;
    keymapSelector.update(keymaps[currentKeymap].keymapName);
  }

  keymaps[currentKeymap].UI->select();

  return 0;

}
