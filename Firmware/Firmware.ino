#define PIN_LED_WHITE    9
#define PIN_LED_BLUE     10

#define PIN_RS485_DE     3

void setup() {
  pinMode(PIN_LED_WHITE, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  
  // Turn off both LEDs
  analogWrite(PIN_LED_WHITE, 60);
  digitalWrite(PIN_LED_BLUE, 0);
  
  // Enable output mode on the RS485 interface
  pinMode(PIN_RS485_DE, OUTPUT);
  digitalWrite(PIN_RS485_DE, HIGH);
  
  Serial1.begin(38400);
}


void loop() {
  if(Serial.available()) {
    digitalWrite(PIN_LED_BLUE, 1);
    Serial1.write(Serial.read());  
    digitalWrite(PIN_LED_BLUE, 0);
  }
}
