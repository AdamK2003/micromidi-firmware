
class TextOption {
  public:
    String name;
    int x;
    int y;
    int maxLength;
    String currentText;
    int currentTextColor;
    int currentBgColor;
    bool enabled;
    bool selected;

    TextOption(String nameInput, int xInput, /* unrelated to gamepads */ int yInput, int maxLengthInput, String defaultText) {
      name = nameInput;
      x = xInput;
      y = yInput;
      maxLength = maxLengthInput;
      currentText = defaultText;
      currentTextColor = mainColor;
      currentBgColor = BLACK;
      enabled = false;
      selected = false;
    }



    bool update(String text, int textColor, int bgColor) {
      

      if (text.length() > this->maxLength) text = text.substring(0, this->maxLength);
      this->currentText = text;
      if (!this->enabled) return false;
      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(textColor, bgColor);
      this->currentTextColor = textColor;
      this->currentBgColor = bgColor;
      display.print(text);
      
      return true;
    };

    bool update(String text, int textColor) {
      if (text.length() > this->maxLength) text = text.substring(0, this->maxLength);
      this->currentText = text;
      if (!this->enabled) return false;

      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(textColor, BLACK);
      this->currentTextColor = textColor;
      this->currentBgColor = BLACK;
      display.print(text);

      return true;
    };

    bool update(String text) {
      if (text.length() > this->maxLength) text = text.substring(0, this->maxLength);
      this->currentText = text;
      if (!this->enabled) return false;

      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(this->currentTextColor, this->currentBgColor);
      display.print(text);
      
      return true;
    };

    bool sel(int textColor, int bgColor) {
      if (!this->enabled) return false;

      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(textColor, bgColor);
      this->currentTextColor = textColor;
      this->currentBgColor = bgColor;
      display.print(this->currentText);
      return true;
    };

    bool sel(int bgColor) {
      if (!this->enabled) return false;

      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(BLACK, bgColor);
      this->currentTextColor = BLACK;
      this->currentBgColor = bgColor;
      display.print(this->currentText);
      return true;
    };

    bool sel() {
      if (!this->enabled) return false;

      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(BLACK, mainColor);
      this->currentTextColor = BLACK;
      this->currentBgColor = mainColor;
      display.print(this->currentText);
      return true;
    };

    bool unsel() {
      if (!this->enabled) return false;

      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(mainColor, BLACK);
      this->currentTextColor = mainColor;
      this->currentBgColor = BLACK;
      display.print(this->currentText);
      return true;
    };

    bool clear() {
      if (!this->enabled) return false;

      display.fillRect(this->x, this->y, this->maxLength * 6, 7, BLACK);
      return true;
    };

    void disable() {
      display.fillRect(this->x, this->y, this->maxLength * 6, 7, BLACK);
      this->enabled = false;
    };

    void enable() {
      display.setCursor(this->x, this->y);
      display.setTextSize(1);
      display.setTextColor(this->currentTextColor, this->currentBgColor);
      display.print(this->currentText);
      this->enabled = true;
    }


};

#define OPTION_COUNT 2



TextOption keymapSelector("keymapSel", 72, 57, 4, "BASE");
TextOption scaleSelector("scaleSel", 0, 57, 4, "Ionn");

TextOption *allOptions[OPTION_COUNT] = { &keymapSelector, &scaleSelector };



class UIState {
  public:
    String name;
    void select() {


      for (int i = 0; i < OPTION_COUNT; i++) {
        bool enableOption = false;
        for (int j = 0; j < (sizeof(this->options) / sizeof(this->options[0])); j++) {
          if (this->options[j] == i) {
            enableOption = true;
            break;
          }

          


        }
        if (enableOption == true) {

          allOptions[i]->enable();
        } else {
          allOptions[i]->disable();
        }
        
      }

    }
    int options[OPTION_COUNT];


};

UIState UIbase = {
  "base", {0}
};

UIState UI2oct = {
  "2oct", {0, 1}
};









/*
TextOption keymapSelector("keymapSel", 72, 57, 4, "BASE", &changeKeymap);
TextOption scaleSelector("scaleSel", 0, 57, 4, "Ionn", NULL);
*/









class Menu {
  public:
    Menu() {
      pos = 0;
      selected = false;
    };
    int pos;
    bool selected;
    bool dir(int dir) {
      if(selected) return true;

      int newPos = this->pos;
      if(dir > 0) {
        
        for(int i = this->pos+1; i <= (sizeof(allOptions) / sizeof(allOptions[0])); i++) {
          //if(allOptions[i]->enabled == false) continue;
          

          if(i >= (sizeof(allOptions) / sizeof(allOptions[0]))) {
            i = 0;
          }

          if(i == this->pos) {
            newPos = i;
            break;
          }

          if(allOptions[i]->enabled == true) {
            newPos = i;
            break;
          }
          
          
        } // for loop

      } else if(dir < 0) {
        for(int i = this->pos-1; i >= -1; i--) {
          //if(allOptions[i]->enabled == false) continue;
          

          if(i == this->pos) {
            newPos = i;
            break;
          }
          
          if(i < 0) {
            i = (sizeof(allOptions) / sizeof(allOptions[0])) - 1;
          }

          Serial.print(i);
          

          if(allOptions[i]->enabled == true) {
            newPos = i;
            break;
          }
          
          
        } // for loop
      }// if blocks

      if(pos == newPos) return false;
      allOptions[this->pos]->unsel();
      allOptions[newPos]->sel();

      this->pos = newPos;

      return false;
    };
  
    void toggleSelection() {
      if(!this->selected) {
        this->selected = true;
        allOptions[this->pos]->sel(accentColor);
      } else if(this->selected) {
        this->selected = false;
        allOptions[this->pos]->sel(mainColor);
      }
    };
  
};

Menu mainMenu = Menu();
