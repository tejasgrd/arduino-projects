// 4 Channel 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THROTTLE A3
#define YAW A2
#define PITCH A1
#define ROLL A0

const uint64_t  pipeOut = 0x2AA72940;
RF24 radio(8,9); //CE, CSn


struct Signal{
  byte throttle; // A0
  byte yaw; // A1
  byte  pitch; //A2
  byte roll; //A3
};

Signal data;

void resetData(){
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  
}

void setup() {
   //TEST
  Serial.begin(9600);
  
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(pipeOut);
  radio.stopListening();
  resetData();

  Serial.print("Data Begin: ");

}

int mapJoystickValues(int val , int lower, int middle, int upper, bool reverse)
{
  Serial.print("inside mapJoystickValues");
  val = constrain(val, lower, upper);
  if(val < middle)
  {
    val = map(val, lower, middle, 0, 128);
  }else
  {
    val = map(val, middle, upper, 128, 255);
  }

  return ( reverse ? 255 - val : val );
}
void loop() {
      Serial.print("Inside loop(): ");
      data.throttle = mapJoystickValues( analogRead(THROTTLE), 12, 524, 1015, true );
      data.yaw = mapJoystickValues( analogRead(YAW), 12, 524, 1020, true );
      data.pitch = mapJoystickValues( analogRead(PITCH), 12, 524, 1020, true );
      data.roll = mapJoystickValues( analogRead(ROLL), 12, 524, 1020, true );
      //SEND DATA TO TX
      radio.write(&data,sizeof(Signal));
      //TEST
      Serial.print("Throttle: ");
      Serial.print(data.throttle);
      Serial.print(" , Yaw : ");
      Serial.print(data.yaw);
      Serial.print(", pitch : ");
      Serial.print(data.pitch);
      Serial.print(", Roll : ");
      Serial.print(data.roll);
      Serial.println("------");
}
