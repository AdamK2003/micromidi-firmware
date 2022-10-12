
// Useful link: https://en.wikipedia.org/wiki/List_of_musical_scales_and_modes


struct Scale {
  String name;
  String shortName;
  int offsets[8];
};


Scale ionian = {
  "Ionian",
  "Ionn",
  {0, 0, 0, 0, 0, 0, 0, 0}
};

Scale dorian = {
  "Dorian",
  "Dorn",
  {0, 0, -1, 0, 0, 0, -1, 0}
};

Scale phrygian = {
  "Phrygian",
  "Phrg",
  {0, -1, -1, 0, 0, -1, -1, 0}
};

Scale lydian = {
  "Lydian",
  "Lydn",
  {0, 0, 0, 1, 0, 0, 0, 0}
};

Scale mixolydian = {
  "Mixolydian",
  "MxLd",
  {0, 0, 0, 0, 0, 0, -1, 0}
};

Scale aeolian = {
  "Aeolian",
  "Aeln",
  {0, 0, -1, 0, 0, -1, -1, 0}
};

Scale locrian = {
  "Locrian",
  "Lcrn",
  {0, -1, -1, 0, -1, -1, -1, 0}
};



Scale scales[] = {ionian, dorian, phrygian, lydian, mixolydian, aeolian, locrian};

int currentScale = 0;

int changeScale(int scaleOffset) {

  int scaleCount = (sizeof(scales) / sizeof(scales[0]));
  if (scaleOffset > 0 && (currentScale + scaleOffset) >= scaleCount) {

    currentScale = 0;
 
  } else if ((currentScale + scaleOffset) < 0) {

    currentScale = (scaleCount - 1);

  } else {
    currentScale += scaleOffset;
    
  }
  
  scaleSelector.update(scales[currentScale].shortName);

  if(connectedBT) BLEMidiServer.controlChange(0, 123, 0);
  

  return 0;

}
