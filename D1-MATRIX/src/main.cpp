#include <Arduino.h>

byte button_pin[] = {5,4,0,2};
const int button_count = 4;

int ledArray[4][2] = {
    {14},
    {12, 13},
    {15, 14},
    {15}};

void play_light();

void setup()
{
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);

  for (uint8_t i = 0; i < button_count; i++)
  {
    pinMode(button_pin[i], INPUT_PULLUP);
  }
}

void loop()
{
  for (uint8_t i = 0; i < button_count; i++)
  {
    int value = digitalRead(button_pin[i]);
    if(value == LOW){
      Serial.println("Pressed: ");
      Serial.println(button_pin[i]);
    }
  }
}

void play_light()
{
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