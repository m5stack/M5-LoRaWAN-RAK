/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "rak3172_rf.hpp"

bool RAK3172RF::startRssi()
{
    return (sendCommand("AT+TRSSI?"));
}

bool RAK3172RF::startRadioFrequency()
{
    return (sendCommand("AT+TTONE"));
}

bool RAK3172RF::startSend(uint16_t num)
{
    return (sendCommand("AT+TTX=" + String(num)));
}

bool RAK3172RF::startRecv(uint16_t num)
{
    return (sendCommand("AT+TRX=" + String(num)));
}

bool RAK3172RF::setConfig(long freq, int pwr, int bw, int sf, int cr, int mode, int prlen, uint16_t fsk)
{
    return (sendCommand("AT+TCONF=" + String(freq) + ":" + String(pwr) + ":" + String(bw) + ":" + String(sf) + ":" +
                        String(cr) + ":0:0:" + String(mode) + ":" + String(prlen) + ":" + String(fsk) + ":0:0"));
}

bool RAK3172RF::setSendFrequencyHopping(uint32_t Fstart, uint32_t Fstop, uint16_t Fdelta, int len)
{
    return (sendCommand("AT+TTH=" + String(Fstart) + ":" + String(Fstop) + ":" + String(Fdelta) + ":" + String(len)));
}

bool RAK3172RF::stop()
{
    return (sendCommand("AT+TOFF?"));
}

bool RAK3172RF::loraCertification()
{
    return (sendCommand("AT+CERTIF?"));
}

bool RAK3172RF::RFcontinuity(long freq, int pwr, int time)
{
    return (sendCommand("AT+CW=" + String(freq) + ":" + String(pwr) + ":" + String(time)));
}

bool RAK3172RF::setRandomFrequencyHopping(uint32_t Fstart, uint32_t Fstop, uint16_t Fdelta, int len)
{
    return (sendCommand("AT+TRTH=" + String(Fstart) + ":" + String(Fstop) + ":" + String(Fdelta) + ":" + String(len)));
}