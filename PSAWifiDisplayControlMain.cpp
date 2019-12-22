#pragma region Includes

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>

#include "Config.h"
#include "src/SerialPort/AbstractSerial.h"

#include "src/SerialPort/HardwareSerialAbs.h"
#include "src/Can/CanMessageSenderEsp32Arduino.h"
#include "src/Can/Structs/CanMenuStructs.h"

#include "src/Can/Structs/CanRadioRemoteStructs.h"
#include "src/Helpers/CaptiveRequestHandler.h"
#include "src/Helpers/index_html.h"

#pragma endregion

TaskHandle_t CANIgnitionDataTask;
TaskHandle_t CANReadTask;

AbstractCanMessageSender* CANInterface;
AbsSer *serialPort;
CanRadioButtonPacketSender* radioButtonPacketSender;

DNSServer dnsServer;
AsyncWebServer* webServer;

IPAddress apIP(192, 168, 1, 1);

/// <summary> 
/// Helper method to print an array to the serial port
/// </summary>
void PrintArrayToSerial(const uint8_t dataArray[], uint8_t dataArrayLength)
{
    char tmp[3];
    for (uint8_t i = 0; i < dataArrayLength; i++)
    {
        snprintf(tmp, 3, "%02X", dataArray[i]);
        if (i != dataArrayLength - 1)
        {
            serialPort->print(tmp);
            serialPort->print(" ");
        }
        else
        {
            serialPort->println(tmp);
        }
    }
}

#ifdef TEST_MODE
/// <summary> 
/// This task is sending the ignition signal to the radio and the display so we can test without a BSI
/// </summary>
void CANIgnitionTaskFunction(void* parameter)
{
    for (;;)
    {
        uint8_t data1[] = { 0x0E, 0x00, 0x05, 0x2F, 0x21, 0x80, 0x00, 0xA0 };
        uint8_t data2[] = { 0x08, 0x32, 0x00, 0x1F, 0x00, 0x0D, 0x40, 0x01 };

        CANInterface->SendMessage(0x036, 0, 8, data1);
        CANInterface->SendMessage(0x0F6, 0, 8, data2);

        vTaskDelay(65 / portTICK_PERIOD_MS);
        esp_task_wdt_reset();
    }
}
#endif

/// <summary> 
/// This task is reading the CAN bus so when the SeekUp and SeekDown pressed simultaneously on the remote stalk we can change the mode on the display
/// </summary>
void CANReadTaskFunction(void * parameter)
{
    uint8_t canReadMessage[20] = { 0 };
    uint8_t canReadMessageLength = 0;
    uint16_t canId = 0;

    for (;;)
    {
        canId = 0;
        canReadMessageLength = 0;
        CANInterface->ReadMessage(&canId, &canReadMessageLength, canReadMessage);
        if (canId > 0)
        {
            //serialPort->print(canId, HEX);
            //serialPort->print(" ");
            //PrintArrayToSerial(canReadMessage, canReadMessageLength);
            if (canId == CAN_ID_RADIO_REMOTE)
            {
                const CanRadioRemotePacket packet = DeSerialize<CanRadioRemotePacket>(canReadMessage);
                if (packet.data.RemoteButton.SeekDownPressed == 1 && packet.data.RemoteButton.Seek_UpPressed == 1)
                {
                    radioButtonPacketSender->SendButtonCode(CONST_MODE_BUTTON);
                }
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
        esp_task_wdt_reset();
    }
}

void notFound(AsyncWebServerRequest* request) {
    serialPort->println("not found");
    request->send(404, "text/plain", "Not found");
}

void setup()
{
    serialPort = new HwSerAbs(Serial);

    serialPort->begin(SERIAL_BAUD);

    CANInterface = new CanMessageSenderEsp32Arduino(CAN_RX_PIN, CAN_TX_PIN);
    CANInterface->Init();
    radioButtonPacketSender = new CanRadioButtonPacketSender(CANInterface);

    webServer = new AsyncWebServer(80);

    IPAddress IP;
#ifdef WIFI_AP_MODE
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

    delay(200);

    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    dnsServer.start(53, "*", apIP);
    IP = WiFi.softAPIP();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        serialPort->println("Connecting to WiFi...");
    }
    IP = WiFi.localIP();
#endif

    serialPort->print("IP address: ");
    serialPort->println(IP);

    AsyncCallbackJsonWebHandler* controlAcHandler = new AsyncCallbackJsonWebHandler("/rest/button", [](AsyncWebServerRequest* request, JsonVariant& json) {

        int buttonId = json["button"];

        serialPort->println(buttonId);
        radioButtonPacketSender->SendButtonCode(buttonId);

        AsyncResponseStream* response = request->beginResponseStream("application/json");

        DynamicJsonDocument doc(1024);
        doc["success"] = "OK";
        serializeJson(doc, *response);

    }, 1024);

    webServer->addHandler(controlAcHandler);
    webServer->onNotFound(notFound);
    webServer->on("/index.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", index_html);
    });
    webServer->addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);


    webServer->begin();

    xTaskCreatePinnedToCore(
        CANReadTaskFunction,            // Function to implement the task
        "CANReadTask",                  // Name of the task
        10000,                          // Stack size in words
        NULL,                           // Task input parameter
        0,                              // Priority of the task
        &CANReadTask,                   // Task handle.
        1);                             // Core where the task should run

#ifdef TEST_MODE
    xTaskCreatePinnedToCore(
        CANIgnitionTaskFunction,        // Function to implement the task
        "CANIgnitionTask",              // Name of the task
        10000,                          // Stack size in words
        NULL,                           // Task input parameter
        1,                              // Priority of the task
        &CANIgnitionDataTask,           // Task handle.
        1);                             // Core where the task should run
#endif

    esp_task_wdt_init(TASK_WATCHDOG_TIMEOUT, true);
}

void loop()
{
#ifdef WIFI_AP_MODE
    dnsServer.processNextRequest();
#endif
    vTaskDelay(10 / portTICK_PERIOD_MS);
}
