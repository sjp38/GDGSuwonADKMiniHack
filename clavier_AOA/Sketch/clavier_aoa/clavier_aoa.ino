#include <Usb.h>
#include <AndroidAccessory.h>

#define LED0          13

// Device To Accessory
#define DTA_PLAY    0x1
#define DTA_STOP    0x2
// Accessory To Device
#define ATD_NIGHT   0x3

int speakerPin = 9;
int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
boolean mute = true;
int play_tone = 0;
int play_level = 1;


int data = 2; 
int clock = 3;
int latch = 4;


// RGB LED leads connected to PWM pins
const int RED_LED_PIN = 10;
const int GREEN_LED_PIN = 11;
const int BLUE_LED_PIN = 12;
int reds[] = {0xff, 0xff, 0xff, 0x32, 0x00, 0x48, 0xee, 0xdc};
int greens[] = {0, 0x45, 0xd7, 0xcd, 0x00, 0x3d, 0x82, 0x14};
int blues[] = {0, 0, 0x00, 0x32, 0xff, 0x8b, 0xee, 0x3c};


//PhotoResistor Pin
int lightPin = 0;
boolean night = false;


// Potentiometer
int potentiometerPin = 3;

AndroidAccessory acc("GDG Suwon",            // Manufacturer
                     "clavier_aoa",                 // Model
                     "MiniHack project Arduino Board",   // Description
                     "1.0",                     // Version
                     "http://gdg-suwon.blogspot.kr/2012/06/gdg-suwon-hack-time-for-android-adk.html",  // URI
                     "0000000012345678");       // Serial


void playTone() {
  digitalWrite(speakerPin, HIGH);
  delayMicroseconds(play_tone / play_level);
  digitalWrite(speakerPin, LOW);
  delayMicroseconds(play_tone / play_level);
}

void setTone(int value) {
  mute = false;
  play_tone = value;
}

void turnOffTone() {
  mute = true;
}

void setPlayLevel(int value) {
  play_level = value;
}


/*
 * updateLEDs() - sends the LED states set in ledStates to the 74HC595
 * sequence
 */
void updateLEDs(int value){
  digitalWrite(latch, LOW);     //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH);   //Pulls the latch high displaying the data
}

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\nADK Started\r\n");

  pinMode(speakerPin, OUTPUT);
  pinMode(LED0, OUTPUT);
  
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT);  

  // Power On Android Accessory interface and init USB controller
  acc.powerOn();
}


void loop()
{
  byte data[2];
  if (acc.isConnected()) {
    int len = acc.read(data, sizeof(data), 1);
    if (len > 0) {
      if (data[0] == DTA_PLAY) {
        setTone(tones[data[1]]);
        updateLEDs(1 << data[1]);
        analogWrite(RED_LED_PIN, reds[data[1]]);
        analogWrite(GREEN_LED_PIN, greens[data[1]]);
        analogWrite(BLUE_LED_PIN, blues[data[1]]);
        Serial.println("play");
      } else if (data[0] == DTA_STOP) {
        turnOffTone();
        updateLEDs(0);
        analogWrite(RED_LED_PIN, 0);
        analogWrite(GREEN_LED_PIN, 0);
        analogWrite(BLUE_LED_PIN, 0);
        Serial.println("mute");
      }
    }
    
    if (!mute) {
      playTone();
    }
    
    int lightLevel = analogRead(lightPin);
    boolean newNight = lightLevel > 700;
    // comment next before release
    //Serial.println(lightLevel);
    if (night != newNight) {
      Serial.println("time changed!");
      Serial.println(night ? "night" : "morning");
      night = newNight;
      data[0] = ATD_NIGHT;
      data[1] = night ? 0x1 : 0x0;
      acc.write(data, 2);
    }
    night = newNight;
    
    
    int potention = analogRead(potentiometerPin);
    //Serial.println(potention);
    setPlayLevel(potention / 400);
    
  }

  delay(10);
}

