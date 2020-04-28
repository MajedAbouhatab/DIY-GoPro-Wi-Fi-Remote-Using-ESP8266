#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
HTTPClient http;
char ssid[] = "YourGoProSSID";
char pass[] = "YourGoProPassword";
int OnOffState = 0;
int SwitchPinNumber = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SwitchPinNumber, INPUT_PULLUP);
  while (digitalRead(SwitchPinNumber)) yield();
}

void loop() {
  if (!digitalRead(SwitchPinNumber)) {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED) yield();
    }
    OnOffState = 1 - OnOffState;
    SendCommand("bacpac", "PW", "0" + String(OnOffState));
    if (OnOffState) SendCommand("bacpac", "SH", "0" + String(OnOffState));
    else delay(5000);
    tone(LED_BUILTIN, OnOffState * 2);
  }
}

void SendCommand(String P1, String P2, String Op) {
  http.begin("http://10.5.5.9/" + P1 + "/" + P2 + "?t=" + pass + "&p=%" + Op);
  while (http.GET() != 200) yield();
  http.end();
}