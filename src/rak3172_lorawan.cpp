/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "rak3172_lorawan.hpp"

bool RAK3172LoRaWAN::init(HardwareSerial* serial, int rx, int tx, rak3172_bps_t baudRate)
{
    RAK3172::init(serial, rx, tx, baudRate);
    delay(100);
    return (sendCommand("AT+NWM=1") && sendCommand("AT"));
}

bool RAK3172LoRaWAN::setApplicationIdentifier(const String& identifier)
{
    if (!checkString(identifier, 8)) {
        return false;
    }
    return sendCommand("AT+APPEUI=" + identifier);
}

bool RAK3172LoRaWAN::setApplicationKey(const String& key)
{
    if (!checkString(key, 16)) {
        return false;
    }
    return sendCommand("AT+APPKEY=" + key);
}

bool RAK3172LoRaWAN::setApplicationSessionKey(const String& key)
{
    if (!checkString(key, 16)) {
        return false;
    }
    return sendCommand("AT+APPSKEY=" + key);
}

bool RAK3172LoRaWAN::setNetworkSessionKey(const String& key)
{
    if (!checkString(key, 16)) {
        return false;
    }
    return sendCommand("AT+NWKSKEY=" + key);
}

bool RAK3172LoRaWAN::setDevAddr(const String& addr)
{
    if (!checkString(addr, 4)) {
        return false;
    }
    return sendCommand("AT+DEVADDR=" + addr);
}

bool RAK3172LoRaWAN::setDevEUI(const String& eui)
{
    if (!checkString(eui, 8)) {
        return false;
    }
    return sendCommand("AT+DEVEUI=" + eui);
}

bool RAK3172LoRaWAN::setBAND(String band, String channel_mask)
{
    if (band == "1" || band == "5" || band == "6") {
        return sendCommand("AT+BAND=" + band) && sendCommand("AT+MASK=" + channel_mask);
    }
    return sendCommand("AT+BAND=" + band);
}

bool RAK3172LoRaWAN::setOTAA(String deveui, String appeui, String appkey)
{
    _join_mode = OTAA;
    return (sendCommand("AT+NJM=1") && sendCommand("AT+DEVEUI=" + deveui) && sendCommand("AT+APPEUI=" + appeui) &&
            sendCommand("AT+APPKEY=" + appkey));
}

bool RAK3172LoRaWAN::setABP(String devaddr, String nwkskey, String appskey)
{
    _join_mode = ABP;
    return (sendCommand("AT+NJM=0") && sendCommand("AT+DEVADDR=" + devaddr) && sendCommand("AT+NWKSKEY=" + nwkskey) &&
            sendCommand("AT+APPSKEY=" + appskey));
}

bool RAK3172LoRaWAN::setADDMulc(String mode, String devaddr, String nwkskey, String appskey, uint32_t freq,
                                uint8_t dataRate, uint8_t periodicity)
{
    return ("AT+ADDMULC=" + String(mode) + ":" + String(devaddr) + ":" + String(nwkskey) + ":" + String(appskey) + ":" +
            String(freq) + ":" + String(dataRate) + ":" + String(periodicity));
}

bool RAK3172LoRaWAN::detelRmvmulc(String devaddr)
{
    return ("AT+RMVMULC=" + String(devaddr));
}

bool RAK3172LoRaWAN::setMode(lorawan_dev_class_t mode)
{
    if (mode == CLASS_A) {
        return sendCommand("AT+CLASS=A");
    }
    if (mode == CLASS_B) {
        return sendCommand("AT+CLASS=B");
    }
    if (mode == CLASS_C) {
        return sendCommand("AT+CLASS=C");
    }
    return false;
}

bool RAK3172LoRaWAN::setLinkCheck(lorawan_linkcheck_t mode)
{
    if (mode == DIS_LINKCHECK) {
        return sendCommand("AT+LINKCHECK=0");
    }
    if (mode == ONCE_LINKCHECK) {
        return sendCommand("AT+LINKCHECK=1");
    }
    if (mode == ALLWAYS_LINKCHECK) {
        return sendCommand("AT+LINKCHECK=2");
    }
    return false;
}

bool RAK3172LoRaWAN::setADR(bool adr)
{
    if (adr) {
        return sendCommand("AT+ADR=1");
    } else {
        return sendCommand("AT+ADR=0");
    }
}

