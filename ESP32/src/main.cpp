#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <MFRC522.h>
#include <SPI.h>

// THIS is on board 0xa0, 0xdd, 0x6c, 0x04, 0x38, 0x38

#define RST_PIN 16
#define MOSI 18
#define MISO 17
#define SCK 19
#define SS 21
byte pins[] = {RST_PIN, MOSI, MISO, SCK, SS};

MFRC522 mfrc522;
typedef struct struct_message
{
  bool isOpen;
} struct_message;

String card_uid;

String card_serial;
struct_message myData;
esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0xa0, 0xdd, 0x6c, 0x04, 0x38, 0x38};

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

String byteArrayToHexString(byte *buffer, byte bufferSize)
{
  String result = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    if (buffer[i] < 0x10)
    {
      result += " 0";
    }
    else
    {
      result += " ";
    }
    result += String(buffer[i], HEX);
  }
  return result;
}

char **read_string_array(byte *buffer, byte bufferSize)
{
  // Allocate memory for the array of strings
  char **result = new char *[bufferSize];
  for (byte i = 0; i < bufferSize; i++)
  {
    // Allocate memory for each string (3 characters: " 0x" + null terminator)
    result[i] = new char[4];
    // Format the byte as a string and store it in the array
    sprintf(result[i], "%02X", buffer[i]);
  }
  return result;
}

void verify_card(byte card_id, bool isOpen)
{
  // if (card_id)
  // {
  myData.isOpen = isOpen;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }
  // }
}


void setup()
{
  Serial.begin(115200);

  // Setup rfid
  SPI.begin(SCK, MISO, MOSI, SS); // SCK, MISO, MOSI, SS
  mfrc522.PCD_Init(SS, RST_PIN);

  // Setup ESP Now
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}
void loop()
{

  Serial.println(mfrc522.PICC_ReadCardSerial());
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
      Serial.println("Card Added");

      card_uid = byteArrayToHexString(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println(card_uid);

      // Halt PICC
      mfrc522.PICC_HaltA();
      
      // Stop encryption on PCD
      mfrc522.PCD_StopCrypto1();
      }
    
  
  delay(100);
}
