//#include <bufferfiller.h>
//#include <enc28j60.h>
//#include <avr/pgmspace.h>
//#include <EtherCard.h>
//#include <net.h>
//#include <stash.h>
//#include <bufferfiller.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <Wire.h>
#include <ArpUtil.h>
#include "rgb_lcd.h"
rgb_lcd lcd;
byte work_mode = 0, pr_mode = 1;
byte flag1 = 0; 
boolean buttonWasUp = true;  // была ли кнопка отпущена? 
byte result;
//char str[12] = {0};
void setup() {
    pinMode(6, OUTPUT);// жёлтый светодиод
    pinMode(8, INPUT);// кнопка 1
    pinMode(9, INPUT);// кнопка 2
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
  if (pr_mode == 1)
  {
    //Serial.println("begin");
     while (work_mode == 0) 
    {
      bool current = digitalRead(8);
      if (current && flag1 == 0)
      {
        while (current != LOW) {                  // Старое значение отличается от полученного
          delay(10);                                  // Ждем пока состояние стабилизируется - игнорируем дребезг
          current = digitalRead(8);           // Считываем стабилизированное значение
        }
        flag1 = 1;
        work_mode = 1;
        
      }
      if (digitalRead(9)==HIGH)
      {
        work_mode = 2;
      } 
    }
    arputil.net_pool(work_mode);
    result = arputil.result;
    if (digitalRead(8)==HIGH && flag1 != 1) 
    {
      flag1 = 1;
      pr_mode = 2;
    //  work_mode = 3;
    }
    flag1 = 0;
    //Serial.println(arputil.result);
  } else if (pr_mode == 2)
  {
    if (work_mode == 1)
    { //вывод на дисплей
     // Serial.println(result);
      if (result == 2)
      {
        printRemoteMAC();
        //delay(1000);
      } else 
      { 
        printNoHost();
        delay(1000);
      }
    } else if (work_mode == 2)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Attacked:");
      //delay(1000);
      if (result == 3) 
      {
        printRemoteMAC();
        //delay(1000);
        //printRemoteIP();
        //delay(1000);
      } else printNoHost();
    }
  }
}

void printRemoteMAC()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Remote MAC");
  lcd.setCursor(0, 1);
  for (byte i = 0; i < 6; i++) {
    lcd.print(arputil.remotemac[i], HEX);    
  }
  delay(1000);
}

void printRemoteIP()
{
  //lcd.begin(16, 2);
  //lcd.setRGB(0, 100, 200);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.setCursor(0, 1);
  for (byte i = 0; i < 4; i++) {
    lcd.print(arputil.remoteip[i]);    
  }
}

void printNoHost()
{
  //lcd.begin(16, 2);
  //lcd.setRGB(0, 100, 200);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No host");
}

//if (result == 3)
//  {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("remote_mac:");
//    lcd.setCursor(0, 1);
//    for (byte i = 0; i < 6; i++) {
//     lcd.print(arputil.remotemac[i]);    
//    }
//  } else if (result == 4)
//  {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Hacking");
//  }
