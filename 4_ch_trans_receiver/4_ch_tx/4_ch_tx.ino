// 4 Channel 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THROTTLE A0
#define YAW A1
#define PITCH A1
#define ROLL A1

const uint64_t  pipeOut = 0x2AA72940;
RF24 radio (9,8); // CE, CSN pins 


struct Signal{
  byte throttle; // A0
  byte yaw; // A1
  byte  pitch; //A2
  byte roll; //A3
};

Signal data;

void resetData(){
  data.throttle = 127;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  
}

void setup() {
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.stopListening();
  resetData();

}

int mapJoystickValues(int val , int lower, int middle, int upper, bool reverse)
{
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
      data.throttle = mapJoystickValues( analogRead(THROTTLE), 524, 524, 1015, true );
      data.yaw = mapJoystickValues( analogRead(YAW), 12, 524, 1020, true );
      data.pitch = mapJoystickValues( analogRead(PITCH), 12, 524, 1020, true );
      data.roll = mapJoystickValues( analogRead(ROLL), 12, 524, 1020, true );
      //SEND DATA TO TX
      radio.write(&data,sizeof(Signal));
}
