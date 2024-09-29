#include <Arduino.h>
#include <Controllino.h>
#include <string.h>
#include <Wire.h>

#include <Ethernet.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <elapsedMillis.h>
//#include <DigitalToggle.h>
#include <MQTT.h>
#include <NewPing.h>
#include <SPI.h>//#define rst  8  // you can also connect this to the Arduino reset
const int numReadings = 20;
int readings[numReadings];      // the readings from the analog input
int indexread = 0;                  // the index of the current reading
unsigned long totalread = 0;                  // the running total
int averageread = 0;                // the average
int verbr2; // Variable zur Speicherung eines Wertes im Zusammenhang mit verbr2
int inputPin = A4; // Analoger Eingangspin A4
int loopcount = 200; // Schleifenzähler, initialisiert auf 200
float ans; // Variable zur Speicherung einer Gleitkommazahl als Antwort
float count2; // Variable zur Speicherung einer Gleitkommazahl als Zähler
int av; // Variable zur Speicherung eines ganzzahligen Wertes
float verbrauch2; // Variable zur Speicherung einer Gleitkommazahl im Zusammenhang mit verbrauch2
int loopcount4; // Ein weiterer Schleifenzähler
int hh; // Variable zur Speicherung der Stunden
int mm; // Variable zur Speicherung der Minuten
int ss; // Variable zur Speicherung der Sekunden
unsigned long epoch2; // Variable zur Speicherung der Epoch-Zeit
unsigned long oneYear; // Variable zur Speicherung eines Jahres in Millisekunden
int sensstor; // Variable zur Speicherung eines Sensorwertes
bool triggeronce = true; // Boolesche Variable, um einmalig auszulösen
int k; // Allgemeine ganzzahlige Variable
//Sd2Card card; // SD-Kartenobjekt (auskommentiert)
//SdVolume volume; // SD-Volume-Objekt (auskommentiert)
//SdFile root; // SD-Dateiobjekt (auskommentiert)
int wheelstore; // Variable zur Speicherung der Radanzahl
// Ändern Sie dies, um es an Ihr SD-Shield oder Modul anzupassen;
// Arduino Ethernet Shield: Pin 4
// Adafruit SD Shields und Module: Pin 10
// Sparkfun SD Shield: Pin 8
// Microduino SD Shield: Pin 7
const int chipSelect = 4; // Chip-Select-Pin für die SD-Karte
//Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst); // TFT-Display-Objekt (auskommentiert)
//LiquidCrystal lcd2(22, 23, 24, 25, 26, 27); // LCD-Objekt (auskommentiert)
String Labl[8]; // Array von Strings zur Speicherung von Labels
String chars; // String-Variable zur Speicherung von Zeichen
String fchars; // String-Variable zur Speicherung von formatierten Zeichen
boolean Pstat; // Boolesche Variable zur Speicherung des Pumpenstatus
boolean heaterOn = false; // Boolesche Variable zur Speicherung des Heizungsstatus, initialisiert auf false
boolean waiting = false; // Boolesche Variable zur Speicherung des Wartezustands, initialisiert auf false
long loopcountAll; // Long-Variable zur Speicherung der gesamten Schleifenanzahl
char c; // Zeichenvariable
char fileName[13] = "test.txt"; // Zeichenarray zur Speicherung des Dateinamens
char outBuf[128]; // Zeichenarray zur Speicherung des Ausgabepuffers
char outCount; // Zeichenvariable zur Speicherung der Ausgabemenge
char thisChar; // Zeichenvariable zur Speicherung des aktuellen Zeichens
const int Brenner = CONTROLLINO_RELAY_00; // Konstante für den Brenner-Relais-Pin
const int Mischer_AUF = CONTROLLINO_RELAY_01; // Konstante für den Mischer_AUF-Relais-Pin
const int Mischer_ZU = CONTROLLINO_RELAY_02; // Konstante für den Mischer_ZU-Relais-Pin
const int Pumpe = CONTROLLINO_RELAY_03; // Konstante für den Pumpen-Relais-Pin
int tempsoll = 30; // Ganzzahlige Variable zur Speicherung der Solltemperatur, initialisiert auf 30
elapsedMillis Mpause; // Variable zur Speicherung der verstrichenen Zeit für Pausen
elapsedMillis Mtimeauf; // Variable zur Speicherung der verstrichenen Zeit für Mtimeauf
elapsedMillis Mtimezu; // Variable zur Speicherung der verstrichenen Zeit für Mtimezu
elapsedMillis logIntervall; // Variable zur Speicherung des Log-Intervalls
elapsedMillis spread; // Variable zur Speicherung der verstrichenen Zeit für Spread
elapsedMillis timeOut; // Variable zur Speicherung der verstrichenen Zeit für Timeout
elapsedMillis timeOut2; // Variable zur Speicherung der verstrichenen Zeit für das zweite Timeout
elapsedMillis weather; // Variable zur Speicherung der verstrichenen Zeit für Wetter
elapsedMillis regelZeit; // Variable zur Speicherung der verstrichenen Zeit für Regelzeit
elapsedMillis scheibe; // Variable zur Speicherung der verstrichenen Zeit für Scheibe
elapsedMillis publishintervall; // Variable zur Speicherung des Veröffentlichungsintervalls
elapsedMillis TankIntervall; // Variable zur Speicherung des Tankintervalls
///TESTCOMMIT
float Aist; // Gleitkommazahl-Variable zur Speicherung des Aist-Wertes
float Bist; // Gleitkommazahl-Variable zur Speicherung des Bist-Wertes
float Kist; // Gleitkommazahl-Variable zur Speicherung des Kist-Wertes
float R1; // Gleitkommazahl-Variable zur Speicherung des R1-Wertes
float R2; // Gleitkommazahl-Variable zur Speicherung des R2-Wertes
float V1; // Gleitkommazahl-Variable zur Speicherung des V1-Wertes
float V2; // Gleitkommazahl-Variable zur Speicherung des V2-Wertes
float tempist; // Gleitkommazahl-Variable zur Speicherung des Tempist-Wertes
int Ksoll = 45; // Ganzzahlige Variable zur Speicherung des Sollwertes für Kist, initialisiert auf 45
int Spanne = 10; // Ganzzahlige Variable zur Speicherung des Spanne-Wertes, initialisiert auf 10
int V2soll; // Ganzzahlige Variable zur Speicherung des Sollwertes für V2
int col[9]; // Array von Ganzzahlen zur Speicherung von Farbwerten
int count = 1; // Ganzzahlige Variable zur Speicherung des Zählers, initialisiert auf 1
int dif; // Ganzzahlige Variable zur Speicherung des Differenzwertes
int hyst = 2; // Ganzzahlige Variable zur Speicherung des Hysterese-Wertes, initialisiert auf 2
int i = 0; // Ganzzahlige Variable zur Speicherung des Index, initialisiert auf 0
int len; // Ganzzahlige Variable zur Speicherung der Länge
int mstat = 0; // Ganzzahlige Variable zur Speicherung des Mstat-Wertes, initialisiert auf 0
int sens[99]; // Array von Ganzzahlen zur Speicherung von Sensorwerten
int tcastfrueh; // Ganzzahlige Variable zur Speicherung des Tcastfrueh-Wertes
int tcastspaet; // Ganzzahlige Variable zur Speicherung des Tcastspaet-Wertes
int tempstor = 0; // Ganzzahlige Variable zur Speicherung des Tempstor-Wertes, initialisiert auf 0
int xPos = 10; // Ganzzahlige Variable zur Speicherung der x-Position, initialisiert auf 10
int yCoord[9]; // Array von Ganzzahlen zur Speicherung der y-Koordinaten
int sensnow; // Ganzzahlige Variable zur Speicherung des aktuellen Sensorwertes
int senshigh; // Ganzzahlige Variable zur Speicherung des höchsten Sensorwertes
int senslow; // Ganzzahlige Variable zur Speicherung des niedrigsten Sensorwertes

