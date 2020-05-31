
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t  pipeIn = 0x2AA72940;
RF24 radio(7,8); // CE, CSN pins 

struct Signal{
  byte throttle; // A0
  byte yaw; // A1
  byte  pitch; //A2
  byte roll; //A3
};

Signal data;
unsigned long lastRecvTime = 0;

void resetData(){
  data.throttle = 127;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  
}

void receiveData()
{
  if(radio.available())
  {
    radio.read(&data, sizeof(Signal));
    // Store last data received time
    lastRecvTime = millis();
  }
}
void setup() {

  //TEST
  Serial.begin(9600);
  
  resetData();
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  radio.startListening(); 

}

void loop() {
  receiveData();
  unsigned long now = millis();
  if((now - lastRecvTime) > 1000)// if no signal received for 100 ms then reset data to original position
  {
    resetData();
  }
  //TEST
  Serial.println("Throttle: ");
  Serial.println(data.throttle);
  Serial.println("Yaw : ");
  Serial.println(data.yaw);
  Serial.println("pitch : ");
  Serial.println(data.pitch);
  Serial.println("Roll : ");
  Serial.println(data.roll);
}