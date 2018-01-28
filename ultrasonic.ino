/* ----------ICEBERG ROBOTS----------
 *  Ultraschallnano 2018
 *  - misst kontinuierlich Messwerte der Ultraschallsensoren
 *  - zum Abrufen sendet der Hauptkontroller einen Interupt-Befehl
*/

#include <NewPing.h>

//Pins der Ultraschall-Sensoren
#define US_Front 12                   //Der Ultraschallsensor vorne
#define US_Back 9                     //Der Ultraschallsensor hinten
#define US_Left 10                    //Der Ultraschallsensor links
#define US_Right 11                   //Der Ultraschallsensor rechts

#define MAX_DISTANCE 250              //Die maximale Distanz (in cm), welche die US-Sensoren messen

#define INTERRUPT 3                 //Interrupt-Pin, der aktiviert wird, um die US-Werte abzufragen
#define LOOP_TIME 30                  //Zeit nach der die Schleife wiederholt wird
#define LED_TIME 1000                 //Zeit nach der die Led umschaltet

// Debug Modus, um Daten über USB auszulesen
#define DEBUG true
#define debug(_str_); if(DEBUG){Serial.print(_str_);}
#define debugln(_str_); if(DEBUG){Serial.println(_str_);}
unsigned long lastLoop = 0;
unsigned long lastLed = 0;
bool isLed = true;

//------------------------------------------------------------

int wert[] = {255, 255, 255, 255};                  //In diesem Array werden die Werte, die die US-Sensoren messen, gespeichert

NewPing sonarF(US_Front, US_Front, MAX_DISTANCE);   //Hier wird das Objekt für den vorderen US-Sensor erstellt
NewPing sonarB(US_Back, US_Back, MAX_DISTANCE);     //Hier wird das Objekt für den hinteren US-Sensor erstellt
NewPing sonarL(US_Left, US_Left, MAX_DISTANCE);     //Hier wird das Objekt für den linken US-Sensor erstellt
NewPing sonarR(US_Right, US_Right, MAX_DISTANCE);   //Hier wird das Objekt für den rechten US-Sensor erstellt

//------------------------------------------------------------

void setup(){
  pinMode(13,OUTPUT);
  Serial.begin(9600);                                                        //startet die Serielle Kommunikation
  pinMode(INTERRUPT,INPUT);                                                //definiert den Interupt-Pin als Eingang
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), usAusgeben, RISING);     //erstellt den Interrupt -> wenn das Signal am Interruptpin ansteigt, dann wird die Methode usAusgeben ausgeführt
}
//------------------------------------------------------------

void loop(){                        //Loop-Methode
  digitalWrite(13,isLed);
  wert[0] = sonarR.ping_cm();       //Auslesen des rechten Ultraschallsensors
  wert[1] = sonarF.ping_cm();       //Auslesen des vorderen Ultraschallsensors
  wert[2] = sonarL.ping_cm();       //Auslesen des linken Ultraschallsensors
  wert[3] = sonarB.ping_cm();       //Auslesen des hinteren Ultraschallsensors
  debugln("Right="+(String)wert[0]+" Front="+(String)wert[1]+" Left="+(String)wert[2]+" Back="+(String)wert[3]);
  do {    //kurze Wartezeit, da die Ultraschallsensoren nicht direkt hintereinander ausgelesen werden können
    if(millis()-lastLed>=LED_TIME) {
      lastLed = millis();
      isLed = !isLed;
    }
  } while(millis()-lastLoop<LOOP_TIME);
  lastLoop = millis();
  
}

//------------------------------------------------------------

void usAusgeben(){                  //Methode wird beim Interrupt aufgerufen und gibt dem Mega die US-Werte aus
  for(int i=0;i<4;i++){
    Serial.write(wert[i]);          //sendet die Werte über UART an den Arduino Mega
  } 
}
