#include "ABlocksIOTMQTTClient.h"
#include <string.h>

void ABlocksIOTMQTTClientInit(ABlocksIOTMQTTClient* client, Network* network, const char* username, const char* password, const char* clientID, messageHandler defaultHandler)
{
	int i;
	MQTTClientInit(&client->mqttClient, network, 30000, client->sendbuf, ABLOCKSIOT_MAX_MESSAGE_SIZE, client->readbuf, ABLOCKSIOT_MAX_MESSAGE_SIZE, defaultHandler );

 	client->mqttClient.defaultMessageHandler = defaultHandler; //MQTTMessageArrived;
	client->username = username;
	client->password = password;
	client->clientID = clientID;
}

int ABlocksIOTMQTTConnect(ABlocksIOTMQTTClient* client)
{
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	data.MQTTVersion = 3;
	data.clientID.cstring = (char*)client->clientID;
	data.username.cstring = (char*)client->username;
	data.password.cstring = (char*)client->password;
	return MQTTConnect(&client->mqttClient, &data);
}

int ABlocksIOTMQTTDisconnect(ABlocksIOTMQTTClient* client)
{
	return MQTTDisconnect(&client->mqttClient);
}

int ABlocksIOTMQTTConnected(ABlocksIOTMQTTClient* client)
{
	return client->mqttClient.isconnected;
}

int ABlocksIOTMQTTYield(ABlocksIOTMQTTClient* client, int time)
{
	return MQTTYield(&client->mqttClient, time);
}
