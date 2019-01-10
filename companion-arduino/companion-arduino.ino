#include <SPI.h>
#include <Ethernet.h>


int inputPin_1 = 9;
int inputPin_2 = 8;
int inputPin_3 = 7;
int inputPin_4 = 6;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(10,255,255,254);
IPAddress server(10,255,255,253);
EthernetClient client;
char input[4];

void setup() {
  Ethernet.begin(mac, ip);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  pinMode(inputPin_1, INPUT_PULLUP);
  pinMode(inputPin_2, INPUT_PULLUP);
  pinMode(inputPin_3, INPUT_PULLUP);
  pinMode(inputPin_4, INPUT_PULLUP);

  memset(&input,1,4);
  
  if (client.connect(server, 10002)) {
    //Serial.println("connected");
  } 
  else {
    //Serial.println("connection failed");
  }
}

void loop() {

  if (client.available()) {
    char c = client.read();
  }


  if (client.connected()) {
    if (digitalRead(inputPin_1) != input[0]) {
      input[0] = digitalRead(inputPin_1);

      if (input[0] == 0) {
        client.println("bank-press 99 1");
      }
      delay(50);
    }

    if (digitalRead(inputPin_2) != input[1]) {
      input[1] = digitalRead(inputPin_2);

      if (input[1] == 0) {
        client.println("bank-press 99 2");
      }
      delay(50);
    }

    if (digitalRead(inputPin_3) != input[2]) {
      input[2] = digitalRead(inputPin_3);

      if (input[2] == 0) {
        client.println("bank-press 99 3");
      }
      delay(50);
    }

    if (digitalRead(inputPin_4) != input[3]) {
      input[3] = digitalRead(inputPin_4);

      if (input[3] == 0) {
        client.println("bank-press 99 4");
      }
      delay(50);
    }
  }


  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    client.stop();
    while(!client.connect(server, 51234)) {
      delay(1);
    }
  }

  delay(1);
}




