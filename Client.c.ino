#include <Keypad.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AES.h>
#include <Crypto.h>
#include <string.h>
#include <Curve25519.h>
#include "tiny.h"
#include "TinyProtocol.h"

Tiny::ProtoLight proto;
Tiny::Packet<256> packet;

uint8_t f[32];
uint8_t k[32];
uint8_t response[32];

uint8_t type;

AES128 aes128;
char input_password[64];
StaticJsonDocument<200> doc;
StaticJsonDocument<200> payload;

uint8_t key[] size_t AES128::keySize() const
{
  return 16;
}

byte output[16];
String json_object;
String data;
int phase = 0;

const int ROW_NUM = 4;    //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};      //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

//const String password_1 = "001A"; // change your password here
//const String password_2 = "002B";  // change your password here
//const String password_3 = "003C";   // change your password here
//const String password_4 = "104A";   // change your password here
//const String password_5 = "105B";   // change your password here
//const String password_6 = "106C";   // change your password here
//const String password_7 = "107D";   // change your password here
//const String password_8 = "208A";   // change your password here
//const String password_9 = "209B";   // change your password here
//const String password_10 = "210C";   // change your password here

String userinput;

// void AESTiny128::encrypt(data, uint8_t key)
// {
//    AESTiny128::setKey(key, size_t len);
//    AESTiny128::encryptBlock(output, (uint8_t*)data.c_str());
//    Serial.print("Password is encrypted");
// }

// void data_store(char data[])
// {
//   doc["InputPassword"] = data;
//   serializeJson(doc, userinput);
//   Serial.print(userinput);
//   AESTiny128::encrypt(userinput, key);
// }
  
void setup()
{

  Serial1.begin(115200);
  proto.beginToSerial1();
}

void senddh1()
{
  // send packet
  type = 0;
  Curve25519::dh1(k, f);
  packet.clear();
  packet.put(type);
  for (int i=0;i<k.size;i++){
    packet.put(k[i]);
  }
  
  proto.write(packet);
}

void loop()
{
  get_password();
  if (phase == 0)
  {
    senddh1();
  }
  else if (phase == 1)
  {
    receivedhResponse()
  }
  else if(phase==2){
    sendPassword()
  }
  else if (phase==3)
  {
    
  }
  
}

void receiveDoorAccess(){
   if (Serial1.available())
  {
    int len = proto.read(packet);
    if (len > 0)
    {
      packet.getByte();
     Serial.print(packet.getString());
    }
     phase=4;
  }
}

// Loop to get input password from user
void sendPassword(){
  char json[] =
            "{\"Pass\":"+input_password+",\"TIMECODE\":"+millis()+"}"   // use strcat if errors
  
  DeserializationError error = deserializeJson(payload, json);
  packet.clear()
  type=2;
  packet.put(type);
  String jsonString;
  serializeJson(payload,jsonString)
  packet.put( AES128::encrypt(jsonString))
  phase=3;
  }

void receivedhResponse()
{
  if (Serial1.available())
  {
    int len = proto.read(packet);
    if (len > 0)
    {
      if(packet.getbyte()==1){ // DH Response code 

        for (int i=0;i<32;i++){
          response[i]=packet.getByte();
        }
        
      }
    }
     phase=2;
  }
 
}

void get_password()
{
  char key = keypad.getKey();

  if (key)
  {
    Serial.println(key);

    if (key == '*')
    {
      input_password = ""; // reset the input password
    }
    else if (key == '#')
    {
      //if(input_password == password_1 || input_password == password_2 || input_password == password_3 || input_password == password_4 || input_password == password_5 || input_password == password_6 || input_password == password_7 || input_password == password_8 || input_password == password_9 || input_password == password_10) {
      Serial.println(input_password);
      data_store(input_password);
      //delay(2000);
    }
    else
    {
      input_password[i] += key; // append new character to input password string
      Serial.println(input_password);
    }
  }
}
