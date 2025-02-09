#include <WebServer.h>

#ifndef AUTOLIGHT_HTTP_SERVICE_H
#define AUTOLIGHT_HTTP_SERVICE_H

class HttpService
{
public:
    void Init();
    void Send();
    HttpService();
    ~HttpService();
};

static WebServer HttpServer(80);
#endif
