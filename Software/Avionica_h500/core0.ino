void loopCore0(void *arg ){
  while(true){
      coleta(); 
      ler_dados(); 
      delay(10);
  } 
}

void ejecao(){
   
  if(millis()-cont_test>400){
    cont_test=millis();
    alt_ant = alt;
  }
  if((alt-alt_ant)<(-0.7)){
    Serial.println("Ejetou");
    servo.write(105);
  }
}

//Testado - Tudo OK
void getDataBMP(){
  float alt0=0,alt1=0,alt2=0,alt3=0,media_alt,percent=0.001;
  alt0=bmp.readAltitude(pressaoNivelMar);
  delay(30);
  alt1=bmp.readAltitude(pressaoNivelMar);
  delay(30);
  alt2=bmp.readAltitude(pressaoNivelMar); 
  delay(30);
  alt3=bmp.readAltitude(pressaoNivelMar);
  media_alt = (alt1+alt2+alt3)/3;
  
  if(alt0>=(media_alt+(media_alt*percent)) || alt0<=(media_alt-(media_alt*percent))){ // verifica se o valor coletado está dentro do intervalo gerado pela média
    alt = media_alt - alt_inicial;
  }else{
    alt =alt0 - alt_inicial;
  }
}


void coleta() {
  // Uma estimativa de 1045000 dados podem ser lidos durante aproximadamente 25min
  if (floatAddr > 4180000)ligada = false;
  if (ligada == true) {
    unsigned long tempo = millis();
    getDataBMP();
    floatAddr = flash.getAddress(sizeof(float)); 
    flash.writeFloat(floatAddr, tempo); 
    //Serial.println("Tempo " + String(flash.readFloat(floatAddr))); 
    floatAddr = flash.getAddress(sizeof(float)); 
    flash.writeFloat(floatAddr, alt); 
    delay(40);
    //Serial.print("Float Address "); 
    //Serial.println(floatAddr / 4);  
    Serial.println("Valor: " + String(flash.readFloat(floatAddr))); 
  }
}
