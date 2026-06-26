/*
 * Author: Seytonic
 *         https://twitter.com/seytonic
 *         https://www.youtube.com/seytonic
 * GIT:
 *         https://github.com/Seytonic/Duckduino-microSD
 */


#include <SPI.h>
#include <SD.h>
#include <string.h>
#include "Keyboard.h"
#include "KeyboardLayout_de_CH.h"

File myFile;
const char* DEFAULT_FILE_NAME = "script.txt";

void setup() {
  pinMode(13, OUTPUT);

  if (!SD.begin(4)) {
    while(1) {
      digitalWrite(13, HIGH);
      delay(500);
      digitalWrite(13, LOW);
      delay(500);
    }
  }
  
  myFile = SD.open(DEFAULT_FILE_NAME);
  if (myFile) {
    Keyboard.begin(KeyboardLayout_de_CH);
    
    char line[256];
    int line_index = 0;
    while (myFile.available()) {
      char m = myFile.read();
      if (m == '\n'){
        line[line_index] = '\0';
        Line(line);
        line_index = 0;
      }
      else if((int) m != 13)
      {
        if (line_index < 255) {
          line[line_index++] = m;
        }
      }
    }
    line[line_index] = '\0';
    if (line_index > 0) {
      Line(line);
    }
    
    myFile.close();
  } else {
    while(1) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
  }

  Keyboard.end();
}

void Line(char* l)
{
  if (strlen(l) == 0) return;

  char* space_ptr = strchr(l, ' ');

  if (space_ptr == NULL)
  {
    Press(l);
  }
  else if (strncmp(l, "STRING ", 7) == 0)
  {
    char* str = l + 7;
    while (*str) {
      if (*str == (char)0xC3) {
        str++;
        if (!*str) break;
        if (*str == (char)0xA7) { // ç
          Keyboard.print((char)0x11);
        }
      } else if (*str == (char)0xC2) {
        str++;
        if (!*str) break;
        if (*str == (char)0xB0) { // °
          Keyboard.print((char)0x12);
        } else if (*str == (char)0xA2) { // ¢
          Keyboard.print((char)0x13);
        } else if (*str == (char)0xA8) { // ¨
          Keyboard.print((char)0x14);
          Keyboard.releaseAll();
          delay(5);
          Keyboard.print(' ');
        } else if (*str == (char)0xB4) { // ´
          Keyboard.print((char)0x15);
          Keyboard.releaseAll();
          delay(5);
          Keyboard.print(' ');
        }
      } else {
        Keyboard.print(*str);
        if (*str == '~' || *str == '^' || *str == '`') {
          Keyboard.releaseAll();
          delay(5);
          Keyboard.print(' ');
        }
      }
      str++;
    }
  }
  else if (strncmp(l, "DELAY ", 6) == 0)
  {
    int delaytime = atoi(l + 6);
    delay(delaytime);
  }
  else if(strncmp(l, "REM ", 4) == 0 || strcmp(l, "REM") == 0){}
  else
  {
      char* token = strtok(l, " ");
      while (token != NULL) {
          Press(token);
          token = strtok(NULL, " ");
          if (token != NULL) {
              delay(5);
          }
      }
      delay(5);
  }

  Keyboard.releaseAll();
}


void Press(char* b)
{
  if(strlen(b) == 1)
  {
    char c = b[0];
    Keyboard.press(c);
  }
  else if (strcmp(b, "ENTER") == 0)
  {
    Keyboard.press(KEY_RETURN);
  }
  else if (strcmp(b, "CTRL") == 0)
  {
    Keyboard.press(KEY_LEFT_CTRL);
  }
  else if (strcmp(b, "SHIFT") == 0)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
  }
  else if (strcmp(b, "ALT") == 0)
  {
    Keyboard.press(KEY_LEFT_ALT);
  }
  else if (strcmp(b, "GUI") == 0)
  {
    Keyboard.press(KEY_LEFT_GUI);
  }
  else if (strcmp(b, "UP") == 0 || strcmp(b, "UPARROW") == 0)
  {
    Keyboard.press(KEY_UP_ARROW);
  }
  else if (strcmp(b, "DOWN") == 0 || strcmp(b, "DOWNARROW") == 0)
  {
    Keyboard.press(KEY_DOWN_ARROW);
  }
  else if (strcmp(b, "LEFT") == 0 || strcmp(b, "LEFTARROW") == 0)
  {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  else if (strcmp(b, "RIGHT") == 0 || strcmp(b, "RIGHTARROW") == 0)
  {
    Keyboard.press(KEY_RIGHT_ARROW);
  }
  else if (strcmp(b, "DELETE") == 0)
  {
    Keyboard.press(KEY_DELETE);
  }
  else if (strcmp(b, "PAGEUP") == 0)
  {
    Keyboard.press(KEY_PAGE_UP);
  }
  else if (strcmp(b, "PAGEDOWN") == 0)
  {
    Keyboard.press(KEY_PAGE_DOWN);
  }
  else if (strcmp(b, "HOME") == 0)
  {
    Keyboard.press(KEY_HOME);
  }
  else if (strcmp(b, "ESC") == 0)
  {
    Keyboard.press(KEY_ESC);
  }
  else if (strcmp(b, "INSERT") == 0)
  {
    Keyboard.press(KEY_INSERT);
  }
  else if (strcmp(b, "TAB") == 0)
  {
    Keyboard.press(KEY_TAB);
  }
  else if (strcmp(b, "END") == 0)
  {
    Keyboard.press(KEY_END);
  }
  else if (strcmp(b, "CAPSLOCK") == 0)
  {
    Keyboard.press(KEY_CAPS_LOCK);
  }
  else if (strcmp(b, "F1") == 0)
  {
    Keyboard.press(KEY_F1);
  }
  else if (strcmp(b, "F2") == 0)
  {
    Keyboard.press(KEY_F2);
  }
  else if (strcmp(b, "F3") == 0)
  {
    Keyboard.press(KEY_F3);
  }
  else if (strcmp(b, "F4") == 0)
  {
    Keyboard.press(KEY_F4);
  }
  else if (strcmp(b, "F5") == 0)
  {
    Keyboard.press(KEY_F5);
  }
  else if (strcmp(b, "F6") == 0)
  {
    Keyboard.press(KEY_F6);
  }
  else if (strcmp(b, "F7") == 0)
  {
    Keyboard.press(KEY_F7);
  }
  else if (strcmp(b, "F8") == 0)
  {
    Keyboard.press(KEY_F8);
  }
  else if (strcmp(b, "F9") == 0)
  {
    Keyboard.press(KEY_F9);
  }
  else if (strcmp(b, "F10") == 0)
  {
    Keyboard.press(KEY_F10);
  }
  else if (strcmp(b, "F11") == 0)
  {
    Keyboard.press(KEY_F11);
  }
  else if (strcmp(b, "F12") == 0)
  {
    Keyboard.press(KEY_F12);
  }
  else if (strcmp(b, "SPACE") == 0)
  {
    Keyboard.press(' ');
  }
  else
  {
    while(1) {
      for(int i=0; i<3; i++) {
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13, LOW);
        delay(100);
      }
      delay(500);
    }
  }
}

void loop() {
  // nothing happens after setup
}

