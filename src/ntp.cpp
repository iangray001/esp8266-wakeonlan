/*
 * The printTimestamp, sendNTPpacket and checkReceivedNTPUpdate functions in this file use code from:
 * https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/NTPClient/NTPClient.ino
 * by Michael Margolis, Tom Igoe, and Ivan Grokhotkov
 */

#include <ntp.h>

const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(WiFiUDP& udp, IPAddress& address) {
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	udp.beginPacket(address, 123);
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}

void requestNTPUpdate(WiFiUDP& udp) {
	IPAddress timeServerIP;

	WiFi.hostByName(ntpServerName, timeServerIP);
	sendNTPpacket(udp, timeServerIP);
}

bool checkReceivedNTPUpdate(WiFiUDP& udp, unsigned long *currentTime) {
	int len = udp.parsePacket();
	if (len > 0) {
		udp.read(packetBuffer, NTP_PACKET_SIZE);

		//the timestamp starts at byte 40 of the received packet and is four bytes,
		unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
		unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
		*currentTime = (highWord << 16 | lowWord);

		return true;
	} else {
		return false;
	}
}


unsigned long getUNIXTimeFromNTP(long ntptime) {
	//NTP time is in seconds since 1900 but Unix time starts on Jan 1 1970
	//In seconds, 70 years is 2208988800:
	const unsigned long seventyYears = 2208988800UL;
	unsigned long epoch = ntptime - seventyYears;
	return epoch;
}


void printTimestampAsUTC(long ntptime) {
	unsigned long epoch = getUNIXTimeFromNTP(ntptime);

	// print the hour, minute and second:
	Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
	Serial.print(':');
	if ( ((epoch % 3600) / 60) < 10 ) {
		// In the first 10 minutes of each hour, we'll want a leading '0'
		Serial.print('0');
	}
	Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
	Serial.print(':');
	if ( (epoch % 60) < 10 ) {
		// In the first 10 seconds of each minute, we'll want a leading '0'
		Serial.print('0');
	}
	Serial.print(epoch % 60); // print the second
}


void printTimestampAsUNIX(long ntptime) {
	Serial.print(getUNIXTimeFromNTP(ntptime));
}

