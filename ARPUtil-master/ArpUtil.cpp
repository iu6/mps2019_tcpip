/*
 * Copyright 2017 Nikolai M. Langer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * ArpUtil.cpp
 *
 *  Created on: 16 Sep 2017
 *      Author: n1k0ism
 */
#include "ArpUtil.h"

#include <Arduino.h>
#include <enc28j60.h>
#include <EtherCard.h>
#include <net.h>
#include "ArpPacket.h"
#include <avr/pgmspace.h>
#define ETH_arp be16toword(0x0806)
#define ETH_ip be16toword(0x0800)
#define ARP_eth be16toword(0x0001)
#define ARP_ip be16toword(0x0800)
#define ARP_REQ be16toword(1)
//#define ARP_REPLY be16toword(2)
#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
#define ip_join(a,b,c,d) (((uint32_t)a)|((uint32_t)b<<8)|((uint32_t)c<<16)|((uint32_t)d<<24))
#define IP_ADDR ip_join(192,168,1,100)
#define ENC28J60_MAXFRAME 512

ArpUtil arputil;

uint8_t ArpUtil::ETH_ARP[2]  = { 0x08, 0x06 };
uint8_t ArpUtil::ARP_ETH[2]  = { 0x00, 0x01 };
uint8_t ArpUtil::ARP_IP[2]  = { 0x08, 0x00 };
uint8_t ArpUtil::ETH_IP[2]  = { 0x08, 0x00 };
uint8_t ArpUtil::ARP_REQUEST[2] = { 0x00, 0x01 };
uint8_t ArpUtil::ARP_REPLY[2] = { 0x00, 0x02 };

uint8_t Ethernet::buffer[512];
const uint8_t ArpUtil::myip[4] = {192,168,1,100};
// ethernet interface ip netmask
const uint8_t ArpUtil::mask[4] = {255,255,255,0};
// gateway ip address
const uint8_t ArpUtil::gwip[4] = {192,168,1,10};
const uint8_t ArpUtil::mymac[6] = {0x74,0x69,0x60,0x2D,0x30,0x34};
uint8_t net_buf[ENC28J60_MAXFRAME];
const char str1[60] = {0};
uint8_t current_arp_index=0;
//static long timer;
//ArpUtil::arp_record_ptr arp_rec[5];
//char myChar;

void ArpUtil::initPacket() {
	// these initial values can be changed any time by using
	// the provided functions
	//set_DstMac("ff:ff:ff:ff:ff:ff");
	set_SrcMac("74:69:60:2D:30:34");
	set_SrcIp("192.168.1.100");
	set_OpCode(ARP_REPLY);

	// hardware and protocol settings (do not change)
	memcpy(packet.data.l2.type, ETH_ARP, 2);
	memcpy(packet.data.arp.typeHw, ARP_ETH, 2);
	memcpy(packet.data.arp.typeProto, ARP_IP, 2);
	packet.data.arp.sizeHw = SIZE_ETHER;
	packet.data.arp.sizeProto = SIZE_IPV4;
}

bool ArpUtil::parseuint8_ts(const char* str, char delim, uint8_t* uint8_ts) {
	// defaults for mac address
	int base = 16;
	int siz = SIZE_ETHER;

	// adapt if it is an ip address
	if (delim == '.') {
		base = 10;
		siz = SIZE_IPV4;
	}
	// iterate through delimited string
	for (int i = 0; i < siz; i++) {
		// convert to uint8_t
		uint8_ts[i] = strtoul(str, NULL, base);
		// get next
		str = strchr(str, delim);
		// finished?
		if (str == NULL || *str == '\0') {
			return ((i + 1) == siz);
		}
		str++;
	}
	return false;
}
void ArpUtil::setMac(const char* mac, uint8_t* field) {
	this->parseuint8_ts(mac, ':', buffer);
	setMac(buffer, field);
}

void ArpUtil::setIp(const char* ip, uint8_t* field) {
	this->parseuint8_ts(ip, '.', buffer);
	setIp(buffer, field);
}

void ArpUtil::setMac(const uint8_t* mac, uint8_t* field) {
	memcpy(field, mac, SIZE_ETHER);
}

