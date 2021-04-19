#ifndef _ABLOCKSIOTMQTTESP8266SHIELD_h
#define _ABLOCKSIOTMQTTESP8266SHIELD_h

//#define INFO_CONNECTION "ESP8266Shield"

#include "Client.h"
#include "ESP8266SerialLibrary.h"
#include "ABlocksIOTArduinoDefines.h"
#include "ABlocksIOTDefines.h"
#include "ABlocksIOTArduinoMQTTClient.h"

#ifdef ESP8266
#error This code is not intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#ifndef ABLOCKSIOT_ESP8266_MUX
#define ABLOCKSIOT_ESP8266_MUX  1
#endif

#ifndef ESP8266_CLIENT_BUFFER_SIZE
#define ESP8266_CLIENT_BUFFER_SIZE ABLOCKSIOT_MAX_MESSAGE_SIZE
#endif

#define WRITE_CHUNK_SIZE 40 // The chunk size to use when sending data to prevent sending too much data at once.

class ESP8266Client : public Client
{
public:
	ESP8266Client() : _esp8266(NULL), _connected(false), _domain(NULL), _port(0), _head(0), _tail(0) {}

	void initialize(ESP8266* esp8266) {
		_esp8266 = esp8266;
		_esp8266->setOnData(onData, this);
	}

	bool connectWiFi(const char* ssid, const char* pass)
	{
		::delay(500);
		if (!_esp8266->kick()) {
			ABLOCKSIOT_LOG("ESP is not responding");
			return false;
		}
		if (!_esp8266->setEcho(0)) {
			ABLOCKSIOT_LOG("Failed to disable Echo");
			return false;
		}
		if (!_esp8266->enableMUX()) {
			ABLOCKSIOT_LOG("Failed to enable MUX");
		}
		if (!_esp8266->setOprToStation()) {
			ABLOCKSIOT_LOG("Failed to set STA mode");
			return false;
		}
		if (!_esp8266->joinAP(ssid, pass)) {
			ABLOCKSIOT_LOG("Failed to connect WiFi: %s", ssid);
			return false;
		}
		ABLOCKSIOT_LOG("Connected to WiFi");
		return true;
	}

	int connect(IPAddress ip, uint16_t port) {
		// Not currently implemented
		_connected = false;
		return _connected;
	}

	int connect(const char *host, uint16_t port) {
		_connected = _esp8266->createTCP(ABLOCKSIOT_ESP8266_MUX, host, port);
		return _connected;
	}

	size_t write(uint8_t) {}

	size_t write(const uint8_t *buf, size_t size) {
		if (_esp8266->send(ABLOCKSIOT_ESP8266_MUX, buf, size)) {
			return size;
		}
		return 0;
	}

	int available() {
		_esp8266->run();
		return ((unsigned int)(ESP8266_CLIENT_BUFFER_SIZE + _head - _tail)) % ESP8266_CLIENT_BUFFER_SIZE;
	}

	int read() {
		// if the head isn't ahead of the tail, we don't have any characters
		if (_head == _tail) {
			return -1;
		}
		else {
			unsigned char c = _buffer[_tail];
			_tail = (uint8_t)(_tail + 1) % ESP8266_CLIENT_BUFFER_SIZE;
			return c;
		}
	}

	int read(uint8_t *buf, size_t size) {
		return readBytes(buf, size);
	}

	int peek() {}

	void flush() {}

	void stop() {
		_connected = false;
		_head = _tail = 0;
		_esp8266->releaseTCP(ABLOCKSIOT_ESP8266_MUX);
	}

	uint8_t connected() { return _connected; }

	operator bool() {}

private:
	static void onData(uint8_t mux_id, uint32_t len, void* ptr) {
		((ESP8266Client*)ptr)->onData(mux_id, len);
	}

	void onData(uint8_t mux_id, uint32_t len) {
		if (mux_id != ABLOCKSIOT_ESP8266_MUX) {
			return;
		}
		Stream* puart = _esp8266->getUart();
		unsigned long start = millis();
		while (len) {
			if (millis() - start > 5000) {
				len = 0;
				_head = _tail = 0;
				ABLOCKSIOT_LOG_DEBUG("Timeout reading data");
			}
			if (puart->available()) {
				uint8_t c = puart->read();
				uint8_t i = (unsigned int)(_head + 1) % ESP8266_CLIENT_BUFFER_SIZE;

				if (i != _tail) {
					_buffer[_head] = c;
					_head = i;
				}
				else {
					ABLOCKSIOT_LOG_DEBUG("Buffer overflow");
				}
				len--;
			}
		}
	}

	ESP8266* _esp8266;
	bool _connected;
	const char* _domain;
	uint16_t _port;
	unsigned char _buffer[ESP8266_CLIENT_BUFFER_SIZE];
	uint8_t _head;
	uint8_t _tail;
};


class ABlocksIOTMQTTESP8266Client : public ABlocksIOTArduinoMQTTClient
{
public:

	void begin(const char* mqttserver, int mqttport, const char* username, const char* password, const char* clientID, ESP8266& esp8266, const char* ssid, const char* pass, messageHandler _mh, subscribeHandler _sH)
	{
		_client.initialize(&esp8266);
		while (!_client.connectWiFi(ssid, pass)) {
			delay(1000);
		}
		ABlocksIOTArduinoMQTTClient::begin(_client, mqttserver,mqttport, username, password, clientID, WRITE_CHUNK_SIZE, _mh, _sH);
	}

	void loop(int yieldTime = 1000) {
		unsigned long start = millis();
		while (millis() - start < yieldTime) {
			// Loop repeatedly to ensure ESP serial buffer is read quickly enough to prevent buffer overflows.
			ABlocksIOTArduinoMQTTClient::loop(0);
		}
	}

private:
	ESP8266Client _client;
};

ABlocksIOTMQTTESP8266Client ABlocksIOT;

#endif
