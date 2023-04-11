#include <SPI.h>
#include<SPIMemory.h>

#define flashCS 5

bool ligada = true;
float valor;
uint32_t floatAddr;
SPIFlash flash(flashCS, &SPI);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  if (flash.begin(MB(4))) {
    Serial.println(F("Device finded and supported!"));
  } else {
    Serial.println(F("Problem to discover and configure device, check wiring also!"));
  }
  Serial.print(F("Flash size: "));
  Serial.print((long)(flash.getCapacity()/1000));
  Serial.println(F("Kb"));
}

void loop() {
  // put your main code here, to run repeatedly:
  // Uma estimativa de 1045000 dados podem ser lidos durante aproximadamente 25min
  for (int i=0;i<1045000;i=i+4) {
    valor = flash.readFloat(i);
    Serial.println("End: ");
    Serial.println(i);
    Serial.println("Valor: ");
    Serial.println(valor);
  

    delay(10);
  }
}
