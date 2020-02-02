#include <bufferfiller.h>
#include <enc28j60.h>
#include <avr/pgmspace.h>
#include <EtherCard.h>
#include <net.h>
#include <stash.h>
#include <bufferfiller.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <Wire.h>
#include <ArpUtil.h>
#include "rgb_lcd.h"
rgb_lcd lcd;
byte flag_1=0;  
byte result;
//char str[12] = {0};
//const int colorR = 0;//0
//const int colorG = 100;//191
//const int colorB = 200;//255
void setup() {
    pinMode(6, OUTPUT);// жёлтый светодиод
    pinMode(7, OUTPUT);// зелёный светодиод 
    pinMode(8, INPUT);// кнопка 1
    pinMode(9, INPUT);// кнопка 2
    //digitalWrite(6, HIGH);
    //digitalWrite(7, HIGH);
    //Подготовка формата arp-пакета
  	arputil.begin("74:69:60:2D:30:34");
  	//Соединение с Ethernet-портом
  	arputil.connect();
    //flag_GW = 1;
    //Проверка соединения
  	while (!arputil.isConnected()) {
        digitalWrite(6, LOW);
        delay(1000);
  	} 
    lcd.begin(16, 2);
    lcd.setRGB(0, 100, 200);
     lcd.setCursor(0, 0);
     lcd.print("connected to LAN");
     digitalWrite(6, HIGH);
}

void loop() {
  if (digitalRead(8)==HIGH)
  {
    flag_1 = 2;
  } 
 // Serial.println(flag_1);
  arputil.net_pool(flag_1);
  //flag_1 = 0;
}
