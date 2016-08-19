#include <ddns.h>

const char *ddns_host = "ipv4.nsupdate.info";
const char *ddns_update_url = "/nic/update";
const char *ddns_credentials = DDNS_BASE64;

#define TIMEOUT_MILLIS 5000

bool updateDDNS(WiFiClient& client, bool verbose) {
	if(verbose) Serial.println("Updating DDNS service...");

	if(!client.connect(ddns_host, 80)) {
		Serial.println("DDNS failed to connect.");
		return false;
	}

	client.print(String("GET ") + ddns_update_url + " HTTP/1.1\r\n" + "Host: " + ddns_host + "\r\n" +
			"Authorization: Basic " + ddns_credentials + "\r\n" +
			"Connection: close\r\n\r\n");

	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > TIMEOUT_MILLIS) {
			Serial.println("DDNS update timeout.");
			client.stop();
			return false;
		}
	}

	while(client.available()){
		String line = client.readStringUntil('\r');
		//if(verbose) Serial.println(line);
	}

	if(verbose) Serial.println("\nDDNS update complete.");
	client.stop();

	return true;
}
