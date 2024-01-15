#include <Arduino.h>

int ledArray[4][2] = {
    {14},
    {12, 13},
    {15, 14},
    {15}};

void play_light();

void setup()
{
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
}

void loop()
{


}

void play_light() {
  for (uint8_t i = 0; i < 3; i++)
  {
    for (uint8_t j = 0; j < 2; j++)
    {
      digitalWrite(ledArray[i][j], HIGH);
    }

    delay(1000);

    for (uint8_t j = 0; j < 2; j++)
    {
      digitalWrite(ledArray[i][j], LOW);
    }
  }
}