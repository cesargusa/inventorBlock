#ifndef _ABLOCKSIOTMQTTETHERNETCLIENT_h
#define _ABLOCKSIOTMQTTETHERNETCLIENT_h

#include "ABlocksIOTArduinoDefines.h"
#include "ABlocksIOTArduinoMQTTClient.h"

static const uint32_t HASH_PRIME = 0x01000193; //   16777619 - Default prime value recommended for fnv hash
static const uint32_t HASH_SEED = 0x811C9DC5;  // 2166136261 - Default seed value recommended for fnv hash

class ABlocksIOTMQTTEthernetClient : public ABlocksIOTArduinoMQTTClient
{
public:
	/**
	* Begins ABlocksIOT session
	* @param username ABlocksIOT username
	* @param password ABlocksIOT password
	* @param clientID Cayennne client ID
	* @param mac Mac address for device
	*/
	//void begin(const char* username, const char* password, const char* clientID, const byte mac[] = NULL)
  void begin(const char* mqttserver, int mqttport, const char* username, const char* password, const char* clientID, byte mac[], messageHandler _mh, subscribeHandler _sH)
	{
		//byte* macAddress = (byte*)GetMACAddress(clientID, mac);
   
		while (!Ethernet.begin(mac)) {
			ABLOCKSIOT_LOG("DHCP failed, retrying");
			delay(100);
		}
		IPAddress local_ip = Ethernet.localIP();
		ABLOCKSIOT_LOG("IP: %d.%d.%d.%d", local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
		
		ABlocksIOTArduinoMQTTClient::begin(_ethernetClient, mqttserver,mqttport, username, password, clientID, 0, _mh,_sH);
	}

	/**
	* Begins ABlocksIOT session
	* @param username ABlocksIOT username
	* @param password ABlocksIOT password
	* @param clientID Cayennne client ID
	* @param local Static IP address of device
	* @param mac Mac address for device
	*/
  /*
	void begin(const char* username, const char* password, const char* clientID, IPAddress local, const byte mac[] = NULL)
	{
		Ethernet.begin((byte*)GetMACAddress(clientID, mac), local);
		IPAddress local_ip = Ethernet.localIP();
		ABLOCKSIOT_LOG("IP: %d.%d.%d.%d", local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
		ABlocksIOTArduinoMQTTClient::begin(_ethernetClient, username, password, clientID);
	}
  */

	/**
	* Begins ABlocksIOT session
	* @param username ABlocksIOT username
	* @param password ABlocksIOT password
	* @param clientID Cayennne client ID
	* @param local Static IP address of device
	* @param dns IP address of DNS server
	* @param gateway IP address of gateway
	* @param subnet Subnet mask
	* @param mac Mac address for device
	*/
  /*
	void begin(const char* username, const char* password, const char* clientID, IPAddress local, IPAddress dns, IPAddress gateway, IPAddress subnet, const byte mac[] = NULL)
	{
		Ethernet.begin((byte*)GetMACAddress(clientID, mac), local, dns, gateway, subnet);
		IPAddress local_ip = Ethernet.localIP();
		ABLOCKSIOT_LOG("IP: %d.%d.%d.%d", local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
		ABlocksIOTArduinoMQTTClient::begin(_ethernetClient, username, password, clientID);
	}
  */
private:
	/**
	* Get MAC address fror the device
	* @param token Authentication token from ABlocksIOT site
	* @param mac User defined mac address for device
	* @returns Mac address for device
	*/
	const byte* GetMACAddress(const char* token, const byte mac[])
	{
		if (mac != NULL)
			return mac;

		_mac[0] = 0xFE;
		//Generate MAC from token to prevent collisions on the same network.
		_mac[1] = token[0] ? token[0] : 0xED;
		*((uint32_t*)&_mac[2]) = fnv1a(token);
		ABLOCKSIOT_LOG("MAC: %X-%X-%X-%X-%X-%X", _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
		return _mac;
	}

	/**
	* Create hash value using FNV-1a algorithm.
	* @param text Null terminated string to generate hash from
	* @returns hash Seed value for hash
	*/
	uint32_t fnv1a(const char* text, uint32_t hash = HASH_SEED)
	{
		while (*text)
			hash = (*text++ ^ hash) * HASH_PRIME;
		return hash;
	}

	EthernetClient _ethernetClient;
	byte _mac[6];
};

ABlocksIOTMQTTEthernetClient ABlocksIOT;

#endif
