#include "tiny.h"
#include <Crypto.h>
#include <Curve25519.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

Tiny::ProtoLight proto;
Tiny::Packet<256> packet;
StaticJsonDocument<200> doc;
int codes[50];
uint8_t public_key[32]; //diffie-hellman public key
uint8_t secret_key[32]; //diffie-hellman secret key
uint8_t shared_key[32]; //diffie-hellman shared key
int phase;

uint8_t f[32];
uint8_t k[32];
uint8_t response[32];

uint8_t type;

AES128 aes128;
char input_password[64];
StaticJsonDocument<200> doc;
StaticJsonDocument<200> payload;
// void gen_sec_pub(bool message) {

//   Serial.println((message)?"REQUEST" : "RESPONSE");
//   Curve25519::deffie(public_key, secret_key);

//   packet.clear(); //Clears existing packet content
//   packet.put((uint8_t)((message)?DH_REQUEST : DH_RESPONSE)); //insert packet type as first byte
//   //insert packet data:
//   for (uint8_t i = 0; i < 32; i++){
//     packet.put(public_key[i]);
//   }

//     /* Send packet over UART to other side */
//      proto.write(packet);

// }

// void parse_packet(){

//   memcpy(shared_key, packet.buf()+1, min(sizeof(unint8_t)*packet.size(),sizeof(shared_key)));
// }

// void send_data(char *buf, uint8_t len, PacketType packettype)
// {
//   packet.clear(); //Clears existing packet content
//   packet.put((char)packettype); //insert packet type as first byte
//   //insert packet data:
//   for (uint8_t i = 0; i < len; i++){
//     packet.put(public_key[i]);
//   }

//     /* Send packet over UART to other side */
//      proto.write(packet);
// }

void readJson()
{
  File file = SD.open("database.json");
  deserializeJson(doc, file);
  file.close();
  codes = doc["Pass"];
}

void setup()
{
  /* No timeout, since we want non-blocking UART operations */
  Serial.setTimeout(0);
  /* Initialize serial protocol for test purposes */
  Serial.begin(115200);
  readJson();
  /* We do want to use simple checkSum */
  proto.enableCheckSum();
  /* Redirect all protocol communication to Serial0 UART */
  proto.beginToSerial();

  gen_sec_pub(true);
}

void loop()
{
  if (phase == 0)
  {
    readdh1()
  }
  else if (phase == 1)
  {
    senddh()
  }
  else if (phase == 2)
  {
    receivePayload();
  }
  else if (phase == 3)
  {
    sendAccess()
  }
}

void sendAccess()
{

  char[] jsonString = "{\"GRANTED\":true + ,\"LOCKTIME\":0, \"TIMECODE\"" + millis() + "}"; // use strcat if errors

  DeserializationError error = deserializeJson(payload, jsonString);
  packet.clear();
  type = 3;
  packet.put(type);
  String jsonStringRes;
  serializeJson(payload,jsonStringRes)
  packet.put(AES128::encrypt(jsonStringRes,response) //encrypting AES128 with key from server
  phase=4;
}

void senddh()
{

  // send packet
  type = 1;
  Curve25519::dh1(k, f);
  packet.clear();
  packet.put(type);
  for (int i = 0; i < k.size; i++)
  {
    packet.put(k[i]);
  }

  proto.write(packet);
  phase = 2
}

void receivePayload()
{

  if (Serial1.available())
  {
    packet.clear();
    int len = proto.read(packet);
    if (len > 0)
    {
      phase = 3;
      if (packet.getbyte() == 2)
      { // DH Response code
        String jsonString = packet.getString();
        DeserializationError error = deserializeJson(payload, jsonString);
        encrypt
        k=response;
        Curve25519::dh2(k, f);
      
        String password = AES128::decrypt(payload["Pass"],k);
        bool found = true;
        for (int i = 0; i < codes.size; i++)
        {
          if (strcmp(password, doc[i]) == 0)
          {
            found = true;
          }
        }

        // if (strcmp(namet2, nameIt2) != 0){

        // }
      }
    }
  }
}

void readdh1()
{

  if (Serial1.available())
  {
    packet.clear();
    int len = proto.read(packet);
    if (len > 0)
    {
      phase = 1;

      if (packet.getbyte() == 0)
      { // DH Response code

        for (int i = 0; i < 32; i++)
        {
          response[i] = packet.getByte();
        }
      }
    }
  }
}
