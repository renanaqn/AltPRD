#include <WiFi.h> 
#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
const char* ssid = "brisa-2531506"; 
const char* password = "idy1klt1";

WiFiServer server(80);

String header;

void setup() 
{
Serial.begin(9600);
if (!bmp.begin()) 
{
Serial.println("Not connected with BMP180/BMP085 sensor, check connections ");
while (1) {}
}

Serial.print("Connecting to Wifi Network");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("Successfully connected to WiFi.");
Serial.println("IP address of ESP32 is : ");
Serial.println(WiFi.localIP());
server.begin();
Serial.println("Server started");
}

void loop() {
Serial.print("Temp = ");
Serial.print(bmp.readTemperature());
Serial.println(" *C");

Serial.print("Pressure = ");
Serial.print(bmp.readPressure());
Serial.println(" Pa");


Serial.print("Altitude = ");
Serial.print(bmp.readAltitude());
Serial.println(" meters");


Serial.println();
delay(2000);
WiFiClient client = server.available();

if (client) 
{ 
Serial.println("Web Client connected ");
String request = client.readStringUntil('\r'); 
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("</style></head><body><h1>Altimetro PRD v0</h1>");
client.println("<h2>Coleta de Dados:</h2>");
client.println("<table><tr><th>Leitura</th><th>Valor</th></tr>");
client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
client.println(bmp.readTemperature());
client.println(" *C</span></td></tr>"); 
client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
client.println(bmp.readPressure());
client.println(" pa</span></td></tr>"); 
client.println("<tr><td>Altitude</td><td><span class=\"sensor\">");
client.println(bmp.readAltitude());
client.println(" meters</span></td></tr>"); 
client.println("</body></html>"); 
client.println();
Serial.println("Client disconnected.");
Serial.println("");
}
}