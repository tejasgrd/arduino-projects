
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

int ch_width_1 = 0;
//ch_2
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
const uint64_t  pipeIn = 0x2AA72940;
RF24 radio(7, 8); // CE, CSn

Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;

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

  //Setup Servo Throttle
  ch1.attach(2);

  ch2.attach(3);
  ch3.attach(4);
  ch4.attach(5);
  
  resetData();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
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
  Serial.print("Throttle: ");
  Serial.print(data.throttle);
  ch_width_1 = map(data.throttle, 0, 252, 1000, 2000);
  Serial.print(", Yaw : ");
  Serial.print(data.yaw);
  ch_width_2 = map(data.pitch,    0, 255, 1000, 2000);     // pin D3 (PWM signal)
  ch_width_3 = map(data.roll,     0, 255, 1000, 2000);     // pin D4 (PWM signal)
  ch_width_4 = map(data.yaw,      0, 255, 1000, 2000); 
  Serial.print(", pitch : ");
  Serial.print(data.pitch);
  Serial.print(", Roll : ");
  Serial.print(data.roll);
  Serial.println("------END-------");

  ch1.writeMicroseconds(ch_width_1);
  ch2.writeMicroseconds(ch_width_2);
  ch3.writeMicroseconds(ch_width_3);
  ch4.writeMicroseconds(ch_width_4);
  
}
