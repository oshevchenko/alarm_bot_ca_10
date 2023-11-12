
#include <ESP8266WiFi.h>
#include <FastBot.h>
#include "CP866_decoder.h"
#include "passwords.h"

// #define BOT_TOKEN "xxxxxx"
// #define MY_CHAT_ID "xxxxxx"
// #define WIFI_SSID "xxxxxx"
// #define WIFI_PASS "xxxxxx"

#define TXD 14 // GPIO1/TXD01

FastBot bot(BOT_TOKEN);
void setup() {
  pinMode(TXD, OUTPUT);
  Serial.begin(115200);
  connectWiFi();
  //wait until serial transmission finished
  delay(100);
  Serial.end();
  //Connect to Satel CA-10
  Serial.begin(1200, SERIAL_8N1); //GPIO1 (TX) and GPIO3 (RX), 9600kbps, 8-bit data, no parity, 1-bit stop
  Serial.swap(); //GPIO15 (TX) and GPIO13 (RX)
  Serial.flush(); //clear serial buffer

  bot.setChatID(MY_CHAT_ID);
  bot.sendMessage("Hello!");
  bot.attach(newMsg);
  bot.showMenu("LED_ON \n LED_OFF");
}
// обработчик сообщений
void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  //Serial.print(msg.chatID);     // ID чата 
  //Serial.print(", ");
  //Serial.print(msg.username);   // логин
  //Serial.print(", ");
  //Serial.println(msg.text);     // текст
  if (msg.text == "/hello") bot.sendMessage("Hello!", msg.chatID);
  else if (msg.text == "/ledon" || msg.text == "LED_ON") {
    digitalWrite(TXD, HIGH);
    bot.sendMessage("LED is ON!", msg.chatID);
  }
  else if (msg.text == "/ledoff" || msg.text == "LED_OFF") {
    digitalWrite(TXD, LOW);
    bot.sendMessage("LED is OFF!", msg.chatID);
  }
}
void loop() {
  static char incomingByte = 0; // for incoming serial data
  static char incomingByteOld = 0; // for incoming serial data
  static String inStr = "";

  bot.tick();
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByteOld = incomingByte;
    incomingByte = Serial.read();
    if (incomingByteOld == '\r' && incomingByte == '\n') {
      // say what you got:
      bot.sendMessage(inStr);
      inStr = "";
    } else if (incomingByte > 31) {
      CP866toString(inStr, incomingByte);
    }

  }
  // digitalWrite(TXD, HIGH);
  // delay(500);
  // digitalWrite(TXD, LOW);
  // delay(500);
}
void connectWiFi() {
  delay(2000);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.printf("Connected, IP address: %s\n", WiFi.localIP().toString().c_str());

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}
