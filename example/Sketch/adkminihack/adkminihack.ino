#include <Usb.h>
#include <AndroidAccessory.h>

#define LED          13

AndroidAccessory acc("GDG Suwon",            // Manufacturer
                     "MiniHackArduino",                 // Model
                     "MiniHack sample Arduino Board",   // Description
                     "1.0",                     // Version
                     "http://gdg-suwon.blogspot.kr/2012/06/gdg-suwon-hack-time-for-android-adk.html",  // URI
                     "0000000012345678");       // Serial
int buttonState = 0;

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\nADK Started\r\n");

  pinMode(LED, OUTPUT);

  // Power On Android Accessory interface and init USB controller
  acc.powerOn();
}


void loop()
{
  byte data[2];
  if (acc.isConnected()) {
    int len = acc.read(data, sizeof(data), 1);
    if (len > 0) {
      if (data[0] == 0x1) {
        digitalWrite(LED, data[1] ? HIGH : LOW);
        Serial.print("Toggle LED\r\n");
        data[0] = 0x2;
        acc.write(data, 2);
      }
    }
  }

  delay(10);
}

