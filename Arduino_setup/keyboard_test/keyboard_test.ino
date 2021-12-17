#include <Keyboard.h>
#include "circular_buffer.hpp"
#define CLOCKPIN 3
#define DATAPIN 8

/*Setting up of the keyboard PS2 interface*/
byte keystroke;
int bitCount = 11;
bool parity;
bool parity_check;
bool stopbit;
bool startbit;

bool flag;
/*End of setup*/

CircularBuffer key_buffer = CircularBuffer();

void setup() {
  // put your setup code here, to run once:
  pinMode(DATAPIN, INPUT_PULLUP);
  pinMode(CLOCKPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCKPIN), readPs2, FALLING);
  
  flag = false;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("Testing");
  byte key_press;
  char actual_key;
  if (key_buffer.isEmpty() == false) {
    //Keyboard.begin();
    key_press = key_buffer.removeByte();

    if (key_press != 0xF0){
      actual_key = key_translate(key_press);
      Serial.print(actual_key);
      Serial.print("\n");
      //Keyboard.print('h');
      //Keyboard.end();
    }
      
    flag = false;
  }
}

char key_translate(byte ps2_code) {

  switch(ps2_code){
    case 0x69:
      return '1';
    case 0x70:
      return '0';
    case 0x71:
      return '.';
    case 0x72:
      return '2';
    case 0x73:
      return '5';
    case 0x74:
      return '6';
    case 0x75:
      return '8';
    case 0x7A:
      return '3';
    case 0x6B:
      return '4';
    case 0x6C:
      return '7';
    case 0x7D:
      return '9';
    default:
      break;
  }
}

void readPs2(){
  //When there is falling edge, read the input pin immediately
  bool currentValue;
  switch(bitCount){
    case 11:
      //check start bit
      startbit = digitalRead(DATAPIN);
      if (startbit == true){
        exit(1);
      }
      keystroke = 0x00;
      bitCount--;
      parity_check = true;
      break;
    case 10:
    case 9:
    case 8:
    case 7:
    case 6:
    case 5:
    case 4:
    case 3:
      // read the bit and push the value into the number
      currentValue = digitalRead(DATAPIN);
      if (currentValue) {
        keystroke = (keystroke >> 1) + 0x80;
        parity_check = parity_check ^ true;
      }
      else {
        keystroke = (keystroke >> 1) + 0x00;
      }
      bitCount--;
      break;
    case 2:
      //parity check
      parity = digitalRead(DATAPIN);
      if (parity != parity_check) {
        exit(1);
      }
      bitCount--;
      break;
    
    case 1:
      //stop bit check and reset to 11
      stopbit = digitalRead(DATAPIN);
      if (stopbit == false){
        exit(1);
      }
      bitCount = 11;
      flag = true;
      
      //Now, store byte into buffer
      if (key_buffer.isFull() == true) {
        break;
      }
      key_buffer.addByte(keystroke);
      break;
    default:
      break;
    
  }
}
