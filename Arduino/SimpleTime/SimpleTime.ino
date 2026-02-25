#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid       = "WiggleIOT";
const char* password   = "rapids8disk9ivy";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const char* time_zone = "EST5EDT,M3.2.0,M11.1.0";

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");

  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );
  sntp_servermode_dhcp(1);    // (optional)
  configTzTime(time_zone, ntpServer1, ntpServer2);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

}

void loop()
{
  delay(5000);
  printLocalTime();     // it will take some time to sync time :)
}


