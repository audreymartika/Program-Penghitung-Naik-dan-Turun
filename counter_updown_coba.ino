/*
  E18-D80NK-Infrared-Switch
  counter up ya coba
*/
#include <Keypad.h>
#include <TimerOne.h>
#include <EEPROM.h>

int Pin_ir = 2;
int Pin_ir1 = 3;
int counter;
int maxcounter;
int hitObject = false;
bool isMaxCounter;
const byte numRows = 4; //baris pada keypad
const byte numCols = 4; //Kolom pada keypad

/*keymap mendefinisikan tombol ditekan sesuai
  dengan baris dan kolom seperti muncul pada keypad*/
char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[numRows] = {11, 10, 9, 8};
byte colPins[numCols] = {7, 6, 5, 4};


Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

bool enableUp;
bool enableDown;

void detectUp() {
  if (enableDown == true) down();
  else enableUp = true;
  Serial.print("UP => "); Serial.println(enableUp);
}

void detectDown() {
  if (enableUp == true) up();
  else enableDown = true;
  Serial.print("DOWN => "); Serial.println(enableDown);
}

void up() {
  if (enableUp) {
    if (counter < maxcounter - 1) {
      counter++;
      EEPROM.write(1, counter);
      isMaxCounter = false;
    }
    else isMaxCounter = true;
    enableUp = false;
  }
}

void down() {
  if (enableDown) {
    if (counter > 0)counter--;
    enableDown = false;
    EEPROM.write(1, counter);
  }
}

void setup() {
  maxcounter = EEPROM.read(0);
  counter = EEPROM.read(1);
  pinMode(Pin_ir , INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Pin_ir), detectUp, RISING);
  pinMode(Pin_ir1 , INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Pin_ir1), detectDown, RISING);
  Serial.begin(9600);
  Timer1.initialize(100000);
  Timer1.attachInterrupt(keyPad);
  //  while(true){

  //  }
}

void loop() {
  Serial.println("Counter : " + (String)counter);
  if (isMaxCounter == true)Serial.println("DAH PENUH");
  else Serial.println("MONGGO");
  delay(500);
}
String data;
void keyPad() {
  char keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
  {
    data += keypressed;
    if (keypressed == 'A') {
      maxcounter = data.toInt();
      EEPROM.write(0, maxcounter);
      Serial.println("Setting max counter : " + (String)maxcounter);
      data = "";
      //      Serial.println(keypressed);
      //      counter = keypressed - '0';
    }
    if (keypressed == 'B') {
      counter = data.toInt();
      EEPROM.write(1, counter);
      counter = EEPROM.read(1);
      Serial.println("Counter : " + (String)counter);
      data = "";
    }
    if (keypressed == 'C') {
      EEPROM.write(0, 0);
      EEPROM.write(1, 0);
      counter = 0;
      maxcounter = 0;
      data = "";
    }
  }

}
