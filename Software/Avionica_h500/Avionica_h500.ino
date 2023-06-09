#include <Wire.h> 
#include <SPI.h> 
#include <Adafruit_BMP280.h> 
#include<SPIMemory.h> 
#include <WiFi.h> 
#include "BluetoothSerial.h" 
#include <ESP32Servo.h>

#define flashCS 5 
#define pressaoNivelMar 1012
#define pinServo 13

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED) 
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it 
#endif 
Servo servo;

uint32_t floatAddr;
float alt = 0, alt_ant=0,alt_inicial=0;
bool ligada = true;
bool ler = false;
int i = 0, cont_eject = 0;
long timeini = 0,cont_test=0;
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;
const char* ssid = "Avionica";
const char* password = "123456789";
static uint8_t CoreZero = 0;
static uint8_t CoreOne  = 1;

Adafruit_BMP280 bmp; 
SPIFlash flash(flashCS, &SPI); 
BluetoothSerial SerialBT; 
WiFiServer server(80); 

void setup() { 
  pinMode(0, INPUT_PULLUP); 
  Serial.begin(115200); 
  xTaskCreatePinnedToCore(loopCore0, "coreTaskZero", 10000, NULL, 1, NULL, CoreZero); 
  delay(500); 
  xTaskCreatePinnedToCore(loopCore1, "coreTaskZero", 10000, NULL, 1, NULL, CoreOne); 
  delay(500); 
  servo.attach(pinServo);
  servo.write(50);
  while (!bmp.begin(0x76)) { 
    Serial.println("Erro no BMP");
    delay(100); 
  }
  if (flash.begin(MB(8))) {
    Serial.println(F("Device finded and supported!"));
  } else {
    Serial.println(F("Problem to discover and configure device, check wiring also!"));
  }
  Serial.println("BMP: " + String(bmp.sensorID(), 16));

  Serial.print(F("Flash size: "));
  Serial.print((long)(flash.getCapacity() / 1000));
  Serial.println(F("Kb"));

  //SerialBT.begin("ESP32test"); //Bluetooth device name
  /*while(!SerialBT.hasClient()){ //CÓDIGO DO BT
  }*/
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  String s = WiFi.localIP().toString().c_str();
  writeString(s);
  //SerialBT.write(ptr1, sizeof(s)*sizeof(char));
  server.begin();

}

void writeString(String stringData){
  for(int i = 0; i < stringData.length();i++){
    SerialBT.write(stringData[i]);
  }
}

void reset_memory() {
  if (flash.eraseChip()) {
    Serial.print("Memory reset");
    ESP.restart();
  }
  Serial.print("Memory reset erro");
}

void ler_dados () {
  if (i > 4180000)ler = false; 
  if (ler == true) {
    float valor = flash.readFloat(i); 
    Serial.print("End: ");
    Serial.println(i/4);
    //Serial.print("Tempo: ");
    //Serial.println(flash.readFloat(i));
    Serial.print("Valor: ");
    Serial.println(valor);
    
    delay(10);
    i = i + 4;
  }
}

void loop() {
}
