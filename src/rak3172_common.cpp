/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "rak3172_common.hpp"

String encodeMsg(String str)
{
    char buf[str.length() + 1];
    char tempbuf[((str.length() + 1) * 2)];
    str.toCharArray(buf, str.length() + 1);
    int i = 0;
    for (const char* p = buf; *p; ++p) {
        sprintf((char*)(tempbuf + i), "%02x", *p);
        i += 2;
    }
    return String(tempbuf);
}

String decodeMsg(String hexEncoded)
{
    if ((hexEncoded.length() % 2) == 0) {
        char buf[hexEncoded.length() + 1];
        char tempbuf[((hexEncoded.length() + 1))];
        hexEncoded.toCharArray(buf, hexEncoded.length() + 1);
        int i = 0;
        for (int loop = 2; loop < hexEncoded.length() + 1; loop += 2) {
            String tmpstr = hexEncoded.substring(loop - 2, loop);
            sprintf(&tempbuf[i], "%c", strtoul(tmpstr.c_str(), nullptr, 16));
            i++;
        }
        return String(tempbuf);
    } else {
        return hexEncoded;
    }
}

String bytes2hex(const uint8_t* buf, size_t size)
{
    String res = "";
    for (int i = 0; i < size; i++) {
        if (buf[i] <= 0x0f) {
            res += "0";
        }
        res += String(buf[i], HEX);
    }
    return res;
}

void hex2bytes(String hexEncoded, uint8_t* buf, size_t size)
{
    if ((hexEncoded.length() & 1) == 0) {
        char tempbuf[((hexEncoded.length() + 1))];
        hexEncoded.toCharArray(tempbuf, hexEncoded.length() + 1);
        int i = 0;
        for (int loop = 2; loop < hexEncoded.length() + 1; loop += 2) {
            String tmpstr = hexEncoded.substring(loop - 2, loop);
            buf[i]        = strtoul(tmpstr.c_str(), nullptr, 16);
            i++;
        }
    }
}

long hex2bin(String hex)
{
    if ((hex.length() & 1) == 0) {
        char buf[hex.length() + 1];
        hex.toCharArray(buf, hex.length() + 1);
        int i     = 0;
        long byte = 0;
        for (int loop = 2; loop < hex.length() + 1; loop += 2) {
            String tmpstr = hex.substring(loop - 2, loop);
            byte          = byte << 8;
            byte |= strtoul(tmpstr.c_str(), nullptr, 16);
        }
        return byte;
    } else {
        return false;
    }
}

bool checkString(const String& key, size_t len)
{
    if (key.length() != len) {
        return false;
    }
    for (char c : key) {
        if (!isxdigit(c)) {
            return false;
        }
    }
    return true;
}

bool RAK3172::init(HardwareSerial* serial, int rx, int tx, rak3172_bps_t baudRate)
{
    int baud;
    switch (baudRate) {
        case RAK3172_BPS_115200:
            baud = 115200;
            break;
        case RAK3172_BPS_9600:
            baud = 9600;
            break;
        case RAK3172_BPS_4800:
            baud = 4800;
            break;
    }
    _serial = serial;
    _tx_pin = tx;
    _rx_pin = rx;
    _serial->setTimeout(200);
    _serial->begin(baud, SERIAL_8N1, rx, tx);
    _serial_mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(_serial_mutex);
    return sendCommand("AT");
}

bool RAK3172::sendCommand(String cmd)
{
    if (xSemaphoreTake(_serial_mutex, portMAX_DELAY) == pdTRUE) {
        _serial->println(cmd);

#if defined RAK3172_DEBUG
        serialPrint("SEND CMD: ");
        serialPrintln(cmd);
#else
#endif

        String res = _serial->readString();

#if defined RAK3172_DEBUG
        serialPrint("RESPONSE: ");
        serialPrint(res);
#else
#endif

        xSemaphoreGive(_serial_mutex);
        if (res.indexOf("OK") != -1) {
            return true;
        }
    }
    return false;
}

bool RAK3172::setBaudRate(rak3172_bps_t baudRate)
{
    int baud;
    switch (baudRate) {
        case RAK3172_BPS_115200:
            baud = 115200;
            break;
        case RAK3172_BPS_9600:
            baud = 9600;
            break;
        case RAK3172_BPS_4800:
            baud = 4800;
            break;
    }
    bool result = sendCommand("AT+BAUD=" + String(baud));
    if (result) {
        _serial->updateBaudRate(baud);
    }
    return result;
}

bool RAK3172::setLPM(bool enable)
{
    if (enable) {
        return sendCommand("AT+LPM=1");
    } else {
        return sendCommand("AT+LPM=0");
    }
}

bool RAK3172::setLPMLevel(rak3172_sleep_t level)
{
    return sendCommand("AT+LPMLVL=" + String(level));
}

bool RAK3172::sleep(uint32_t time_ms)
{
    if (time_ms == 0) {
        return sendCommand("AT+SLEEP");
    }
    return sendCommand("AT+SLEEP=" + String(time_ms));
}

bool RAK3172::setDeviceAlias(String alias)
{
    return sendCommand("AT+ALIAS=" + alias);
}

String RAK3172::getCommand(String cmd)
{
    String data = "";
    if (xSemaphoreTake(_serial_mutex, portMAX_DELAY) == pdTRUE) {
        _serial->println(cmd);

#if defined RAK3172_DEBUG
        Serial.print("SEND CMD: ");
        Serial.println(cmd);
#else
#endif

        String res = _serial->readString();

#if defined RAK3172_DEBUG
        serialPrint("RESPONSE: ");
        serialPrintln(res);
#else
#endif
        xSemaphoreGive(_serial_mutex);
        int index = res.indexOf('=');
        if (index != -1) {
            int endIndex = res.indexOf(' ', index);
            if (endIndex == -1) {
                endIndex = res.indexOf('\n', index);
                if (endIndex == -1) {
                    endIndex = res.length();
                }
            }
            data = res.substring(index + 1, endIndex);
        }
    }
    return data;
}

String RAK3172::getVersion()
{
    return getCommand("AT+VER=?");
}

String RAK3172::getEquipmentNumber()
{
    return getCommand("AT+SN=?");
}

String RAK3172::getBaudRate()
{
    return getCommand("AT+BAUD=?");
}

String RAK3172::getBAT()
{
    return getCommand("AT+BAT=?");
}

String RAK3172::getBuildTime()
{
    return getCommand("AT+BUILDTIME=?");
}

String RAK3172::getRepository()
{
    return getCommand("AT+REPOINFO=?");
}

String RAK3172::getCLIVersion()
{
    return getCommand("AT+CLIVER=?");
}

String RAK3172::getAPIVersion()
{
    return getCommand("AT+APIVER=?");
}

String RAK3172::getHWModel()
{
    return getCommand("AT+HWMODEL=?");
}

String RAK3172::getHWId()
{
    return getCommand("AT+HWID=?");
}

String RAK3172::getDeviceAlias()
{
    return getCommand("AT+ALIAS=?");
}

String RAK3172::getSysbat()
{
    return getCommand("AT+SYSV=?");
}