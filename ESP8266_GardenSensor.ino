/* Akshat Sahay
 * ESP8266 Garden Sensor
 * Samples soil moisture once every 15 minutes, then goes to sleep
 * Modified from https://github.com/SensorsIot/MQTT-Examples/blob/master/ESP_MQTT_PUBSUB_LIBRARY/ESP_MQTT_PUBSUB_LIBRARY.ino
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DHT_U.h>
#include <stdlib.h>

#define ssid          ""
#define password      ""

#define SERVER        "io.adafruit.com"
#define SERVERPORT    1883

#define IO_USERNAME   ""
#define IO_KEY        ""
#define IO_PATH       ""

unsigned long entry;
int moisture;
char data[128];

int count = 0;

void callback (char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  Serial.write(payload, length);
  Serial.println("");
}

WiFiClient WiFiClient;

// create MQTT object
PubSubClient client(WiFiClient);

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("ESP8266 Adafruit IO Tester");
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print(" ");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);
  
  if (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("", IO_USERNAME, IO_KEY)) {
      Serial.println("Connected");
      // ... and resubscribe
      client.subscribe(IO_PATH);
    } 
    
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 second");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  if (client.connected()) {
    Serial.println("Measure");
    entry = millis();
    moisture = analogRead(A0);
    moisture = map(moisture, 1024, 400, 0, 100); // calibration, (HIGH, LOW, 0, 100);

    Serial.println("Publish Moisture Levels");
    itoa(moisture, data, 10); 
    client.publish(IO_PATH, data);
  }

  delay(1000); // delay to ensure proper propogation
  Serial.println("Going to sleep");
  ESP.deepSleep(15 * 60 * 1000000, WAKE_RFCAL); //sleepTime = time_in_mins * 60 * 1000000;
}

void loop() {
  Serial.println("ERROR: Code in main loop");
}
