#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// Connect to the WiFi
const char* ssid = "ssid";
const char* password = "pswd";
const char* mqtt_server = "broker.mqttdashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

//Pins are connected to relays
const int RED_PIN = 12;    //D6: Red Pin
const int GREEN_PIN = 14;  //D5: Green Pin
const int BLUE_PIN = 13; //D7: Blue Pin

// The function below connects your NodeMCU to the wireless network
void setup_wifi(){

    Serial.begin(9600);
    Serial.println();
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(2000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    if(receivedChar=='0'){
      Serial.print("Relays On");
      OnRelays();
     ColourChange();
   }
   if(receivedChar=='1'){
    Serial.print("Relays Off");
    OffRelays();}
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Reception")) {    // "explicit name for each client"
      Serial.println("Reception is connected to MQTT server");
      // ... and subscribe to topic
      client.subscribe("RGB/Output");//Topic subscribed to
    } else {
      Serial.print("Failed to connect to MQTT...");
      Serial.println();
      Serial.println("Attempting to connect again in 3 seconds.");
      // Wait  seconds before retrying
      delay(3000);
    }
  }
}

//Led starts by lighting up Green
void InitialLedState(){
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
  client.publish("RGB/Output", "Green");
}
void OnRelays(){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}
void OffRelays(){
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}
void setup()
{
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  InitialLedState();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(RED_PIN, red_light_value);
  analogWrite(GREEN_PIN, green_light_value);
  analogWrite(BLUE_PIN, blue_light_value);
}

void ColourChange(){
  RGB_color(255, 0, 0); // Red
    delay(1000);
    RGB_color(0, 255, 0); // Green
    delay(1000);
    RGB_color(0, 0, 255); // Blue
    delay(1000);
    RGB_color(255, 255, 125); // Raspberry
    delay(1000);
    RGB_color(0, 255, 255); // Cyan
    delay(1000);
    RGB_color(255, 0, 255); // Magenta
    delay(1000);
    RGB_color(255, 255, 0); // Yellow
    delay(1000);
    RGB_color(255, 255, 255); // White
    delay(1000);
}
