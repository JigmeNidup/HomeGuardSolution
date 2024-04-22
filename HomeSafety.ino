#include <ArduinoJson.h> //JSON library

// WIFI and MQTT library
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>

// OLED
#include <Wire.h>
#include "SSD1306.h"

// Temperature and Humidity Sensor
#include "DHT.h"
#define DHT11_PIN 33

DHT dht11(DHT11_PIN, DHT11);

// set i2c pins for OLED
#define I2C_SDA 25
#define I2C_SCL 26
HW_I2C I2C_2 = I2C_TWO;
// HW_I2C I2C_2 = I2C_ONE;
OLEDDISPLAY_GEOMETRY g = GEOMETRY_128_64;
SSD1306 display(0x3c, I2C_SDA, I2C_SCL, g, I2C_2);

// bitmap image refer:  https://github.com/ThingPulse/esp8266-oled-ssd1306/issues/53
const uint8_t bitmap_Logo[] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x30,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x78,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x7E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x3F,
    0xFF,
    0xFF,
    0x1F,
    0x00,
    0x00,
    0x00,
    0x80,
    0x1F,
    0xFF,
    0xFF,
    0x1F,
    0x00,
    0x00,
    0x00,
    0x80,
    0x8F,
    0xFF,
    0xFF,
    0x3F,
    0x00,
    0x00,
    0x00,
    0xC0,
    0xCF,
    0x03,
    0x1F,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xE0,
    0xE7,
    0x01,
    0x06,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xF0,
    0xF3,
    0x00,
    0x06,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x79,
    0x38,
    0x06,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xFC,
    0x3C,
    0x3E,
    0x06,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x7E,
    0x3E,
    0xFC,
    0x07,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x3E,
    0x1F,
    0xFC,
    0x03,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x1F,
    0x0F,
    0xFC,
    0x01,
    0x3C,
    0x00,
    0x00,
    0x80,
    0x8F,
    0x07,
    0xF8,
    0x01,
    0x3C,
    0x00,
    0x00,
    0xC0,
    0xC7,
    0x03,
    0xF8,
    0x01,
    0x3C,
    0x00,
    0x00,
    0xE0,
    0xE7,
    0x01,
    0xF8,
    0x00,
    0x3C,
    0x00,
    0x00,
    0xF0,
    0xF3,
    0x00,
    0xF0,
    0x00,
    0x3C,
    0x00,
    0x00,
    0xF8,
    0x79,
    0x00,
    0xF0,
    0x01,
    0x3C,
    0x00,
    0x00,
    0xF8,
    0x78,
    0x00,
    0xF0,
    0x00,
    0x3C,
    0x00,
    0x00,
    0xF8,
    0x79,
    0x00,
    0x20,
    0x00,
    0x3C,
    0x00,
    0x00,
    0xF0,
    0xF1,
    0x00,
    0x00,
    0x00,
    0x3C,
    0x00,
    0x00,
    0xE0,
    0xE3,
    0x01,
    0x00,
    0x00,
    0x3C,
    0x00,
    0x00,
    0xC0,
    0xE7,
    0x03,
    0x00,
    0x00,
    0x3C,
    0x00,
    0x00,
    0xC0,
    0xCF,
    0x03,
    0x00,
    0x60,
    0x3C,
    0x00,
    0x00,
    0x80,
    0x9F,
    0x07,
    0x00,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x3F,
    0x0F,
    0x00,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x7E,
    0x1E,
    0x00,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x7C,
    0x3C,
    0x00,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x78,
    0x00,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xF0,
    0xF9,
    0x00,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xF0,
    0xF3,
    0x00,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xF8,
    0xE7,
    0x01,
    0xF0,
    0x3C,
    0x00,
    0x00,
    0x00,
    0xFC,
    0xCF,
    0x03,
    0xF8,
    0x3D,
    0x00,
    0x00,
    0x00,
    0xFC,
    0x8F,
    0xFF,
    0xFF,
    0x3F,
    0x00,
    0x00,
    0x00,
    0xFC,
    0x1F,
    0xFF,
    0xFF,
    0x3F,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x3F,
    0xFF,
    0xFF,
    0x1F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x7E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x78,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x30,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

// Gas sensor
const int mq135 = 34;
const int mq9 = 35;

// buzzers
const int buzzer = 14;

