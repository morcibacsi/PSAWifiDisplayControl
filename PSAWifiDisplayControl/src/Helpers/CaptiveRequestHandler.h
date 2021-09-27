//CaptiveRequestHandler.h
#pragma once

#ifndef _CaptiveRequestHandler_h
    #define _CaptiveRequestHandler_h

#include <ESPAsyncWebServer.h>

class CaptiveRequestHandler : public AsyncWebHandler {
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest* request) {
        //request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest* request) {
        request->redirect("/index.html");
    }
};
#endif