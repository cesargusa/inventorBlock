#ifndef _ABLOCKSIOTMQTTCLIENT_h
#define _ABLOCKSIOTMQTTCLIENT_h

#include "MQTTClient.h"
#include "ABlocksIOTDefines.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(WIN32_DLL) || defined(WIN64_DLL)
  #define DLLImport __declspec(dllimport)
  #define DLLExport __declspec(dllexport)
#elif defined(LINUX_SO)
  #define DLLImport extern
  #define DLLExport  __attribute__ ((visibility ("default")))
#else
  #define DLLImport
  #define DLLExport
#endif

	typedef struct ABlocksIOTMQTTClient
	{
		MQTTClient mqttClient; 
		const char* username;
		const char* password; 
		const char* clientID; 
		unsigned char sendbuf[ABLOCKSIOT_MAX_MESSAGE_SIZE + 1]; /**< Buffer used for sending data. */
		unsigned char readbuf[ABLOCKSIOT_MAX_MESSAGE_SIZE + 1]; /**< Buffer used for receiving data. */
    
	} ABlocksIOTMQTTClient;

	DLLExport void ABlocksIOTMQTTClientInit(ABlocksIOTMQTTClient* client, Network* network, const char* username, const char* password, const char* clientID, messageHandler _mh); 

	DLLExport int ABlocksIOTMQTTConnect(ABlocksIOTMQTTClient* client);

	DLLExport int ABlocksIOTMQTTDisconnect(ABlocksIOTMQTTClient* client);
 
	DLLExport int ABlocksIOTMQTTConnected(ABlocksIOTMQTTClient* client);

	DLLExport int ABlocksIOTMQTTYield(ABlocksIOTMQTTClient* client, int time);

#if defined(__cplusplus)
   }
#endif

#endif
