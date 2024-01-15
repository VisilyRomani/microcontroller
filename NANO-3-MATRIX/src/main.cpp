#include <Arduino.h>

 byte led_pins[] = {PD3,PD4,PD5,PD6};
 int led_count = 4;
void play_light();

int ledArray[4][2] = {
    {PD3},
    {PD4,PD5},
    {PD5,PD6},
    {PD4}};



void setup()
{
  Serial.begin(9600);
  for (uint8_t i = 0; i < led_count; i++)
  {
   pinMode(led_pins[i], OUTPUT); 
  }
  
}

void loop()
{
  int value = analogRead(A0);
  delay(100);
  // put your main code here, to run repeatedly:
  if (value > 800 && value < 1000)
  {
    play_light();
  }
  else if (value > 750 && value < 800)
  {
  }
  else if (value > 650 && value < 750)
  {
  }
  else if (value > 480 && value < 650)
  {
  }
  else if (value < 480)
  {
  }
}


void play_light()
{
  for (uint8_t i = 0; i < 4; i++)
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