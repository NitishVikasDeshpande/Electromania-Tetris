#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "pitches.h"
#define music 15
#define res_score A0
#define receive_score A1
#define k 1.0 //15.876
#define startButton 17
#define pauseButton 16
#define left A2
#define right A3
#define down A4
#define acw A5
#define cwc A6
#define reset 19
#define ce 20
#define dc 21
#define din 22
#define sclk 23

//notes in the melody:

unsigned short melody1[] = {
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2,
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5,
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5,
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0
};

unsigned short melody2[] = {
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3,
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3,
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3,
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3,
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2,
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5,
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5,
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0
};

//note durations: 4 = quarter note, 8 = eighth note, etc

unsigned short noteDurations1[] = {
  8, 8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 4, 8, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4
};

unsigned short noteDurations2[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 4, 8, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4
};

int score = 0;
int level = score / 3 + 1;
int scoreflag = 0;
int pointflag = 0;
int prevPause;

Adafruit_PCD8544 nokia = Adafruit_PCD8544(sclk, din, dc, ce, reset);// 19 reset 23 sclk 21 dc 22 din 20 ce

void setup() {
  nokia.begin();
  nokia.setContrast(50);
  nokia.clearDisplay();
  pinMode(res_score, INPUT);
  pinMode(receive_score, INPUT);
  pinMode(startButton,INPUT);
  pinMode(pauseButton,INPUT);
  pinMode(acw,INPUT);
  pinMode(cwc,INPUT);
  pinMode(down,INPUT);
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  pinMode(music, OUTPUT);
  digitalWrite(music, LOW);
  //startGame();
  digitalWrite(music, LOW);
  prevPause = digitalRead(pauseButton);
}
void point()
{
tone(music,k*NOTE_B4, 70/k);
delay(70/k);
tone(music,k*NOTE_C4, 70/k);
delay(70/k);
noTone(music);
}
void pause()
{
  tone(music, k * NOTE_E5, 70 / k);
  delay(100 / k);
  noTone(music);
  tone(music, k * NOTE_E5, 70 / k);
  delay(100);
  noTone(music);
}
void mov()
{
  tone(music, k * NOTE_A4, 120 / k);
  delay((7 * 120 / 6) / k);
  noTone(music);
  delay((7 * 240 / 6) / k);
  if (digitalRead(res_score) == HIGH) {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 20);
    nokia.print("  GAME  OVER  ");
    nokia.display();
    nokia.clearDisplay();
    startGame();
    score = 0;
    level = 1;
  }
  if (digitalRead(receive_score) == HIGH && scoreflag == 0) {
    score++;
    scoreflag = 1;
    if (score % 3 != 0) {
      point();
    }
    else
    {
      startGame();
    }
  }
  else if (digitalRead(receive_score) == LOW) {
    scoreflag = 0;
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause != digitalRead(pauseButton);
    pause();
  }
  while (!digitalRead(pauseButton) && digitalRead(startButton))
  {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 0);
    nokia.print("\n Press  PAUSE    button to      resume.");
    nokia.display();
    nokia.clearDisplay();
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause = digitalRead(pauseButton);
    pause();
  }
}
void movdown() {
  tone(music, k * NOTE_A4, 120 / k);
  delay((7 * 60 / 6) / k);
  noTone(music);
  delay((7 * 30 / 6) / k); level = score / 3 + 1;
  if (digitalRead(res_score) == HIGH) {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 20);
    nokia.print("  GAME  OVER  ");
    nokia.display();
    nokia.clearDisplay();
    startGame();
    score = 0;
    level = 1;
  }
  if (digitalRead(receive_score) == HIGH && scoreflag == 0) {
    score++;
    scoreflag = 1;
    if (score % 3 != 0) {
      point();
    }
    else
    {
      startGame();
    }
  }
  else if (digitalRead(receive_score) == LOW) {
    scoreflag = 0;
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause != digitalRead(pauseButton);
    pause();
  }
  while (!digitalRead(pauseButton) && digitalRead(startButton))
  {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 0);
    nokia.print("\n Press  PAUSE    button to      resume.");
    nokia.display();
    nokia.clearDisplay();
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause = digitalRead(pauseButton);
    pause();
  }
  if (digitalRead(acw) || digitalRead(cwc))rotate();
  else if (digitalRead(left) || digitalRead(right))mov();
}
void rotate()
{
  tone(music, k * NOTE_A4, 80 / k);
  delay((5 * 80 / 4) / k);
  tone(music, k * NOTE_B4, 80 / k);
  delay((5 * 80 / 4) / k);
  noTone(music);
  delay((5 * 160 / 4) / k);
  if (digitalRead(res_score) == HIGH) {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 20);
    nokia.print("  GAME  OVER  ");
    nokia.display();
    nokia.clearDisplay();
    startGame();
    score = 0;
    level = 1;
  }
  if (digitalRead(receive_score) == HIGH && scoreflag == 0) {
    score++;
    scoreflag = 1;
    if (score % 3 != 0) {
      point();
    }
    else
    {
      startGame();
    }
  }
  else if (digitalRead(receive_score) == LOW) {
    scoreflag = 0;
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause != digitalRead(pauseButton);
    pause();
  }
  while (!digitalRead(pauseButton) && digitalRead(startButton))
  {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 0);
    nokia.print("\n Press  PAUSE    button to      resume.");
    nokia.display();
    nokia.clearDisplay();
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause = digitalRead(pauseButton);
    pause();
  }
}
void loop() {
  level = score / 3 + 1;
  nokia.setTextColor(BLACK, WHITE);
  nokia.setRotation(2);
  nokia.setCursor(20, 13);
  nokia.print("SCORE: ");
  nokia.println(10 * score);
  nokia.println("");
  nokia.setCursor(20, 27);
  nokia.print("LEVEL: ");
  nokia.println(level);
  nokia.println("");
  nokia.display();
  nokia.clearDisplay();
  if (digitalRead(res_score) == HIGH) {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 20);
    nokia.print("  GAME  OVER  ");
    nokia.display();
    nokia.clearDisplay();
    startGame();
    score = 0;
    level = 1;
  }
  if (digitalRead(receive_score) == HIGH && scoreflag == 0) {
    score++;
    scoreflag = 1;
    if (score % 3 != 0) {
      point();
    }
    else
    {
      startGame();
    }
  }
  else if (digitalRead(receive_score) == LOW) {
    scoreflag = 0;
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause != digitalRead(pauseButton);
    pause();
  }
  while (!digitalRead(pauseButton) && digitalRead(startButton))
  {
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 0);
    nokia.print("\n Press  PAUSE    button to      resume.");
    nokia.display();
    nokia.clearDisplay();
  }
  if (digitalRead(pauseButton) != prevPause)
  {
    prevPause = digitalRead(pauseButton);
    pause();
  }
  if (digitalRead(acw) || digitalRead(cwc))rotate();
  else if (digitalRead(left) || digitalRead(right))mov();
  while (digitalRead(down)) {
    movdown();
  }
  int c;
  while (!digitalRead(startButton))
  { c = 0; score = 0;
    level = 1;
    nokia.setTextColor(BLACK, WHITE);
    nokia.setRotation(2);
    nokia.setCursor(0, 0);
    nokia.print("\n Press  START    button to      play!!!");
    nokia.display();
    nokia.clearDisplay();
    while (c < 2)
    {
      for ( int thisNote = 0; (!digitalRead(startButton)) && (thisNote < sizeof(melody1) / sizeof(melody1[0])); thisNote++)
      { /*
          to calculate the note duration, take one second divided by the note type.
          e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        */
        int noteDuration = 1.3 * (1000 / noteDurations1[thisNote]) / k;
        tone(music, k * melody1[thisNote], noteDuration);
        /*
          to distinguish the notes, set a minimum time between them.
          the note's duration + 30% seems to work well:
        */
        int pauseBetweenNotes = noteDuration * 1.3;
        delay(pauseBetweenNotes);
        noTone(music); //stop the tone playing:
      }
      c++;
    }
    for ( int thisNote = 0; (!digitalRead(startButton)) && (thisNote < sizeof(melody2) / sizeof(melody2[0])); thisNote++)
    { /*
        to calculate the note duration, take one second divided by the note type.
        e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      */
      int noteDuration = 1.3 * (1000 / noteDurations2[thisNote]) / k;
      tone(music, k * melody2[thisNote], noteDuration);
      /*
        to distinguish the notes, set a minimum time between them.
        the note's duration + 30% seems to work well:
      */
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(music); //stop the tone playing:
    }
    if (digitalRead(startButton))startGame();
    digitalWrite(11, LOW);
  }
}
void startGame()
{
  tone(music, k * NOTE_E4, 100 / k);
  delay(100 / k);
  tone(music, k * NOTE_FS4, 100 / k);
  delay(100 / k);
  tone(music, k * NOTE_A4, 100 / k);
  delay(100 / k);
  tone(music, k * NOTE_D5, 100 / k);
  delay(100 / k);
  noTone(music);
}
