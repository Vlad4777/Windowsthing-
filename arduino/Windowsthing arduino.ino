#include <SPI.h>
#include <Wire.h>
#include<U8g2lib.h>
#include<HID-Project.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

//--config--
const int8_t swPin  = 4;
const int8_t pinA = 6;
const int8_t pinB = 7; //do not use other pins, encoder wont work reliably

bool rusEnable = true; //enable russian text. interface will look a bit worse

volatile int  encoderChange = 0;
int lastEncoderChange = 0;
int currentEncoder = 0; 

bool buttonState = HIGH;
bool lastButtonState = HIGH;


String title = "";
String artist = "";
String playback = "";
String timeline = "";
String previousTitle = "";

String cutArtist = "";

int16_t x;          

bool containsCyrillic(const String& s) {
    const char* p = s.c_str();
    while (*p) {
        uint8_t b1 = (uint8_t)p[0];
        uint8_t b2 = (uint8_t)p[1];

        // А–Я
        if (b1 == 0xD0 && b2 >= 0x90 && b2 <= 0xAF) return true;
        // а–я
        if (b1 == 0xD0 && b2 >= 0xB0 && b2 <= 0xBF) return true;
        if (b1 == 0xD1 && b2 >= 0x80 && b2 <= 0x8F) return true;

        p += (b1 < 0x80) ? 1 : 2; 
    }
    return false;
}

void setup() {
  pinMode(swPin, INPUT_PULLUP);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinB), encoderISR, CHANGE);
  Serial.begin(9600);
  Serial.setTimeout(200);  
  Consumer.begin();
  u8g2.begin();
  x = 0;
}


void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');

    int index1 = line.indexOf('|');
    int index2 = line.indexOf('|', index1 + 1);
    int index3 = line.indexOf('|', index2 + 1);


    if (index1 > 0 && index2 > index1 && index3 > index2) {
      title = line.substring(0, index1);
      artist = line.substring(index1 + 1, index2);
      playback = line.substring(index2 + 1, index3);
      timeline = line.substring(index3 + 1); 
      
    }else{
      u8g2.setFontMode(1);
      u8g2.setBitmapMode(1);
      u8g2.setFont(u8g2_font_profont17_tr);
      u8g2.drawStr(10, 21, ":( COM Error");
      u8g2.sendBuffer();
    }
  }

  buttonState = digitalRead(swPin);

  noInterrupts();
  currentEncoder = encoderChange;
  interrupts(); 

  
  if (currentEncoder < lastEncoderChange){
    if (buttonState == LOW){
      Consumer.write(MEDIA_PREVIOUS); 
    }else{
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
  }else if (currentEncoder > lastEncoderChange){
    if(buttonState == LOW){
      Consumer.write(MEDIA_NEXT);
    }else{
      Consumer.write(MEDIA_VOLUME_UP); 
    }
  }else if (buttonState == LOW && lastButtonState == HIGH){
      Consumer.write(MEDIA_PLAY_PAUSE);
   }
  
  
  lastButtonState = buttonState;
  lastEncoderChange = currentEncoder; 
  
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  if (rusEnable && containsCyrillic(title)) {
    u8g2.setFont(u8g2_font_unifont_t_cyrillic);
  } else {
    u8g2.setFont(u8g2_font_t0_17b_tr);
  }

  if (u8g2.getUTF8Width(title.c_str()) > 110){
    u8g2.drawUTF8(x, 13, title.c_str());
    x--;
    if (x < -u8g2.getUTF8Width(title.c_str()) or title != previousTitle) {
      x = 0;
    }
   }else{
    u8g2.drawUTF8(0, 13, title.c_str());
    }
    
   if (rusEnable && containsCyrillic(artist)) {
    u8g2.setFont(u8g2_font_unifont_t_cyrillic);
   } else {
    u8g2.setFont(u8g2_font_t0_11_tr);
   }
  
  if (artist.length() > 9){
    cutArtist = artist;
    cutArtist.remove(9);
    cutArtist = cutArtist + "..";
    u8g2.drawUTF8(0, 28, cutArtist.c_str());
  }else{
    u8g2.drawUTF8(0, 28, artist.c_str());
  }
    
  
  
  u8g2.setDrawColor(0); 
  u8g2.drawBox(110, 0, 18, 17); 
  u8g2.drawBox(65, 16, 63, 17);
  u8g2.setDrawColor(1);

  if (rusEnable == true){
    if (containsCyrillic(artist) == true){
      u8g2.setFont(u8g2_font_t0_11_tr);
    }
  }
  
  u8g2.drawStr(67, 28, timeline.c_str());

  if (playback == "playing"){
    u8g2.drawBox(116, 2, 2, 11);
    u8g2.drawBox(121, 2, 2, 11);
  }
  else if (playback == "paused"){
    u8g2.drawLine(115, 3, 115, 12);
    u8g2.drawLine(115, 3, 122, 7);
    u8g2.drawLine(115, 12, 123, 8);

  }
  previousTitle = title;
  u8g2.sendBuffer();
}

void encoderISR() {
  if (digitalRead(pinA) == digitalRead(pinB))
    encoderChange++;
  else
    encoderChange--;
}
