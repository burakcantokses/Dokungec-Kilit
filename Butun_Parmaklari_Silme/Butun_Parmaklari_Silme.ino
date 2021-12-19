#include <Adafruit_Fingerprint.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 6);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
  while (!Serial); 
  delay(100);

  Serial.println("\n\nBütün Parmakları Silme Paneli");
  Serial.println("Parmakları silmek icin 'Y' tuşuna basın!");

  while (1) {
    if (Serial.available() && (Serial.read() == 'Y')) {
      break;
    }
  }

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Parmak izi sensöru bulundu!");
  } else {
    Serial.println("Parmak izi sensöru bulunamadi :(");
    while (1);
  }
  
  finger.emptyDatabase();

  Serial.println("Bütün parmaklar uzaya gönderildi :)");
}

void loop() {
}
