#ifndef SRC_NTP_H_
#define SRC_NTP_H_

#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <ESP8266WiFi.h>

/*
 * Send a UDP request to the NTP service for the current time.
 */
void requestNTPUpdate(WiFiUDP& udp);

/*
 * Check if there is a packet in the UDP buffer which looks like an NTP update.
 * Update *currentTime if so with the current timestamp.
 * Returns true if an update was found, or false if not.
 */
bool checkReceivedNTPUpdate(WiFiUDP& udp, unsigned long *currentTime);

/*
 * Print a human-readable version of an NTP timestamp to the serial
 */
void printTimestampAsUTC(long ntptime);

/*
 * Print NTP time as a UNIX timestamp
 */
void printTimestampAsUNIX(long ntptime);

#endif /* SRC_NTP_H_ */