bool RAK3172LoRaWAN::setDCS(bool dcs)
{
    if (dcs) {
        return sendCommand("AT+DCS=1");
    } else {
        return sendCommand("AT+DCS=0");
    }
}

bool RAK3172LoRaWAN::setLBT(bool lb)
{
    if (lb) {
        return sendCommand("AT+LBT=1");
    } else {
        return sendCommand("AT+LBT=0");
    }
}

bool RAK3172LoRaWAN::join(bool enable, bool boot_auto_join, uint8_t retry_interval, uint8_t retry_times)
{
    if (_join_mode == OTAA) {
        return sendCommand("AT+JOIN=" + String(enable) + ":" + String(boot_auto_join) + ":" + String(retry_interval) +
                           ":" + String(retry_times));
    } else {
        return false;
    }
}

bool RAK3172LoRaWAN::setRetransmission(uint8_t num)
{
    return sendCommand("AT+RETY=" + String(num));
}

bool RAK3172LoRaWAN::setJoinRX1Delay(uint8_t sec)
{
    if (_join_mode == OTAA) {
        return sendCommand("AT+JN1DL=" + String(sec));
    } else {
        return false;
    }
}

bool RAK3172LoRaWAN::setJoinRX2Delay(uint8_t sec)
{
    if (_join_mode == OTAA) {
        return sendCommand("AT+JN2DL=" + String(sec));
    } else {
        return false;
    }
}

bool RAK3172LoRaWAN::setRX1Delay(uint8_t sec)
{
    return sendCommand("AT+RX1DL=" + String(sec));
}

bool RAK3172LoRaWAN::setRX2Delay(uint8_t sec)
{
    return sendCommand("AT+RX2DL=" + String(sec));
}

bool RAK3172LoRaWAN::setRX2DR(uint8_t dr)
{
    return sendCommand("AT+RX2DR=" + String(dr));
}

bool RAK3172LoRaWAN::setDR(uint8_t dr)
{
    return sendCommand("AT+DR=" + String(dr));
}

bool RAK3172LoRaWAN::setOutPower(uint8_t power)
{
    return sendCommand("AT+TXP=" + String(power));
}

bool RAK3172LoRaWAN::setComfirm(bool comfirm)
{
    if (comfirm) {
        return sendCommand("AT+CFM=1");
    } else {
        return sendCommand("AT+CFM=0");
    }
}

size_t RAK3172LoRaWAN::send(String data, int port)
{
    String hexEncoded = encodeMsg(data);
    if (sendCommand("AT+SEND=" + String(port) + ":" + hexEncoded)) {
        return data.length();
    };
    return 0;
}

size_t RAK3172LoRaWAN::send(const uint8_t* buf, size_t size, int port)
{
    String hexEncoded = bytes2hex(buf, size);
    if (sendCommand("AT+SEND=" + String(port) + ":" + hexEncoded)) {
        return size;
    };
    return 0;
}

void RAK3172LoRaWAN::parse(String frame)
{
    lorawan_frame_t res;
    // +EVT:RX_1:-38:13:UNICAST:1:12312312
    int index = frame.indexOf("+EVT:RX_");
    if (index != -1) {
        String tmp     = frame.substring(index + 10);
        int rssi       = tmp.substring(0, tmp.indexOf(":")).toInt();
        tmp            = tmp.substring(tmp.indexOf(":") + 1);
        int snr        = tmp.substring(0, tmp.indexOf(":")).toInt();
        tmp            = tmp.substring(tmp.indexOf(":") + 1);
        bool unicast   = tmp.substring(0, tmp.indexOf(":")) == "UNICAST";
        tmp            = tmp.substring(tmp.indexOf(":") + 1);
        int port       = tmp.substring(0, tmp.indexOf(":")).toInt();
        tmp            = tmp.substring(tmp.indexOf(":") + 1);
        String payload = decodeMsg(tmp);
        res.rssi       = rssi;
        res.snr        = snr;
        res.unicast    = unicast;
        res.port       = port;
        res.len        = payload.length();
        payload.toCharArray(res.payload, payload.length() + 1);
        _frames.push_back(res);
    }
}

