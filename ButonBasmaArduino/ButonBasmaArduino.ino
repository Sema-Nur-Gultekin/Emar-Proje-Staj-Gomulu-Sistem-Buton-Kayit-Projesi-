#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* WIFI_ADI = "";
const char* WIFI_SIFRE = "";

const char* API_ADRESI = "http://API:5186/api/button";
const char* API_ANAHTARI = "key";

const int BUTON_PIN = D3; 

unsigned long sonTetiklenmeMs = 0;
const unsigned long DEBOUNCE_DELAY_MS = 50;
bool butonBasiliMi = false;

void setup() {
    Serial.begin(115200);
    delay(100);

    pinMode(BUTON_PIN, INPUT_PULLUP);

    wifiBaglan();
}

void loop() {

    int anlikButonOkumasi = digitalRead(BUTON_PIN);

    if (anlikButonOkumasi == LOW && (millis() - sonTetiklenmeMs) > DEBOUNCE_DELAY_MS) {
        sonTetiklenmeMs = millis();

        if (!butonBasiliMi) {
            butonBasiliMi = true;
            Serial.println("Butona basildi! Backend'e istek gonderiliyor...");
            butonBasildiGonder();
        }
    } else if (anlikButonOkumasi == HIGH) {

        butonBasiliMi = false;
    }
}

void wifiBaglan() {
    Serial.print("Wi-Fi'a baglaniliyor: ");
    Serial.println(WIFI_ADI);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_ADI, WIFI_SIFRE);

    // Bağlantı başarılı olana kadar bekler
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi baglantisi basarili!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
}

void butonBasildiGonder() {

    if (WiFi.status() != WL_CONNECTED) {
        wifiBaglan(); 

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Wi-Fi baglantisi yok, istek gonderilemiyor.");
            return;
        }
    }

    WiFiClient istemci;
    HTTPClient http;

    if (!http.begin(istemci, API_ADRESI)) {
        Serial.println("HTTP baglantisi baslatilamadi.");
        return;
    }

    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-KEY", API_ANAHTARI);

    // JSON verisini oluşturma
    String veri = "{\"source\":\"esp-12e\"}";
    
    int httpKod = http.POST(veri);

    // Yanıtı kontrol etme
    if (httpKod > 0) {

        Serial.printf("HTTP Kodu: %d\n", httpKod);

        if (httpKod == HTTP_CODE_OK || httpKod == HTTP_CODE_CREATED) {
            String yanit = http.getString();
            Serial.print("Backend yaniti: ");
            Serial.println(yanit);
        }
    } else {

        Serial.printf("HTTP istegi basarisiz, hata: %s\n", http.errorToString(httpKod).c_str());
    }

    http.end(); 
}