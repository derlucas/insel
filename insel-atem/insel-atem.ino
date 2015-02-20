
#include <SPI.h>
#include <Ethernet.h>
#include <ATEM.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };
IPAddress ip(192,168,10,99);

// Connect to an ATEM switcher on this address and using this local port:
// The port number is chosen randomly among high numbers.
ATEM AtemSwitcher(IPAddress(192, 168, 10, 240), 56417);


void setup() {

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  Serial.begin(9600);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Initialize a connection to the switcher:
  AtemSwitcher.serialOutput(false);
  AtemSwitcher.connect();
  
  for(int i = 0; i< 5; i++) {
    delay(200);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }

}

int pushButton = 0;
boolean useAtem = true;

void loop() {

  if(useAtem) {
    AtemSwitcher.runLoop();
  
    digitalWrite(2, AtemSwitcher.isConnected());
    digitalWrite(3, AtemSwitcher.getProgramTally(3));
    digitalWrite(4, AtemSwitcher.getProgramTally(4));
    digitalWrite(5, AtemSwitcher.getProgramTally(5));
  }
  
  // der große Button zum wechseln des Live Programms
  if (digitalRead(A0) == LOW)  {
    if (pushButton != 1)  {
      pushButton = 1;
      Serial.println("Select 1");
      if(useAtem) AtemSwitcher.changeProgramInput(3);
    }
  } else if (pushButton == 1) {
    pushButton = 0; 
  } else if (digitalRead(A2) == LOW)  {
    if (pushButton != 2)  {
      pushButton = 2;
      Serial.println("Select 2");
      if(useAtem) AtemSwitcher.changeProgramInput(4);
    }
  } else if (pushButton == 2) {
    pushButton = 0; 
  } else if (digitalRead(A4) == LOW)  {
    if (pushButton != 3)  {
      pushButton = 3;
      Serial.println("Select 3");
      if(useAtem) AtemSwitcher.changeProgramInput(5);
    }
  } else if (pushButton == 3) {
    pushButton = 0; 
  }
  
  
  // der kleine Button zum Mischen des Signals ins Programm
  if (digitalRead(A1) == LOW)  {
    if (pushButton != 4)  {
      pushButton = 4;
      Serial.println("Mix 1");
      //AtemSwitcher.changeProgramInput(1);
    }
  } else if (pushButton == 4) {
    pushButton = 0; 
  } 
  
  if (digitalRead(A3) == LOW)  {
    if (pushButton != 5)  {
      pushButton = 5;
      Serial.println("Mix 2");
      //AtemSwitcher.changeProgramInput(2);
    }
  } else if (pushButton == 5) {
    pushButton = 0; 
  }
 
  if (digitalRead(A5) == LOW)  {
    if (pushButton != 6)  {
      pushButton = 6;
      Serial.println("Mix 3");
    }
  } else if (pushButton == 6) {
    pushButton = 0; 
    Serial.println("Mix 3 aus");
  }
  
  
  if (useAtem && AtemSwitcher.isConnectionTimedOut())  {
    digitalWrite(2, LOW);
    AtemSwitcher.connect();
  }
  
}

