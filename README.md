# Telegram bot to report the status of Satel CA-10 alarm system
The **Satel CA-10** alarm system was initially designed to transmit alarm notifications through a wired telephone network. Additionally, all events were reported as text messages to the RS-232 interface. In the absence of a wired telephone network at my residence, I opted to use an ESP8266 chip connected to my home WiFi network. A simple Arduino sketch captures status messages from the **Satel CA-10** serial interface and seamlessly redirects them to a designated Telegram bot.

![phone_screenshot](https://github.com/oshevchenko/alarm_bot_ca_10/blob/master/pictures/photo_phone.jpg)
## Satel CA-10 alarm system
In **Satel CA-10** alarm system all the events that happen like "Alarm", "Reboot", "Real time clock error" or "AC voltage disconnected" are reported on RS-232 interface in Ukrainian language using the CP866 (IBM866) code page. Originally RS-232 interface was intended to connect a printer. It is also used for programming by *DLOAD10* control software.

For example, on reboot the following messages could be observed on RS-232 interface (in Ukrainian language):
```
16/07/2023   01:12 - ПЕРЕЗАПУСК СИСТЕМИ         (С)
01/01/2023   00:00 - ЗБIЙ ГОДИННИКА             (С)
```
## Hardware
**Wemos D1 R2** ESP8266-based development board is used.  
**TXD** signal from **Satel CA-10** via **MAX232** RS-232-to-serial level converter is connected to the pin **GPIO13 (RX)**.  
Active **DTR** signal **MUST** be provided to **Satel CA-10**, otherwise no data is transmitted.
If using  **MAX232** RS-232-to-serial level converter, **TxIN** should be connected to GND and **TxOUT** should be connected to **DSR** pin of the  **Satel CA-10** RS-232 connector.  
If using  **MAX232** RS-232-to-serial level converter which has 5V VCC voltage, add current limiting 100 Ohm resistor to the data line, because ESP8266 power supply voltage is only 3.3V.
![MAX232 pinout](https://github.com/oshevchenko/alarm_bot_ca_10/blob/master/pictures/rs-232-dsr-dtr.jpg)
6-Pin RJ-12 connector is used for RS-232 interface in **Satel CA-10**
![Satel CA-10 RS-232 pinout](https://github.com/oshevchenko/alarm_bot_ca_10/blob/master/pictures/rs-232-connection.jpg)


## Software
1. Create a Telegram bot using https://t.me/BotFather (send */newbot* command, etc.), put the token to `passwords.h`
```cpp
#define BOT_TOKEN "xxxxxx"
```
2. Get your ID from https://t.me/myidbot (send */getid* command), put the ID to `passwords.h`
```cpp
#define MY_CHAT_ID "XXXXXXXXX"
```
3. Put the WiFi network name and password to the `passwords.h`
```cpp
#define WIFI_SSID "XXXXXXXX"
#define WIFI_PASS "12345678"
```
4. Compile and flash to **Wemos D1 R2** board using Arduino IDE.