// relay for fan
const int relay_fan = 13;
// relay for humidifier
//  const int relay_humidifier = 12;

// WIFI button
const int wifi_btn = 4;
const int wifi_led = 18;

//---- WiFi settings
// const char* ssid = "bruh";
// const char* password = "megabruh";

// WIFI MANAGER
const char *ssid_AP = "ESP32HomeAP";

//---- HiveMQ Cloud Broker settings
const char *mqtt_server = "a3cda3bb26e348b6b1f410d39aab4ef9.s1.eu.hivemq.cloud"; // replace with your HiveMQ Cluster URL
const char *mqtt_username = "esp32cam";                                          // replace with your Username
const char *mqtt_password = "ESP32cam";                                          // replace with your Password
const int mqtt_port = 8883;

WiFiClientSecure espClient;
PubSubClient client(espClient);

bool isOnline = false;

unsigned long lastMsg = 0;
unsigned long prevTime = 0;

const char *temp_humi_topic = "temp_humi";
const char *gas_topic = "gas_sensor";

const char *fan_topic1 = "fan_relay/post";
const char *fan_topic2 = "fan_relay/get";

// HiveMQ Cloud Let's Encrypt CA certificate
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

void init_Actuators()
{
    digitalWrite(buzzer, LOW);
    digitalWrite(relay_fan, LOW);
    digitalWrite(wifi_led, LOW);
}

// set wifi credentials
void OnDemandWifi()
{

    init_Actuators();

    display.clear();
    display.drawString(0, 0, "Welcome to WiFi Manager");
    display.drawString(0, 10, "1.Connect to WIFI: ");
    display.drawString(0, 20, ssid_AP);
    display.drawString(0, 30, "2.Open Your Browser at: ");
    display.drawString(0, 40, "192.168.4.1");
    display.drawString(0, 50, "3.Enter WIFI credentials: ");
    display.display();

    WiFiManager wm;

    // reset settings - for testing
    //  wm.resetSettings();

    int timeout = 120; // 120 seconds to run for ie 2 min
    // set configportal timeout
    // 2 min to connect to given wifi
    wm.setConfigPortalTimeout(timeout);

    if (!wm.autoConnect("HomeSafetyAP", "password"))
    {
        Serial.println("failed to connect and hit timeout");
        display.clear();
        display.drawString(0, 0, "Failed to Connect");
        display.drawString(0, 20, "Offline mode");
        display.display();
        delay(3000);
        isOnline = false;
        delay(5000);
    }
    else
    {
        // connected to the WiFi
        Serial.println("connected to wifi");
        isOnline = true;
        digitalWrite(wifi_led, HIGH);

        Serial.println("");
        Serial.println("WiFi connected");
        display.clear();
        display.drawString(0, 0, "WiFi connected");
        display.display();

        Serial.print("ESP32-CAM IP Address: ");
        Serial.println(WiFi.localIP());
        display.drawString(0, 10, "ESP32-CAM IP Address: ");
        display.drawString(0, 20, String(WiFi.localIP()));
        display.display();
        delay(2000);

        display.clear();
        display.drawString(0, 0, "Connecting to MQTT Server: ");
        display.display();

        espClient.setCACert(root_ca);
        client.setServer(mqtt_server, mqtt_port);
        client.setCallback(callback);
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived: ");
    String recievedMsg = "";
    for (int i = 0; i < length; i++)
    {
        recievedMsg += (char)payload[i];
    }
    Serial.println(recievedMsg);
}

void reconnect()
{
    // Loop until we’re reconnected
    int attempt = 0;
    int max_attempt = 10;
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection… ");
        String clientId = "ESP32Client";
        attempt++;

        // Attempt to connect
        if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("connected!");
            isOnline = true;
            digitalWrite(wifi_led, HIGH);
            display.drawString(0, 20, "MQTT connected");
            display.display();

            // client.subscribe("testTopic2");
        }
        else
        {
            Serial.print("failed, rc = ");
            Serial.print(client.state());
            Serial.println(" try again in 1 seconds");
            isOnline = false;
            digitalWrite(wifi_led, LOW);
            display.clear();
            display.drawString(0, 10, "MQTT Retry: " + String(attempt) + " / " + String(max_attempt));
            display.drawString(0, 20, "MQTT Failed");
            display.display();
            if (attempt >= max_attempt)
            {
                break;
            }
            // Wait
            // Wait 1 seconds before retrying
            delay(1000);
        }
    }
}