unsigned long coldtime = 0; // Unsigned long Variable zur Speicherung der Kaltzeit, initialisiert auf 0
unsigned long diftime = 0; // Unsigned long Variable zur Speicherung der Differenzzeit, initialisiert auf 0
unsigned long interval = 80000; // Unsigned long Variable zur Speicherung des Intervalls, initialisiert auf 80000
unsigned long warmtime = 0; // Unsigned long Variable zur Speicherung der Warmzeit, initialisiert auf 0
//File myFile; // Dateiobjekt (auskommentiert)
int wheelcount; // Ganzzahlige Variable zur Speicherung der Radanzahl
unsigned int localPort = 8889; // Lokaler Port
//IPAddress timeServer(192, 168, 1, 1); // time.nist.gov NTP-Server
//const int NTP_PACKET_SIZE = 48; // NTP-Zeitstempel steht in den ersten 48
// Bytes der NAchricht
//byte packetBuffer[NTP_PACKET_SIZE]; // Puffer fu¨r ein- und ausgehende Pakete
// UDP-Instanz zum Senden und Empfangen von UDP-Paketen
//EthernetUDP Udp;

//#pragma region Onewire
/**
 * @brief Initializes a OneWire instance on the specified pin.
 * 
 * This instance is used to communicate with devices on a OneWire bus.
 * 
 * @param pin The pin number where the OneWire bus is connected.
 */
