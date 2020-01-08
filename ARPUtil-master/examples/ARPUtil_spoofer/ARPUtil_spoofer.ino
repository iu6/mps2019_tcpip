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
byte flag_1=0, flag_2=0,flag_GW=0, flag_1_dc=0, flag_2_dc=0;  
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
//    lcd.begin(16, 2);
//    lcd.setRGB(0, 100, 200);
//    lcd.setCursor(0, 0);
//    lcd.print("connected to LAN");
     digitalWrite(6, HIGH);
}

void loop() {
  if (digitalRead(8)==HIGH)
  {
    flag_1 = 1;
  } else if (digitalRead(9)==HIGH)
  {
    flag_1 = 2;
  }
  arputil.net_pool(flag_1);
  //КНОПКА 1
//    if(digitalRead(8)==HIGH&&flag_1==0){
//       flag_1 = 1;
//       lcd.clear(); 
//       lcd.setCursor(0, 0); lcd.print("MAC_my:");
//       lcd.setCursor(0, 1);
//       for (byte i = 0; i < 6; ++i) {
//        lcd.print(arputil.mymac[i], HEX);
//        if ((i%2) == 0)lcd.print(':');       
//       }
//       flag_1_dc = 1;
}
