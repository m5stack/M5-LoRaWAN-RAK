/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "rak3172_p2p.hpp"

void RAK3172P2P::parse(String frame)
{
    p2p_frame_t res;
    int index = frame.indexOf("+EVT:RXP2P:");
    if (index != -1) {
        String tmp     = frame.substring(index + 11);
        int rssi       = tmp.substring(0, tmp.indexOf(":")).toInt();
        tmp            = tmp.substring(tmp.indexOf(":") + 1);
        int snr        = tmp.substring(0, tmp.indexOf(":")).toInt();
        tmp            = tmp.substring(tmp.indexOf(":") + 1);
        String payload = decodeMsg(tmp);
        res.rssi       = rssi;
        res.snr        = snr;
        res.len        = payload.length();
        payload.toCharArray(res.payload, payload.length() + 1);
        _frames.push_back(res);
    }
}

void RAK3172P2P::update()
{
    if (xSemaphoreTake(_serial_mutex, portMAX_DELAY) == pdTRUE) {
        String res = _serial->readStringUntil('\n');
        xSemaphoreGive(_serial_mutex);
        Serial.print(res);
        if (res.indexOf("+EVT:RXP2P") != -1) {
            if (res.indexOf("ERROR") != -1) {
            } else {
                res.remove(res.length() - 1);
                parse(res);
            }
        }
        if (res.indexOf("+EVT:TXP2P DONE") != -1) {
        }
    }
}

bool RAK3172P2P::init(HardwareSerial* serial, int rx, int tx, rak3172_bps_t baudRate)
{
    RAK3172::init(serial, rx, tx, baudRate);
    restart();
    delay(100);
    return (sendCommand("AT") && sendCommand("AT+NWM=0"));
}

bool RAK3172P2P::restart()
{
    sendCommand("ATZ");
    return true;
}

bool RAK3172P2P::config(long freq, int sf, int bw, int cr, int prlen, int pwr)
{
    return sendCommand("AT+P2P=" + String(freq) + ":" + String(sf) + ":" + String(bw) + ":" + String(cr) + ":" +
                       String(prlen) + ":" + String(pwr));
}

bool RAK3172P2P::setMode(p2p_mode_t mode, time_t timeout)
{
    bool status = false;
    if (mode == P2P_TX_MODE) {
        status = sendCommand("AT+PRECV=0");
    }
    if (mode == P2P_RX_MODE) {
        status = sendCommand("AT+PRECV=" + String(timeout));
    }

    if (mode == P2P_TX_RX_MODE) {
        status = sendCommand("AT+PRECV=65533");
    }

    if (status) {
        _mode = mode;
    }
    return status;
}

bool RAK3172P2P::setFreq(long freq)
{
    return sendCommand("AT+PFREQ=" + String(freq));
}

bool RAK3172P2P::setSpreadingFactor(uint8_t sf)
{
    sf = constrain(sf, 6, 12);
    return sendCommand("AT+PSF=" + String(sf));
}

bool RAK3172P2P::setBandWidth(int bw)
{
    bw = constrain(bw, 125, 500);
    return sendCommand("AT+PBW=" + String(bw));
}

bool RAK3172P2P::setCodingRate(int8_t cr)
{
    cr = constrain(cr, 0, 3);
    return sendCommand("AT+PCR=" + String(cr));
}

bool RAK3172P2P::setOutPower(uint8_t power)
{
    return sendCommand("AT+PTP=" + String(power));
}

bool RAK3172P2P::setPreambleLength(uint16_t preambleLength)
{
    return sendCommand("AT+PPL=" + String(preambleLength));
}

bool RAK3172P2P::setSyncword(int sync)
{
    String syncStr = String(sync);
    while (syncStr.length() < 4) {
        syncStr = "0" + syncStr;  // Add leading zeros
    }
    return sendCommand("AT+SYNCWORD=" + syncStr);
}

bool RAK3172P2P::setEncipher(bool en)
{
    return sendCommand("AT+ENCRY=" + String(en));
}

bool RAK3172P2P::setEncryptionKey(const String& key)
{
    if (!checkString(key, 16)) {
        return false;
    }
    return sendCommand("AT+ENCKEY=" + String(key));
}

bool RAK3172P2P::setPasswordState(bool en)
{
    return sendCommand("AT+PCRYPT=" + String(en));
}

bool RAK3172P2P::setEncryptionDecryptionKey(const String& key)
{
    if (!checkString(key, 8)) {
        return false;
    }
    return sendCommand("AT+PKEY=" + String(key));
}

bool RAK3172P2P::setEncryptionIV(const String& key)
{
    if (!checkString(key, 16)) {
        return false;
    }
    return sendCommand("AT+CRYPIV=" + String(key));
}

bool RAK3172P2P::setFSKrate(uint16_t rate)
{
    return sendCommand("AT+PBR=" + String(rate));
}

bool RAK3172P2P::setFSKFrequencyDeviation(uint16_t freq)
{
    return sendCommand("AT+PFDEV=" + String(freq));
}

String RAK3172P2P::getFreq()
{
    return getCommand("AT+PFREQ=?");
}

String RAK3172P2P::getSpreadingFactor()
{
    return getCommand("AT+PSF=?");
}

String RAK3172P2P::getBandwidth()
{
    String data = getCommand("AT+PBW=?");
    if (strcmp(data.c_str(), "0")) {
        return "125";
    } else if (strcmp(data.c_str(), "1")) {
        return "250";
    } else if (strcmp(data.c_str(), "2")) {
        return "500";
    } else {
        return "error";
    }
}

String RAK3172P2P::getCodingRate()
{
    String data = getCommand("AT+PCR=?");
    if (strcmp(data.c_str(), "0")) {
        return "4/5";
    } else if (strcmp(data.c_str(), "1")) {
        return "4/6";
    } else if (strcmp(data.c_str(), "2")) {
        return "4/7";
    } else if (strcmp(data.c_str(), "3")) {
        return "4/8";
    } else {
        return "error";
    }
}

String RAK3172P2P::getOutPower()
{
    return getCommand("AT+PTP=?");
}

String RAK3172P2P::getSyncword()
{
    return getCommand("AT+SYNCWORD=?");
}

String RAK3172P2P::getEncipher()
{
    return getCommand("AT+ENCRY=?");
}

String RAK3172P2P::getEncryptionKey()
{
    return getCommand("AT+ENCKEY=?");
}

String RAK3172P2P::getPasswordState()
{
    return getCommand("AT+PCRYPT=?");
}

String RAK3172P2P::getEncryptionDecryptionKey()
{
    return getCommand("AT+PKEY=?");
}

String RAK3172P2P::getEncryptionIV()
{
    return getCommand("AT+CRYPIV=?");
}

String RAK3172P2P::getFSKrate()
{
    return getCommand("AT+PBR=?");
}

String RAK3172P2P::getFSKFrequencyDeviation()
{
    return getCommand("AT+PFDEV=?");
}

int RAK3172P2P::available()
{
    return _frames.size();
}

size_t RAK3172P2P::write(const uint8_t* buf, size_t size)
{
    String hexEncoded = bytes2hex(buf, size);
    if (sendCommand("AT+PSEND=" + hexEncoded)) {
        return size;
    };
    return 0;
}

size_t RAK3172P2P::print(const char* str)
{
    String hexEncoded = encodeMsg(String(str));
    if (sendCommand("AT+PSEND=" + hexEncoded)) {
        return strlen(str);
    };
    return 0;
}

std::vector<p2p_frame_t> RAK3172P2P::read()
{
    return _frames;
}

void RAK3172P2P::flush()
{
    _serial->flush();
    _frames.clear();
}
