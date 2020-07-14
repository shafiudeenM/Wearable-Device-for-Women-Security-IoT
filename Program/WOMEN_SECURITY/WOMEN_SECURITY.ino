#include "DHT.h"           
// including the library of DHT11 temperature and humidity sensor
#include <SimpleTimer.h>//including the library of SimpleTimer
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DHTTYPE DHT11
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int a;
int b;
int c;
int e;
#define dht_dpin 2
DHT dht(dht_dpin, DHTTYPE); 
SimpleTimer timer;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

float l;

float  lo;
int button;
float h;
float t;


const char auth[] = "X33fFYKSKKP4kwpfraYzKyZPwuzj-gLa";


const char ssid[] = "Shafi"; // Name of your network (HotSpot or Router name)

const char pass[] = "12345678"; // Corresponding Password
const long utcOffsetInSeconds = 19800;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

BLYNK_WRITE(V1) {
  GpsParam gps(param);
  // Print 6 decimal places for Lat, Lon
  Serial.print("Lat: ");
  Serial.println(gps.getLat(), 7);
  l=(gps.getLat());
  Serial.println("l");
  Serial.println(l);
  Serial.print("Lon: ");
  Serial.println(gps.getLon(), 7);
  lo=(gps.getLon());
  Serial.println("lo");
  Serial.println(lo);
  // Print 2 decimal places for Alt, Speed
  Serial.print("Altitute: ");
  Serial.println(gps.getAltitude(), 2);

  Serial.print("Speed: ");
  Serial.println(gps.getSpeed(), 2);
  Serial.println();
}


BLYNK_WRITE(V2)
{
button=param.asInt();
  digitalWrite(button,HIGH);
  Serial.println("button");
  Serial.println(button);
}
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
//Blynk.begin(auth1, ssid, pass);
  dht.begin();
    timer.setInterval(2000, sendUptime);
    while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void sendUptime()
{
  t = dht.readTemperature(); 

  Serial.print("temperature = ");
  Serial.print(t); 
  Blynk.virtualWrite(V3, t);
}
void tim() 
{
display.clearDisplay();
timeClient.update();
Serial.print(timeClient.getHours());
display.print(" ");
if(timeClient.getHours()>12)
{
 e=timeClient.getHours()-12;
 Serial.print(e);
 display.print(e);
}
else
{
Serial.print(timeClient.getHours());
a=timeClient.getHours();
Serial.print(a);
display.print(a);
}
Serial.print(":");
Serial.print(timeClient.getMinutes());
Serial.print(":");
b=timeClient.getMinutes();
display.print(":");
display.print(b);
Serial.println(timeClient.getSeconds());
c=timeClient.getSeconds();   
display.print(":");
display.print(c);
display.println(" ");
if(timeClient.getHours()>12)
{
Serial.println("pm");
}
else{
Serial.println("am");
}
 // Serial.println(timeClient.getFormattedTime()); 
Serial.print(daysOfTheWeek[timeClient.getDay()]);
display.print(" ");
//Serial.print(", ");
display.println(daysOfTheWeek[timeClient.getDay()]);
delay(500);
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(2, 10);
display.display(); 
}
void loop()
{
Blynk.run();
timer.run();
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
{ // Address 0x3D for 128x64
Serial.println(F("SSD1306 allocation failed"));
for(;;);
}
delay(500);
tim();
if (button==HIGH)
{    // is a character available 
Blynk.notify("location Tracked-Check your Mail");
Blynk.email("srishakthi7072@gmail.com",l,lo );   
}
sendUptime();
if (t>=27)
{    // is a character available?
Blynk.notify("I am in trouble");
Blynk.email("srishakthi7072@gmail.com",l,lo );   
}
}
