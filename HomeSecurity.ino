//wifi and http
// #include <WiFi.h>
#include <WiFiClientSecure.h>
#include <base64.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

//CAMERA
#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

//RFID
#include <MFRC522.h>  //library responsible for communicating with the module RFID-RC522
#include <SPI.h>      //library responsible for communicating of SPI bus

//OLED
#include <Wire.h>
#include "SSD1306.h"

#define SS_PIN 15
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

//set spi pins for RFID
#define SCK1 14
#define MISO1 12
#define MOSI1 13
#define SS1 15

//set i2c pins for OLED
#define I2C_SDA 1
#define I2C_SCL 3
SSD1306 display(0x3c, I2C_SDA, I2C_SCL);
// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid = "bruh";
const char* password = "megabruh";

// ===========================
// Enter your server information
// ===========================
//https://home-security-jigmenidup.vercel.app/api/upload  cloud

//http://192.168.50.92:3000/api/rfid  local
// const char* serverName = "http://192.168.50.92:3000/api/rfid";
const char* serverRFID = "https://home-security-jigmenidup.vercel.app/api/rfid";

const char* serverUpload = "https://home-security-jigmenidup.vercel.app/api/upload";

int relay_pin = 4;

bool allowAccess = false;

//rfid valid list
int num_users = 1;
const int max_num_users = 5;
String validlist[max_num_users] = { "B4 43 B6 07" };
String validName[max_num_users] = { "SYS admin" };

void getUID() {
  String responseBody;
  display.clear();
  display.drawString(0, 0, "Connecting to server");
  display.display();

  if (WiFi.status() == WL_CONNECTED) {
    // WiFiClient client;
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    http.begin(client, serverRFID);
    display.drawString(0, 10, "Connection successful!");
    display.drawString(0, 20, "Fetch RFID...");
    display.display();

    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      String payload = "{}";
      payload = http.getString();
      JSONVar myObject = JSON.parse(payload);

      JSONVar keys = myObject.keys();

      for (int i = 0; i < keys.length(); i++) {
        //limit num of users
        if (i + 1 > max_num_users) {
          break;
        }

        JSONVar value = myObject[keys[i]];
        String rfid_val = JSON.stringify(value);
        rfid_val.remove(12, 1);
        rfid_val.remove(0, 1);

        validlist[i + 1] = rfid_val;

        String rfid_user = JSON.stringify(keys[i]);
        validName[i + 1] = rfid_user;
        num_users += 1;  //increase num of users
        display.clear();
        display.drawString(10, 10, rfid_val);
        display.drawString(10, 20, rfid_user);
        display.display();
        delay(3000);
      }

      display.drawString(0, 30, "Fetched RFID details...");
      display.display();
      delay(2000);

    } else {
      display.drawString(0, 40, "Response: " + String(httpResponseCode));
      display.display();
    }
    http.end();

  } else {
    responseBody = "Connection to server failed.";
    display.drawString(0, 10, "Response: " + String(responseBody));
    display.display();
    delay(2000);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //disable brownout detector


  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  //I2C - OLED
  display.init();  //INITIALIZE OLED

  display.clear();
  display.drawString(0, 0, "Starting....");
  display.display();
  delay(2000);

  //SPI - RFID
  SPI.begin(SCK1, MISO1, MOSI1, SS1);

  mfrc522.PCD_Init();  // Initiate MFRC522

  //setup wifi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  display.clear();
  display.drawString(0, 0, "Connecting to WIFI: " + String(ssid));
  display.display();
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.drawString(i % 128, i / 128 * 5 + 5, ".");
    i += 2;
    display.display();
  }

  display.clear();
  display.drawString(0, 0, "WiFi connected");
  display.display();

  display.drawString(0, 10, "ESP32-CAM IP Address: ");
  display.drawString(0, 20, String(WiFi.localIP()));
  display.display();

  delay(2000);
  getUID();

  // setup CAMERA
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_SVGA;  // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // // Init Camera
  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    display.clear();
    display.drawString(0, 0, "Camera init failed");
    display.display();
    delay(3000);
  } else {
    display.clear();
    display.drawString(0, 0, "Camera init");
    display.display();
    delay(1000);
  }



  display.clear();
  display.drawString(0, 10, "Bring Card to the reader");
  display.display();
}

bool ValidateID(String id) {

  bool isvalid = false;
  for (int i = 0; i < num_users; i++) {
    if (id == validlist[i]) {
      display.drawString(0, 30, validName[i]);
      isvalid = true;
      break;
    }
  }
  return isvalid;
}

void takePicture() {

  String responseBody;
  display.clear();

  camera_fb_t* fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    display.drawString(0, 10, "Camera capture failed");
    display.display();
    delay(3000);
    // ESP.restart();
    allowAccess = false;
  }

  display.drawString(0, 0, "Connecting to server: ");
  display.display();

  if (WiFi.status() == WL_CONNECTED) {
    // WiFiClient client;
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    http.begin(client, serverUpload);
    display.drawString(0, 10, "Connection successful!");
    display.drawString(0, 20, "Sending Image...");
    display.display();

    String encoded = base64::encode(fb->buf, fb->len);
    display.drawString(0, 30, "Encoded Base64");
    display.display();

    http.addHeader("Content-Type", "application/json");
    String body = "{\"imgdata\":\"";
    body += encoded;
    body += "\"}";
    int httpResponseCode = http.POST(body);

    display.drawString(0, 40, "Response Code: " + String(httpResponseCode));
    display.display();

    if (httpResponseCode == 200) {
      allowAccess = true;
    } else {
      allowAccess = false;
    }
    esp_camera_fb_return(fb);
    http.end();

  } else {
    responseBody = "Connection to server failed.";
    display.drawString(0, 20, responseBody);
    display.display();
    esp_camera_fb_return(fb);
    allowAccess = false;
  }
}

void loop() {
  delay(100);
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  display.clear();
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  display.drawString(0, 10, "UID tag: " + content.substring(1));
  if (ValidateID(content.substring(1))) {
    display.drawString(0, 20, "Authorized RFID");
    display.display();

    //take 3 photos and if one is valid then open lock
    for (int i = 0; i < 3; i++) {
      if (i == 1) {
         //skip first image response ie flush the camera buffer
      } else if (allowAccess) {
        break;
      }
      takePicture();
      delay(1500);
    }
    if (allowAccess) {
      //Light the LED to indicate success
      display.drawString(0, 50, "Authorized");
      display.display();
      digitalWrite(relay_pin, HIGH);
      delay(5000);
    } else {
      display.drawString(0, 50, "Access Denied");
      display.display();
      delay(5000);
    }

    delay(5000);
    digitalWrite(relay_pin, LOW);
  } else {

    display.drawString(0, 20, "Access Denied");
    display.display();

    digitalWrite(relay_pin, LOW);
    delay(5000);
  }
  display.clear();
  display.drawString(0, 10, "Bring Card to the reader");
  display.display();

  allowAccess = false;
}