void ArpUtil::setIp(const uint8_t* ip, uint8_t* field) {
	memcpy(field, ip, SIZE_IPV4);
}

void ArpUtil::begin(const char* devMac) {
	memcpy(deviceMac, devMac, 18);
#ifdef DEBUG
	Serial.begin(115200);
	Serial.println(deviceMac);
#endif
	pinMode(LED, OUTPUT);
	initPacket();
}

bool ArpUtil::isConnected() {
	return connected;
}

void ArpUtil::send() {
	digitalWrite(LED, HIGH);
	for (unsigned int i = 0; i < sizeof(packet); i++) {
		ether.buffer[i] = this->packet.raw[i];
		ether.packetSend(sizeof(packet));
	}
	Serial.println();
	digitalWrite(LED, LOW);
}

bool ArpUtil::connect() {
#ifdef DEBUG
	Serial.print(F("Opening Ethernet port...\n"));
#endif

	 if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0) {
		Serial.print(F( "Failed to access Ethernet controller\n"));
		connected = false;
		return connected;
	 } else {
	     ether.staticSetup(myip, gwip, NULL, mask);
			Serial.print(F("success!\n"));
	} 
	
		//timer = millis();
		ArpUtil::myIp = ether.myip;
		ArpUtil::gwIp = ether.gwip;
		ArpUtil::netmask = ether.netmask;
		//ArpUtil::dnsIp = ether.dnsip;
		connected = true;
#ifdef DEBUG
		ether.printIp("my_ip    ", myIp);
		ether.printIp("net_mask ", netmask);
		ether.printIp("gateway ", gwIp);
		//ether.printIp("dns     ", dnsIp);
#endif
	
	return connected;
}

void ArpUtil::set_L2_DstMac(const char* mac) {
	setMac(mac, packet.data.l2.dstMac);
}

void ArpUtil::set_L2_SrcMac(const char* mac) {
	setMac(mac, packet.data.l2.srcMac);
}

void ArpUtil::set_ARP_DstMac(const char* mac) {
	setMac(mac, packet.data.arp.dstMac);
}

void ArpUtil::set_ARP_SrcMac(const char* mac) {
	setMac(mac, packet.data.arp.srcMac);
}

void ArpUtil::set_DstMac(const char* mac) {
	set_L2_DstMac(mac);
	set_ARP_DstMac(mac);
}

void ArpUtil::set_SrcMac(const char* mac) {
	set_L2_SrcMac(mac);
	set_ARP_SrcMac(mac);
}

void ArpUtil::set_OpCode(const uint8_t* opcode) {
	memcpy(packet.data.arp.opcode, opcode, 2);
}

void ArpUtil::set_SrcIp(const char* ip) {
	setIp(ip, packet.data.arp.srcIp);
}

void ArpUtil::set_SrcIp(const uint8_t* ip) {
	setIp(ip, packet.data.arp.srcIp);
}

void ArpUtil::set_DstIp(const char* ip) {
	setIp(ip, packet.data.arp.dstIp);
}
void ArpUtil::set_DstIp(const uint8_t* ip) {
	setIp(ip, packet.data.arp.dstIp);
}

void ArpUtil::net_pool(void)
{
  uint16_t len;
  enc28j60_frame_ptr *frame = (void*)net_buf;

  while((len = ether.enc28j60_packetReceive(net_buf, sizeof(net_buf))))
  {
    eth_read(frame, len);
  }
}

void ArpUtil::eth_read(enc28j60_frame_ptr *frame, uint16_t len)
{
     // Serial.println("Read eth");
  if(len >= sizeof(enc28j60_frame_ptr))
  {
    sprintf(str1,"%02X:%02X:%02X:%02X:%02X:%02X-%02X:%02X:%02X:%02X:%02X:%02X; %d; %04X",
    frame->addr_src[0],frame->addr_src[1],frame->addr_src[2],frame->addr_src[3],frame->addr_src[4],frame->addr_src[5],
    frame->addr_dest[0],frame->addr_dest[1],frame->addr_dest[2],frame->addr_dest[3],frame->addr_dest[4],frame->addr_dest[5],len,be16toword(frame->type));
    Serial.print(str1);
    if (frame->type==ETH_arp)
    {
      Serial.print(F("; arp\n"));
      arp_read(frame, len - sizeof(enc28j60_frame_ptr));
    }
    if (frame->type==ETH_ip) Serial.print(F("; ip"));
    Serial.println();
  }
}

