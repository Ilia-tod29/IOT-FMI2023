// Needed materials: ESP32, NeoPixel Ring, Led Display
#include <WiFiClientSecure.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <NeoPixelBus.h>
 
#define colorSaturation 128
 
const uint16_t PixelCount = 16; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 25;  // make sure to set this to the correct pin, ignored for Esp8266
 
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
 
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);
RgbColor grey(colorSaturation/2, colorSaturation/2, colorSaturation/2);
RgbColor yellow(colorSaturation, colorSaturation, 0);
RgbColor orange = RgbColor(255, 140, 0);
 
HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);
HslColor hslGrey(grey);
HslColor hslYellow(yellow);
HslColor hslOrange(orange);
 
unsigned long previousTime = 0;
unsigned long interval = 15000;
 
LiquidCrystal_PCF8574 lcd(0x27); // s
 
const char* ssid     = "A1_A240A4";     // your network SSID (name of wifi network)
const char* password = "caa2ec52"; // your network password
 
const char*  server = "www.weatherapi.com";  // Server URL
 
 
const char* test_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
     "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
     "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
     "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
     "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
     "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
     "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
     "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
     "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
     "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
     "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
     "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
     "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
     "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
     "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
     "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
     "-----END CERTIFICATE-----\n";
 
 
const size_t JSON_BUFFER_SIZE = 50000;
DynamicJsonDocument jsonDoc(JSON_BUFFER_SIZE);
 
 
WiFiClientSecure client;
 
void setup() {
   int error;
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(100);
 
  Wire.begin(21, 22);
  Wire.beginTransmission(0x27);
  Serial.print("heslsl");
  error = Wire.endTransmission();
 
  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2); 
 
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
  } 
 
  delay(500);
 
  WiFi.begin(ssid, password);
 
  int i = 0;
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
 
     lcd.clear();
     lcd.print("Connecting... " + i);
     i++;
    // wait 1 second for re-trying
    delay(1000);
  }
 
  delay(500);
 
  client.setCACert(test_root_ca);
 
   strip.Begin();
   strip.Show();
}
 
void loop() {
  unsigned long currentTime = millis();
 
 
  // Check if the interval has elapsed
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;  // Update the previous time
 
    Serial.println("\nStarting connection to server...");
    if (!client.connect(server, 443))
      Serial.println("Connection failed!");
    else {
      Serial.println("Connected to server!");
      // Make a HTTP request:
      client.println("GET http://api.weatherapi.com/v1/forecast.json?key=ab7b5be92d54447689f174645231406&q=Sofia&days=1&aqi=no&alerts=no HTTP/1.1");
      client.println("Host: www.weatherapi.com");
      client.println("Connection: close");
      client.println();
 
      char c;
      bool shouldRead = false;
      String payload = "";
      // Read and print the response
      while (client.connected() || client.available()) {
        if (client.available()) {
          c = client.read();
 
          if (c == '{') {
            shouldRead = true;
          }
 
          if (payload.indexOf("astro") > 0) {
            break;
          }
 
          if (shouldRead) {
            payload += c;
          }
        }
      }
 
      payload.remove(payload.length() - 7, 7);
      payload += "}]}}";
      Serial.println(payload);
 
 
      DeserializationError error = deserializeJson(jsonDoc, payload);
          if (error) {
            Serial.print("Error parsing JSON: ");
            Serial.println(error.c_str());
          }
 
      String location = jsonDoc["location"]["name"];
      int temp = jsonDoc["current"]["temp_c"];
      String condition = jsonDoc["current"]["condition"]["text"];
      int cloudPercentage = jsonDoc["current"]["cloud"];
      int rainPercentage = jsonDoc["forecast"]["forecastday"][0]["day"]["daily_chance_of_rain"];
 
      strip.ClearTo(RgbColor(0));
      if ((condition.indexOf("rain") >= 0) || (condition.indexOf("Rain") >= 0) || (condition.indexOf("drizzle") >= 0)) {
          int endPixelIndex = (int) (16 * (rainPercentage / 100.0));
 
          for (int i = 0; i < endPixelIndex; i++) {
            strip.SetPixelColor(i, blue);
          }
 
          strip.Show();
      }
 
      if ((condition.indexOf("loudy") >= 0) || (condition.indexOf("Fog") >= 0) || (condition.indexOf("Mist") >= 0)) {
          int endPixelIndex = (int) (16 * (cloudPercentage / 100.0));
 
          for (int i = 0; i < endPixelIndex; i++) {
            strip.SetPixelColor(i, hslGrey);
          }
 
          strip.Show();
      }
 
      if ((condition.indexOf("Sunny") >= 0)) {
          int endPixelIndex = 16;
 
          RgbColor decidedColor;
          temp = 32;
          if (temp < 27) {
            decidedColor = yellow;
          }
          else if (temp < 36) {
            decidedColor = orange;
          }
          else if (temp >= 36) {
            decidedColor = red;
          }
 
          for (int i = 0; i < endPixelIndex; i++) {
            strip.SetPixelColor(i, decidedColor);
          }
 
          strip.Show();
      }
      lcd.clear();
      lcd.print(location);
      lcd.print(", ");
      lcd.print(temp);
      lcd.print("C, Rain:");
      lcd.print(rainPercentage);
      lcd.print("%");
      lcd.print(", Cloud:");
      lcd.print(cloudPercentage);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print(condition);
      // Disconnect from the server
      client.stop();
  }
  }
 
  static int scrollPos = 0;
 
  lcd.scrollDisplayLeft();
  scrollPos++;
 
  if (scrollPos >= 16) {
    lcd.setCursor(16, 1); // Set the cursor to the end of the second row
    lcd.print(" "); // Clear the last character
    lcd.scrollDisplayRight(); // Scroll back to the starting position
    scrollPos = 0; // Reset the scrolling position
  }
 
  delay(250);
}