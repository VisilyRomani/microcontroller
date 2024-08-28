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
struct_message myData;

bool locked = false;

byte nuidPICC[4];

String current_card;

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

bool PICC_IsAnyCardPresent() {
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);
  
  // Reset baud rates
  mfrc522.PCD_WriteRegister(mfrc522.TxModeReg, 0x00);
  mfrc522.PCD_WriteRegister(mfrc522.RxModeReg, 0x00);
  // Reset ModWidthReg
  mfrc522.PCD_WriteRegister(mfrc522.ModWidthReg, 0x26);
  
  MFRC522::StatusCode result = mfrc522.PICC_WakeupA(bufferATQA, &bufferSize);
  return (result == MFRC522::STATUS_OK || result == MFRC522::STATUS_COLLISION);
} 

void loop()
{

Serial.println(current_card);
   bool cardPresent = PICC_IsAnyCardPresent();
  
  // Reset the loop if no card was locked an no card is present.
  // This saves the select process when no card is found.
  if (! locked && ! cardPresent)
    return;

  // When a card is present (locked) the rest ahead is intensive (constantly checking if still present).
  // Consider including code for checking only at time intervals.

  // Ask for the locked card (if rfid.uid.size > 0) or for any card if none was locked.
  // (Even if there was some error in the wake up procedure, attempt to contact the locked card.
  // This serves as a double-check to confirm removals.)
  // If a card was locked and now is removed, other cards will not be selected until next loop,
  // after rfid.uid.size has been set to 0.
  MFRC522::StatusCode result = mfrc522.PICC_Select(&mfrc522.uid,8*mfrc522.uid.size);

  if(!locked && result == MFRC522::STATUS_OK)
  {
    locked=true;
    // Action on card detection.
    Serial.print(F("locked! NUID tag: "));
    current_card = byteArrayToHexString(mfrc522.uid.uidByte, mfrc522.uid.size);
    // printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
  } else if(locked && result != MFRC522::STATUS_OK)
  {
    locked=false;
    mfrc522.uid.size = 0;
    // Action on card removal.
    current_card = "no card";
    
    Serial.print(F("unlocked! Reason for unlocking: "));
    Serial.println(mfrc522.GetStatusCodeName(result));
  } else if(!locked && result != MFRC522::STATUS_OK)
  {
    // Clear locked card data just in case some data was retrieved in the select procedure
    // but an error prevented locking.
    mfrc522.uid.size = 0;
  }

  mfrc522.PICC_HaltA();
  // Serial.println(current_card);
  // if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  // {
  //   current_card = byteArrayToHexString(mfrc522.uid.uidByte, mfrc522.uid.size);
  // }

  // if(is_card && !anyCardPresent()){
  //   current_card = "";
  // }

  // Serial.println(byteArrayToHexString(mfrc522.uid.uidByte, mfrc522.uid.size));

  // if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  // {
  //   Serial.println("Card Added");

  //   if (mfrc522.uid.uidByte[0] != nuidPICC[0] ||
  //       mfrc522.uid.uidByte[1] != nuidPICC[1] ||
  //       mfrc522.uid.uidByte[2] != nuidPICC[2] ||
  //       mfrc522.uid.uidByte[3] != nuidPICC[3])
  //   {
  //     Serial.println(F("A new card has been detected."));

  //     // Store NUID into nuidPICC array
  //     for (byte i = 0; i < 4; i++)
  //     {
  //       nuidPICC[i] = mfrc522.uid.uidByte[i];
  //     }
  //     current_card = byteArrayToHexString(mfrc522.uid.uidByte, mfrc522.uid.size);
  //   }else{
  //     Serial.println("Card Prev READ");
  //   }

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  //   // card_uid = byteArrayToHexString(mfrc522.uid.uidByte, mfrc522.uid.size);
  //   // Serial.println(card_uid);
  // }else{
  //   current_card = byteArrayToHexString(mfrc522.uid.uidByte, mfrc522.uid.size);

  // }

  // Serial.println(current_card);

  delay(1000);
}
