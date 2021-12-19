#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(7, 6); 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()  
{
  Serial.begin(9600);
  while (!Serial);  
  delay(100);
  Serial.println("\n\nParmak Kayıt Etme Paneli");

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Parmak izi sensöru bulundu!");
  } else {
    Serial.println("Parmak izi sensöru bulunamadi pinlerini ve kablolarını kontrol et :(");
    while (1) { delay(1); }
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()       
{
  Serial.println("Parmak izini kaydetmek için herşey hazır");
  Serial.println("Lütfen bu parmağınızı kaydetmek istediğiniz kimlik numarasını (1'den 127'ye kadar) yazın ...");
  id = readnumber();
  if (id == 0) {
     return;
  }
  Serial.print("Kayıt Kimliği #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Geçerli parmağın kaydedilmesi bekleniyor"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Çekilen resim");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim hatası");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Görüntüleme hatası");
      break;
    default:
      Serial.println("Bilinmeyen hata");
      break;
    }
  }


  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Görüntü dönüştürüldü");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Görüntü çok dağınık");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(" İletişim hatası");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Parmak izi özellikleri bulunamadı");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Parmak izi özellikleri bulunamadı");
      return p;
    default:
      Serial.println("Bilinmeyen hata");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Çekilen resim");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(" İletişim hatası");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Görüntüleme hatası");
      break;
    default:
      Serial.println("Bilinmeyen hata");
      break;
    }
  }


  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Görüntü dönüştürüldü");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Görüntü çok dağınık");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim hatası");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Parmak izi özellikleri bulunamadı");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Parmak izi özellikleri bulunamadı");
      return p;
    default:
      Serial.println("Bilinmeyen hata");
      return p;
  }
  
  Serial.print("Kimlik için model oluşturuluyor");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Baskılar eşleşti!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("İletişim hatası");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Parmak izleri eşleşmedi");
    return p;
  } else {
    Serial.println("Bilinmeyen hata");
    return p;
  }   
  
  Serial.print("Kimlik "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Başarılı!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(" İletişim hatası");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Bu konumda depolanamadı");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Flash'a yazma hatası");
    return p;
  } else {
    Serial.println("Bilinmeyen hata");
    return p;
  }   
}
