#include <Adafruit_Fingerprint.h>

/*************************************************************/
#define BLYNK_PRINT Serial
#include <Adafruit_Fingerprint.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#define BLYNK_MAX_SENDBYTES 256
//d4 + d5
SoftwareSerial mySerial(2, 14);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

/*************************************************************/
int i = 0;

int bs = 0 , j = 0;
String ids = " ";
double d;

/*************************************************************/
char auth[] = "0490cbaf2d0c4346821e69949bd173e8";
char ssid[] = "androidAP";
char pass[] = "Success7056";

/*************************************************************/
void setup() {
  Wire.begin(D1, D2);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.print("Welcome to smart"); // Start Printing
  lcd.setCursor(0, 1);
  lcd.print("attendance system");
  delay(1500);
  lcd.clear();
  // Debug console
  Serial.begin(9600);
  // while (!Serial);  // For Yun/Leo/Micro/Zero/...
  // delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.print("sensor Found ");
    tone(15, 150, 200);
    delay(1000);
    lcd.clear();
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.print("sensor not found");
    lcd.setCursor(1, 1);
    lcd.print("check & restart");
    while (1) {
      delay(1);
    }
  }
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V3, "clr");
  Blynk.virtualWrite(V3 , "add" , 0 , "enrolled", i);
}
/*************************************************************/
void loop() {
  Blynk.run();
  if (bs == HIGH) {

    
    getFingerprintIDez();
    delay(50);
  }
}
/*************************************************************/
//from lib
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    // while (! Serial.available());
    num = d;//Serial.parseInt();
  }
  return num;
}
/*************************************************************/
uint8_t getFingerprintEnroll() {

  lcd.clear();

  lcd.print("ENROLL NOW");
  lcd.setCursor(1, 1);
  lcd.print("id#" + String(id));
  delay(1700);
  lcd.clear();
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK: {
          lcd.clear();
          Serial.println("Image taken");
          lcd.print("Image taken");
          tone(15, 50, 100);
          delay(1000);

          break;
        }
      case FINGERPRINT_NOFINGER: {
          lcd.clear();
          tone(15, 50, 100);
          Serial.println(".");
          lcd.print("Waiting......");

        }
        break;
      case FINGERPRINT_PACKETRECIEVEERR: {
          lcd.clear();
          tone(15, 160, 100);
          delay(500);
          tone(15, 160, 100);
          Serial.println("Communication error");
          lcd.print("ERROR !!!!");
          tone(15, 50, 400);
          delay(1000);

          break;
        }
      case FINGERPRINT_IMAGEFAIL: {
          lcd.clear();
          tone(15, 160, 100);
          delay(500);
          tone(15, 160, 100);
          Serial.println("Imaging error");
          lcd.print("Imaging error");
          tone(15, 50, 500);
          delay(1000);

          break;
        }
      default: {
          lcd.clear();
          tone(15, 160, 100);
          delay(500);
          tone(15, 160, 100);
          Serial.println("Unknown error");
          lcd.print("Unknown error");
          delay(1000);

          break;
        }
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK: {
        lcd.clear();
        tone(15, 160, 300);
        Serial.println("Image converted");
        lcd.print("Image converted");
        delay(1000);

        break;
      }
    case FINGERPRINT_IMAGEMESS: {
        lcd.clear();
        tone(15, 160, 100);
        delay(500);
        tone(15, 160, 100);
        Serial.println("Image too messy");
        lcd.print("Image too messy");
        delay(1000);
        return p;
      }
    case FINGERPRINT_PACKETRECIEVEERR: {
        lcd.clear();
        tone(15, 160, 100);
        delay(1000);
        tone(15, 160, 100);
        Serial.println("Communication error");
        lcd.print("ERROR!!!");
        delay(1000);

        return p;
      }
    case FINGERPRINT_FEATUREFAIL: {
        lcd.clear();
        tone(15, 160, 100);
        delay(500);
        tone(15, 160, 100);
        Serial.println("Could not find fingerprint features");
        lcd.print("no fingerprint");
        lcd.setCursor(1, 1);
        lcd.print("features");
        delay(1000);

        return p;
      }
    case FINGERPRINT_INVALIDIMAGE: {
        lcd.clear();
        tone(15, 160, 100);
        delay(500);
        tone(15, 160, 100);
        Serial.println("Could not find fingerprint features");
        lcd.print("no fingerprint");
        lcd.setCursor(1, 1);
        lcd.print("features");
        delay(1000);

        return p;
      }
    default: {
        lcd.clear();
        tone(15, 160, 100);
        delay(500);
        tone(15, 160, 100);
        Serial.println("Unknown error");
        lcd.print("Unknown error");
        delay(1000);

        return p;
      }
  }

  Serial.println("Remove finger");
  lcd.print("Remove finger");
  tone(15, 160, 100);
  delay(1500);
  lcd.clear();
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  lcd.clear();
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  tone(15, 160, 100);
  lcd.print("place same");
  lcd.setCursor(1, 1);
  lcd.print("finger again");
  delay(1000);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK: {
          lcd.clear();
          Serial.println("Image taken");
          lcd.print("Image taken");
          tone(15, 160, 100);
          delay(1000);

        }
        break;
      case FINGERPRINT_NOFINGER: {
          lcd.print("waiting...");
          Serial.print(".");
          delay(10);
          lcd.clear();
          break;
        }
      case FINGERPRINT_PACKETRECIEVEERR: {
          lcd.clear();
          Serial.println("Communication error");
          lcd.print("ERROR!!!");
          delay(1000);
          break;
        }
      case FINGERPRINT_IMAGEFAIL: {
          lcd.clear();
          Serial.println("Imaging error");
          lcd.print("Imaging error");
          tone(15, 160, 100);
          delay(1000);

          break;
        }
      default: {
          lcd.clear();
          Serial.println("Unknown error");
          lcd.print("Unknown error");
          tone(15, 160, 100);
          delay(1000);

          break;
        }
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK: {
        tone(15, 160, 300);
        lcd.clear();
        Serial.println("Image converted");
        lcd.print("Image converted");
        delay(1000);

        break;
      }
    case FINGERPRINT_IMAGEMESS: {
        lcd.clear();
        Serial.println("Image too messy");
        lcd.print("Image too messy");
        tone(15, 160, 100);
        delay(1000);

        return p;
      }
    case FINGERPRINT_PACKETRECIEVEERR: {
        lcd.clear();
        Serial.println("Communication error");
        lcd.print("ERROR!!!");
        tone(15, 160, 100);
        delay(1000);

        return p;
      }
    case FINGERPRINT_FEATUREFAIL: {
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        lcd.setCursor(1, 1);
        lcd.print("finger features");
        delay(1000);

        return p;
      }
    case FINGERPRINT_INVALIDIMAGE: {
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        lcd.setCursor(1, 1);
        lcd.print("finger features");
        delay(1000);

        return p;
      }
    default: {
        lcd.clear();
        Serial.println("Unknown error");
        lcd.print("Unknown error");
        delay(200);

        return p;
      }
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);
  lcd.print("Created model");
  lcd.setCursor(1, 1);
  lcd.print("for id#" + String(id));
  delay(1000);
  lcd.clear();
  tone(15, 35, 150);
  delay(1000);
  tone(15, 35, 150);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    Serial.println("Prints matched!");
    lcd.print("Prints matched!");
    delay(1000);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    Serial.println("Communication error"); {
      lcd.print("ERROR!!!");
      delay(1000);
      return p;
    }
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    Serial.println("Fingerprints did not match"); {
      lcd.print("DONT MATCH!!!");
      delay(1000);

      return p;
    }
  } else {
    lcd.clear();
    Serial.println("Unknown error");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    Serial.println("Stored!");
    lcd.print("Stored!");
    delay(1000);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    Serial.println("Communication error");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    Serial.println("Could not store in that location");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    Serial.println("Error writing to flash");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  } else {
    lcd.clear();
    Serial.println("Unknown error");
    lcd.print("Unknown error");
    delay(1000);

    return p;
  }
}
/*************************************************************/
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK: {
        lcd.clear();
        Serial.println("Image taken");
        lcd.print("Image taken");
        delay(1000);

        break;
      }
    case FINGERPRINT_NOFINGER: {
        lcd.clear();
        Serial.println("No finger detected");
        lcd.print("!finger detected");
        delay(1000);

        return p;
      }
    case FINGERPRINT_PACKETRECIEVEERR: {
        lcd.clear();
        Serial.println("Communication error");
        lcd.print("ERROR!!!");
        delay(1000);

        return p;
      }
    case FINGERPRINT_IMAGEFAIL: {
        lcd.clear();
        Serial.println("Imaging error");
        lcd.print("Imaging error");
        delay(1000);

        return p;
      }
    default: {
        lcd.clear();
        Serial.println("Unknown error");
        lcd.print("ERROR!!!");
        delay(1000);

        return p;
      }
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK: {
        lcd.clear();
        Serial.println("Image converted");
        lcd.print("Image converted");
        delay(1000);

        break;
      }
    case FINGERPRINT_IMAGEMESS: {
        lcd.clear();
        Serial.println("Image too messy");
        lcd.print("Image too messy");
        delay(1000);

        return p;
      }
    case FINGERPRINT_PACKETRECIEVEERR: {
        lcd.clear();
        Serial.println("Communication error");
        lcd.print("ERROR!!!");
        delay(1000);

        return p;
      }
    case FINGERPRINT_FEATUREFAIL: {
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        lcd.setCursor(1, 1);
        lcd.print("finger features");
        delay(1000);

        return p;
      }
    case FINGERPRINT_INVALIDIMAGE: {
        lcd.clear();
        Serial.println("Could not find fingerprint features");
        lcd.print("Could not find");
        lcd.setCursor(1, 1);
        lcd.print("finger features");
        delay(1000);

        return p;
      }
    default: {
        lcd.clear();
        Serial.println("Unknown error");
        lcd.print("ERROR!!!");
        delay(1000);

        return p;
      }
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    Serial.println("Found a print match!");
    lcd.print("MATCH FOUND");
    delay(1000);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    Serial.println("Communication error");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.clear();
    Serial.println("Did not find a match");
    lcd.print("NO MATCH");
    delay(1000);

    return p;
  } else {
    lcd.clear();
    Serial.println("Unknown error");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  }
  lcd.clear();
  // found a match!
  Serial.print("Found ID #");
  lcd.print("FOUND ID #" + String(finger.fingerID));

  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  delay(1000);

  return finger.fingerID;
}
/*************************************************************/
int getFingerprintIDez() {
   lcd.clear();
  lcd.print("Hello");
  lcd.setCursor(0, 1);
  lcd.print("Enroll now");
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
 lcd.clear();
  Serial.print("Found ID #");
  lcd.print("FOUND ID #" + String(finger.fingerID));
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  i++;
  Blynk.virtualWrite(V3 , "update" , 0 , "enrolled", i);
  Blynk.virtualWrite(V3 , "pick", 0);
  ids +=   String(finger.fingerID) + " - "  ;
  Blynk.virtualWrite(V3 , "add" , i , "student", finger.fingerID);
  delay(1000);

  return finger.fingerID;
}
/*************************************************************/
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    lcd.clear();
    Serial.println("Deleted!");
    lcd.print("Deleted!");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    Serial.println("Communication error");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    Serial.println("Could not delete in that location");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    Serial.println("Error writing to flash");
    lcd.print("ERROR!!!");
    delay(1000);

    return p;
  } else {
    lcd.clear();
    lcd.print("ERROR!!!");
    tone(15, 196, 123);
    Serial.print("Unknown error: 0x");
    lcd.print("id exist/!found");
    Serial.println(p, HEX);

    return p;
  }
}
/*************************************************************/
BLYNK_WRITE(V6) {
  lcd.clear();
  Serial.println("email sent."); // This can be seen in the Serial Monitor
  Blynk.email( String(i) + " students ids", String(i) + " students present: " + ids);
  lcd.print("<-email sent->");
  Serial.println(String(i) + " students ids: " + ids);
  Blynk.virtualWrite(V3, "clr");
  delay(1000);

}
/*************************************************************/
BLYNK_WRITE(V2) {

  switch (param.asInt()) {
    case 4: {
        lcd.clear();
        tone(15, 100, 100);
        Serial.println("Ready to enroll a fingerprint!");
        Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
        id = readnumber();
        if (id == 0) {// ID #0 not allowed, try again!
          return;
        }
        lcd.setCursor(0, 0);
        Serial.print("Enrolling ID #");
        Serial.println(id);
        lcd.print("Enrolling ID #" + String(id));

        while (!  getFingerprintEnroll() );
        break;
      }

    case 3: {
        lcd.clear();
        tone(15, 200, 100);
        Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
        lcd.setCursor(0, 0);
        lcd.print("enter id to del");

        uint8_t id = readnumber();
        delay(1000);
        lcd.clear();
        if (id == 0) {// ID #0 not allowed, try again!
          return;
        }

        Serial.print("Deleting ID #");
        Serial.println(id);
        lcd.print("Deleting ID #");
        deleteFingerprint(id);
        delay(1000);

        break;
      }

    case 2: {
        lcd.clear();
        tone(15, 160, 100);
        delay(500);
        tone(15, 32, 200);
        delay(500);
        tone(15, 200, 300);
        bs = LOW;
        lcd.print("system closed");
        break;
      }
    case 1: {
        lcd.clear();
        tone(15, 50, 100);
        finger.getTemplateCount();
        Serial.print("Sensor contains ");
        Serial.print(finger.templateCount);
        Serial.println(" templates");
        Serial.println("Waiting for valid finger...");
        lcd.print("Hello");
        lcd.setCursor(0, 1);
        lcd.print("Enroll now");
        bs = HIGH;
      }

  }

}
/*************************************************************/
BLYNK_WRITE(V1) {
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  d = param.asDouble();
  
}


