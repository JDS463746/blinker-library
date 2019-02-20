#ifndef BLINKER_HTTP_AIR202_H
#define BLINKER_HTTP_AIR202_H

#if ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

// #include "Adapters/BlinkerSerialMQTT.h"
#include "Blinker/BlinkerATMaster.h"
#include "Blinker/BlinkerConfig.h"
#include "Blinker/BlinkerDebug.h"
#include "Blinker/BlinkerStream.h"
#include "Blinker/BlinkerUtility.h"

// #if defined(ESP32)
//     #include <HardwareSerial.h>

//     HardwareSerial *HSerialMQTT;
// #else
//     #include <SoftwareSerial.h>

//     SoftwareSerial *SSerialMQTT;
// #endif

#define BLINKER_HTTP_AIR202_DEFAULT_TIMEOUT 5000UL

enum air202_http_status_t
{
    http_init,
    http_init_success,
    http_init_failed,
    http_para_set,
    http_para_set_success,
    http_para_set_failed,
    http_start,
    http_start_success,
    http_start_failed,
    http_data_set,
    http_data_set_success,
    http_data_set_failed,
    http_data_post,
    http_data_post_success,
    http_data_post_failed,
    http_upload_success,
    http_upload_failed,
    http_read_response,
    http_read_success,
    http_read_failed,
    http_end,
    http_end_failed,
    http_end_success
};

class BlinkerHTTPAIR202
{
    public :
        BlinkerHTTPAIR202(Stream & s, bool isHardware = false)
        { stream = s; isHWS = isHardware; }

