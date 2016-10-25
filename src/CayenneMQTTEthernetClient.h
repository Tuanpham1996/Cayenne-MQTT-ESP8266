/*
The MIT License(MIT)

Cayenne Arduino Client Library
Copyright � 2016 myDevices

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files(the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This software uses open source arduino-mqtt library - see MQTTClient-LICENSE.md
*/

#ifndef _CAYENNEMQTTETHERNETCLIENT_h
#define _CAYENNEMQTTETHERNETCLIENT_h

#include "CayenneArduinoMQTTClient.h"

class CayenneMQTTEthernetClient : public CayenneArduinoMQTTClient
{
public:
	/**
	* Begins Cayenne session
	* @param clientID Cayennne client ID
	* @param username Cayenne username
	* @param password Cayenne password
	* @param mac Mac address for device
	*/
	void begin(char* clientID, char* username, char* password, const byte mac[] = NULL)
	{
		byte* macAddress = (byte*)GetMACAddress(clientID, mac);
		while (!Ethernet.begin(macAddress)) {
			CAYENNE_LOG("DHCP failed, retrying");
			delay(100);
		}
		IPAddress local_ip = Ethernet.localIP();
		CAYENNE_LOG("IP: %d.%d.%d.%d", local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
		CayenneArduinoMQTTClient::begin(_ethernetClient, clientID, username, password);
	}

	/**
	* Begins Cayenne session
	* @param clientID Cayennne client ID
	* @param username Cayenne username
	* @param password Cayenne password
	* @param local Static IP address of device
	* @param mac Mac address for device
	*/
	void begin(char* clientID, char* username, char* password, IPAddress local, const byte mac[] = NULL)
	{
		Ethernet.begin((byte*)GetMACAddress(clientID, mac), local);
		IPAddress local_ip = Ethernet.localIP();
		CAYENNE_LOG("IP: %d.%d.%d.%d", local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
		CayenneArduinoMQTTClient::begin(_ethernetClient, clientID, username, password);
	}

	/**
	* Begins Cayenne session
	* @param clientID Cayennne client ID
	* @param username Cayenne username
	* @param password Cayenne password
	* @param local Static IP address of device
	* @param dns IP address of DNS server
	* @param gateway IP address of gateway
	* @param subnet Subnet mask
	* @param mac Mac address for device
	*/
	void begin(char* clientID, char* username, char* password, IPAddress local, IPAddress dns, IPAddress gateway, IPAddress subnet, const byte mac[] = NULL)
	{
		Ethernet.begin((byte*)GetMACAddress(clientID, mac), local, dns, gateway, subnet);
		IPAddress local_ip = Ethernet.localIP();
		CAYENNE_LOG("IP: %d.%d.%d.%d", local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
		CayenneArduinoMQTTClient::begin(_ethernetClient, clientID, username, password);
	}

private:
	/**
	* Get MAC address fror the device
	* @param token Authentication token from Cayenne site
	* @param mac User defined mac address for device
	* @returns Mac address for device
	*/
	const byte* GetMACAddress(const char* token, const byte mac[])
	{
		if (mac != NULL)
			return mac;

		_mac[0] = 0xFE;
		if (strnlen(token, 10) != 10)
		{
			CAYENNE_LOG("Using default MAC");
			_mac[1] = 0xED;
			_mac[2] = 0xBA;
			_mac[3] = 0xFE;
			_mac[4] = 0xFE;
			_mac[5] = 0xED;
		}
		else
		{
			//Generate MAC from token to prevent collisions on the same network.
			_mac[1] = token[0] + token[1];
			_mac[2] = token[2] + token[3];
			_mac[3] = token[4] + token[5];
			_mac[4] = token[6] + token[7];
			_mac[5] = token[8] + token[9];
		}
		CAYENNE_LOG("MAC: %X-%X-%X-%X-%X-%X", _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
		return _mac;
	}

	EthernetClient _ethernetClient;
	byte _mac[6];
};

CayenneMQTTEthernetClient Cayenne;

#endif