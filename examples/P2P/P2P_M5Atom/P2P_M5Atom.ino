/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 *
 * @Hardwares: M5Atom + Unit LoRaWAN CN470、EU868、US915 、AS923
 * @Dependent Library:
 * M5Atom@^0.1.3: https://github.com/m5stack/M5Atom
 * M5-LoRaWAN-RAK:https://github.com/m5stack/M5-LoRaWAN-RAK
 */

#include <M5Atom.h>
#include "rak3172_p2p.hpp"

#define LORA_FREQ         915E6
#define LORA_CONFIG_PRLEN 8
#define LORA_CONFIG_PWR   22

RAK3172P2P lora;

int selectedMode = 0;
bool baudMode    = false;
bool loraselect  = false;
int msgCount     = 0;
int cr           = 0;
int sf           = 12;
int bw           = 125;

void setup()
{
    M5.begin(true, false, true);
    Serial.begin(115200);
    M5.dis.fillpix(0x0000ff);
    while (!lora.init(&Serial2, 19, 22, RAK3172_BPS_115200)) {
        delay(1000);
    }

    lora.setMode(P2P_TX_RX_MODE);
}

void loop()
{
    M5.update();
    lora.update();
    if (M5.Btn.wasPressed()) {
        Serial.printf("M5.Btn.wasPressed\n");
        String message = "Hello:" + String(msgCount);
        if (lora.print(message.c_str())) {
            M5.dis.fillpix(0x00ff00);
            Serial.println("Sending " + message);
            msgCount++;
        }
    }
    if (M5.Btn.pressedFor(2000)) {
        loraselect = !loraselect;
        if (loraselect) {
            sf = 7;
            bw = 500;
            Serial.printf("bw:500 sf:7\n");
        } else {
            sf = 12;
            bw = 125;
            Serial.printf("bw:125 sf:12\n");
        }
        lora.setMode(P2P_RX_MODE, 0);
        lora.config(LORA_FREQ, sf, bw, cr, LORA_CONFIG_PRLEN, LORA_CONFIG_PWR);
        lora.setMode(P2P_TX_RX_MODE);
        delay(1000);
    }
    if (lora.available()) {
        std::vector<p2p_frame_t> frames = lora.read();
        for (int i = 0; i < frames.size(); i++) {
            Serial.print("RSSI: ");
            Serial.print(frames[i].rssi);
            Serial.print(" SNR: ");
            Serial.print(frames[i].snr);
            Serial.print(" LEN: ");
            Serial.print(frames[i].len);
            Serial.print(" Payload: ");
            for (uint8_t j = 0; j < frames[i].len; j++) {
                Serial.printf("%02X", frames[i].payload[j]);
            }
            Serial.println();
        }
        lora.flush();
    }
}