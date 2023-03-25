  #include <ArduinoBLE.h>

BLEService newService("180A"); // creating the service
BLEByteCharacteristic switchChar("2A57", BLERead | BLEWrite); // creating the LED characteristic

const int ledPin = 2;
long previousMillis = 0;



void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
    pinMode(ledPin, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

    //initialize BLE library
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("Arduino NANO RP2040"); //Setting a name that will appear when scanning for bluetooth devices
  BLE.setAdvertisedService(newService);
  newService.addCharacteristic(switchChar); //add characteristics to a service  
  BLE.addService(newService);  // adding the service

  switchChar.writeValue(0); //set initial value for characteristics
  BLE.advertise(); //start advertising the service
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central(); // wait for a BLE central
 if (central) {  // if a central is connected to the peripheral
    Serial.print("Connected to central: ");
    
    Serial.println(central.address()); // print the central's BT address
    
    digitalWrite(LED_BUILTIN, HIGH); // turn on the LED to indicate the connection

    // check the battery level every 200ms
    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      
      if (currentMillis - previousMillis >= 200) { // if 200ms have passed, we check the battery level
        previousMillis = currentMillis;

        if (switchChar.written()) {
          Serial.println(switchChar.value());
          if ( String(switchChar.value()) == "48" || switchChar.value() == 0 ) {   // any value other than 0  == 48 é 0 em utf8
               // will turn the LED off
             Serial.println(F("LED off"));
            digitalWrite(ledPin, LOW);      
          } else {// will turn the LED on
            Serial.println("LED on");
            digitalWrite(ledPin, HIGH);                               // a 0 value
                    
          }
        }

      }
    }
   digitalWrite(LED_BUILTIN, LOW); // when the central disconnects, turn off the LED
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
