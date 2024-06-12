#include <ArduinoBLE.h>

// UUIDs for services and characteristics
BLEService geoService("cd48409a-f3cc-11ed-a05b-0242ac120003");
BLECharacteristic geoCharacteristic("cd48409a-f3cc-11ed-a05b-0242ac120003", BLERead | BLEWrite, 20);
BLECharacteristic startStopCharacteristic("cd484232-f3cc-11ed-a05b-0242ac120003", BLERead | BLEWrite, 1);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("drone_arduino");
  BLE.setAdvertisedService(geoService);
  
  geoService.addCharacteristic(geoCharacteristic);
  geoService.addCharacteristic(startStopCharacteristic);
  
  BLE.addService(geoService);

  BLE.advertise();

  Serial.println("BLE Geolocation Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (geoCharacteristic.written()) {
        const uint8_t* receivedValue = geoCharacteristic.value();
        size_t length = geoCharacteristic.valueLength();
        
        char receivedData[length + 1];
        memcpy(receivedData, receivedValue, length);
        receivedData[length] = '\0'; 
        
        Serial.println("Received geolocation: " + String(receivedData));
      }

      if (startStopCharacteristic.written()) {
        uint8_t command = startStopCharacteristic.value()[0];
        if (command == 0x01) {
          Serial.println("Received start command");
        } else if (command == 0x00) {
          Serial.println("Received stop command");
        }
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
