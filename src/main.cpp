#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <SPI.h>
#include <WiFiUDP.h>
#include "settings.h"

MDNSResponder mdns;
WiFiUDP udp;
ESP8266WebServer server(80);

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

void sendWOL(const IPAddress ip, const byte mac[]);
void beginWifi();
void macStringToBytes(const String mac, byte *bytes);
void beginOTA();

void setup(void){
	Serial.begin(115200);
	beginWifi();

	mdns.begin("esp8266");
	udp.begin(9);

	server.on("/", []() {
		//Requests to the root respond with a "Hello World"
		digitalWrite(LED_BUILTIN, 1);
		server.send(200, "text/plain", "ESP8266 WOL online");
		digitalWrite(LED_BUILTIN, 0);
	});
	server.on("/wol", [](){
		//GET requests to /wol send Wake-On-LAN frames
		// the mac parameter is mandatory and should be a non-delimited MAC address
		// the ip parameter is optional, and if not provided the local broadcast will be used
		// example: GET /wol?mac=112233aabbcc&ip=192.168.0.1
		String mac = server.arg("mac");
		String ip = server.arg("ip");

		//If IP is specified, read it. Else attempt to determine the local broadcast.
		IPAddress target_ip;
		if(ip.length() < 7) {
			//This will only work with class C networks.
			//A proper implementation should check WiFi.subnetMask() and mask accordingly.
			target_ip = WiFi.localIP();
			target_ip[3] = 255;
		} else {
			target_ip = IPAddress((const uint8_t *) ip.c_str());
		}

		byte target_mac[6];
		macStringToBytes(mac, target_mac);

		sendWOL(target_ip, target_mac);
		server.send(200, "text/plain", "WOL sent to " + target_ip.toString() + " " + mac);
	});
	server.onNotFound([](){
		server.send(404, "text/plain", "");
	});

	server.begin();

	beginOTA();

	mdns.addService("http", "tcp", 80);
	Serial.println("HTTP server started");
}

void loop(void){
	server.handleClient();
	ArduinoOTA.handle();
}


void beginWifi() {
	WiFi.begin(ssid, password);
	Serial.println("");

	while (WiFi.status() != WL_CONNECTED) {
		delay(250);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void beginOTA() {
	ArduinoOTA.setPort(8266);
	ArduinoOTA.setHostname("esp8266");
	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
	});
	ArduinoOTA.begin();
}

/*
 * Send a Wake-On-LAN packet for the given MAC address, to the given IP
 * address. Often the IP address will be the local broadcast.
 */
void sendWOL(const IPAddress ip, const byte mac[]) {
	byte preamble[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	udp.beginPacket(ip, 9);
	udp.write(preamble, 6);
	for (uint8 i = 0; i < 16; i++) {
		udp.write(mac, 6);
	}
	udp.endPacket();
}


byte valFromChar(char c) {
	if(c >= 'a' && c <= 'f') return ((byte) (c - 'a') + 10) & 0x0F;
	if(c >= 'A' && c <= 'F') return ((byte) (c - 'A') + 10) & 0x0F;
	if(c >= '0' && c <= '9') return ((byte) (c - '0')) & 0x0F;
	return 0;
}

/*
 * Very simple converter from a String representation of a MAC address to
 * 6 bytes. Does not handle errors or delimiters, but requires very little
 * code space and no libraries.
 */
void macStringToBytes(const String mac, byte *bytes) {
	if(mac.length() >= 12) {
		for(int i = 0; i < 6; i++) {
			bytes[i] = (valFromChar(mac.charAt(i*2)) << 4) | valFromChar(mac.charAt(i*2 + 1));
		}
	} else {
		Serial.println("Incorrect MAC format.");
	}
}
