//Shift Register 74HC595   

//Button 
boolean buttonOn = 0;
boolean buttonOff = 0;
int pinIn = 12;
int pinOut = 11;
int debounceDelay = 10;
//Ones
int latchPinOnes = 4; //latch storage pin ST_CP
int clockPinOnes = 2; //shift clock pin SH_CP 
int dataPinOnes = 6; //data pin DS 
//Tens
int latchPinTens = 7; 
int clockPinTens = 4;
int dataPinTens = 11;
// var
int wait = 500;
const int arrayStart = 0;
const int arrayEnd = 10;
//data arrays
byte data;
byte dataArray[arrayEnd];

void setup() 
{
  //button pin designation
  pinMode (pinIn, INPUT);
  pinMode (pinOut, OUTPUT);
  //set Tens pins 
  pinMode(latchPinTens, OUTPUT);
  //Serial.begin(9600);
  //set Ones pins 
  pinMode(latchPinOnes, OUTPUT);
  Serial.begin(9600);
  //Hex value number output as comment
  dataArray[0] = 0x3F;//0
  dataArray[1] = 0x06;//1 
  dataArray[2] = 0x5B;//2 
  dataArray[3] = 0x4F;//3 
  dataArray[4] = 0x66;//4 
  dataArray[5] = 0x6D;//5 
  dataArray[6] = 0x7D;//6 
  dataArray[7] = 0x07;//7 
  dataArray[8] = 0x7F;//8 
  dataArray[9] = 0x67;//9
  dataArray[10] = 0;
//  dataArray[10] = 0x77;//A
//  dataArray[11] = 0x7C;//b
//  dataArray[12] = 0x39;//c 
//  dataArray[13] = 0x5B;//C 
//  dataArray[14] = 0x5E;//d 
//  dataArray[15] = 0x79;//E 
//  dataArray[16] = 0x71;//F 
//  dataArray[17] = 0x76;//H 
//  dataArray[18] = 0x04;//i
//  dataArray[19] = 0x1E;//J
//  //dataArray[20] = 0x;
//  dataArray[21] = 0x38;//L
//  //dataArray[22] = 0x; 
//  dataArray[23] = 0x54;//n
//  dataArray[24] = 0x5C;//o
//  dataArray[25] = 0x3F;//O 
//  dataArray[26] = 0x73;//P
//  //dataArray[27] = 0x; 
//  dataArray[28] = 0x50;//r
//  dataArray[29] = 0x6D;//S
//  //dataArray[30] = 0x;
//  dataArray[31] = 0x1C;//u
//  dataArray[32] = 0x3E;//U
//  //dataArray[33] = 0x;//V
//  //dataArray[34] = 0x;//W 
//  //dataArray[35] = 0x;//X 
//  //dataArray[36] = 0x;//Y 
//  //dataArray[37] = 0x;//Z 
   
//function that blinks all the LEDs
//gets passed the number of blinks and the pause time
blinkAll_2Bytes(2,500); 
}
//Button
//change button input and stablize
boolean debounce (boolean was)
{
  boolean is = digitalRead (pinIn);
  if (is != was)
    {
      delay (debounceDelay);
      is = digitalRead (pinIn);
    }
  return is;
}
//B end
void loop() 
{
   //Button
   buttonOn = debounce(buttonOff);
   if(buttonOff == LOW && buttonOn == HIGH)
   {
   //B end
    data = dataArray[arrayEnd];
    digitalWrite(latchPinTens, 0);
    shiftOut(dataPinTens, clockPinTens, data);
    digitalWrite(latchPinTens, 1);
    digitalWrite(latchPinOnes, 0);
    shiftOut(dataPinOnes, clockPinOnes, data);
    digitalWrite(latchPinOnes, 1);
      //tens
      for (int t = arrayStart; t < arrayEnd; t++) 
      {
        data = dataArray[t];
        digitalWrite(latchPinTens, 0);
        shiftOut(dataPinTens, clockPinTens, data);
        digitalWrite(latchPinTens, 1);
          //ones
          for (int o = arrayStart; o < arrayEnd; o++) 
          {
            data = dataArray[o];
            digitalWrite(latchPinOnes, 0);
            shiftOut(dataPinOnes, clockPinOnes, data);
            digitalWrite(latchPinOnes, 1);
            delay(wait);
           } 
       } 
    }   
data = dataArray[arrayEnd + 1];
digitalWrite(latchPinTens, 0);
shiftOut(dataPinTens, clockPinTens, data);
digitalWrite(latchPinTens, 1);
data = dataArray[arrayEnd + 1];
digitalWrite(latchPinOnes, 0);
shiftOut(dataPinOnes, clockPinOnes, data);
digitalWrite(latchPinOnes, 1);
//Button
buttonOff = buttonOn;
//B end   
} 
// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) 
{
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low
  
  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE arrayING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  
  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) 
    {
      pinState= 1;
    }
    else
    {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }
  //stop shifting
  digitalWrite(myClockPin, 0);
}
   

//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) 
{
  digitalWrite(latchPinTens, 0);
  shiftOut(dataPinTens, clockPinTens, 0);
  shiftOut(dataPinTens, clockPinTens, 0);
  digitalWrite(latchPinTens, 1);
  delay(200);
  for (int x = 0; x < n; x++) 
  {
    digitalWrite(latchPinTens, 0);
    shiftOut(dataPinTens, clockPinTens, 255);
    shiftOut(dataPinTens, clockPinTens, 255);
    digitalWrite(latchPinTens, 1);
    digitalWrite(latchPinOnes, 0);
    shiftOut(dataPinOnes, clockPinOnes, 255);
    shiftOut(dataPinOnes, clockPinOnes, 255);
    digitalWrite(latchPinOnes, 1);
    delay(d);
    digitalWrite(latchPinTens, 0);
    shiftOut(dataPinTens, clockPinTens, 0);
    shiftOut(dataPinTens, clockPinTens, 0);
    digitalWrite(latchPinTens, 1);
    digitalWrite(latchPinOnes, 0);
    shiftOut(dataPinOnes, clockPinOnes, 0);
    shiftOut(dataPinOnes, clockPinOnes, 0);
    digitalWrite(latchPinOnes, 1);
    delay(d);
  }
}
//void blinkAllOnes_2Bytes(int n, int d) 
//{
//  
//  delay(200);
//  for (int x = 0; x < n; x++) 
//  {
//    digitalWrite(latchPinOnes, 0);
//    shiftOut(dataPinOnes, clockPinOnes, 255);
//    shiftOut(dataPinOnes, clockPinOnes, 255);
//    digitalWrite(latchPinOnes, 1);
//    delay(d);
//    digitalWrite(latchPinOnes, 0);
//    shiftOut(dataPinOnes, clockPinOnes, 0);
//    shiftOut(dataPinOnes, clockPinOnes, 0);
//    digitalWrite(latchPinOnes, 1);
//    delay(d);
//  }
//}
