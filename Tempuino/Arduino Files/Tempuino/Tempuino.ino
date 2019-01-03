#include <SPI.h>
#include <WiFi101.h>
#include <DHT.h>;
#include <PubSubClient.h>

//Constants

#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
char ssid[] = "Unifi-AP-92";    
char pass[] = "dV11qYKRPnQdea8h"; 
int status = WL_IDLE_STATUS; 
const char *mqtt_server = "bee.rmq.cloudamqp.com";
const int mqtt_port = 1883;
const char *mqtt_user = "sviekext:sviekext";
const char *mqtt_pass = "uhR7oc-YUsNB7Gvr-B0SKGnTUMVybjIN";
const char *mqtt_client_name = "Tempuino";
char* topic = "temperature";
char* topicOut = "messages";
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
int tempDelay = 10000;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
  }
  establishConnection();
  startMQTTClient();
  dht.begin();
  
}

void establishConnection(){
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("You're connected to the network");
}

void printTemperature(){
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    //sendData(hum,temp);
    delay(tempDelay); 
  }

void sendData(long humidity, long temperature){
    String stringHumid = String(humidity);
    String stringTemp = String(temperature);
    String stringToSend = String(stringHumid + stringTemp);
    if (client.publish(topic,stringToSend.c_str())) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void startMQTTClient(){
  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);
    while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(mqtt_client_name, mqtt_user, mqtt_pass )) {
      Serial.println("connected");
      Serial.println("Topic is:");
      Serial.println(topic);  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(1000);
    }
    }
    client.subscribe(topicOut);
   if (client.publish(topic, "hello")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
    
  }
    

void loop()
{
  client.loop();
 printTemperature();
}