        bool begin(String host, String uri) { _host = host; _uri = uri; }
        void setTimeout(uint16_t timeout)   { _httpTimeout = timeout; }
        bool GET()
        {
            uint32_t http_time = millis();
            air202_http_status_t http_status = http_init;

            stream->println(BLINKER_CMD_HTTPINIT_RESQ);

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_init_success"));
                        http_status = http_init_success;
                        break;
                    }
                }
            }

            if (http_status != http_init_success) return false;

            stream->println(STRING_format(BLINKER_CMD_HTTPPARA_RESQ) + \
                                "=\"CCID\",1");
            http_status = http_para_set;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_para_set_success 1"));
                        http_status = http_para_set_success;
                        break;
                    }
                }
            }

            if (http_status != http_para_set_success) return false;

            stream->println(STRING_format(BLINKER_CMD_HTTPPARA_RESQ) + \
                                "=\"URL\",\"" + host + url + "\"");
            http_status = http_para_set;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_para_set_success 2"));
                        http_status = http_para_set_success;
                        break;
                    }
                }
            }

            if (http_status != http_para_set_success) return false;

            stream->println(STRING_format(BLINKER_CMD_HTTPACTION_RESQ) + \
                                "=0");
            http_status = http_start;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_start_success"));
                        http_status = http_start_success;
                        break;
                    }
                }
            }

            if (http_status != http_start_success) return false;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    _masterAT = new BlinkerMasterAT();
                    _masterAT->update(STRING_format(streamData));

                    if (_masterAT->getState() != AT_M_NONE &&
                        _masterAT->reqName() == BLINKER_CMD_HTTPACTION)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_upload_success"));
                        http_status = http_upload_success;
                    }

                    free(_masterAT);

                    break;
                }
            }

            if (http_status != http_upload_success) return false;

            stream->println(STRING_format(BLINKER_CMD_HTTPREAD_RESQ));
            http_status = http_read_response;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    _masterAT = new BlinkerMasterAT();
                    _masterAT->update(STRING_format(streamData));

                    if (_masterAT->getState() != AT_M_NONE &&
                        _masterAT->reqName() == BLINKER_CMD_HTTPREAD)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_read_success"));
                        http_status = http_read_success;
                    }

                    free(_masterAT);

                    break;
                }
            }

            if (http_status != http_read_success) return false;

            stream->println(STRING_format(BLINKER_CMD_HTTPERM_RESQ));
            http_status = http_end;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_end_success"));
                        http_status = http_end_success;
                        break;
                    }
                }
            }

            if (http_status != http_end_success) return false;

            return true;
        }

        bool POST(String _msg, String _type, String _application)
        {
            uint32_t http_time = millis();
            air202_http_status_t http_status = http_init;

            BProto::serialPrint(BLINKER_CMD_HTTPINIT_RESQ);

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_init_success"));
                        http_status = http_init_success;
                        break;
                    }
                }
            }

            if (http_status != http_init_success) return false;

            BProto::serialPrint(STRING_format(BLINKER_CMD_HTTPPARA_RESQ) + \
                                "=\"CCID\",1");
            http_status = http_para_set;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_para_set_success 1"));
                        http_status = http_para_set_success;
                        break;
                    }
                }
            }

            if (http_status != http_para_set_success) return false;

            BProto::serialPrint(STRING_format(BLINKER_CMD_HTTPPARA_RESQ) + \
                                "=\"URL\",\"" + _host + _url + "\"");
            http_status = http_para_set;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_para_set_success 2"));
                        http_status = http_para_set_success;
                        break;
                    }
                }
            }

            if (http_status != http_para_set_success) return false;

            BProto::serialPrint(STRING_format(BLINKER_CMD_HTTPPARA_RESQ) + \
                                "=\"" + _type + "\",\"" + _application + "\"");
            http_status = http_para_set;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_para_set_success 3"));
                        http_status = http_para_set_success;
                        break;
                    }
                }
            }

            if (http_status != http_para_set_success) return false;

            BProto::serialPrint(STRING_format(BLINKER_CMD_HTTPDATA_RESQ) + \
                                "=" + _msg.length() + ",10000");
            http_status = http_data_set;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_DOWNLOAD) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_data_set_success"));
                        http_status = http_data_set_success;
                        break;
                    }
                }
            }

            if (http_status != http_data_set_success) return false;

            BProto::serialPrint(_msg);
            http_status = http_data_post;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_data_post_success"));
                        http_status = http_data_post_success;
                        break;
                    }
                }
            }

            if (http_status != http_data_post_success) return false;

            BProto::serialPrint(STRING_format(BLINKER_CMD_HTTPACTION_RESQ) + \
                                "=1");
            http_status = http_start;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_start_success"));
                        http_status = http_start_success;
                        break;
                    }
                }
            }

            if (http_status != http_start_success) return false;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    _masterAT = new BlinkerMasterAT();
                    _masterAT->update(STRING_format(streamData));

                    if (_masterAT->getState() != AT_M_NONE &&
                        _masterAT->reqName() == BLINKER_CMD_HTTPACTION)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_upload_success"));
                        http_status = http_upload_success;
                    }

                    free(_masterAT);

                    break;
                }
            }

            if (http_status != http_upload_success) return false;

            BProto::serialPrint(STRING_format(BLINKER_CMD_HTTPREAD_RESQ));
            http_status = http_read_response;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    _masterAT = new BlinkerMasterAT();
                    _masterAT->update(STRING_format(streamData));

                    if (_masterAT->getState() != AT_M_NONE &&
                        _masterAT->reqName() == BLINKER_CMD_HTTPREAD)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_read_success, data len: "), _masterAT->getParam(0));
                        http_status = http_read_success;

                        dataLen = _masterAT->getParam(0).toInt();
                    }

                    free(_masterAT);

                    break;
                }
            }

            if (http_status != http_read_success) return false;

            /*TBD read data, httpData*/

            BProto::serialPrint(STRING_format(BLINKER_CMD_HTTPERM_RESQ));
            http_status = http_end;

            while(millis() - http_time < _httpTimeout)
            {
                if (available())
                {
                    if (strcmp(streamData, BLINKER_CMD_OK) == 0)
                    {
                        BLINKER_LOG_ALL(BLINKER_F("http_end_success"));
                        http_status = http_end_success;
                        break;
                    }
                }
            }

            if (http_status != http_end_success) return false;

            return true;
        }

        String getString()

    protected :
        class BlinkerMasterAT * _masterAT;
        Stream* stream;
        char*   streamData;
        bool    isFresh = false;
        bool    isHWS = false;
        String  _host;
        String  _uri;

        uint16_t _httpTimeout = BLINKER_HTTP_NBIOT_DEFAULT_TIMEOUT;

        bool available()
        {
            if (!isHWS)
            {
                #if defined(__AVR__) || defined(ESP8266)
                    if (!SSerialBLE->isListening())
                    {
                        SSerialBLE->listen();
                        ::delay(100);
                    }
                #endif
            }

            if (stream->available())
            {
                if (isFresh) free(streamData);
                streamData = (char*)malloc(1*sizeof(char));
                
                int16_t dNum = 0;
                int c_d = timedRead();
                while (dNum < BLINKER_MAX_READ_SIZE && 
                    c_d >=0 && c_d != '\n')
                {
                    if (c_d != '\r')
                    {
                        streamData[dNum] = (char)c_d;
                        dNum++;
                        streamData = (char*)realloc(streamData, (dNum+1)*sizeof(char));
                    }

                    c_d = timedRead();
                }
                dNum++;
                streamData = (char*)realloc(streamData, dNum*sizeof(char));

                streamData[dNum-1] = '\0';
                stream->flush();
                
                BLINKER_LOG_ALL(BLINKER_F("handleSerial: "), streamData);
                BLINKER_LOG_FreeHeap_ALL();
                
                if (strlen(streamData) < BLINKER_MAX_READ_SIZE)
                {
                    if (streamData[strlen(streamData) - 1] == '\r')
                        streamData[strlen(streamData) - 1] = '\0';

                    isFresh = true;
                    return true;
                }
                else
                {
                    free(streamData);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
};

#endif