// Include libraries
#include <LiquidCrystal.h>
#include <DHT.h>
#define Type DHT11 // define the analog you need to connect
int sensePin = 3; DHT HT(sensePin, Type);
float humidity; float tempC; float tempF;
int setTime = 500; int DelayTime = 4000;

// Define input pins
#define motionSensor 13
#define temperatureSensor 3
#define lightSensor A0
#define overrideButton 5


// Define output pins
#define motorPin 2
#define buzzerPin 6
#define ledPin 4

// Initialize components
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // (rs, enable, d4, d5, d6, d7)
bool overrideMode = false;

void setup() {
  // Set up input and output pins
  pinMode(motionSensor, INPUT);
  pinMode(temperatureSensor, INPUT);
  pinMode(lightSensor, INPUT);
  pinMode(overrideButton, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Set up LCD display
  lcd.begin(16, 2);

  // Set up serial communication
  Serial.begin(9600);

  // Set up temp sensor
  HT.begin();
  
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Setup");
  lcd.setCursor(0, 1);
  lcd.print("Complete");
  delay(1000);
  lcd.clear();
}

void loop() {
  
    // Check if override button is pressed
  if (digitalRead(overrideButton) == HIGH) {
    overrideMode = true;
    // Set cursor position and notify override mode is enabled
    lcd.setCursor(0, 0);
    lcd.print("Override mode");
    lcd.setCursor(0, 1);
    lcd.print("enabled");
    Serial.println("Override mode enabled");
    delay(10000); // Debounce button
    overrideMode = false;
    lcd.clear();
  }
  // Read input sensors
  int motionDetected = digitalRead(motionSensor);
  tempC = HT.readTemperature();
  int lightLevel = analogRead(lightSensor);
  
  if (!overrideMode){
    // Display temperature on LCD screen
    lcd.setCursor(0, 0);
    lcd.print("Temperature:");
    lcd.setCursor(0, 1);
    lcd.print(tempC);
  }
  
  // Check if motion is detected and not in override mode
  if (motionDetected == 1 && !overrideMode) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Welcome");
    lcd.setCursor(0,1);
    lcd.print("Home!");
    digitalWrite(ledPin, HIGH); // Turn on LED
    delay(5000);
  } else {
    digitalWrite(ledPin, LOW); // Turn off LED
  }

  // Check if temperature is above acceptable levels and not in override mode
  if (tempC > 30 && !overrideMode) {
    lcd.clear();
    digitalWrite(motorPin, HIGH); // Turn on fan
    lcd.setCursor(0, 0);
    lcd.print("Activating Fan");
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(tempC);
    delay(5000);
  } else {
    digitalWrite(motorPin, LOW); // Turn off fan
  }

  // Check if light levels are too low and temperature is above acceptable levels, indicating a fire
  if (lightLevel < 10 && tempC > 40  && !overrideMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fire!!!!");
    lcd.setCursor(0, 1);
    lcd.print("Evacuate ASAP!");

      digitalWrite(buzzerPin, HIGH);
      delay(1000);
      digitalWrite(buzzerPin, LOW);
      delay(1000); // wait for 1 second before generating the next tone
  } else {
    digitalWrite(buzzerPin, LOW); // Turn off buzzer
  }
  
  // Send data to serial monitor
  Serial.print("Motion detected: ");
  Serial.println(motionDetected);
  Serial.print("Temperature: ");
  Serial.println(tempC);
  Serial.print("Light level: ");
  Serial.println(lightLevel);
  Serial.println("--------------------");

  //Delay before looping again
  delay(1000);
}
