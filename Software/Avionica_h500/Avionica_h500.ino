#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include<SPIMemory.h>
#include <WiFi.h>

#define flashCS 5

uint32_t floatAddr;
bool ligada=false;
long timeini=0;
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
const char* ssid = "SCD";
const char* password = "123456789";
static uint8_t CoreZero = 0;
static uint8_t CoreOne  = 1;

Adafruit_BMP280 bmp;
SPIFlash flash(flashCS, &SPI);
WiFiServer server(80);

void setup() {
  pinMode(0, INPUT_PULLUP);
  Serial.begin(115200);
  xTaskCreatePinnedToCore(loopCore0,"coreTaskZero",10000,NULL,1, NULL,CoreZero);                  
  delay(500);
  xTaskCreatePinnedToCore(loopCore1,"coreTaskZero",10000,NULL,1, NULL,CoreOne);                  
  delay(500);
  
  while(!bmp.begin(0x76)){
    Serial.println("Erro no BMP");
    delay(100);
  }
  if (flash.begin(MB(4))) {
      Serial.println(F("Device finded and supported!"));
  } else {
      Serial.println(F("Problem to discover and configure device, check wiring also!"));
  }
  Serial.println("BMP: "+String(bmp.sensorID(),16));
  
  Serial.print(F("Flash size: "));
  Serial.print((long)(flash.getCapacity()/1000));
  Serial.println(F("Kb"));
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
}

void coleta(){
  // Uma estimativa de 1045000 dados podem ser lidos durante aproximadamente 25min
  if (floatAddr>4180000)ligada=false;
  if(ligada==true){
    floatAddr = flash.getAddress(sizeof(float));
    float alt = bmp.readAltitude(1013.25);
    flash.writeFloat(floatAddr, alt);
    Serial.print("Float Address ");
    Serial.println(floatAddr/4);
    Serial.println("Valor: "+String(flash.readFloat(floatAddr)));
  }
}
void reset_memory(){
    if(flash.eraseChip()){
      Serial.print("Memory reset");
      ESP.restart();
    }
    Serial.print("Memory reset erro");
}

void loop() {
}
