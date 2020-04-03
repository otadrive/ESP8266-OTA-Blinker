#include <Arduino.h>
#include <ESP8266httpUpdate.h>

String version = "1.0.0.0";
char SSID[33] = "SohaDevice";
char PASS[33] = "DamkpKCk";
void doUpdate();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Blinker version ");
  Serial.println(version);

  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(150);
  }

  Serial.print("Wifi IP:");
  Serial.println(WiFi.localIP());

  // Setup LED GPIO
  pinMode(2,OUTPUT);
}

int updateCheckTimer = 0;
void loop()
{
  // Lets blink
  digitalWrite(2, 1);
  delay(50);
  digitalWrite(2, 0);
  delay(50);

  updateCheckTimer++;
  if(updateCheckTimer > 45)
  {
    updateCheckTimer = 0;
    doUpdate();
  }
}

void doUpdate()
{
  String url = "http://otadrive.com/DeviceApi/GetEsp8266Update?k=7e5dab9b-6101-498e-ba3b-53aed55962a2";
  WiFiClient client;
  url += "&s=" + String(CHIPID);
  url += "&v=" + version;

  Serial.println("Get firmware from url:");
  Serial.println(url);

  t_httpUpdate_return ret = ESPhttpUpdate.update(client, url, version);
  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.println("Update faild!");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("No new update available");
    break;
  // We can't see this, because of reset chip after update OK
  case HTTP_UPDATE_OK:
    Serial.println("Update OK");
    break;

  default:
    break;
  }
}