byte ArpUtil::arp_read(enc28j60_frame_ptr *frame, uint16_t len) //парсинг arp-запросов с внешних устройств
{
	//Serial.println("Read arp");
  byte res = 0;
  arp_msg_ptr *msg = (void*)(frame->data);
  if(len >= sizeof(arp_msg_ptr))
  {
    if( (msg->net_tp == ARP_eth) && (msg->proto_tp == ARP_ip) )
    {
      if( (msg->op == ARP_REQ) && (msg->ipaddr_dst == IP_ADDR) )
      {
        sprintf(str1,"request\nmac_src %02X:%02X:%02X:%02X:%02X:%02X\n",
        msg->macaddr_src[0], msg->macaddr_src[1], msg->macaddr_src[2], msg->macaddr_src[3], msg->macaddr_src[4], msg->macaddr_src[5]);
        Serial.print(str1);
        sprintf(str1,"ip_src %ld.%ld.%ld.%ld\n",
        msg->ipaddr_src & 0x000000FF, (msg->ipaddr_src>>8) & 0x000000FF, (msg->ipaddr_src>>16) & 0x000000FF, msg->ipaddr_src>>24);
        Serial.print(str1);
        sprintf(str1,"mac_dst %02X:%02X:%02X:%02X:%02X:%02X\n",
        msg->macaddr_dst[0], msg->macaddr_dst[1], msg->macaddr_dst[2], msg->macaddr_dst[3], msg->macaddr_dst[4], msg->macaddr_dst[5]);
        Serial.print(str1);
        sprintf(str1,"ip_dst %ld.%ld.%ld.%ld\n",
        msg->ipaddr_dst & 0x000000FF, (msg->ipaddr_dst>>8) & 0x000000FF, (msg->ipaddr_dst>>16) & 0x000000FF, msg->ipaddr_dst>>24);
        Serial.print(str1);
		//arp_table_fill(frame);
        //arputil.set_OpCode(ARP_REPLY);
        sprintf(str1,"%ld.%ld.%ld.%ld",
        msg->ipaddr_src & 0x000000FF, (msg->ipaddr_src>>8) & 0x000000FF, (msg->ipaddr_src>>16) & 0x000000FF, msg->ipaddr_src>>24);
        arputil.set_DstIp(str1);
        sprintf(str1,"%02X:%02X:%02X:%02X:%02X:%02X",
        msg->macaddr_src[0], msg->macaddr_src[1], msg->macaddr_src[2], msg->macaddr_src[3], msg->macaddr_src[4], msg->macaddr_src[5]);
        arputil.set_DstMac(str1);
		arputil.set_ARP_SrcMac("74:69:60:2D:30:34");
        arputil.send();
		 res=1;
      }
    }
  }
  return res;
}

/*void ArpUtil::arp_table_fill(enc28j60_frame_ptr *frame)
{
	uint8_t i;
	arp_msg_ptr *msg = (void*)frame->data;
	//добавить запись
	arp_rec[current_arp_index].ipaddr=msg->ipaddr_src;
	memcpy(arp_rec[current_arp_index].mac_addr,msg->macaddr_src,6);
	arp_rec[current_arp_index].sec=timer;
	if(current_arp_index<4) current_arp_index++;
	else current_arp_index=0;
	//смотрим ARP-таблицу
	for (i=0;i<5;i++)
	{
		sprintf(str1,"%ld.%ld.%ld.%ld - %02X:%02X:%02X:%02X:%02X:%02X - %lu\r\n",
		arp_rec[i].ipaddr & 0x000000FF,(arp_rec[i].ipaddr>>8) & 0x000000FF,
		(arp_rec[i].ipaddr>>16) & 0x000000FF, arp_rec[i].ipaddr>>24,
		arp_rec[i].mac_addr[0],arp_rec[i].mac_addr[1],arp_rec[i].mac_addr[2],
		arp_rec[i].mac_addr[3],arp_rec[i].mac_addr[4],arp_rec[i].mac_addr[5],
		arp_rec[i].sec);
		Serial.print(str1);
	}
}*/
