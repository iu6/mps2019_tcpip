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
int flag_1=0, flag_2=0,flag_GW=0, flag_1_dc=0, flag_2_dc=0;  
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
        delay(1000);
  	} 
    lcd.begin(16, 2);
    lcd.setRGB(0, 100, 200);
    lcd.setCursor(0, 0);
    lcd.print("connected to LAN");
    digitalWrite(6, HIGH);
}

void loop() {
  arputil.net_pool();
  //КНОПКА 1
    if(digitalRead(8)==HIGH&&flag_1==0){
       flag_1 = 1;
       lcd.clear(); 
       lcd.setCursor(0, 0); lcd.print("MAC_my:");
       lcd.setCursor(0, 1);
       for (byte i = 0; i < 6; ++i) {
        lcd.print(arputil.mymac[i], HEX);
        if ((i%2) == 0)lcd.print(':');       
       }
       flag_1_dc = 1;
   }
      if(digitalRead(8)==LOW&&flag_1_dc==1){flag_1_dc=0; flag_1=0;}
//       if(digitalRead(8)==HIGH&&flag_1==1&&flag_1_dc==1){
//       lcd.clear(); 
//       lcd.setCursor(0, 0); lcd.print("MAC_neighbour:");
//       lcd.setCursor(0, 1);
//       for (byte i = 0; i < 6; ++i) {
//        lcd.print(arputil.packet.data.arp.dstMac[i], HEX);
//        if ((i%2) == 0)lcd.print(':');       
//       }
//       flag_1_dc = 2;
//   }
//   if(digitalRead(8)==HIGH&&flag_1==1&&flag_1_dc==1){
//       lcd.clear(); 
//       lcd.setCursor(0, 0); lcd.print("IP_my:");
//       lcd.setCursor(0, 1);
//       for (byte i = 0; i < 4; ++i) {
//        lcd.print(arputil.myip[i], HEX);
//        if(i!=3)lcd.print('.');       
//       }
//       flag_1_dc = 2;
//   }
//   if(digitalRead(8)==LOW&&flag_1==1){flag_1=0;}
//   if(digitalRead(8)==LOW&&flag_1_dc==2){flag_1_dc=0;}
   //КНОПКА 2
//   if(digitalRead(9)==HIGH&&flag_2==0&&flag_1_dc==0){
//       flag_2 = 1;
//       lcd.clear(); 
//       lcd.setCursor(0, 0); lcd.print("MAC_neighbour:");
//       lcd.setCursor(0, 1);
//       for (byte i = 1; i <= 6; ++i) {
//        lcd.print(arputil.packet.data.arp.dstMac[i], HEX);
//        if ((i%2) == 0)lcd.print(':');       
//       }
//       flag_2_dc = 1;
//   }
//   if(digitalRead(9)==HIGH&&flag_2_dc==1&&flag_2==0){
//       flag_2_dc = 2;
//       lcd.clear(); 
//       lcd.setCursor(0, 0); 
//       lcd.print("IP_neighbour:");
//       lcd.setCursor(0, 1);
//       for (byte i = 0; i < 4; ++i) {
//        lcd.print(arputil.packet.data.arp.dstIp[i], HEX);
//        if(i!=3)lcd.print('.');       
//       }
//       flag_2_dc = 2;
//   } 
//   if(digitalRead(9)==LOW&&flag_2==1){flag_2=0;}
//   if(digitalRead(9)==LOW&&flag_2_dc==2){flag_2_dc=0;}
}
//

//void eth_read(enc28j60_frame_ptr *frame, uint16_t len)
//{
//  const char str1[60] ={0};
//  if(len >= sizeof(enc28j60_frame_ptr))
//  {
//    sprintf(str1,"%02X:%02X:%02X:%02X:%02X:%02X-%02X:%02X:%02X:%02X:%02X:%02X; %d; %04X",
//    frame->addr_src[0],frame->addr_src[1],frame->addr_src[2],frame->addr_src[3],frame->addr_src[4],frame->addr_src[5],
//    frame->addr_dest[0],frame->addr_dest[1],frame->addr_dest[2],frame->addr_dest[3],frame->addr_dest[4],frame->addr_dest[5],len,be16toword(frame->type));
//    Serial.print(str1);
//    if (frame->type==arputil.ETH_ARP)
//    {
//      Serial.print(F("; arp"));
//      arp_read(frame, len - sizeof(enc28j60_frame_ptr));
//    }
//    if (frame->type==arputil.ARP_IP) Serial.print(F("; ip"));
//    Serial.println();
//  }
//}
//
//void arp_read(enc28j60_frame_ptr *frame, uint16_t len) //парсинг arp-запросов с внешних устройств
//{
//  const char str1[60] ={0};
//  //digitalWrite(7, HIGH);
//  //byte res = 0;
//  arp_msg_ptr *msg = (void*)(frame->data);
//  if(len >= sizeof(arp_msg_ptr))
//  {
//    if( (msg->net_tp == arputil.ARP_ETH) && (msg->proto_tp == arputil.ARP_IP) )
//    {
//      if( (msg->op == arputil.ARP_REQUEST) && (msg->ipaddr_dst == IP_ADDR) )
//      {
//        Serial.print(sprintf(str1,"request\nmac_src %02X:%02X:%02X:%02X:%02X:%02X\n",
//        msg->macaddr_src[0], msg->macaddr_src[1], msg->macaddr_src[2], msg->macaddr_src[3], msg->macaddr_src[4], msg->macaddr_src[5]));
//        Serial.print(str1);
//        sprintf(str1,"ip_src %ld.%ld.%ld.%ld\n",
//        msg->ipaddr_src & 0x000000FF, (msg->ipaddr_src>>8) & 0x000000FF, (msg->ipaddr_src>>16) & 0x000000FF, msg->ipaddr_src>>24);
//        Serial.print(str1);
//        sprintf(str1,"mac_dst %02X:%02X:%02X:%02X:%02X:%02X\n",
//        msg->macaddr_dst[0], msg->macaddr_dst[1], msg->macaddr_dst[2], msg->macaddr_dst[3], msg->macaddr_dst[4], msg->macaddr_dst[5]);
//        Serial.print(str1);
//        sprintf(str1,"ip_dst %ld.%ld.%ld.%ld\n",
//        msg->ipaddr_dst & 0x000000FF, (msg->ipaddr_dst>>8) & 0x000000FF, (msg->ipaddr_dst>>16) & 0x000000FF, msg->ipaddr_dst>>24);
//        Serial.print(str1);
//        //res=1;
//    //arp_table_fill(frame);
//        arputil.set_OpCode(arputil.ARP_REPLY);
//        sprintf(str1,"%ld.%ld.%ld.%ld",
//        msg->ipaddr_src & 0x000000FF, (msg->ipaddr_src>>8) & 0x000000FF, (msg->ipaddr_src>>16) & 0x000000FF, msg->ipaddr_src>>24);
//        arputil.set_DstIp(str1);
//        sprintf(str1,"%02X:%02X:%02X:%02X:%02X:%02X",
//        msg->macaddr_src[0], msg->macaddr_src[1], msg->macaddr_src[2], msg->macaddr_src[3], msg->macaddr_src[4], msg->macaddr_src[5]);
//        arputil.set_DstMac(str1);
//        arputil.send();
//      }
//    }
//  }
//  //digitalWrite(7, LOW);
// // return res;
//}
