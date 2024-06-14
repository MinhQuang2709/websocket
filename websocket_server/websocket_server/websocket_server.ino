#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "ws_server";
const char* password = "1234567890";
WebSocketsServer websocket = WebSocketsServer(81);

void setup()
{
	Serial.begin(115200);
	WiFi.mode(WIFI_AP);
	bool check = WiFi.softAP(ssid, password);
	if (check)
	{
		Serial.print("access point started");
	}
	else
	{
		Serial.print("fail");
	}
	Serial.print("AP IP address: ");
	Serial.println(WiFi.softAPIP());
	websocket.begin();
	websocket.onEvent(webSocketEvent);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length)
{
	switch (type)
	{
	case WStype_DISCONNECTED:
	{
		Serial.printf("[%u] disconnected\n", num);
		break;
	}
	case WStype_CONNECTED:
	{
		IPAddress ip = websocket.remoteIP(num);
		Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
		break;
	}
	case WStype_TEXT:
		Serial.printf("[%u] Received text: %s\n", num, payload);
		break;
	}
}

void sendMessage(uint8_t num)
{
	while (Serial.available())
	{
		String input = Serial.readStringUntil('\n');
		websocket.sendTXT(num, "received message: " + input);
	}
}

void loop()
{
	websocket.loop();
	uint8_t num;
	IPAddress ip = websocket.remoteIP(num);
	sendMessage(num);
}