void RAK3172LoRaWAN::update()
{
    String res = "";
    if (xSemaphoreTake(_serial_mutex, portMAX_DELAY) == pdTRUE) {
        String res = _serial->readStringUntil('\n');
        xSemaphoreGive(_serial_mutex);
        if (res.indexOf("+EVT:LINKCHECK") != -1) {
        }

        if (res.indexOf("+EVT:JOINED") != -1) {
            if (_onJoin) {
                _onJoin(true);
            }
        }
        if (res.indexOf("+EVT:JOIN_FAILED") != -1) {
            if (_onJoin) {
                _onJoin(false);
            }
        }
        if (res.indexOf("+EVT:TX_DONE") != -1) {
            if (_onSend) {
                // _onSend();
            }
        }
        if (res.indexOf("+EVT:RX_") != -1) {
            res.remove(res.length() - 1);
            parse(res);
        }
    }
}

bool RAK3172LoRaWAN::onReceive(void (*callback)(lorawan_frame_t))
{
    _onReceive = callback;
    return true;
}

bool RAK3172LoRaWAN::onSend(void (*callback)())
{
    _onSend = callback;
    return true;
}

bool RAK3172LoRaWAN::onJoin(void (*callback)(bool))
{
    _onJoin = callback;
    return true;
}

bool RAK3172LoRaWAN::onError(void (*callback)(char*))
{
    _onError = callback;
    return true;
}

int RAK3172LoRaWAN::available()
{
    return _frames.size();
}

std::vector<lorawan_frame_t> RAK3172LoRaWAN::read()
{
    return _frames;
}

void RAK3172LoRaWAN::flush()
{
    _serial->flush();
    _frames.clear();
}

String RAK3172LoRaWAN::getApplicationIdentifier()
{
    return getCommand("AT+APPEUI=?");
}

String RAK3172LoRaWAN::getApplicationKey()
{
    return getCommand("AT+APPKEY=?");
}

String RAK3172LoRaWAN::getApplicationSessionKey()
{
    return getCommand("AT+APPSKEY?");
}

String RAK3172LoRaWAN::getNetworkSessionKey()
{
    return getCommand("AT+NWKSKEY=?");
}

String RAK3172LoRaWAN::getNetworkId()
{
    return getCommand("AT+NETID=?");
}

String RAK3172LoRaWAN::getDevAddr()
{
    return getCommand("AT+DEVADDR=?");
}

String RAK3172LoRaWAN::getDevEUI()
{
    return getCommand("AT+DEVEUI=?");
}

String RAK3172LoRaWAN::getADR()
{
    return getCommand("AT+ADR=?");
}

String RAK3172LoRaWAN::getDCS()
{
    return getCommand("AT+DCS=?");
}

String RAK3172LoRaWAN::getDutyTime()
{
    return getCommand("AT+DUTYTIME=?");
}

String RAK3172LoRaWAN::getDR()
{
    return getCommand("AT+DR?");
}

String RAK3172LoRaWAN::getJoinRX1Delay()
{
    return getCommand("AT+JN1DL=?");
}

String RAK3172LoRaWAN::getJoinRX2Delay()
{
    return getCommand("AT+JN2DL=?");
}

String RAK3172LoRaWAN::getRX1Delay()
{
    return getCommand("AT+RX1DL=?");
}

String RAK3172LoRaWAN::getRX2Delay()
{
    return getCommand("AT+RX2DL=?");
}

String RAK3172LoRaWAN::getRX2DR()
{
    return getCommand("AT+RX2DR=?");
}

String RAK3172LoRaWAN::getRX2Freq()
{
    return getCommand("AT+RX2FQ=?");
}

String RAK3172LoRaWAN::getOutPower()
{
    return getCommand("AT+TXP=?");
}

String RAK3172LoRaWAN::getRetransmission()
{
    return getCommand("AT+RETY=?");
}

String RAK3172LoRaWAN::getChannelMask()
{
    return getCommand("AT+MASK=?");
}

String RAK3172LoRaWAN::getBAND()
{
    return getCommand("AT+BAND=?");
}

String RAK3172LoRaWAN::getLinkCheck()
{
    return getCommand("AT+LINKCHECK=?");
}

String RAK3172LoRaWAN::getLstMulc()
{
    return getCommand("AT+LSTMULC=?");
}

String RAK3172LoRaWAN::getNetworkState()
{
    return getCommand("AT+NJS=?");
}