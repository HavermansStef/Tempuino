#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>   // MQTT library
#include <stdlib.h>

const char *ssid =  "StefLaptop";   // cannot be longer than 32 characters!
const char *pass =  "Stef12345";   //

const char *mqtt_server = "bee.rmq.cloudamqp.com";
const int mqtt_port = 1883;
const char *mqtt_user = "sviekext";
const char *mqtt_pass = "0H6dUnK2fXeSP79FMQW1k5drtJH7zEtJ";
const char *mqtt_client_name = "Tempuino"; // Client connections cant have the same connection name

#define BUFFER_SIZE 100

unsigned long previousMillis = 0;
const long interval = 10000;

WiFiClient wclient;  //Initialiseert de Wifi client
PubSubClient pubSubClient; // Initialiseert de PubSubClient

//void callback(const MQTT::Publish& pub) {
//  Serial.print(pub.topic());
//  Serial.print(" => ");
//  if (pub.has_stream()) {
//    uint8_t buf[BUFFER_SIZE];
//    int read;
//    while (read = pub.payload_stream()->read(buf, BUFFER_SIZE)) {
//      Serial.write(buf, read);
//    }
//    pub.payload_stream()->stop();
//
//    //Check if the buffer is -1
//    if(buf[0]=='-' && buf[1]=='1'){
//    //Code to blink the LED -- its strange that I can't blink the LED for more than 1sec.
//       digitalWrite(LED_BUILTIN, HIGH);
//       delay(1000);
//       digitalWrite(LED_BUILTIN, LOW);
//    }
//
//    Serial.println("");
//  } else
//    Serial.println(pub.payload_string());
//}

void setup() {
  setup_wifi();
  // Setup console
  pubSubClient.setClient(wclient);
  pubSubClient.setServer(mqtt_server, mqtt_port);
  Serial.begin(115200);  //set the baud rate
  delay(10);
  Serial.println(); Serial.println();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(" Not connected");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  //client object makes connection to server
  if (!pubSubClient.connected()) {
    Serial.println("Connecting to MQTT server");
    //Authenticating the client object
    if (pubSubClient.connect(mqtt_client_name, mqtt_user, mqtt_pass)) {
      Serial.println("Connected to MQTT server");
      pubSubClient.subscribe("Tempuino");
    } else {
      Serial.print("Failed");
      delay(5000);
    }
  }

  if (pubSubClient.connected()) {
    pubSubClient.loop();
  }
  SendTempHumid();
}

// this will send the dummy temparature reading

// Non-Blocking delay
void SendTempHumid() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) { // checks if 10 delay is over
    // save the last time you read the sensor
    previousMillis = currentMillis;

    Serial.print("Temparture-sensor");

    pubSubClient.publish("Temparture-sensor", "Hello" );
  }
}
