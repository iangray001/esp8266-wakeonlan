#ifndef SRC_DDNS_H_
#define SRC_DDNS_H_

#include <WiFiClient.h>
#include <Arduino.h>
#include "settings.h"

/*
 * Connect to www.nsupdate.info to update a DDNS entry.
 *
 * HTTP basic authentication is performed by encoding your credentials as base64
 * such as with the following command:
 *   echo -n "username:password" | base64
 * and then using the resulting string as the value of the Authorization HTTP header
 *
 * The credentials string should be included in settings.h as follows:
 * #define DDNS_BASE64 "AaAaAa123456789AaAaAa123456789=="
 *
 */
bool updateDDNS(WiFiClient& client, bool verbose);


#endif /* SRC_DDNS_H_ */
