#include <SPI.h>
#include <WiFi101.h>
#include <DHT.h>;
#include <PubSubClient.h>
#include <LiquidCrystal.h>

//Constants

#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//Variables
//char ssid[] = "Frilinglei";
//char pass[] = "BwrthG8wPH";
char ssid[] = "Unifi-AP-92";    
char pass[] = "dV11qYKRPnQdea8h"; 
//char ssid[] = "Orange-Stef&Elise";
//char pass[] = "5T777Q9Q7M0";
int status = WL_IDLE_STATUS; 
const char *mqtt_server = "m15.cloudmqtt.com";
const int mqtt_port = 16661;
const char *mqtt_user = "eblgndhd";
const char *mqtt_pass = "n_jbOQxQAaTt";
const char *mqtt_client_name = "Tempuino";
char* topicOut = "delay";
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
int tempDelay = 10000;


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Tempuino !");
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
    lcd.setCursor(0, 1);
    lcd.print("Attempting to connect to WPA SSID");
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("You're connected to the network");
  lcd.setCursor(0, 1);
    lcd.print("Connected");
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
    sendData(hum,temp);
    lcd.setCursor(0, 1);
    lcd.print("T : " + String(temp) +" H: " + String(hum));
    delay(tempDelay); 
  }

void sendData(long humidity, long temperature){
  int numt = temperature;
  int numh = humidity;
  char cstr[16];
  char cshr[16];
  itoa(numt,cstr,10);
  itoa(numh,cshr,10);
   client.publish("dht",cstr);
   client.publish("bmp",cshr);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  lcd.setCursor(0, 1);
    lcd.print("Received a message");
  Serial.println("-----------------------");
  if (topic ="delay"){
     payload[length] = '\0';
  int pwmVal = atoi((char *)payload);
    tempDelay = pwmVal * 60 * 1000;
    Serial.print("The delay has been changed to ");
    char delayOut[16];
  itoa(tempDelay,delayOut,10);
    Serial.println(delayOut);
  }
  lcd.setCursor(0, 1);
    lcd.print("Delay changed");
}

void startMQTTClient(){
  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);
    while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(mqtt_client_name, mqtt_user, mqtt_pass )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(1000);
    }
    }
    client.subscribe(topicOut);    
  }
    

void loop()
{
  client.loop();
 printTemperature();
}
