#pragma once

#ifndef _Config_h
    #define _Config_h

#define TASK_WATCHDOG_TIMEOUT 15

#define WIFI_AP_MODE

#ifdef WIFI_AP_MODE
    #define WIFI_SSID "PSA display AP"
    #define WIFI_PASSWORD "99999999"
#else
    #define WIFI_SSID "Your_WiFI_SSID"
    #define WIFI_PASSWORD "your_password"
#endif

#define SERIAL_BAUD 115200

// Test should be only used outside of the car
//#define TEST_MODE

#define HW_VERSION 11

#if HW_VERSION == 11
    #define CAN_RX_PIN 33
    #define CAN_TX_PIN 32
#elif HW_VERSION == 14
    #define CAN_RX_PIN 18
    #define CAN_TX_PIN 15
#endif

#endif
