#ifndef ESP8266WIFIGENERIC_H_
#define ESP8266WIFIGENERIC_H_

#include "ESP8266WiFiType.h"
#include <functional>
#include <memory>

#ifdef DEBUG_ESP_WIFI
#ifdef DEBUG_ESP_PORT
#define DEBUG_WIFI_GENERIC(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#endif
#endif

#ifndef DEBUG_WIFI_GENERIC
#define DEBUG_WIFI_GENERIC(...)
#endif

struct WiFiEventHandlerOpaque;
typedef std::shared_ptr<WiFiEventHandlerOpaque> WiFiEventHandler;
typedef void (*WiFiEventCb)(WiFiEvent_t);

class ESP8266WiFiGenericClass {
    public:
        ESP8266WiFiGenericClass();
        // Note: this function is deprecated. Use one of the functions below instead.
        void onEvent(WiFiEventCb cb, WiFiEvent_t event = WIFI_EVENT_ANY) __attribute__((deprecated));
        // Subscribe to specific event and get event information as an argument to the callback
        WiFiEventHandler onStationModeConnected(std::function<void(const WiFiEventStationModeConnected&)>);
        WiFiEventHandler onStationModeDisconnected(std::function<void(const WiFiEventStationModeDisconnected&)>);
        WiFiEventHandler onStationModeAuthModeChanged(std::function<void(const WiFiEventStationModeAuthModeChanged&)>);
        WiFiEventHandler onStationModeGotIP(std::function<void(const WiFiEventStationModeGotIP&)>);
        WiFiEventHandler onStationModeDHCPTimeout(std::function<void(void)>);
        WiFiEventHandler onSoftAPModeStationConnected(std::function<void(const WiFiEventSoftAPModeStationConnected&)>);
        WiFiEventHandler onSoftAPModeStationDisconnected(std::function<void(const WiFiEventSoftAPModeStationDisconnected&)>);
        WiFiEventHandler onSoftAPModeProbeRequestReceived(std::function<void(const WiFiEventSoftAPModeProbeRequestReceived&)>);
        // WiFiEventHandler onWiFiModeChange(std::function<void(const WiFiEventModeChange&)>);
        int32_t channel(void);

        bool setSleepMode(WiFiSleepType_t type);
        WiFiSleepType_t getSleepMode();

        bool setPhyMode(WiFiPhyMode_t mode);
        WiFiPhyMode_t getPhyMode();

        void setOutputPower(float dBm);

        void persistent(bool persistent);

        bool mode(WiFiMode_t);
        WiFiMode_t getMode();

        bool enableSTA(bool enable);
        bool enableAP(bool enable);

        bool forceSleepBegin(uint32 sleepUs = 0);
        bool forceSleepWake();

    protected:
        static bool _persistent;
        static WiFiMode_t _forceSleepLastMode;

        static void _eventCallback(void *event);

    public:

        int hostByName(const char* aHostname, IPAddress& aResult);
        int hostByName(const char* aHostname, IPAddress& aResult, uint32_t timeout_ms);

    protected:

        friend class ESP8266WiFiSTAClass;
        friend class ESP8266WiFiScanClass;
        friend class ESP8266WiFiAPClass;
};

#endif /* ESP8266WIFIGENERIC_H_ */
