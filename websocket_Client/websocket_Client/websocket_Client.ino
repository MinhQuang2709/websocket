#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient websocket;

const char* ssid = "ws_server";
const char* password = "1234567890";
const char* ip_host = "192.168.4.1";
const uint16_t port = 81;

void setup()
{
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	Serial.print("connetinf wifi");
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print('.');
		delay(500);
	}
	Serial.println();
	Serial.print("connected wifi: ");
	Serial.println(WiFi.localIP());

	websocket.begin(ip_host, port, "/");
	websocket.onEvent(webSocketEvent);
	websocket.setReconnectInterval(5000);
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length)
{
	switch (type)
	{
	case WStype_DISCONNECTED:
		Serial.println("Disconnected!");
		break;
	case WStype_CONNECTED:
		Serial.println("Connected to server");
		websocket.sendTXT("Hello from client");
		break;
	case WStype_TEXT:
		Serial.printf("Received text: %s\n", payload);
		break;
	}
}

void sendToServer()
{
	while (Serial.available())
	{
		String message = Serial.readStringUntil('\n');
		websocket.sendTXT("message from client: " + message);
	}
}
void loop()
{
	websocket.loop();
	sendToServer();
} 