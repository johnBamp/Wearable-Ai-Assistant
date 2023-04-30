#include <SdFat.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SdFat.h>
#include <SPI.h>

const int chipSelect = 15; // SD card CS pin
const int micPin = A1;
const int sampleRate = 32000;

const char* ssid = "TheLanai";
const char* password = "PennyLuke5776$";
const char* server_url = "http://10.0.0.106:5000/upload";

SdFat SD;
bool recording = false;
unsigned long startTime;
SdFile rawFile;

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
