#define PIN_LED_WHITE    9
#define PIN_LED_BLUE     10

#define PIN_RS485_DE     3


#define NUMBER_OF_CHANNELS 16
uint16_t testValues[NUMBER_OF_CHANNELS];

void setup() {
  pinMode(PIN_LED_WHITE, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  
  // Turn off both LEDs
  analogWrite(PIN_LED_WHITE, 60);
  digitalWrite(PIN_LED_BLUE, 0);
  
  // Enable output mode on the RS485 interface
  pinMode(PIN_RS485_DE, OUTPUT);
  digitalWrite(PIN_RS485_DE, HIGH);
  
  Serial1.begin(230400);
  
  // Preset the test values
  for(int i = 0; i < NUMBER_OF_CHANNELS; i++) {
    testValues[i] = i*(65535.0/NUMBER_OF_CHANNELS);
  }
}

void sendTestFrame() {
  Serial1.write(0xde);
  Serial1.write(0xad);
  Serial1.write(NUMBER_OF_CHANNELS*2);
  for(int i = 0; i < NUMBER_OF_CHANNELS; i++) {
    Serial1.write(testValues[i] & 0xFF);        // low byte
    Serial1.write((testValues[i] >> 8) & 0xFF); // high byte
    
    testValues[i] += 500;
  }
  Serial1.write(0xff);  // TODO: Implement CRC
}

void loop() {
//  if(Serial.available()) {
    while(1) {
      if (Serial.available()) {
        Serial1.write(Serial.read());  
      }
    }
//  }
//  
//  sendTestFrame();
}
