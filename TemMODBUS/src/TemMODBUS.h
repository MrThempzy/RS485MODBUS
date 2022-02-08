#include "Arduino.h"



class TemMODBUS
{
public:
    TemMODBUS(long Baudrate,int DEPin);
   void SendData(byte Slaveaddress, byte functionCode, byte startaddressHigh, byte startaddressLow, byte nuberofPointsHigh, byte nuberofPointsLow);
   String ReadData();
   
private:
    int _DEPin;
    byte ModReadBuffer[8];
    uint16_t CRC16_MODBUS(byte Slaveaddress, byte functionCode, byte startaddressHigh, byte startaddressLow, byte nuberofPointsHigh, byte nuberofPointsLow);
};





