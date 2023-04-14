void loopCore1(void *arg ){
  while(true){
      WiFiClient client = server.available();   
      if (client) {                             
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client.");          
        String currentLine = "";                
        while (client.connected() && currentTime - previousTime <= timeoutTime) {  
          currentTime = millis();
          if (client.available()) {            
            char c = client.read();            
            Serial.write(c);                    
            header += c;
            if (c == '\n') {                   
              // if the current line is blank, you got two newline characters in a row.
              // that's the end of the client HTTP request, so send a response:
              if (currentLine.length() == 0) {
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                
                // turns the GPIOs on and off
                if (header.indexOf("GET /ligada/true") >= 0) {
                  ligada=true;
                } else if (header.indexOf("GET /ligada/false") >= 0) {
                  ligada=false;
                } else if (header.indexOf("GET /reset") >= 0) {
                   reset_memory();
                } else if (header.indexOf("GET /ler_dados/true") >= 0) {
                   ler=true;
                   //ler_dados();
                } else if (header.indexOf("GET /ler_dados/false") >= 0) {
                   ler=false;
                   i =0;
                   //ler_dados();
                }
                
                // Display the HTML web page
                client.println("<!DOCTYPE html><html>");
                client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
                // CSS to style the on/off buttons 
                // Feel free to change the background-color and font-size attributes to fit your preferences
                client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                client.println(".button2 {background-color: #555555;}</style></head>");
                
                // Web Page Heading
                client.println("<body><h1>Avionica H500</h1>");
                
                // Display current state, and ON/OFF buttons for GPIO 26  
                client.println("<p> Status -  " + String(ligada?"Ligado":"Desligado") + "</p>");
                // If the output26State is off, it displays the ON button       
                if (ligada==false) {
                  client.println("<p><a href=\"/ligada/true\"><button class=\"button\">ON</button></a></p>");
                } else {
                  client.println("<p><a href=\"/ligada/false\"><button class=\"button button2\">OFF</button></a></p>");
                } 
                client.println("<p> Apagar toda a memoria</p>");
                client.println("<p><a href=\"/reset\"><button class=\"button\">Reset</button></a></p>");


                client.println("<p> Ler Dados -  " + String(ler?"Lendo":"Nao Lendo") + "</p>");
                  if (ler==false) {
                    client.println("<p><a href=\"/ler_dados/true\"><button class=\"button\">Comecar Leitura</button></a></p>");
                  } else {
                    client.println("<p><a href=\"/ler_dados/false\"><button class=\"button button2\">Parar Leitura</button></a></p>");
                  } 
                
                client.println("</body></html>");
                
                client.println();
                
                break;
              } else { 
                currentLine = "";
              }
            } else if (c != '\r') {  
              currentLine += c;      
            }
          }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        
      }
     }
      delay(10); 
}
