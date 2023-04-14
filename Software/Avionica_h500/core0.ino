void loopCore0(void *arg ){
  while(true){
      digitalWrite(LED,1);
      delay(300);
      digitalWrite(LED,0);
      delay(300);
      coleta(); 
      ler_dados(); 
      //ejecao (); 
      delay(10);
  } 
}
