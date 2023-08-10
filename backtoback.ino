#include <WiFi.h>
#include <PubSubClient.h>

#define POTENTIOMETER_PIN 34 // ESP32 pin GIOP36 (ADC0) connected to Potentiometer pin
#define LED_R           18 // ESP32 pin GIOP21 connected to LED's pin
#define LED_O           5 // ESP32 pin GIOP21 connected to LED's pin
#define LED_V           19 // ESP32 pin GIOP21 connected to LED's pin
#define BUZZER           4 // ESP32 pin GIOP21 connected to BUZZER's pin

#define LED_R2           21 // ESP32 pin GIOP21 connected to LED's pin
#define LED_V2           2 // ESP32 pin GIOP21 connected to LED's pin


#define ANALOG_THRESHOLD  4000
#define ANALOG_RISK 1000

// Replace the next variables with your SSID/Password combination
const char* ssid = "TT_29B0";
const char* password = "f7crdgg32n";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.17";
const char* sending_topic = "My_Topic";
const char* mqtt_username = "bank"; // MQTT username
const char* mqtt_password = "bank"; // MQTT password
const char* clientID = "Noeuds1"; // MQTT client ID

WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float flame = 0;


void setup() {
  Serial.begin(115200);
 
  setup_wifi();
  //client.setCallback(callback);
pinMode(LED_R, OUTPUT); // set ESP32 pin to output mode
  pinMode(LED_O, OUTPUT); // set ESP32 pin to output mode
  pinMode(LED_V, OUTPUT); // set ESP32 pin to output mode
  pinMode(BUZZER, OUTPUT); // set ESP32 pin to output mode
  pinMode(LED_R2, OUTPUT); // set ESP32 pin to output mode
  pinMode(LED_V2, OUTPUT); // set ESP32 pin to output mode


  digitalWrite(LED_R, LOW); // turn off LED
  digitalWrite(LED_O, LOW); // turn off LED
  digitalWrite(LED_V, LOW); // turn off LED
  digitalWrite(BUZZER, LOW); // turn off BUZZER
  digitalWrite(LED_R2, LOW); // turn off LED
  digitalWrite(LED_V2, LOW); // turn off LED
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

/*void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}*/

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Subscribe
      //client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  flame = analogRead(POTENTIOMETER_PIN);  
  //char tempString[8];
  //dtostrf(flame, 1, 2, tempString);

  if (flame > ANALOG_THRESHOLD)
  {
  led_R_Clign();
  digitalWrite(LED_O, LOW); // turn on LED
  digitalWrite(LED_V, LOW); // turn on LED
  buzz_Clign();
  client.publish(sending_topic,"FLAME"); 
  Serial.println("Incendie !!");   
  }
  else if (flame > ANALOG_RISK && flame < ANALOG_THRESHOLD)
  {
  digitalWrite(LED_R, LOW); // turn on LED
  led_O_Clign();
  digitalWrite(LED_V, LOW); // turn on LED
  digitalWrite(BUZZER, LOW); // turn on BUZZER  
  client.publish(sending_topic,"RISK"); 
  Serial.println("Fuite de gaz detecté");   

  }
  else
  {
  digitalWrite(LED_R, LOW); // turn on LED
  digitalWrite(LED_O, LOW); // turn on LED
  digitalWrite(LED_V, HIGH); // turn on LED
  digitalWrite(BUZZER, LOW); // turn on BUZZER
  client.publish(sending_topic,"SAFE"); 
  Serial.println("pas d'incendie detecté");
  }
  Serial.print("Flame: ");   
  Serial.println(flame);
  delay(100);

}

void led_R_Clign(){
  digitalWrite(LED_R, HIGH); // turn on LED
  delay(200);
  digitalWrite(LED_R, LOW); // turn off LED
}
void led_O_Clign(){
  digitalWrite(LED_O, HIGH); // turn on LED
  delay(200);
  digitalWrite(LED_O, LOW); // turn off LED
}
void buzz_Clign(){
  digitalWrite(BUZZER, HIGH); // turn on LED
  delay(200);
  digitalWrite(BUZZER, LOW); // turn off LED
}
