int ldrPin = A0;        // LDR analog pin
int pirPin = 2;         // PIR sensor pin
int ledPin = 9;         // LED pin (PWM-capable)

int ldrValue = 0;
int pirValue = 0;
int threshold = 800;    // Tune this based on lighting condition
unsigned long ledOnTime = 0;
unsigned long ledDuration = 2000; // 6 seconds
bool ledState = false;
bool fading = false;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  ldrValue = analogRead(ldrPin);  // Read LDR value
  pirValue = digitalRead(pirPin); // Read PIR value

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print("  PIR: ");
  Serial.println(pirValue);

  unsigned long currentTime = millis();

  if (ldrValue < threshold && pirValue == HIGH) {
    analogWrite(ledPin, 255);  // Full brightness
    ledState = true;
    fading = false;
    ledOnTime = currentTime; // Reset timer
  }

  // If LED is ON, check if it's time to fade out
  if (ledState && !fading) {
    if (currentTime - ledOnTime >= ledDuration || ldrValue >= threshold) {
      fading = true;
    }
  }

  // Fade out logic
  if (fading) {
    for (int brightness = 255; brightness >= 0; brightness--) {
      analogWrite(ledPin, brightness);
      delay(10); // Adjust for smoother or faster fading
    }
    ledState = false;
    fading = false;
  }

  delay(100); // Basic loop delay
}