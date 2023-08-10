#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino_JSON.h>

#define LED_R2           21 // ESP32 pin GIOP21 connected to LED's pin
#define LED_V2           2 // ESP32 pin GIOP21 connected to LED's pin




//const char* ssid = "Ooredoo3A50D4";
//const char* password = "PSKXEP69NN##2";
const char* ssid = "TT_29B0";
const char* password = "f7crdgg32n";


const char* mqtt_server = "192.168.1.17";
const char* device_name = "Noeuds2";
const char* mqtt_username = "bank"; // MQTT username
const char* mqtt_password = "bank"; // MQTT password

const char* ReceiveTopic = "output";
//const char* SendTopic = "My_Topic";

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);
long lastMsg = 0;
char msg[50];
int value = 0;



void setup() {
  Serial.begin(115200);

  pinMode(LED_R2, OUTPUT); // set ESP32 pin to output mode
  pinMode(LED_V2, OUTPUT); // set ESP32 pin to output mode

  digitalWrite(LED_R2, LOW); // turn off LED
  digitalWrite(LED_V2, LOW); // turn off LED

   setup_wifi();
   client.setCallback(callback);


}




void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
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
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageR;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageR += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "output") {
    Serial.print("Changing output to ");
    if(messageR == "on"){
      //client.publish("esp32/log", "On received");
      Serial.println("on");
      digitalWrite(LED_R2, HIGH);
      digitalWrite(LED_V2, LOW);
    }
    else if(messageR == "off"){
      //client.publish("esp32/log", "Off received");
      Serial.println("off");
      digitalWrite(LED_R2, LOW);
      digitalWrite(LED_V2, HIGH);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(device_name, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
