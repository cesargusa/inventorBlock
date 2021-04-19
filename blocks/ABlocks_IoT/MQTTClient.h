
#if !defined(__MQTT_CLIENT_C_)
#define __MQTT_CLIENT_C_

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

#include "MQTTPacket.h"
#include "stdio.h"
#include "PlatformHeader.h"

#if defined(MQTTCLIENT_PLATFORM_HEADER)
/* The following sequence of macros converts the MQTTCLIENT_PLATFORM_HEADER value
 * into a string constant suitable for use with include.
 */
#define xstr(s) str(s)
#define str(s) #s
#include xstr(MQTTCLIENT_PLATFORM_HEADER)
#endif

#define MAX_PACKET_ID 65535 /* according to the MQTT specification - do not change! */

enum QoS { QOS0, QOS1, QOS2 };

/* all failure return codes must be negative */
enum returnCode { MQTT_BUFFER_OVERFLOW = -2, MQTT_FAILURE = -1, MQTT_SUCCESS = 0 };

/* The Timer structure must be defined in the platform specific header,
 * and have the following functions to operate on it.  */
extern void TimerInit(Timer*);
extern char TimerIsExpired(Timer*);
extern void TimerCountdownMS(Timer*, unsigned int);
extern void TimerCountdown(Timer*, unsigned int);
extern int TimerLeftMS(Timer*);

typedef struct MQTTMessage
{
    enum QoS qos;
    unsigned char retained;
    unsigned char dup;
    unsigned short id;
    void *payload;
    size_t payloadlen;
} MQTTMessage;

typedef struct MessageData
{
    MQTTMessage* message;
    MQTTString* topicName;
} MessageData;

//buffer for topic
char mqtt_topic[128];

//topic,payload,length
typedef void (*messageHandler)(char*, unsigned char*, unsigned int); 
    
typedef struct MQTTClient
{
    unsigned int next_packetid,
    command_timeout_ms;
    size_t buf_size,
    readbuf_size;
    unsigned char *buf, *readbuf;
    unsigned int keepAliveInterval;
    char ping_outstanding;
    int isconnected;
	  int connAckReceived;
	  int subAckReceived;
  	int unsubAckReceived;
	  int pubAckReceived;
	  int pubCompReceived;

    void (*defaultMessageHandler) (char*, unsigned char*, unsigned int); 

    Network* ipstack;
    Timer ping_timer;
	  Timer last_received_timer;
	  Timer ping_response_timer;
    
#if defined(MQTT_TASK)
	  Mutex mutex;
	  Thread thread;
#endif
 
} MQTTClient;

#define DefaultClient {0, 0, 0, 0, NULL, NULL, 0, 0, 0}

DLLExport void MQTTClientInit(MQTTClient* client, Network* network, unsigned int command_timeout_ms,
		unsigned char* sendbuf, size_t sendbuf_size, unsigned char* readbuf, size_t readbuf_size, messageHandler defaultHandler);

DLLExport int MQTTConnect(MQTTClient* client, MQTTPacket_connectData* options);

DLLExport int MQTTPublish(MQTTClient* client, const char*, MQTTMessage*);

DLLExport int MQTTSubscribe(MQTTClient* client, const char* topicFilter, enum QoS, messageHandler);

DLLExport int MQTTUnsubscribe(MQTTClient* client, const char* topicFilter);

DLLExport int MQTTDisconnect(MQTTClient* client);

DLLExport int MQTTYield(MQTTClient* client, int time);

#if defined(MQTT_TASK)
DLLExport int MQTTStartTask(MQTTClient* client);
#endif

#if defined(__cplusplus)
     }
#endif

#endif
