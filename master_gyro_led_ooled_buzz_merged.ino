#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x38, 0x2B, 0x78, 0x03, 0xDB, 0x71};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[20];
  int b;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//Declaration of mpu6050 instance
MPU6050 mpu(Wire);

long timer = 0;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_MODE_STA);
  esp_wifi_set_protocol( WIFI_IF_STA , WIFI_PROTOCOL_LR);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.print("MPU6050 status: ");
  display.println(status);

  while(status!=0){ } // Stop everything and loop forever if could not connect to MPU6050

  Serial.println("Calculating Offsets, do not move MPU6050");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println("Calculating Offsets, do not move MPU6050");

  //wait for proper calibration
  delay(3000);

  mpu.calcOffsets(true, true); // gyro, accelerometer
  delay(3000);

  Serial.println("Done\n");
  display.setCursor(0, 10);
  display.println("Done\n");
  display.display();
}

void loop() {
  mpu.update();

  if(millis() - timer > 1000){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("SphereGuard");
    Serial.print("Roll: ");Serial.println(mpu.getAngleX());
    display.setCursor(0, 20);
    // display.print("Roll: ");
    // display.println(mpu.getAngleX());
    display.println("No Unusual Tilts Detected");
    

    Serial.print("Pitch: ");Serial.println(mpu.getAngleY());
    display.setCursor(0, 40);
    // display.print("Pitch: ");
    // display.println(mpu.getAngleY());
    display.println("No messenges sent");
    // display.startscrollleft(0x00, 0x0F);
    // display.startscrollright(0x0F, 0x00);

    if (mpu.getAngleX() > 30 || mpu.getAngleX() < -30 || mpu.getAngleY() > 30 || mpu.getAngleY() < -30) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("SphereGuard");
      display.setCursor(0, 20);
      display.println("Unusual tilt detected!");
      display.setCursor(0, 30);
      display.println("Care-taker Notified");

      // Set values to send
      strcpy(myData.a, "I need help please");
      myData.b = 1;
  
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
    
    display.display();
    timer = millis();
  }
}
