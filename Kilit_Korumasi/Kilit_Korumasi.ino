#include <Wire.h> 
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C_AvrI2C.h>
#include <SoftwareSerial.h>

const int led_pin_accept = 8;
const int led_pin_deny = 9;
int ledRed = 8;
int ledGreen = 9;
int buzzer = 10;
int kilit = 5;
volatile int finger_status = -1;

LiquidCrystal_I2C_AvrI2C lcd(0x27,16,2);
SoftwareSerial mySerial(7, 6);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
  pinMode(led_pin_accept,OUTPUT);
  pinMode(led_pin_deny,OUTPUT);
  pinMode(ledRed + ledGreen, OUTPUT);
  lcd.begin();
  lcd.backlight();
  pinMode(buzzer, OUTPUT);
  pinMode(kilit, OUTPUT);
  while (!Serial);  
  delay(100);
  Serial.println("\n\nParmak Algılama Paneli");

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Parmak izi sensörü bulundu!");
       lcd.clear();                                          
       lcd.setCursor(0, 0); 
       lcd.print("Sensor ");
       lcd.setCursor(0, 1);
       lcd.print("Bulundu ");
       delay(1250);
       lcd.clear();                                          
       lcd.print("Bas Parmaginizi");
       lcd.setCursor(0, 1);
       lcd.print("Sensore Koyun!");
  } else {
    Serial.println("Parmak izi sensörü bulunamadı pinlerini ve kablolarını kontrol et :(");
       lcd.clear();
       lcd.setCursor(0, 0); 
       lcd.print("Sensor");
       lcd.setCursor(0, 1);
       lcd.print("Bulunamadi :(");
       delay (2000);
       lcd.clear();                                      
       digitalWrite(ledRed, HIGH);
       delay(500);
       digitalWrite(ledRed, LOW);
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Hafızada "); Serial.print(finger.templateCount); Serial.println(" tane parmak izi kayıtlı!");
  Serial.println("Geçerli parmak bekleniyor ...");
  lcd.clear();
  lcd.print("Bas Parmaginizi");
  lcd.setCursor(0, 1);
  lcd.print("Sensore Koyun!");
}

void loop()               
{
  finger_status = getFingerprintIDez();
  if (finger_status!=-1 and finger_status!=-2){
    digitalWrite(led_pin_accept,HIGH);
    delay(1000);
  } else{
    if (finger_status==-2){
       lcd.clear();                                          
       lcd.setCursor(0, 0); 
       lcd.print("Parmak izi");
       lcd.setCursor(0, 1);
       lcd.print("Kayitli Degil");
       delay(1250);
       lcd.clear();                                          
       lcd.print("Parmaginizi");
       lcd.setCursor(0, 1);
       lcd.print("Sensore Koyun!");
       digitalWrite(led_pin_deny,HIGH);
        delay(500);
        digitalWrite(led_pin_deny,LOW);
        delay(500);
    }
    digitalWrite(led_pin_accept,LOW);
  }
  delay(50);          
}


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p!=2){
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)  return -1;
  
  p = finger.image2Tz();
  if (p!=2){
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -2;
  
  Serial.print("Bulunan Kimlik #"); Serial.print(finger.fingerID); 
    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(kilit, HIGH);
    delay(1250);
    digitalWrite(ledGreen, LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(kilit, LOW);
  Serial.print(" güvenle "); Serial.println(finger.confidence);
  if (finger.fingerID == 127) {
      Serial.println("Hoşgeldiniz Burak Bey");
       lcd.clear();      
       delay(100);                                    
       lcd.setCursor(0, 0); 
       lcd.print("Hosgeldiniz");
       lcd.setCursor(0, 1);
       lcd.print("Burak Bey ");
       delay(1250);
       lcd.clear();                                          
       lcd.print("Parmaginizi");
       lcd.setCursor(0, 1);
       lcd.print("Sensore Koyun!");
  }
  
  return finger.fingerID; 
}
