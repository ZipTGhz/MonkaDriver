#pragma once
#include <vector>
#include <string>

namespace monkaDriver
{
    class DeviceManager
    {
    public:
        bool ScanDevices();
        std::vector<std::wstring> GetDevices(const std::wstring &);

    private:
        std::vector<std::wstring> devicePaths_;
    };

}
