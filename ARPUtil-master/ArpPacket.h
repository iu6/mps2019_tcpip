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
 * ArpPacket.h
 *
 *  Created on: 16 Sep 2017
 *      Author: n1k0ism
 */

#ifndef ARPPACKET_H_
#define ARPPACKET_H_
#include <Arduino.h>

#define SIZE_ETHER 0x06
#define SIZE_IPV4 0x04

// the arp packet structure
typedef union {
	// BEGIN structured data view
	struct {
        	// BEGIN L2 header
        	struct {
            		uint8_t dstMac[SIZE_ETHER];	// source mac address
            		uint8_t srcMac[SIZE_ETHER];	// destination mac address
            		uint8_t type[2];		// eg. arp: 0x08 0x06
        	} l2;
        	// END L2 header
        	// BEGIN ARP payload
        	struct {
			uint8_t typeHw[2];		// hardware type e.g. ethernet: 0x0001
        		uint8_t typeProto[2];  		// protocol type e.g. ipv4: 0x0800
            		uint8_t sizeHw;     		// hardware address size e.g. eth: 6 bytes
            		uint8_t sizeProto;  		// protocol address size e.g. ipv4: 4 bytes
            		uint8_t opcode[2];  		// arp op code e.g. request: 0x0001,  reply: 0x0002
            		uint8_t srcMac[SIZE_ETHER];	// source mac address
            		uint8_t srcIp[SIZE_IPV4];	// source ip address
            		uint8_t dstMac[SIZE_ETHER];	// destination mac address
            		uint8_t dstIp[SIZE_IPV4];	// destination ip address
        	} arp;
        	// END ARP payload
    	} data;
    	// END structured data view
    	// BEGIN linear data view
    	uint8_t raw[42]; // raw data (42 bytes)
    	// END linear data view
} ArpPacket;


#endif /* ARPPACKET_H_ */
