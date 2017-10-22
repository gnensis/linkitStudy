#define LED_GPIO_PIN  13

// the setup function runs once when you press reset or power the board
void setup() { // initialize digital pin 13 as an output. 
  pinMode(LED_GPIO_PIN, OUTPUT);
}

void ledOn() {
  digitalWrite(LED_GPIO_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void ledOff() {
  digitalWrite(LED_GPIO_PIN, LOW);    // turn the LED off by making the voltage LOW
}// the loop function runs over and over again forever

void loop() {  
  ledOn();
  delay(1000);              // wait for a second
  ledOff();
  delay(1000);              // wait for a second
}
