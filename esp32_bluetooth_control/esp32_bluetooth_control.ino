#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
int led = 2;
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  SerialBT.begin("SmartHome01"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
void loop() {

  char x = ' ';

  if (SerialBT.available()) {
    x = SerialBT.read();
    Serial.println(x);
    if (x == 'a') {
      digitalWrite(led, LOW);
    }
    if (x == 'b') {
      digitalWrite(led, HIGH);
    }
  }
delay(20);
}
