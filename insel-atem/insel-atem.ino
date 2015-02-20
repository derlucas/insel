#include <SPI.h>
#include <Ethernet.h>
#include <ATEMbase.h>
#include <ATEMkey.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };
IPAddress ip(192,168,10,99);

ATEMkey AtemSwitcher;
bool AtemOnline = false;
int pushButton = 0;


void handleTally() {
  digitalWrite(3, AtemSwitcher.getTallyByIndexTallyFlags(2) & 0x01);
  digitalWrite(4, AtemSwitcher.getTallyByIndexTallyFlags(3) & 0x01);
  digitalWrite(5, AtemSwitcher.getTallyByIndexTallyFlags(4) & 0x01);
}


void setKeyer(uint8_t program, bool onoff) {
  if(onoff) AtemSwitcher.setKeyerFillSource(0, 0, program);
  AtemSwitcher.setKeyerOnAirEnabled(0, 0, onoff);
}

void setupKeyer() {
  AtemSwitcher.setKeyerType(0, 0, 1);
  AtemSwitcher.setKeyChromaHue(0, 0, 1825);
  AtemSwitcher.setKeyChromaGain(0, 0, 1000);
  AtemSwitcher.setKeyChromaYSuppress(0, 0, 1000);
  AtemSwitcher.setKeyChromaLift(0, 0, 0);
  AtemSwitcher.setKeyChromaNarrow(0, 0, false);
}


void handleButtons() {

  if (digitalRead(A0) == LOW)  {
    if (pushButton != 1)  {
      pushButton = 1;
      Serial.println("Select 1");
      AtemSwitcher.setProgramInputVideoSource(0,3);
    }
  } else if (pushButton == 1) {
    pushButton = 0; 
  } else if (digitalRead(A2) == LOW)  {
    if (pushButton != 2)  {
      pushButton = 2;
      Serial.println("Select 2");
      AtemSwitcher.setProgramInputVideoSource(0,4);
    }
  } else if (pushButton == 2) {
    pushButton = 0; 
  } else if (digitalRead(A4) == LOW)  {
    if (pushButton != 3)  {
      pushButton = 3;
      Serial.println("Select 3");
      AtemSwitcher.setProgramInputVideoSource(0,5);
    }
  } else if (pushButton == 3) {
    pushButton = 0; 
  }
  
  
  // check the small green push buttons for keying in
  if (digitalRead(A1) == LOW)  {
    if (pushButton != 4)  {
      pushButton = 4;
      Serial.println("Mix 1");
      //setKeyer(3, true);
      AtemSwitcher.setProgramInputVideoSource(0,6);
    }
  } else if (pushButton == 4) {
    pushButton = 0; 
    //setKeyer(3, false);
  } 
  
  if (digitalRead(A3) == LOW)  {
    if (pushButton != 5)  {
      pushButton = 5;
      Serial.println("Mix 2");
      setKeyer(4, true);
    }
  } else if (pushButton == 5) {
    pushButton = 0; 
    setKeyer(4, false);
  }
 
  if (digitalRead(A5) == LOW)  {
    if (pushButton != 6)  {
      pushButton = 6;
      Serial.println("Mix 3");
      setKeyer(5, true);      
    }
  } else if (pushButton == 6) {
    pushButton = 0; 
    Serial.println("Mix 3 aus");
    setKeyer(5, false);
  }
}



void setup() {

  pinMode(A0, INPUT_PULLUP); pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP); pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP); pinMode(A5, INPUT_PULLUP);
  
  pinMode(2, OUTPUT); pinMode(3, OUTPUT);
  pinMode(4, OUTPUT); pinMode(5, OUTPUT);
  
  Serial.begin(115200);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Initialize a connection to the switcher:
  AtemSwitcher.begin(IPAddress(192, 168, 10, 240), 56417);
  AtemSwitcher.serialOutput(0x80);
  AtemSwitcher.connect();
  
  for(int i = 0; i< 5; i++) {
    delay(200);
    digitalWrite(2, HIGH); digitalWrite(3, HIGH);
    digitalWrite(4, HIGH); digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(2, LOW); digitalWrite(3, LOW);
    digitalWrite(4, LOW); digitalWrite(5, LOW);
  }

}



void loop() {

  AtemSwitcher.runLoop();
  
  
  if (AtemSwitcher.hasInitialized())  {
    if (!AtemOnline)  {
      AtemOnline = true;
          
      // turn on the green status led to indicate a connection
      digitalWrite(2, HIGH);
      AtemSwitcher.setProgramInputVideoSource(0,3);
      
      // configure the keyer accordingly
      setupKeyer();                  
    }
    
    handleButtons();
    handleTally();
    
  } else {
    // at this point the ATEM is not connected and initialized anymore

    if (AtemOnline) {
      AtemOnline = false;
        
      // turn off the green connection idicator
      digitalWrite(2, LOW);
      Serial.println("conn timeout");
    }     
  }
  
}



