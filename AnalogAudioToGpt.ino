#include <WiFi.h>
#include <HTTPClient.h>
#include <SdFat.h>
#include <SPI.h>
#include <ArduinoJson.h>

const int chipSelect = 15; // SD card CS pin
const int micPin = A1;
const int sampleRate = 16000;

SdFat SD;
bool recording = false;
unsigned long startTime;
SdFile rawFile;

const char* ssid = "TheLanai";
const char* password = "PennyLuke5776$";
const char* server_url = "http://10.0.0.106:5000/upload";


const bool modeWifi = true;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect, SPI_HALF_SPEED)) {
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

}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == '1') {
      startRecording();
    } else if (command == '0') {
      stopRecording();
    }
  }
  
  if (recording) {
    saveAudioData();
  }
}

void startRecording() {
  if (!recording) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    recording = true;
    startTime = micros();
    Serial.println("Recording started.");

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
    sendFileToServer("audio.raw");
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
