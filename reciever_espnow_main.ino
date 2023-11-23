#include <espnow.h>
// #include <WiFi.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Wire.h"
// #include <MPU6050_light.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define CHANNEL 0 //set channel number for reciever

// #include <WiFi.h>
// #include <esp8266_wifi.h>

#define buzzerPin  D7
#define redpin D6
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[20];
    int b;
} struct_message;

// Create a struct_message called myData
struct_message myData;
// OLED object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  // display_message(String(myData.a));
  // String val = myData.a;
  buzz_light(myData.b);
  // display_message(val);

}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redpin, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);

  delay(2000);
  display.clearDisplay();
  display.display();
  }  
  // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_AP_STA);
  // esp_wifi_set_protocol( WIFI_AP_STA , 1);

  // Init ESP-NOW
  if (esp_now_init() != ERR_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}