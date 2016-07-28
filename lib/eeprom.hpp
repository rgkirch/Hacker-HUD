#include <WString.h>

namespace Memory
{
    int    setSsid(const String& ssid);
    String getSsid();
    int    setNetworkPassword(const String& networkPassword);
    String getNetworkPassword();
    enum e {SSID = 0, NETWORKPASSWORD = 64};
};

