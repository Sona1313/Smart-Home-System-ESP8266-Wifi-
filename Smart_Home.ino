#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char SSID[] = "OnePlus";      
const char PASS[] = "29032003";  

const char DEVICE_LOGIN_NAME[] = "75ef1d41-fba8-4ede-b86b-ddfde0bd93a5";
const char DEVICE_KEY[] = "XqJDEmG@eLmBGPOjNrDeUv1hq";


const int FAN_PIN = 12;   
const int LIGHT_PIN = 14;

void onFanChange();
void onLightChange();

CloudSwitch fan;
CloudSwitch light;

void initProperties() {
    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
    ArduinoCloud.addProperty(fan, READWRITE, ON_CHANGE, onFanChange);
    ArduinoCloud.addProperty(light, READWRITE, ON_CHANGE, onLightChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void setup() {
    Serial.begin(9600);
    
    pinMode(FAN_PIN, OUTPUT);   
    pinMode(LIGHT_PIN, OUTPUT); 
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Wi-Fi Connected");
    
    initProperties();  
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    setDebugMessageLevel(4);
    ArduinoCloud.printDebugInfo();
}

void loop() {
    ArduinoCloud.update();  

    if (WiFi.status() != WL_CONNECTED) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
        }
        Serial.println("Wi-Fi Reconnected");
    }

    if (ArduinoCloud.connected()) {
        Serial.println("Cloud Connected");
    } else {
        Serial.println("Cloud Not Connected");
        ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    }
    delay(1000);
}

void onFanChange() {
    digitalWrite(FAN_PIN, fan ? HIGH : LOW); 
}

void onLightChange() {
    digitalWrite(LIGHT_PIN, light ? HIGH : LOW);
}