#include "Arduino.h"
#include "TemMODBUS.h"
#include "SoftwareSerial.h"
SoftwareSerial TemSerial(10,11);

TemMODBUS::TemMODBUS(long Baudrate,int DEPin)
{
 _DEPin = DEPin;

 pinMode(_DEPin,OUTPUT);
 TemSerial.begin(Baudrate);

}



void TemMODBUS::SendData(byte Slaveaddress, byte functionCode, byte startaddressHigh, byte startaddressLow, byte nuberofPointsHigh, byte nuberofPointsLow)
{
startaddressHigh=startaddressHigh-0x00;
  startaddressLow=startaddressLow-0x01;
  digitalWrite(_DEPin,HIGH);

  
  uint16_t Checksum;
  Checksum=CRC16_MODBUS(Slaveaddress,functionCode,startaddressHigh,startaddressLow,nuberofPointsHigh,nuberofPointsLow);
byte CRCHigh = 0xFF, CRCLow = 0xFF;
byte CRC[2];
CRC[0] = CRCLow = (byte)(Checksum & 0xFF); 
CRC[1] = CRCHigh = (byte)((Checksum >> 8) & 0xFF); 

ModReadBuffer[0] = (byte)(Slaveaddress);
ModReadBuffer[1] = (byte)(functionCode);
ModReadBuffer[2] = (byte)(startaddressHigh);
ModReadBuffer[3] = (byte)(startaddressLow);
ModReadBuffer[4] = (byte)(nuberofPointsHigh);
ModReadBuffer[5] = (byte)(nuberofPointsLow);
ModReadBuffer[6] = (byte)(CRC[0]);
ModReadBuffer[7] = (byte)(CRC[1]);

for(int j=0;j<8;j++) TemSerial.write(ModReadBuffer[j]);
  
  Serial.flush();
  delayMicroseconds(10);   
    digitalWrite(_DEPin,LOW);
}

uint16_t TemMODBUS::CRC16_MODBUS(byte Slaveaddress, byte functionCode, byte startaddressHigh, byte startaddressLow, byte nuberofPointsHigh, byte nuberofPointsLow)
{

byte frame[8];
frame[0] = (byte)(Slaveaddress);
frame[1] = (byte)(functionCode);
frame[2] = (byte)(startaddressHigh);
frame[3] = (byte)(startaddressLow);
frame[4] = (byte)(nuberofPointsHigh);
frame[5] = (byte)(nuberofPointsLow);

byte checksum[2];
  uint16_t reg_crc = 0xFFFF;
int len = sizeof(frame) / sizeof(frame[0]);

  for (int pos = 0; pos < len-2; pos++) {
    reg_crc ^= frame[pos];          // XOR byte into least sig. byte of crc

    for (int i = 0; i <8 ; i++) {    // Loop over each bit
      if ((reg_crc & 0x01) == 1)  reg_crc = (uint16_t)((reg_crc >> 1) ^ 0xA001);
      else reg_crc = (uint16_t)(reg_crc >> 1);
     
    }
  }

  return reg_crc;  
}

String TemMODBUS::ReadData()
{
  String Respond;
  if(TemSerial.available())
  {
    byte Recived[10];
    
    for(byte i=0;i<9;i++){
    Recived[i] = TemSerial.read();
    Respond += String(Recived[i]);
delayMicroseconds(100);
    }
  }

return Respond;
}