void getTempHumiValue()
{
    float humi = dht11.readHumidity();
    float tempC = dht11.readTemperature();
    float tempF = dht11.readTemperature(true);

    JsonDocument doc;
    doc["sensor"] = "dht11";
    doc["tempC"] = tempC;
    doc["tempF"] = tempF;
    doc["humi"] = humi;

    // turn on fan if temp is above 32 degree Celsius
    if (tempC > 32)
    {
        digitalWrite(relay_fan, HIGH);
    }
    else
    {
        digitalWrite(relay_fan, LOW);
    }

    if (isOnline && tempC > 32)
    {
        client.publish(fan_topic1, "true");
    }
    else
    {
        client.publish(fan_topic1, "false");
    }

    char JSONdata[200];
    serializeJson(doc, JSONdata);
    Serial.println(JSONdata);

    display.drawString(65, 30, "Temp: " + String(tempC));
    display.drawString(65, 40, "Humi: " + String(humi));

    // publish only when online (wifi and mqtt is connected)
    if (isOnline)
    {
        client.publish(temp_humi_topic, JSONdata);
    }
}

int getMQValues(int sensor)
{
    int raw_val[10]; // int array for gas readings
    int ppm = 0;     // int for calculated ppm
    int sum = 0;     // int for averaging

    for (int i = 0; i < 10; i++) // get values 10x over 1 seconds
    {
        raw_val[i] = analogRead(sensor);
        delay(100);
    }

    for (int i = 0; i < 10; i++) // add samples together
    {
        sum += raw_val[i];
    }

    ppm = sum / 10; // divide samples by 10
    return ppm;
}

void getGasValue()
{

    /*
     * Atmospheric CO2 Level..............400ppm
     * Average indoor co2.............350-450ppm
     * Maxiumum acceptable co2...........1000ppm
     * Dangerous co2 levels.............>2000ppm
     */

    int mq135_val = getMQValues(mq135);
    int mq9_val = getMQValues(mq9);

    JsonDocument doc;
    doc["sensor"] = "gas sensor";
    doc["mq135"] = mq135_val;
    doc["mq9"] = mq9_val;

    char JSONdata[200];
    serializeJson(doc, JSONdata);
    Serial.println(JSONdata);

    display.drawString(65, 10, "MQ135: " + String(mq135_val));
    display.drawString(65, 20, "MQ9: " + String(mq9_val));

    // publish only when online
    if (isOnline)
    {
        client.publish(gas_topic, JSONdata);
    }
}

void setup()
{
    delay(500);
    // When opening the Serial Monitor, select 9600 Baud
    Serial.begin(115200);
    dht11.begin(); // initialize DHT11 sensor

    Wire1.begin(I2C_SDA, I2C_SCL);
    display.init(); // INITIALIZE OLED

    pinMode(buzzer, OUTPUT);
    pinMode(relay_fan, OUTPUT);

    pinMode(wifi_btn, INPUT_PULLUP);
    pinMode(wifi_led, OUTPUT);

    init_Actuators();

    display.clear();
    display.drawString(0, 0, "Starting....");
    display.display();
    delay(2000);

    OnDemandWifi();
}

void loop()
{
    if (isOnline)
    {
        if (!client.connected())
        {
            isOnline = false;
            reconnect();
        }
        client.loop();
    }

    if (!isOnline)
    {
        unsigned long currentTime = millis();
        if (currentTime - prevTime > (30 * 60 * 1000))
        { // try to connect to wifi again every 30 min if wifi is lost
            prevTime = currentTime;
            OnDemandWifi();
        }
    }

    if (digitalRead(wifi_btn) == LOW)
    {

        OnDemandWifi();
    }
    unsigned long now = millis();
    if (now - lastMsg > 10000)
    {
        lastMsg = now;

        display.clear();
        if (isOnline)
        {
            display.drawString(65, 0, "Online Mode");
        }
        else
        {
            display.drawString(65, 0, "Offline Mode");
        }
        // display logo
        display.drawFastImage(0, 0, 64, 64, bitmap_Logo);

        getTempHumiValue();
        getGasValue();

        display.display();
    }
}