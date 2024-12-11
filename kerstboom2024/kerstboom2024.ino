// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <tinyNeoPixel.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <EEPROM.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define LED1           1
#define LED2           3
#define LED3           12
#define KNOP           5

#define ROOD1     pixels1.Color(10, 0, 0)
#define ROOD2     pixels1.Color(8, 0, 0)
#define PAARS1    pixels1.Color(12, 0, 4)
#define PAARS2    pixels1.Color(10, 0, 3)
#define BLAUW1    pixels1.Color(0, 0, 10)
#define BLAUW2    pixels1.Color(0, 0, 8)
#define GROEN1    pixels1.Color(0, 10, 0)
#define GROEN2    pixels1.Color(0, 8, 0)
#define GEEL1     pixels1.Color(10, 10, 0)
#define GEEL2     pixels1.Color(8, 8, 0)
#define GOUD2     pixels1.Color(10, 7, 0)
#define ORANJE1   pixels1.Color(12, 4, 0)
#define ORANJE2   pixels1.Color(10, 3, 0)
#define UIT       pixels1.Color(0, 0, 0)
#define WIT       pixels1.Color(7, 7, 7)

// // How many NeoPixels are attached to the Arduino?
// #define NUMPIXELS      3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
tinyNeoPixel pixels1 = tinyNeoPixel(7, LED1, NEO_GRB + NEO_KHZ800);
tinyNeoPixel pixels2 = tinyNeoPixel(7, LED2, NEO_GRB + NEO_KHZ800);
tinyNeoPixel pixels3 = tinyNeoPixel(6, LED3, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second

byte prog;
byte fase;
byte faseb;
byte fasec;
bool actief;

void setup() {
  // cbi(ADC, ADEN);
  wdt_reset();
  wdt_enable(10);
  // WDTCSR != (1<<WDCE) | (1<<WDE);

  pixels1.begin(); // This initializes the NeoPixel library.
  pixels2.begin(); // This initializes the NeoPixel library.
  pixels3.begin(); // This initializes the NeoPixel library.
  pinMode(KNOP, INPUT);

  lees_waarden();

}

void lees_waarden(){
  prog = EEPROM.read(0);
  if (prog == 255) {
    prog = 0;
  }
  fase = EEPROM.read(1);
  if (fase == 255) {
    fase = 0;
  }
  faseb = EEPROM.read(2);
  if (faseb == 255) {
    faseb = 1;
  }
  fasec = EEPROM.read(3);
  if (fasec == 255) {
    fasec = 1;
  }
  if (EEPROM.read(4) == 1){
    actief = true;
  } else if (EEPROM.read(4) == 0){
    actief = false;
  } else 
  actief = true;
}


void backup_waarden(){
  EEPROM.write(0, prog);
  EEPROM.write(1, fase);
  EEPROM.write(2, faseb);
  EEPROM.write(3, fasec);
  if (actief){
    EEPROM.write(4, 1);
  } else {
    EEPROM.write(4, 9);
  }
}


void loop() {
  wdt_reset();
  if (digitalRead(KNOP) == LOW) {
    knop_input();
  }

  if (actief) {
    if (prog == 0) {
      prog0();
    } else if (prog == 1) {
      if (fasec > 10) {
        fasec = 1;
        prog1();
      } else {
        fasec ++;
      }
    } else if (prog == 2) {
      prog2();
    } else {
      prog3p();
    }
    wdt_reset();
    backup_waarden();
    // wdt_reset();
    // wdt_enable(2);
    // set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // sleep_enable();
    // sleep_cpu();
    delay(delayval);
  } else {
    slapen();
  }
}

void knop_input() {
  
  noInterrupts();
  int druk_teller = 0;
  int druk_timer = 300;
  while ((digitalRead(KNOP) == LOW) & (druk_teller < druk_timer)) {
    druk_teller ++;
    delay(50);
  }

  if ((druk_teller >= druk_timer) & (actief)) {
    alles_uit();
    prog --;
    actief = false;
  } else if (actief == false) {
    actief = true;
  }

  if (actief) {
    volgende();
  }
}

void alles_uit(){
  for (int i = 0; i < 7; i++) {
    pixels1.clear();
    pixels2.clear();
    pixels3.clear();
  }
  pixels1.show();
  pixels2.show();
  pixels3.show();
}

void slapen(){
  while (digitalRead(KNOP) == LOW) {
    delay(50);
  }
  delay(100);

  noInterrupts();
  attachInterrupt(digitalPinToInterrupt(KNOP), opstaan, LOW);
  interrupts();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
    
    
}
void opstaan(){
  
  noInterrupts();
  
  sleep_disable();
  actief = true;
  while (digitalRead(KNOP) == LOW) {
    delay(50);
  }
  delay(50);

  attachInterrupt(digitalPinToInterrupt(KNOP), knop_input, LOW);
  
  loop();
}

void volgende(){
  prog ++;
  fase = 0;
  faseb = 1;
  fasec = 1;
  if (prog > 6){
    prog = 0;
    alles_uit();
    actief = false;
  }
  if (prog == 3) {
    // delayval = 50;
    for (int i = 0; i < 7; i++) {
      pixels2.setPixelColor(i, GROEN2);
      pixels3.setPixelColor(i, GROEN2);
    }
    pixels2.show();
    pixels3.show();
  } else if (prog == 4) {
    // delayval = 50;
    for (int i = 0; i < 7; i++) {
      pixels2.setPixelColor(i, ORANJE2);
      pixels3.setPixelColor(i, ORANJE2);
    }
    pixels2.show();
    pixels3.show();
  } else if (prog == 5) {
    // delayval = 50;
    for (int i = 0; i < 7; i++) {
      pixels2.setPixelColor(i, ROOD2);
      pixels3.setPixelColor(i, ROOD2);
    }
    pixels2.show();
    pixels3.show();
  } else if (prog == 6) {
    // delayval = 50;
    for (int i = 0; i < 7; i++) {
      pixels2.setPixelColor(i, GOUD2);
      pixels3.setPixelColor(i, GOUD2);
    }
    pixels2.show();
    pixels3.show();
  }
}

uint32_t random_kleur(){
  int i = random(1, 6);
  if (i == 1){
    return ROOD2;
  } else if (i == 2){
    return PAARS2;
  } else if (i == 3){
    return BLAUW2;
  } else if (i == 4){
    return GROEN2;
  } else if (i == 5){
    return GEEL2;
  } else if (i == 6){
    return GOUD2;
  } else if (i == 7){
    return ORANJE2;
  }
  return WIT;
}

void kaarsjes(){
  for (int i = 0; i < 7; i++){
    int kl = random(3, 8);
    pixels1.setPixelColor(i, kl, kl, 1);
  }
  pixels1.show();
}

void prog0l(uint32_t kleur1, uint32_t kleur2, uint32_t kleur3, int vlammetjes) {
  prog0l(kleur1, kleur2, kleur3, faseb, false);
}
void prog0l(uint32_t kleur1, uint32_t kleur2, uint32_t kleur3, int faseb, bool vlammetjes){
  if (faseb == 1) {
    pixels3.setPixelColor(1, kleur3);
  } else if (faseb == 2){
    pixels3.setPixelColor(0, kleur3);
    pixels3.setPixelColor(2, kleur3);
    pixels3.setPixelColor(3, kleur3);
    pixels3.setPixelColor(4, kleur3);
  } else if (faseb == 3){
    for (int i = 0; i < 7; i++) {
      if (i != 3){
        pixels2.setPixelColor(i, kleur2);
      }
    }
    pixels3.setPixelColor(5, kleur3);
  } else if (faseb == 4){
    if (vlammetjes) {
      for (int i = 0; i < 7; i++) {
        if (i != 3){
          pixels1.setPixelColor(i, kleur1);
        }
      }
    }
    pixels2.setPixelColor(3, kleur2);
  } else {
    if (vlammetjes) {
      if (faseb == 5){
        pixels1.setPixelColor(3, kleur1);
      } else if (faseb == 6){
        pixels1.setPixelColor(3, UIT);
      } else if (faseb == 7){
        pixels1.setPixelColor(3, kleur1);
      } else if (faseb == 8){
        pixels1.setPixelColor(3, UIT);
      } else if (faseb == 9){
        pixels1.setPixelColor(3, kleur1);
      } else if (faseb == 10){
        pixels1.setPixelColor(3, UIT);
      } else if (faseb == 11){
        pixels1.setPixelColor(3, kleur1);
      } else if (faseb == 12){
        pixels1.setPixelColor(3, UIT);
      }
    }
  }
  if (vlammetjes) {
    pixels1.show();
  }
  pixels2.show();
  pixels3.show();
}

void prog0(){
  if (fase == 0) {
    for (int i = 0; i < 7; i++) {
      pixels1.setPixelColor(i, UIT);
      pixels2.setPixelColor(i, UIT);
      pixels3.setPixelColor(i, UIT);
      pixels1.show();
      pixels2.show();
      pixels3.show();
    }
    fasec = 1;
    fase = 1;
  } else if (fasec > 4) {
    fasec = 1;
    if (fase == 1) {
      prog0l(ROOD1, ROOD2, ROOD2, faseb);
    } else if (fase == 2) {
      prog0l(PAARS1, PAARS2, PAARS2, faseb);
    } else if (fase == 3) {
      prog0l(BLAUW1, BLAUW2, BLAUW2, faseb);
    } else if (fase == 4) {
      prog0l(GROEN1, GROEN2, GROEN2, faseb);
    } else if (fase == 5) {
      prog0l(GEEL1, GEEL2, GEEL2, faseb);
    } else if (fase == 6) {
      prog0l(ORANJE1, ORANJE2, ORANJE2, faseb);
    }
    faseb ++;
    if (faseb >= 13){
      faseb = 1;
      fase ++;
    }
  } else {
    fasec ++;
  }
  
  if (fase > 6) {
    fase = 1;
  }

  kaarsjes();
}

void prog1(){

  if (fase == 0) {
    faseb = 1;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
  } else if (fase == 1) {
    faseb = 2;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
  } else if (fase == 2) {
    faseb = 3;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
    faseb = 1;
    prog0l(PAARS1, PAARS2, PAARS2, faseb, true);
  } else if (fase == 3) {
    faseb = 4;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
    faseb = 2;
    prog0l(PAARS1, PAARS2, PAARS2, faseb, true);
  } else if (fase == 4) {
    faseb = 5;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
    faseb = 3;
    prog0l(PAARS1, PAARS2, PAARS2, faseb, true);
    faseb = 1;
    prog0l(BLAUW1, BLAUW2, BLAUW2, faseb, true);
  } else if (fase == 5) {
    faseb = 4;
    prog0l(PAARS1, PAARS2, PAARS2, faseb, true);
    faseb = 2;
    prog0l(BLAUW1, BLAUW2, BLAUW2, faseb, true);
  } else if (fase == 6) {
    faseb = 5;
    prog0l(PAARS1, PAARS2, PAARS2, faseb, true);
    faseb = 3;
    prog0l(BLAUW1, BLAUW2, BLAUW2, faseb, true);
    faseb = 1;
    prog0l(GROEN1, GROEN2, GROEN2, faseb, true);
  } else if (fase == 7) {
    faseb = 4;
    prog0l(BLAUW1, BLAUW2, BLAUW2, faseb, true);
    faseb = 2;
    prog0l(GROEN1, GROEN2, GROEN2, faseb, true);
  } else if (fase == 8) {
    faseb = 5;
    prog0l(BLAUW1, BLAUW2, BLAUW2, faseb, true);
    faseb = 3;
    prog0l(GROEN1, GROEN2, GROEN2, faseb, true);
    faseb = 1;
    prog0l(GEEL1, GEEL2, GEEL2, faseb, true);
  } else if (fase == 9) {
    faseb = 4;
    prog0l(GROEN1, GROEN2, GROEN2, faseb, true);
    faseb = 2;
    prog0l(GEEL1, GEEL2, GEEL2, faseb, true);
  } else if (fase == 10) {
    faseb = 5;
    prog0l(GROEN1, GROEN2, GROEN2, faseb, true);
    faseb = 3;
    prog0l(GEEL1, GEEL2, GEEL2, faseb, true);
    faseb = 1;
    prog0l(ORANJE1, ORANJE2, ORANJE2, faseb, true);
  } else if (fase == 11) {
    faseb = 4;
    prog0l(GEEL1, GEEL2, GEEL2, faseb, true);
    faseb = 2;
    prog0l(ORANJE1, ORANJE2, ORANJE2, faseb, true);
  } else if (fase == 12) {
    faseb = 5;
    prog0l(GEEL1, GEEL2, GEEL2, faseb, true);
    faseb = 3;
    prog0l(ORANJE1, ORANJE2, ORANJE2, faseb, true);
    faseb = 1;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
  } else if (fase == 13) {
    faseb = 4;
    prog0l(ORANJE1, ORANJE2, ORANJE2, faseb, true);
    faseb = 2;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
  } else if (fase == 14) {
    faseb = 5;
    prog0l(ORANJE1, ORANJE2, ORANJE2, faseb, true);
    faseb = 3;
    prog0l(ROOD1, ROOD2, ROOD2, faseb, true);
    faseb = 1;
    prog0l(PAARS1, PAARS2, PAARS2, faseb, true);
  }

  // pixels1.show();
  // pixels2.show();
  // pixels3.show();
  
  fase ++;
  if (fase > 14){
    fase = 3;
  }
}

void prog2() {
  if (fase == 0) {
    for (int i = 0; i < 7; i++) {
      pixels2.setPixelColor(i, random_kleur());
    }
    for (int i = 0; i < 6; i++) {
      pixels3.setPixelColor(i, random_kleur());
    }  
    pixels2.show();
    pixels3.show();
  } else if (fase == 40) {
    fase = -1;
  }
  // pixels2.setPixelColor(0, ROOD2);
  // pixels2.setPixelColor(1, GOUD2);
  // pixels2.setPixelColor(2, ROOD2);
  // pixels2.setPixelColor(3, GOUD2);
  // pixels2.setPixelColor(4, ROOD2);
  // pixels2.setPixelColor(5, GOUD2);
  // pixels2.setPixelColor(6, ROOD2);
  // pixels3.setPixelColor(0, GOUD2);
  // pixels3.setPixelColor(1, ROOD2);
  // pixels3.setPixelColor(2, GOUD2);
  // pixels3.setPixelColor(3, ROOD2);
  // pixels3.setPixelColor(4, ROOD2);
  // pixels3.setPixelColor(5, GOUD2);
  kaarsjes();
  fase ++;
}

void prog3p(){
  kaarsjes();
}