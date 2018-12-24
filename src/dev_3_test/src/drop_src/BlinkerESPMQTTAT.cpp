// #if defined(ESP8266) || defined(ESP32)

// #include "BlinkerESPMQTTAT.h"
// #include "Blinker/BlinkerDebug.h"

// void BlinkerESPMQTTAT::begin()
// {
//     Base::begin();

//     ::delay(100);

//     EEPROM.begin(BLINKER_EEP_SIZE);
//     EEPROM.get(BLINKER_EEP_ADDR_SERIALCFG, serialSet);

//     uint32_t ss_baud = serialSet >> 8 & 0x00FFFFFF;
//     ss_cfg = serConfig();

//     if (ss_baud != 300    || ss_baud != 1200    || ss_baud != 2400   ||
//         ss_baud != 4800   || ss_baud != 9600    || ss_baud != 19200  ||
//         ss_baud != 38400  || ss_baud != 57600   || ss_baud != 74880  ||
//         ss_baud != 115200 || ss_baud != 230400  || ss_baud != 250000 ||
//         ss_baud != 500000 || ss_baud != 1000000 || ss_baud != 2000000)
//     {
//         serialSet = BLINKER_SERIAL_DEFAULT;
//         ss_baud = 9600;
//         ss_cfg = SERIAL_8N1;
        
//         EEPROM.put(BLINKER_EEP_ADDR_SERIALCFG, serialSet);
//     }

//     EEPROM.commit();
//     EEPROM.end();

//     Serial.begin(ss_baud, ss_cfg);
//     this->conn.serialBegin(Serial, true);
//     Base::atBegin();
//     // strcpy(Base::_authKey, this->conn.authKey().c_str());
//     // strcpy(Base::_deviceName, this->conn.deviceName().c_str());
//     Base::loadTimer();
//     BLINKER_LOG(BLINKER_F("BLINKER_MQTT_AT initialized..."));
// }

// #endif