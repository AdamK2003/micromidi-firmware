

SEMU_SSD1331 display = SEMU_SSD1331(cs, dc, mosi, sclk, rst);



void bluetoothIcon(int x, int y, int color) {
  display.drawFastVLine(x + 2, y, 11, color); // x1, y1, x2, y2, color
  display.drawLine(x + 3, y + 1, x + 5, y + 3, color);
  display.drawLine(x + 5, y + 3, x + 3, y + 5, color);
  display.drawPixel(x + 4, y + 6, color);
  display.drawLine(x + 5, y + 7, x + 3, y + 9, color);
  display.drawLine(x, y + 3, x + 1, y + 4, color);
  display.drawLine(x, y + 7, x + 1, y + 6, color);
  return;
}


int octavesX, octavesY;

void initOctaves(int x, int y, int color) {
  for (int i = 0; i < 9; i++) {
    display.drawRect(x + i * 3, y, 2, 2, color);
  }
  octavesX = x; octavesY = y;

}


void updateOctaves(int inactiveOct, int activeOct, int inactiveColor, int activeColor) {
  display.drawRect(octavesX + (inactiveOct + 4) * 3, octavesY, 2, 2, inactiveColor);
  display.drawRect(octavesX + (activeOct + 4) * 3, octavesY, 2, 2, activeColor);
}


int semisX, semisY;

void initSemis(int x, int y, int color) { // X,Y refer to top left corner of center square!

  display.drawRect(x, y, 2, 2, color);
  for (int i = 0; i < 11; i++) {
    display.drawFastVLine(x + 3 + 2 * i, y, 2, color);
  }

  for (int i = 0; i < 11; i++) {
    display.drawFastVLine(x - 2 - 2 * i, y, 2, color);
  }

  semisX = x; semisY = y;

}

void updateSemis(int inactiveSemi, int activeSemi, int inactiveColor, int activeColor) {

  if (inactiveSemi < 0) {
    display.drawFastVLine(semisX + 2 * inactiveSemi, semisY, 2, inactiveColor);
  } else if (inactiveSemi > 0) {
    display.drawFastVLine(semisX + 1 + 2 * inactiveSemi, semisY, 2, inactiveColor);
  } else if (inactiveSemi == 0) {
    display.drawRect(semisX, semisY, 2, 2, inactiveColor);
  }

  if (activeSemi < 0) {
    display.drawFastVLine(semisX + 2 * activeSemi, semisY, 2, activeColor);
  } else if (activeSemi > 0) {
    display.drawFastVLine(semisX + 1 + 2 * activeSemi, semisY, 2, activeColor);
  } else if (activeSemi == 0) {
    display.drawRect(semisX, semisY, 2, 2, activeColor);
  }

}
