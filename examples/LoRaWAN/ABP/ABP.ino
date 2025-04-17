/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include <M5Unified.h>
#include "rak3172_lorawan.hpp"

// ABP Parameters
#define DEVADDR "***********"             // Device Address
#define APPSKEY "**********************"  // Application Session Key
#define NWKSKEY "**********************"  // Network Session Ke
#define TX      5
#define RX      6

RAK3172LoRaWAN lorawan;

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
    auto cfg = M5.config();
    AtomS3.begin(cfg);
    Serial.begin(115200);
    while (!lorawan.init(&Serial2, RX, TX, RAK3172_BPS_115200)) {
        Serial.println("[Init] Failed to initialize module, retrying...");
        delay(1000);
    }
    Serial.println("Device Init OK");
    Serial.println("[Config] Setting band to  EU868...");
    // get or set the channel mask to close or open the channel (only for US915, AU915, CN470)
    while (!lorawan.setBAND(EU868)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.println("[Config] Band set successfully.");
    Serial.println("[Config] Setting ABP parameters...");
    while (!lorawan.setABP(DEVADDR, NWKSKEY, APPSKEY)) {
        delay(1000);
    }
    Serial.println("[Config] ABP parameters set successfully.");
    // Set device mode
    Serial.println("[Config] Setting device mode to CLASS_C...");
    while (!lorawan.setMode(CLASS_C)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.println("[Config] Setting data rate to DR4...");
    while (!lorawan.setDR(4)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.println("[Config] Setting link check...");
    while (!lorawan.setLinkCheck(ALLWAYS_LINKCHECK)) {
        Serial.println(" failed, retrying...");
        delay(1000);
    }
    Serial.println("[Config] Link check set successfully.");
    lorawan.onError(errorCallback);
    Serial.println("set Init OK");
    xTaskCreate(LoRaWANLoopTask, "LoRaWANLoopTask", 1024 * 10, NULL, 5, NULL);
}

void loop()
{
    M5.update();
    if (M5.BtnA.wasReleased()) {
        String data = "UPlink LoRaWAN Frame: " + String(millis());
        Serial.print("[Info] Attempting to send data: ");
        Serial.println(data);
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
            Serial.print("UNICAST: ");
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