OneWire oneWire(20);

DallasTemperature sensors(&oneWire);
// DeviceAddress Kessel = { 0x10, 0xE8, 0x4A, 0x29, 0x02, 0x08, 0x00, 0xFD };
// DeviceAddress Aussen = { 0x10, 0x3C, 0x08, 0x29, 0x02, 0x08, 0x00, 0x8F };
// DeviceAddress Brauch = { 0x28, 0x58, 0x78, 0x96, 0x05, 0x00, 0x00, 0x1F };
// DeviceAddress V1wire = { 0x28, 0x50, 0xBD, 0xB0, 0x05, 0x00, 0x00, 0x37 };
// DeviceAddress R1wire = { 0x28, 0x47, 0x21, 0xB1, 0x05, 0x00, 0x00, 0xF0 };
// DeviceAddress V2wire = { 0x28, 0x4B, 0x54, 0x95, 0x05, 0x00, 0x00, 0x4C };
// DeviceAddress R2wire = { 0x28, 0x30, 0x85, 0x96, 0x05, 0x00, 0x00, 0x37 };




DeviceAddress V1wire = { 0x28, 0xBD, 0x18, 0xAE, 0x31, 0x21, 0x03, 0x3F };  // 1
DeviceAddress R1wire = { 0x28, 0x51, 0x7B, 0xAD, 0x31, 0x21, 0x03, 0xBC };  // 2
DeviceAddress V2wire = { 0x28, 0x92, 0x4A, 0x80, 0x31, 0x21, 0x03, 0xDE };  // 3
DeviceAddress R2wire = { 0x28, 0xF8, 0x20, 0x5C, 0x31, 0x21, 0x03, 0x6D };  // 4
DeviceAddress Aussen = { 0x28, 0xDA, 0xCC, 0x93, 0x31, 0x21, 0x03, 0xED };  // 5
DeviceAddress unset1 = { 0x28, 0x8F, 0x32, 0x9A, 0x31, 0x21, 0x03, 0xBF };  // 6
DeviceAddress Kessel = { 0x28, 0x41, 0x9D, 0x8C, 0x31, 0x21, 0x03, 0x5E };  // 7
DeviceAddress Brauch = { 0x28, 0x7F, 0xCA, 0x4F, 0x31, 0x21, 0x03, 0xD5 };  // 8


//#pragma endregion Onewire


#define TRIGGER_PIN  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
float duration, distance;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
//#pragma region ETHERNET
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 178);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
//IPAddress fserver(141, 38, 3, 183);
//EthernetServer server(23);
//EthernetClient fclient;
//EthernetClient dclient;
EthernetClient client;
//EthernetClient pushbox;
MQTTClient mqttclient;
//String devid = "vD55216BF26119A8";
//char serverName[] = "api.pushingbox.com";
//#pragma endregion ETHERNET
//elapsedMillis addTime;


void mqttpublish(){
  mqttclient.publish("V1", String(V1));
  mqttclient.publish("R1", String(R1));
  mqttclient.publish("V2", String(V2));
  mqttclient.publish("R2", String(R2));
  mqttclient.publish("KESSEL",String(Kist));
  mqttclient.publish("BRAUCHWASSER",String(Bist));
  mqttclient.publish("AUSSEN",String(Aist));
  mqttclient.publish("PUMPE",String(Pstat));
  mqttclient.publish("BRENNER", String(heaterOn));
  mqttclient.publish("MISCHER", String(mstat));
  mqttclient.publish("MISCHERSTELLUNG", String(diftime));
  mqttclient.publish("Heizölvorrat", String(distance));
}

