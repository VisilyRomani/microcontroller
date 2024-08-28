#include <esp_now.h>
#include <WiFi.h>


// A0:DD:6C:04:38:38

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message
{
  bool isOpen;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.isOpen);

  if (myData.isOpen == true)
  {
    digitalWrite(16, LOW);
  }
  else
  {
    digitalWrite(16, HIGH);
  }
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(16, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println(WiFi.macAddress());

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  digitalWrite(16, HIGH);

}

void loop()
{
  // digitalWrite(16, HIGH);
  // Serial.println("HIGH");
  // delay(5000);
  // digitalWrite(16, LOW);
  // Serial.println("LOW");
  // delay(5000);
}