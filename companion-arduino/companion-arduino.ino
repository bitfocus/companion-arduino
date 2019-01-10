/*
 * This file is part of the Companion project
 * Copyright (c) 2018 Bitfocus AS
 * Authors: William Viker <william@bitfocus.io>, Håkon Nessjøen <haakon@bitfocus.io>
 *
 * This program is free software.
 * You should have received a copy of the MIT licence as well as the Bitfocus
 * Individual Contributor License Agreement for companion along with
 * this program.
 *
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the Companion software without
 * disclosing the source code of your own applications.
 *
 * Project: Companion-Arduino
 * Version: 1.2.0
 *
 */

// This projects is only tested on a "Arduino Ethernet" board. Please let us
// know if you find it working with other boards, like arduino with ethernet
// shield, etc. It should work.


#include <SPI.h>
#include <Ethernet.h>

// Which Pins do you use for input? To trigger them, they need to be shorted
// against a GND pin.
int inputPin_1 = 9;
int inputPin_2 = 8;
int inputPin_3 = 7;
int inputPin_4 = 6;

// Custom MAC address for your arduino
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// IP address for the arduino
IPAddress ip(10,255,255,254);

// IP address for the computer running companion
IPAddress server(10,255,255,253);

EthernetClient client;
char input[4];

void setup() {

  Ethernet.begin(mac, ip);

  // give the Ethernet shield a second to initialize:
  delay(1000);

	// The four inputs needs to be set to PULLUP mode.
  pinMode(inputPin_1, INPUT_PULLUP);
  pinMode(inputPin_2, INPUT_PULLUP);
  pinMode(inputPin_3, INPUT_PULLUP);
  pinMode(inputPin_4, INPUT_PULLUP);

	// Set default value to 1 on the four inputs
  memset(&input,1,4);

	// Try connecting until we're successful
  while (!client.connect(server, 51234)) {
		delay(100);
  }

}

void loop() {

	// Let's just read so the input buffer doesn't get too big(?)
  if (client.available()) {
    char c = client.read();
  }

  if (client.connected()) {

		// Read port 1
    if (digitalRead(inputPin_1) != input[0]) {
      input[0] = digitalRead(inputPin_1);

      if (input[0] == 0) {
        client.println("bank-press 99 1");
      }

      delay(50);
    }

		// Read port 2
    if (digitalRead(inputPin_2) != input[1]) {
      input[1] = digitalRead(inputPin_2);

      if (input[1] == 0) {
        client.println("bank-press 99 2");
      }

      delay(50);
    }

		// Read port 3
    if (digitalRead(inputPin_3) != input[2]) {
      input[2] = digitalRead(inputPin_3);

      if (input[2] == 0) {
        client.println("bank-press 99 3");
      }

      delay(50);
    }

		// Read port 4
    if (digitalRead(inputPin_4) != input[3]) {
      input[3] = digitalRead(inputPin_4);

      if (input[3] == 0) {
        client.println("bank-press 99 4");
      }
      delay(50);

    }
  }

  if (!client.connected()) {
    client.stop();

		// Try reconnecting until we're connected again.
    while(!client.connect(server, 51234)) {
      delay(100);
    }

  }

	// Calma..
  delay(1);
}
