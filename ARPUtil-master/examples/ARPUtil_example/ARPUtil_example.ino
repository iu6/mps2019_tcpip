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
 * ARPUtil_example.ino
 *
 *  Created on: 16 Sep 2017
 *      Author: n1k0ism
 */

#include <ArpUtil.h>

void setup() {
	// set your own mac here this will also be
	// the default source mac that you can change
	// later.
	arputil.begin("0c:ee:07:30:65:ef");

	// open the Ethernet port...
	arputil.connect();

	// ...and wait for dhcp assignment.
	while (!arputil.isConnected()) {
		delay(1000);
	}

	// reply instead of a request?
	//arputil.set_OpCode(arputil.ARP_REPLY); // default: ARP_REQUEST

	// set the source ip to the gateway ip.
	arputil.set_SrcIp(arputil.gwIp);
	//arputil.set_SrcMac("aa:bb:cc:11:22:33");

	// specific destination?
	//arputil.set_DstIp("192.168.0.123"); // default: 255.255.255.255
	//arputil.set_DstMac("a1:b2:c3:d4:e5:f6"); // default: ff:ff:ff:ff:ff:ff
}

void loop() {

	delay(2000);

	// send your ARP data!
	arputil.send();
}
