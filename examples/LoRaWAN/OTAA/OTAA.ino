/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include <M5Unified.h>
#include "rak3172_lorawan.hpp"

// 470
#define DEVEUI "****************"
#define APPEUI "****************"
#define APPKEY "********************************"

// get or set the channel mask to close or open the channel (only for US915, AU915, CN470)
#define CHANNEL_MASK "0400"
#define TX           21
#define RX           22

RAK3172LoRaWAN lorawan;

void joinCallback(bool status)
{
    if (status) {
        Serial.println("[LoRaWAN] Join network successful!");
        Serial.println("Device EUI: " + String(DEVEUI));
    } else {
        Serial.println("[LoRaWAN] Join network failed!");
    }
}

void sendCallback()
{
    Serial.println("[LoRaWAN] Uplink confirmed by server");
}

void errorCallback(char* error)
{
    Serial.print("[LoRaWAN] Error: ");
    Serial.println(error);
}

void LoRaWANLoopTask(void* arg)
{
    while (1) {
        lorawan.update();
        vTaskDelay(5);
    }
}

void setup()
{
    M5.begin();
    Serial.begin(115200);
    Serial.println("[Init] Initializing LoRaWAN module...");
    while (!lorawan.init(&Serial2, RX, TX, RAK3172_BPS_115200)) {
        Serial.println("[Init] Failed to initialize module, retrying...");
        delay(1000);
    }
    Serial.println("Device Init OK");
    Serial.print("[Config] Setting band to CN470...");
    // get or set the channel mask to close or open the channel (only for US915, AU915, CN470)
    while (!lorawan.setBAND(CN470, CHANNEL_MASK)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.print("[Config] Setting OTAA parameters...");
    while (!lorawan.setOTAA(DEVEUI, APPEUI, APPKEY)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.print("[Config] Setting device mode to CLASS_A...");
    while (!lorawan.setMode(CLASS_A)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.print("[Config] Setting data rate to DR4...");
    while (!lorawan.setDR(4)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.println("[Config] Data rate set successfully.");
    while (!lorawan.setLinkCheck(ALLWAYS_LINKCHECK)) {
        delay(1000);
    }
    Serial.println("[Config] Data rate set successfully.");
    Serial.println("[Info] Attempting to join the network...");
    if (lorawan.join(true, false, 10, 10)) {
        Serial.println("Start Join...");
    } else {
        Serial.println("Join Fail");
    }
    lorawan.onSend(sendCallback);
    lorawan.onJoin(joinCallback);
    lorawan.onError(errorCallback);
    Serial.println("set Init OK");
    xTaskCreate(LoRaWANLoopTask, "LoRaWANLoopTask", 1024 * 10, NULL, 5, NULL);
}

void loop()
{
    M5.update();
    if (M5.BtnA.wasReleased()) {
        if (lorawan.join(true, false, 10, 10)) {
            Serial.println("Start Join...");
        } else {
            Serial.println("Join Fail");
        }
    }
    if (M5.BtnB.wasReleased()) {
        String data = "UPlink LoRaWAN Frame: " + String(millis());
        if (lorawan.send(data)) {
            Serial.println("send Successful");
        } else {
            Serial.println("send fail");
        }
    }
    if (lorawan.available()) {
        std::vector<lorawan_frame_t> frames = lorawan.read();
        for (int i = 0; i < frames.size(); i++) {
            Serial.print("RSSI: ");
            Serial.println(frames[i].rssi);
            Serial.print("SNR: ");
            Serial.println(frames[i].snr);
            Serial.print("LEN: ");
            Serial.println(frames[i].len);
            Serial.print("PORT: ");
            Serial.println(frames[i].port);
            Serial.print("UNITCAST: ");
            Serial.println(frames[i].unicast);
            Serial.print("Payload: ");
            for (uint8_t j = 0; j < frames[i].len; j++) {
                Serial.printf("%02X", frames[i].payload[j]);
            }
            Serial.println();
        }
        lorawan.flush();
    }
    if (Serial.available()) {             // If the serial port reads data.
        String ch = Serial.readString();  // Copy the data read from the serial port
        lorawan.sendCommand(ch);
    }
}
