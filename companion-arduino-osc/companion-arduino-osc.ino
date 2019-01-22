
#include <SPI.h>
#include <Ethernet.h>
#include <Ethernet.h>
#include <ArdOSC.h> // You need to install the ArdOSC library for arduino :)

// Arduino input pins (you need to short them to GND to "press")
char inputPin[] = {
  9,
  8,
  7,
  6
};

// Which companion button do you want to link this to?
char companionButton[] = {
  99, 1,                       // First input pin -> Page 99, Bank 1
  99, 2,                       // Second input pin -> Page 99, Bank 2
  99, 3,                       // Third input pin -> Page 99, Bank 3
  99, 4                        // Fourth input pin -> Page 99, Bank 4
};

char input[sizeof(inputPin)];

// Arduino IP and custom HW address.
IPAddress ip(10, 255, 255, 254);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Companion IP
byte destIp[]  = { 10, 255, 255, 253 };

// Companion OSC Port
int destPort = 12321; // Companion OSC port

// YOU DON'T NEED TO EDIT ANYTHING BELOW THIS LINE IF YOU DON'T KNOW
// WHAT YOU'RE DOING :)

// Establish a OSC client
OSCClient client;

// Create new OSC message
OSCMessage oscMessage;

void setup() {

  Ethernet.begin(mac, ip);

  // give the Ethernet shield a second to initialize:
  delay(1000);

  // The four inputs needs to be set to PULLUP mode.
  for (char i = 0; i < sizeof(inputPin); i++) {
    pinMode(inputPin[i], INPUT_PULLUP);
  }

  // Set default value to 1 on the four inputs
  memset(&input, 1, sizeof(input));

}

void loop() {

  for (char i = 0; i < sizeof(inputPin); i++) {

    char reading = digitalRead(inputPin[i]);

    if (reading != input[i]) {

      input[i] = reading;

      // If button is pressed
      if (input[i] == 0) {
        press_bank(companionButton[i*2], companionButton[(i*2)+1], 1);
      }

      // If button is released
      else {
        press_bank(companionButton[i*2], companionButton[(i*2)+1], 0);
      }

      delay(50); // Wait, there might be noise.

    }

  }

}

void press_bank(int page, int bank, int dir) {

   // Set destination for the OSC packet.
  oscMessage.setAddress(destIp, destPort);

  // Build the OSC packet
  String oscstring = String("/press/bank/") + String(page) + String("/") + String(bank);
  oscMessage.beginMessage(oscstring.c_str());
  oscMessage.addArgInt32(dir);

  // Send the OSC packet
  client.send(&oscMessage);

  oscMessage.flush(); //object data clear

}
