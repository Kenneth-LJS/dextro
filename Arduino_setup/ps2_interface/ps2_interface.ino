#define CLOCKPIN 3
#define DATAPIN 8

byte keystroke;
int bitCount = 11;
bool parity;
bool parity_check;
bool stopbit;
bool startbit;

bool flag;
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
  if (flag) {
    Serial.print(keystroke, HEX);
    Serial.print("\n");
    flag = false;
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
    
    
  }
}
