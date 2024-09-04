#include <Arduino_MKRIoTCarrier.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD with I2C address 0x27

// Create an instance of the MKRIoTCarrier class
MKRIoTCarrier carrier;

const int pirPin = 7; 
int pirState = LOW;
float temperature;
float humidity;
int move = 1;
float temp1 = 0;
float temp2 = 0;
float hum1 = 0;
float hum2 = 0;
int car = 0;
int var = 5;


// Variables to store RGB color values
int r, g, b;

void setup() {
  // Initialize the Arduino MKR IoT Carrier
  carrier.begin();
  carrier.withCase();

  // Initialize the serial communication at 9600 baud rate
  Serial.begin(9600);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(pirPin, INPUT);
}

void loop() {
  pirState = digitalRead(pirPin);
  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  if (pirState==HIGH){
    move = 1;
  }

  // Check if the Light sensor has RGB color data available
  if (carrier.Light.colorAvailable()) {
    // Read the RGB color values
    carrier.Light.readColor(r,g,b);
    if (r < 10 || g < 10 || b < 10 || pirState == HIGH) {
      Serial.println("Motion detected!");
      Serial.println("I found waste");

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
      delay(2000);
      if(r > 20 || g > 20 || b > 20 && move == 1){
        Serial.print("");
        Serial.print("Recycle");
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Recycle");
        lcd.setCursor(6, 1);
        lcd.print("Waste");
        delay(2000);
        lcd.clear();
      }
      else if (temperature >= 25 && humidity >= 60) {
        temp1 = temperature;
        hum1 = humidity;
        lcd.print("Wait 15 second");
        delay(15000);
        temp2 = temperature;
        hum2 = humidity;
        lcd.clear();
        if (temp2-temp1 >= 1 && hum2-hum1 >= 1) {
        Serial.print("Organic Waste");
        Serial.print(temp2-temp1);
        Serial.print(hum2-hum1);
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Organic");
        lcd.setCursor(6, 1);
        lcd.print("Waste");        }
        else {
          Serial.print("Normal waste");
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("Normal");
          lcd.setCursor(6, 1);
          lcd.print("Waste");
          delay(2000);
        }
        }
      }
    } 
    else {
      Serial.println("No motion.");
      Serial.println("No waste");
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("No Waste");
      lcd.clear();
      delay(100);
    }
  }

