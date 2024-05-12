//Kód nám ve škole ještě fungoval, ale při verifikaci doma vyhazuje chybu o tom, že kompilátor nezná instrukce z FirebaseESP32. Ptal jsem se umělé inteligence na možné opravy této chyby ale bohužel bez úspěchu.

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

#define FIREBASE_HOST "Odkaz"
#define FIREBASE_AUTH "API"
#define WIFI_SSID "Název sítě"
#define WIFI_PASSWORD "Heslo sítě"

#define DHTPIN 24
#define DHTTYPE DHT22
#define FLAME_SENSOR_PIN 33 
#define DFLAME_SENSOR_PIN 7
#define TRIGGER_PIN 14
#define ECHO_PIN 16

FirebaseData firebaseData;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi");

  FirebaseConfig firebaseConfig;
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.auth = FIREBASE_AUTH;

  Firebase.begin(&firebaseConfig, &firebaseData);
  dht.begin();
}

void loop() {
 
  float tep = dht.readTemperature();
  float vlhk = dht.readHumidity();

 
  int hori = analogRead(FLAME_SENSOR_PIN);
  bool boo;
  if(hori >= 512)
  {
   boo = true;
  }
  else 
  {
   boo = false;
  }

 
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  bool boo2;
  if(distance >= 10)
  {
   boo2 = true;
  }
  else 
  {
   boo2 = false;
  }



  if (Firebase.setInt(firebaseData, "/teplota", tep) &&
      Firebase.setInt(firebaseData, "/vlhkost", vlhk) &&
      Firebase.setBool(firebaseData, "/plamen", boo) &&
      Firebase.setBool(firebaseData, "/vetrelec", boo2)) {
    Serial.println("Fajne");
  } else {
    Serial.println("Chyba");
    Serial.println(firebaseData.errorReason());
  }

  delay(5000); 
}
