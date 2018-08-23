/*
	VibHub Socket.io Client
    
    A fork of https://github.com/timum-viw/socket.io-client
*/
#ifndef VhSocket_h
#define VhSocket_h
#include <Arduino.h>
#include <map>
#include <vector>
#include <WebSocketsClient.h>

#define SOCKETIOCLIENT_DEBUG(...) Serial.printf(__VA_ARGS__);
//#define SOCKETIOCLIENT_DEBUG(...)

#define PING_INTERVAL 10000 //TODO: use socket.io server response

//#define SOCKETIOCLIENT_USE_SSL
#ifdef SOCKETIOCLIENT_USE_SSL
	#define SIO_PORT_DEFAULT 443
#else
	#define SIO_PORT_DEFAULT 80
#endif
#define DEFAULT_URL "/socket.io/?transport=websocket"
#define DEFAULT_FINGERPRINT ""


class SocketIoClient {
    private:
        std::vector<String> _packets;
        WebSocketsClient _webSocket;
        int _lastPing;
        std::map<String, std::function<void (const char * payload, size_t length)>> _events;

        void trigger(const char* event, const char * payload, size_t length);
        void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
        void initialize();
    public:
        SocketIoClient(void);
        void beginSSL(const char* host, const int port = SIO_PORT_DEFAULT, const char* url = DEFAULT_URL, const char* fingerprint = DEFAULT_FINGERPRINT);
        void begin(const char* host, const int port = SIO_PORT_DEFAULT, const char* url = DEFAULT_URL);
        void loop();
        void on(const char* event, std::function<void (const char * payload, size_t length)>);
        void emit(const char* event, const char * payload = NULL);
        void disconnect();
};

#endif //VhSocket_h