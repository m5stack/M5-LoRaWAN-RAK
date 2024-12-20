/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include <M5Unified.h>

#ifndef NATIVE_PLATFORM
// 470
#define DEVEUI       "****************"
#define APPEUI       "****************"
#define APPKEY       "********************************"
#define CHANNEL_MASK "0400"

#include "rak3172_lorawan.hpp"

RAK3172LoRaWAN lorawan;

void joinCallback(bool status)
{
    if (status) {
        String title = "Joined! EUI:" + String(DEVEUI);
    }
}

void receiveCallback(lorawan_frame_t data)
{
}

void sendCallback()
{
}

void errorCallback(char* error)
{
}

void LoRaWANLoopTask(void* arg)
{
    while (1) {
        lorawan.update();
        vTaskDelay(5);
    }
}

#endif

void setup()
{
    M5.begin();

#ifndef NATIVE_PLATFORM
    Serial.setTimeout(200);
    while (!lorawan.init(&Serial2, 22, 21, RAK3172_BPS_115200)) {
        delay(1000);
    }

    Serial.println("Device Init OK");
    while (!lorawan.setBAND(CN470, CHANNEL_MASK)) {
        delay(1000);
    }
    while (!lorawan.setOTAA(DEVEUI, APPEUI, APPKEY)) {
        delay(1000);
    }

    // while (!lorawan.setMode(CLASS_C)) {
    while (!lorawan.setMode(CLASS_A)) {
        delay(1000);
    }

    while (!lorawan.setDR(4)) {
        delay(1000);
    }

    while (!lorawan.setLinkCheck(ALLWAYS_LINKCHECK)) {
        delay(1000);
    }

    lorawan.onReceive(receiveCallback);
    lorawan.onSend(sendCallback);
    lorawan.onJoin(joinCallback);
    lorawan.onError(errorCallback);

    Serial.println("set Init OK");

    xTaskCreate(LoRaWANLoopTask, "LoRaWANLoopTask", 1024 * 10, NULL, 5, NULL);
#endif
}

void loop()
{
    M5.update();
#ifndef NATIVE_PLATFORM
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
            Serial.println(frames[i].payload);
        }
        lorawan.flush();
    }
    if (Serial.available()) {             // If the serial port reads data.
        String ch = Serial.readString();  // Copy the data read from the serial port
        lorawan.sendCommand(ch);
    }
#endif
}
