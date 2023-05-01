#include <WiFi.h>
#include <HTTPClient.h>
#include <SdFat.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

#define TFT_CS 14
#define TFT_DC 32
#define SD_CS 15

const char* ssid = "TheLanai";
const char* password = "PennyLuke5776$";

SdFat SD;
SdFile rawFile;

Adafruit_FT6206 ts = Adafruit_FT6206();

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


class aiAssistant{
  private:
    const int micPin = A1;
    const int sampleRate = 16000;
    bool recording = false;
    unsigned long startTime;
    unsigned long lastCheckTime = 0;
    const unsigned long checkInterval = 1000; // 1000 milliseconds (1 second)
    bool record = false;
    bool state = 0;

    const char* server_url = "http://10.0.0.106:5000/upload";


    void startRecording() {
      if (!recording) {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        recording = true;
        startTime = micros();
        Serial.println("Recording started.");

        tft.fillScreen(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.setRotation(3);
        tft.setCursor(0,0);
        tft.setTextColor(ILI9341_WHITE);
        tft.println("recording..");

        String filename = "audio.raw";
        if (rawFile.open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC)) {
          Serial.println("File opened.");
        } else {
          Serial.println("Error opening file.");
          recording = false;
          return;
        }
      }
    }

    void stopRecording() {
      if (recording) {
        recording = false;
        rawFile.close();
        Serial.println("Recording stopped and saved.");

        tft.fillScreen(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.setRotation(3);
        tft.setCursor(0,0);
        tft.setTextColor(ILI9341_WHITE);
        tft.println("processing..");
        
        String message = sendFileToServer("audio.raw");
        tft.fillScreen(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.setRotation(3);
        tft.setCursor(0,0);
        tft.setTextColor(ILI9341_WHITE);
        digitalWrite(33, HIGH);
        tft.println(message);
        delay(100);
        digitalWrite(33, LOW);
      }
    }

    void saveAudioData() {
      unsigned long elapsedTime = micros() - startTime;
      if (elapsedTime >= 1000000 / sampleRate) {
        startTime += 1000000 / sampleRate;
        int16_t micValue = (analogRead(micPin) - 2048) << 6;
        rawFile.write((uint8_t *)&micValue, sizeof(micValue));
      }
    }

    String sendFileToServer(const String &filename) {
      String payload;

      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
      }
      Serial.println("Connected to WiFi");

      File audio = SD.open(filename.c_str());
      if (!audio) {
        Serial.println("Failed to open file for reading");
        return "Error";
      }

      HTTPClient http;
      http.begin(server_url);
      http.addHeader("Content-Type", "application/octet-stream");
      http.addHeader("X-File-Name", filename);

      int httpCode = http.sendRequest("POST", &audio, audio.size());

      if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Server response:");
      } else {
        Serial.print("Error on sending POST request: ");
        Serial.println(httpCode);
      }

      StaticJsonDocument<256> json_doc;
      DeserializationError error = deserializeJson(json_doc, payload);

      if (error) {
        Serial.print(F("Failed to parse JSON: "));
        Serial.println(error.c_str());
        return "Error";
      }

      String message = json_doc["message"];
      // Print the extracted value
      Serial.println(message);

      http.end();
      audio.close();

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      return message;
    }

  public:
    void loop() {
      unsigned long currentTime = millis();

      if (currentTime - lastCheckTime >= checkInterval) {
        lastCheckTime = currentTime;

        if (ts.touched() && record != true) {
          record = true;
        } else if (ts.touched() && record != false) {
          record = false;
        }
      }

      if (record == true) {
        startRecording();
      } else if (record == false) {
        stopRecording();
      }
      
      if (recording) {
        saveAudioData();
      }
    }

};

aiAssistant assistant;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  pinMode(33, OUTPUT);

  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS, SPI_HALF_SPEED)) {
    Serial.println("Card failed or not present");
    while (1);
  }
  Serial.println("Card initialized.");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  tft.begin();

  if (! ts.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setRotation(3);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("test");

}

void loop() {
  assistant.loop();
 
}
