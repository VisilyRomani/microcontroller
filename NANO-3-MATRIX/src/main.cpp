#include <Arduino.h>

byte led_pins[] = {PD3, PD4, PD5, PD6};
int led_count = 4;

void light_pass();
void light_fail();
void play_light();
void check_solution();

int ledArray[4][2] = {
    {PD3},
    {PD4, PD5},
    {PD5, PD6},
    {PD4}};

int button_answer[] = {1, 3, 4, 2};
int button_list[4];
const long led_interval = 1000;
const long button_interval = 500;
unsigned int list_size = 0;
unsigned long current_millis;
unsigned long start_milis;

void setup()
{
  Serial.begin(9600);
  for (uint8_t i = 0; i < led_count; i++)
  {
    pinMode(led_pins[i], OUTPUT);
  }
  start_milis = millis();
}

void loop()
{
  current_millis = millis();
  int value = analogRead(A0);

  if (current_millis - start_milis >= button_interval)
  {

    if (value > 800 && value < 1000)
    {
      play_light();
    }
    else if (value > 750 && value < 800)
    {
      button_list[list_size] = 1;
      list_size = list_size + 1;
    }
    else if (value > 650 && value < 750)
    {
      button_list[list_size] = 2;
      list_size = list_size + 1;
    }
    else if (value > 480 && value < 650)
    {
      button_list[list_size] = 3;
      list_size = list_size + 1;
    }
    else if (value < 480)
    {
      button_list[list_size] = 4;
      list_size = list_size + 1;
    }
    check_solution();
    digitalWrite(led_pins[list_size - 1], HIGH);
  }
}

void check_solution()
{
  if (list_size < 4)
  {
    return;
  }
  else if (list_size > 4)
  {
    light_fail();
  }
  else
  {
    for (size_t i = 0; i < 4; i++)
    {
      Serial.print(button_list[i]);
      Serial.print(" ");
      Serial.println(button_answer[i]);

      if (button_answer[i] != button_list[i])
      {
        light_fail();
        return;
      }
    }
    light_pass();
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

    if (current_millis - start_milis >= led_interval)
    {
      for (uint8_t j = 0; j < 2; j++)
      {
        digitalWrite(ledArray[i][j], LOW);
      }
    }
  }
}

void light_pass()
{
  for (uint8_t i = 0; i < led_count; i++)
  {
    digitalWrite(led_pins[i], HIGH);
  }
  while (1)
  {
  }
}
void light_fail()
{
  for (uint8_t i = 0; i < led_count; i++)
  {
    digitalWrite(led_pins[i], HIGH);
  }
  delay(100);
  for (uint8_t i = 0; i < led_count; i++)
  {
    digitalWrite(led_pins[i], LOW);
  }
  delay(100);

  for (uint8_t i = 0; i < led_count; i++)
  {
    digitalWrite(led_pins[i], HIGH);
  }
  delay(100);
  for (uint8_t i = 0; i < led_count; i++)
  {
    digitalWrite(led_pins[i], LOW);
  }

  list_size = 0;
  memset(button_list, 0, 4);
}