void mqttconnect() {
  Serial.print("connecting...");
if (!mqttclient.connect("Heizungsserver", "ftpuser", "L3Tt1301279")) {
    Serial.print(".");
    
  }

  Serial.println("\nconnected!");

  mqttclient.subscribe("HEIZUNGSOLL");
  mqttclient.subscribe("KESSELSOLL");
  mqttclient.subscribe("PUMPESOLL");
 mqttclient.subscribe("SPANNE");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

if (topic == "HEIZUNGSOLL"){
  tempsoll = payload.toInt();
}

if (topic == "KESSELSOLL"){
  Ksoll = payload.toInt();
}

if (topic == "PUMPESOLL"){
  if (payload == "1")     {digitalWrite(Pumpe, HIGH);	Pstat = true;   }
  if (payload == "0")     {digitalWrite(Pumpe, LOW);  Pstat = false;  }
  
}

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}




void parseSensors() {
  Serial.println("parsing");
	sensors.requestTemperatures();
 //bool conec =  sensors.isConnected(Kessel);
 //Serial.println(conec);
	delay(1);
	  Bist = sensors.getTempC(Brauch);
	tempist = sensors.getTempC(V1wire);
	V1 = tempist;
	R1 = sensors.getTempC(R1wire);
	V2 = sensors.getTempC(V2wire);
	R2 = sensors.getTempC(R2wire);
	Kist = sensors.getTempC(Kessel);
	Aist = sensors.getTempC(Aussen);
  //Bist = sensors.getTempC(Brauch);
  
  Serial.println(Bist);

	sens[1] = V1;
	sens[2] = R1;
	sens[3] = V2;
	sens[4] = R2;
	sens[5] = Kist;
	sens[6] = Bist;
	sens[7] = Aist;

	sens[8] = heaterOn;
	sens[9] = verbr2;
	sens[10] = verbr2;
	//loopcount = 0;
}

/*
void Grapher()

{
for (int i = 1; i < 8; i++)
{
yCoord[i] = map(sens[i], 0, 100, 0, tft.height());
tft.drawPixel(xPos,22222222 tft.height() - yCoord[i], col[i]);
}

if (xPos >= 160)
{
xPos = 10;
}
else
{
xPos = xPos + 1;
}
tft.drawFastVLine(xPos, 30, tft.height(), ST7735_BLACK);
for (int i = 1; i < 9; i++)
{
tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
tft.setCursor(0, (tft.height() - (i * 13)) - 3);
tft.print(i * 10);
tft.setCursor(10, (tft.height() - (i * 13)) - 5);
tft.setTextColor(ST7735_WHITE);
}
for (int i = 1; i < 8; i++)
{
tft.setTextSize(1);
tft.setTextColor(col[i], ST7735_BLACK);
tft.setCursor(i * 15 + 15, 0);
tft.print(Labl[i]);
tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
tft.setCursor(i * 15 + 15, 10);
tft.print(sens[i]);
}
for (int i = 8; i < 161; i += 25){
tft.drawFastVLine(i, (tft.height() - (1 * 13)), 13, ST7735_WHITE);
tft.drawPixel(i, (tft.height() - (2 * 13)), ST7735_WHITE);
tft.drawPixel(i, (tft.height() - (3 * 13)), ST7735_WHITE);
tft.drawPixel(i, (tft.height() - (4 * 13)), ST7735_WHITE);
tft.drawPixel(i, (tft.height() - (5 * 13)), ST7735_WHITE);
tft.drawPixel(i, (tft.height() - (6 * 13)), ST7735_WHITE);
tft.drawPixel(i, (tft.height() - (7 * 13)), ST7735_WHITE);
tft.drawPixel(i, (tft.height() - (8 * 13)), ST7735_WHITE);

tft.drawPixel(i - 12, (tft.height() - (2 * 13)), ST7735_WHITE);
tft.drawPixel(i - 12, (tft.height() - (3 * 13)), ST7735_WHITE);
tft.drawPixel(i - 12, (tft.height() - (4 * 13)), ST7735_WHITE);
tft.drawPixel(i - 12, (tft.height() - (5 * 13)), ST7735_WHITE);
tft.drawPixel(i - 12, (tft.height() - (6 * 13)), ST7735_WHITE);
tft.drawPixel(i - 12, (tft.height() - (7 * 13)), ST7735_WHITE);
tft.drawPixel(i - 12, (tft.height() - (8 * 13)), ST7735_WHITE);
}
}
*/

