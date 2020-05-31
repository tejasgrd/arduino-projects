#define joyX A0
#define joyY A1

int xValue = 0;
int yValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);

  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);

}  
