
#include <SPI.h>
#include <MFRC522.h>
#include <array>

#define RST_PIN D8
#define SS_1_PIN D2
#define SS_2_PIN D3
#define READERS 2

MFRC522 mfrc522[READERS]; // Create MFRC522 instance.
byte ssPins[] = {SS_1_PIN, SS_2_PIN};
const String correctId[READERS] = {"155e777e", "a361d495"};
String currentId[READERS];

String
dump_byte_array(byte *buffer, byte bufferSize);
/**
 * Initialize.
 */
void setup()
{
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus

  for (uint8_t reader = 0; reader < READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader No: "));
    Serial.print(reader);
    Serial.print(F("Version: "));
    mfrc522[reader].PCD_DumpVersionToSerial();
    mfrc522[reader].PCD_ClearRegisterBitMask(mfrc522[reader].RFCfgReg, (0x07 << 4));
    mfrc522[reader].PCD_SetRegisterBitMask(mfrc522[reader].RFCfgReg, (0x00 << 4));
  }
}

/**
 * Main loop.
 */
void loop()
{
  boolean isChanged = false;

  for (uint8_t i = 0; i < READERS; i++)
  {
    String rfidData = "";
    mfrc522[i].PCD_Init();

    if (mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial())
    {
      rfidData = dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);
    }

    if (rfidData != currentId[i])
    {
      isChanged = true;
      currentId[i] = rfidData;
    }

    if (isChanged)
    {
      Serial.print("Reader: ");
      Serial.print(i);
      Serial.print(currentId[i]);
      Serial.println(correctId[i]);
      Serial.println(currentId[i].equals(correctId[i]));
    }
  }
  if (std::equal(std::begin(currentId), std::end(currentId), std::begin(correctId), std::end(correctId)))
  {
    Serial.println("Solved");
    // open door
    while (true)
    {
    }
  }
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
String dump_byte_array(byte *buffer, byte bufferSize)
{
  String result = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    result += (buffer[i] < 0x10 ? "0" : "");
    result += String(buffer[i], HEX);
  }
  return result;
}