/*void mischen()
{
if (millis() > 80000)
{
//WartenA = millis();    //alles 10 Sekunden die Temperaturen vergleichen
if (Mpause > 75000)
{
Mpause = 0;
sensors.requestTemperatures();
delay(750);
Bist = sensors.getTempC(Brauch);
digitalWrite(Mischer_ZU, LOW);
digitalWrite(Mischer_AUF, LOW);

if (tempsoll < diftime  && mstat == 0)  // Mischer zu - Temperatur runter
{
regelZeit = 0;
Mtimezu = (diftime - tempsoll) * 1000;
//.B.tempist=27(Grad) und tempsoll= 22(Grad) dann geht der Mischer 5*400ms also 2,5sec lang zu
digitalWrite(Mischer_ZU, HIGH);

////Serial.print("Mischzeit ist ");
////Serial.println(((tempist - tempsoll) * 400));
////Serial.println("Mischer faehrt ZU");
mstat = 32;
}

if (tempsoll > diftime  && mstat == 0)      // Mischer auf - Temperatur erhoehen
{
regelZeit = 0;
//MischenA = millis();
Mtimeauf = (tempsoll - diftime) * 1000;
digitalWrite(Mischer_AUF, HIGH);
//Serial.print("Mischzeit ist ");
////Serial.println(((tempsoll - tempist) * 400));
////Serial.println("Mischer faehrt AUF");
mstat = 31;
}
}

if (regelZeit > Mtimezu && mstat == 32)
{
digitalWrite(Mischer_ZU, LOW);
mstat = 0;
coldtime = coldtime + Mtimezu;
}
if (regelZeit > Mtimeauf && mstat == 31)
{
warmtime = warmtime + Mtimeauf;
digitalWrite(Mischer_AUF, LOW);
mstat = 0;
}
if (coldtime > warmtime)
{
coldtime = 0;
warmtime = 0;
}

if (warmtime - coldtime > 100000)
{
warmtime = 100000;
coldtime = 0;
// 		}
//
// 		diftime = (warmtime - coldtime) / 1000;
// 	}
// 	else
// 	{
// 		digitalWrite(Mischer_ZU, HIGH);
// 	}
// 	//--------MISCHEN Ende-----------------
//
// 	if (Kist < (Ksoll - Spanne) && !heaterOn && Kist > 0)
// 	{
// 		digitalWrite(Brenner, HIGH);
// 		heaterOn = true;
// 	}
//
// 	if (Kist >= Ksoll)
// 	{
// 		digitalWrite(Brenner, LOW);
// 		heaterOn = false;
// 	}
// }
// */

void mischen()
{
	if (millis() > 120000)
	{
		//WartenA = millis();    //alles 10 Sekunden die Temperaturen vergleichen
		if (Mpause > 120000)
		{parseSensors();
			//sensors.requestTemperatures();
			delay(750);
			 

			digitalWrite(Mischer_ZU, LOW);
			digitalWrite(Mischer_AUF, LOW);

			if (V1 > (tempsoll + 4) && mstat == 0 && diftime > 0)  // Mischer zu - Temperatur runter
			{
				Mtimezu = 0;
				//.B.tempist=27(Grad) und tempsoll= 22(Grad) dann geht der Mischer 5*400ms also 2,5sec lang zu
				digitalWrite(Mischer_ZU, HIGH);
				////Serial.print("Mischzeit ist ");
				////Serial.println(((tempist - tempsoll) * 400));
				////Serial.println("Mischer faehrt ZU");
				mstat = 32;
			}

			if (V1 < (tempsoll - hyst) && mstat == 0 && diftime < 100)      // Mischer auf - Temperatur erhoehen
			{
				//MischenA = millis();
				Mtimeauf = 0;
				digitalWrite(Mischer_AUF, HIGH);
				//Serial.print("Mischzeit ist ");
				////Serial.println(((tempsoll - tempist) * 400));
				////Serial.println("Mischer faehrt AUF");
				mstat = 31;
			}

			Mpause = 0;
		}

		if (Mtimezu > ((tempist - tempsoll) * 400) && mstat == 32)
		{
			digitalWrite(Mischer_ZU, LOW);
			mstat = 0;
			coldtime = coldtime + Mtimezu;
		}
		if (Mtimeauf > (((tempsoll - tempist) * 400) - (diftime - tempist)*dif) && mstat == 31)
			//if (Mtimeauf > ((tempsoll - tempist) * 400 && mstat == 31))
		{
			warmtime = warmtime + Mtimeauf;
			digitalWrite(Mischer_AUF, LOW);
			mstat = 0;
		}
		if (coldtime > warmtime)
		{
			coldtime = 0;
			warmtime = 0;
		}

		if (warmtime - coldtime > 100000)
		{
			warmtime = 100000;
			coldtime = 0;
		}

		diftime = (warmtime - coldtime) / 1000;
	}
	else
	{
		digitalWrite(Mischer_ZU, HIGH);
	}
	//--------MISCHEN Ende-----------------

	if (Kist < (Ksoll - Spanne) && !heaterOn && Kist > 0)
	{Serial.println("Brenner an");
  Serial.println(Kist);
  Serial.println(Ksoll);
		digitalWrite(Brenner, HIGH);
		heaterOn = true;
	}

	if (Kist >= Ksoll && heaterOn)
	{Serial.println("Brenner aus");
  Serial.println( Kist);
  Serial.println( Ksoll);
		digitalWrite(Brenner, LOW);
		heaterOn = false;
	}
}



