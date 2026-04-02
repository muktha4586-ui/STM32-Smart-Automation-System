#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT sensor
#define DHTPIN PA1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pins
#define PIR_PIN PA0
#define LED_PIN PA2
#define BUZZER_PIN PA3
#define MOTOR_PIN PA4

// Variables
int motionState = 0;
float temperature = 0;
int tempThreshold = 30;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  Serial.begin(9600);
  dht.begin();

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {

  // Read sensors
  motionState = digitalRead(PIR_PIN);
  temperature = dht.readTemperature();

  // Motion logic
  if (motionState == HIGH) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(MOTOR_PIN, LOW);
  }

  // Temperature logic
  if (temperature > tempThreshold) {
    digitalWrite(MOTOR_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // OLED display
  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 20);
  if (motionState == HIGH) {
    display.println("Motion: YES");
  } else {
    display.println("Motion: NO");
  }

  display.setCursor(0, 40);
  if (temperature > tempThreshold) {
    display.println("Fan: ON");
  } else {
    display.println("Fan: OFF");
  }

  display.display();

  delay(1000);
}