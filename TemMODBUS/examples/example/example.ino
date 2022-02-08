#include <TemMODBUS.h>
// Default SoftwareSerial Pin 10 , 11 you can confrig it in TemMODBUS.cpp
#define DePin 5
#define Baudrate 9600
TemMODBUS TRS485(Baudrate,DePin); //Set your baudrate and De Pin 

void setup()
{
TRS485.SendData(0x01, 0x03, 0x00, 0x17, 0x00, 0x01); //Input your data from your data sheet
Serial.begin(9600);
}
void loop()
{
String Data = TRS485.ReadData(); //Recive respond data
if(Data!="")
{
    Serial.println(Data);
    Data="";
}

}