void setup(){
	
	Ethernet.begin(mac, ip, gateway, gateway, subnet);
  mqttclient.begin("192.168.1.29", client);
  mqttclient.onMessage(messageReceived);

  mqttconnect();
	//server.begin();
	

	sensors.setResolution(Kessel, 10);
	sensors.setResolution(Aussen, 10);
	sensors.setResolution(Brauch, 10);
	sensors.setResolution(V1wire, 10);
	sensors.setResolution(R1wire, 10);
	sensors.setResolution(V2wire, 10);
	sensors.setResolution(R2wire, 10);
  sensors.begin();
  sensors.requestTemperatures();
	Bist = 50;
	Serial.begin(115200);
	dif = 300;
	senshigh = 1000;
	pinMode(Pumpe, OUTPUT);
	pinMode(Brenner, OUTPUT);
	pinMode(Mischer_AUF, OUTPUT);
	pinMode(Mischer_ZU, OUTPUT);
	Pstat = false;
	col[1] = 0xF87E;
	col[2] = 0x3922;
	col[3] = 0xF800;
	col[4] = 0x07E0;
	col[5] = 0x07FF;
	col[6] = 0xF81F;
	col[7] = 0xFFE0;
	col[8] = 0xFF25;
	Labl[1] = "V1";
	Labl[2] = "R1";
	Labl[3] = "V2";
	Labl[4] = "R2";
	Labl[5] = "KE";
	Labl[6] = "BW";
	Labl[7] = "AT";
	Labl[8] = "MI";
	analogWrite(6, 250);
	Serial.println("initialization done.");
	Serial.print("Chat ser0ver address:");
	Serial.println(Ethernet.localIP());
	//Serial.println("HV;HR;BV;BR;Kessel;Aussen;Brauch;HV SOLL;Pumpe;Mstat;MProzent");
	digitalWrite(Pumpe, LOW);
	//digitalWrite(4, HIGH);
	digitalWrite(Brenner, LOW);
	digitalWrite(Mischer_AUF, LOW);
	digitalWrite(Mischer_ZU, LOW);
	Mpause = 0;
	//tft.initR(INITR_BLACKTAB);
	mstat = 0;
	diftime = 0;


	Bist = 10;
	
	

}

void loop()
{
	mqttclient.loop();
	////Serial.print("Loopcount:   ");
	//Serial.println(loopcount);
	if (loopcount > 200)
	{
		parseSensors();
    loopcount = 0;
	}
	loopcount++;

if(!mqttclient.connected()){
  mqttconnect();
}


//	tcpReq();
	mischen();

if(publishintervall > 6000){
  mqttpublish();
  Serial.println("Publishing");
  publishintervall = 0;
}
if(TankIntervall > 6000){
  distance = sonar.ping_cm();

  // Send results to Serial Monitor
  Serial.print("Distance = ");

  if (distance >= 400 || distance <= 2) {
    Serial.println("Out of range");
	}
	else 
	{
		Serial.print(distance);
		Serial.println(" cm");
	}
	//delay(500);
  
  TankIntervall = 0;


}}






