/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 *
 * @note See lora_config.h in examples/P2P
 * @Hardwares: M5Core Basic v2.7+ Unit LoRaWAN CN470、EU868、US915 、AS923
 * @Dependent Library:
 * M5Unified@^0.2.5: https://github.com/m5stack/M5Unified
 * M5-LoRaWAN-RAK:https://github.com/m5stack/M5-LoRaWAN-RAK
 */

#include <M5Unified.h>
#include "lora_config.h"
#include "rak3172_p2p.hpp"

#define LORA_CONFIG_PRLEN 8
#define LORA_CONFIG_PWR   22

RAK3172P2P lora;

int msgCount = 0;
int cr       = 0;
int sf       = 12;
int bw       = 125;

void LoRaLoopTask(void* arg)
{
    while (1) {
        lora.update();
        vTaskDelay(5);
    }
}

void beep()
{
    M5.Speaker.tone(2000, 100);
    while (M5.Speaker.isPlaying()) {
        M5.update();
    }
}

void setup()
{
    M5.begin();
    M5.Speaker.begin();
    while (!lora.init(&Serial2, 22, 21, RAK3172_BPS_115200)) {
        delay(1000);
    }

    if (lora.config(LORA_FREQ, sf, bw, cr, LORA_CONFIG_PRLEN, LORA_CONFIG_PWR)) {
        beep();
    }

    lora.setMode(P2P_TX_RX_MODE);
    xTaskCreate(LoRaLoopTask, "LoRaLoopTask", 1024 * 10, NULL, 2, NULL);
}

void loop()
{
    M5.update();
    if (M5.BtnB.wasReleased()) {
        sf = 7;
        bw = 500;
        lora.setMode(P2P_RX_MODE, 0);
        if (lora.config(LORA_FREQ, sf, bw, cr, LORA_CONFIG_PRLEN, LORA_CONFIG_PWR)) {
            beep();
        }
        lora.setMode(P2P_TX_RX_MODE);
    }
    if (M5.BtnC.wasReleased()) {
        sf = 12;
        bw = 125;
        lora.setMode(P2P_RX_MODE, 0);
        if (lora.config(LORA_FREQ, sf, bw, cr, LORA_CONFIG_PRLEN, LORA_CONFIG_PWR)) {
            beep();
        }
        lora.setMode(P2P_TX_RX_MODE);
    }

    if (M5.BtnA.wasReleased()) {
        String message = "Hello World: " + String(msgCount);
        if (lora.print(message.c_str())) {
            beep();
            Serial.println("Sending " + message);
            msgCount++;
        }
    }
    if (lora.available()) {
        std::vector<p2p_frame_t> frames = lora.read();
        for (int i = 0; i < frames.size(); i++) {
            beep();
            Serial.print("RSSI: ");
            Serial.print(frames[i].rssi);
            Serial.print(" SNR: ");
            Serial.print(frames[i].snr);
            Serial.print(" LEN: ");
            Serial.print(frames[i].len);
            Serial.print(" Payload: ");
            Serial.println(frames[i].payload);
        }
        lora.flush();
    }
}
