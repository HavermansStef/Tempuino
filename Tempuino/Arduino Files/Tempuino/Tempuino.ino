#include <SPI.h>
#include <WiFi101.h>
#include <DHT.h>;

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
const char *mqtt_user = "sviekext";
const char *mqtt_pass = "0H6dUnK2fXeSP79FMQW1k5drtJH7zEtJ";
const char *mqtt_client_name = "Tempuino";
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
  }
  establishConnection();
  dht.begin();
  
}

void loop()
{
 printTemperature();
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
     delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    delay(10000); //Delay 2 sec.
  }
   
