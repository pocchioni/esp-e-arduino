/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"

/************************* Mqtt setup ****************************************/

#define AIO_SERVER      "broker mqtt"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "user mqtt"
#define AIO_KEY         "pass mqtt"

/************************* esp8266 GPIO ****************************************/

#define D0 16
#define D1 5 // I2C Bus SCL (clock) GPIO5
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)
