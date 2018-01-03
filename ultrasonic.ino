/* ----------ICEBERG ROBOTS----------
 *  Ultraschallnano 2018
*/

//Pins der Ultraschall-Sensoren
#define US_Front 12                   //Der Ultraschallsensor vorne
#define US_Back 9                     //Der Ultraschallsensor hinten
#define US_Left 10                    //Der Ultraschallsensor links
#define US_Right 11                   //Der Ultraschallsensor rechts

#define MAX_DISTANCE 250              //Die maximale Distanz (in cm), welche die US-Sensoren messen

#define UsInterrupt 3                 //Interrupt-Pin, der aktiviert wird, um die US-Werte abzufragen

#include <NewPing.h>

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
  pinMode(UsInterrupt,INPUT);                                                //definiert den Interupt-Pin als Eingang
  attachInterrupt(digitalPinToInterrupt(UsInterrupt), usAusgeben, RISING);   //erstellt den Interrupt -> wenn das Signal am Interruptpin ansteigt, dann wird die Methode usAusgeben ausgeführt
}

//------------------------------------------------------------

void loop(){                        //Loop-Methode
  wert[0] = sonarR.ping_cm();       //Auslesen des rechten Ultraschallsensors
  wert[1] = sonarF.ping_cm();       //Auslesen des vorderen Ultraschallsensors
  wert[2] = sonarL.ping_cm();       //Auslesen des linken Ultraschallsensors
  wert[3] = sonarB.ping_cm();       //Auslesen des hinteren Ultraschallsensors
  delay(30);                        //kurze Wartezeit, da die Ultraschallsensoren nicht direkt hintereinander ausgelesen werden können
}

//------------------------------------------------------------

void usAusgeben(){                  //Methode wird beim Interrupt aufgerufen und gibt dem Mega die US-Werte aus
  for(int i=0;i<4;i++){
    Serial.write(wert[i]);          //sendet die Werte über UART an den Arduino Mega
  } 
}
/* Was ist das??
  byte output = 42;
  byte input;

  //senden:
  Serial.write(output);        //senden eines Bytes

  //empfangen:
  if(Serial.available() > 0){  //prüfen, ob ein Byte gesendet wurde
    input = Serial.read()      //empfangen eines Bytes
  }
*/
