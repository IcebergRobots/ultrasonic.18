/* ----------ICEBERG ROBOTS----------
    Ultraschallnano 2018
    - misst kontinuierlich Messwerte der Ultraschallsensoren
    - zum Abrufen sendet der Hauptkontroller einen Interupt-Befehl
*/

#include <NewPing.h>

//Pins der Ultraschall-Sensoren
#define INTERRUPT 3 //Interrupt-Pin, der aktiviert wird, um die US-Werte abzufragen
#define LED 13      // Status-LED für Interrupt
#define US_Front 12 // Ultraschallsensor vorne
#define US_Back 9   // Ultraschallsensor hinten
#define US_Left 10  // Ultraschallsensor links
#define US_Right 11 // Ultraschallsensor rechts

#define MAX_DISTANCE 253  // Die maximale Distanz (in cm), welche die US-Sensoren messen
#define START_MARKER 254  // Startzeichen einer Nachricht
#define END_MARKER 255    // Endzeichen einer Nachricht

// Debug Modus, um Daten über USB auszulesen
unsigned long ledTimer = 0;
unsigned long timestamp = 0;

//------------------------------------------------------------

int values[] = {255, 255, 255, 255};                  //In diesem Array werden die Werte, die die US-Sensoren messen, gespeichert

NewPing sonarF(US_Front, US_Front, MAX_DISTANCE);   //Hier wird das Objekt für den vorderen US-Sensor erstellt
NewPing sonarB(US_Back, US_Back, MAX_DISTANCE);     //Hier wird das Objekt für den hinteren US-Sensor erstellt
NewPing sonarL(US_Left, US_Left, MAX_DISTANCE);     //Hier wird das Objekt für den linken US-Sensor erstellt
NewPing sonarR(US_Right, US_Right, MAX_DISTANCE);   //Hier wird das Objekt für den rechten US-Sensor erstellt

//------------------------------------------------------------

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);                                                   // startet die Serielle Kommunikation
  Serial.print("\nICEBERG ROBOTS\nULTRASONIC\n");                         // Startnachricht
  pinMode(INTERRUPT, INPUT);                                              // definiert den Interupt-Pin als Eingang
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), usAusgeben, RISING);  // erstellt den Interrupt -> wenn das Signal am Interruptpin ansteigt, dann wird die Methode usAusgeben ausgeführt
}
//------------------------------------------------------------

void loop() {                       //Loop-Methode
  values[0] = sonarR.ping_cm();       //Auslesen des rechten Ultraschallsensors
  digitalWrite(LED, millis() <= ledTimer);
  values[1] = sonarF.ping_cm();       //Auslesen des vorderen Ultraschallsensors
  digitalWrite(LED, millis() <= ledTimer);
  values[2] = sonarL.ping_cm();       //Auslesen des linken Ultraschallsensors
  digitalWrite(LED, millis() <= ledTimer);
  values[3] = sonarB.ping_cm();       //Auslesen des hinteren Ultraschallsensors
  timestamp = millis();
  do {    //kurze Wartezeit, da die Ultraschallsensoren nicht direkt hintereinander ausgelesen werden können
    digitalWrite(LED, millis() <= ledTimer);
  } while (millis() - timestamp < 30);
  //Serial.print(String(values[0])+","+String(values[1])+","+String(values[2])+","+String(values[3])+"\n");

}

//------------------------------------------------------------

void usAusgeben() {                 //Methode wird beim Interrupt aufgerufen und gibt dem Mega die US-Werte aus
  ledTimer = millis() + 50;
  digitalWrite(LED, HIGH);
  Serial.write(START_MARKER);
  for (int i = 0; i < 4; i++) {
    if (values[i] == 0) {
      values[i] = 253;
    }
    Serial.write(constrain(values[i],0,253));          //sendet die Werte über UART an den Arduino Mega
  }
  Serial.write(END_MARKER);
}
