#ifndef _ABLOCKSIOTARDUINOMQTTCLIENT_h
#define _ABLOCKSIOTARDUINOMQTTCLIENT_h

#include "ABlocksIOTMQTTClient.h"

typedef void (*subscribeHandler)(void); 

const int MAX_CHANNEL_ARRAY_SIZE = 4;

class ABlocksIOTArduinoMQTTClient
{
public:

	void begin(Client& _client, const char* _mqttserver, int _mqttport, const char* _username, const char* _password, const char* _clientID, int _chunkSize, messageHandler _mh, subscribeHandler _sH) 
	{
    m_connected=false;
    mqtt_subscriber=_sH;
    
    //strcpy(mqtt_server,_mqttserver);
    mqtt_server=_mqttserver;
    mqtt_port=_mqttport;
    
		NetworkInit(&_network, &_client, _chunkSize);
		ABlocksIOTMQTTClientInit(&_mqttClient, &_network, _username, _password, _clientID, _mh);
		connect(_mqttserver,_mqttport);
	}

	void connect(const char* mqttserver,int mqttport) {

		ABLOCKSIOT_LOG("Connecting to %s:%d", mqttserver, mqttport);
		int error = MQTT_FAILURE;
		do {
			if (!NetworkConnect(&_network, mqttserver, mqttport)) {
				ABLOCKSIOT_LOG("Network connect failed");
				delay(1000);
			}
			else if ((error = ABlocksIOTMQTTConnect(&_mqttClient)) != MQTT_SUCCESS) {
				ABLOCKSIOT_LOG("MQTT connect failed, error %d", error);
				NetworkDisconnect(&_network);
				delay(1000);
			}
		}
		while (error != MQTT_SUCCESS);

    if(mqtt_subscriber!=NULL)mqtt_subscriber();

    m_connected=true;

		ABLOCKSIOT_LOG("Connected");
	}
 
	void loop(int yieldTime = 1000) {

		ABlocksIOTMQTTYield(&_mqttClient, yieldTime);
  
		if (!NetworkConnected(&_network) || !ABlocksIOTMQTTConnected(&_mqttClient))
		{
      m_connected=false;

			ABlocksIOTMQTTDisconnect(&_mqttClient);
			NetworkDisconnect(&_network);
			ABLOCKSIOT_LOG("Disconnected");

			connect(mqtt_server,mqtt_port);
		}
	}

  bool isConnected(){
    return m_connected;  
  }
  
  void Publish(String _topic, String _value){

      MQTTMessage message;
      message.qos = QOS0;
      message.retained = 0; //1 (0->Thingspeak working!)
      message.dup = 0;
      message.payload = (void*)_value.c_str(); 
      message.payloadlen = strlen(_value.c_str());
      
      MQTTPublish(&_mqttClient.mqttClient, _topic.c_str(), &message);       
  }

 void Subscribe(String _topic){

      MQTTSubscribe(&_mqttClient.mqttClient, _topic.c_str(), QOS0, NULL); 
 }

  subscribeHandler mqtt_subscriber;

	static ABlocksIOTMQTTClient _mqttClient;
	Network _network;

  const char* mqtt_server;
  int mqtt_port;  
  bool m_connected;
};

ABlocksIOTMQTTClient ABlocksIOTArduinoMQTTClient::_mqttClient;

#endif
