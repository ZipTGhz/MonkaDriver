#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace monkaDriver {

    class HidDevice {
    public:
        HidDevice();
        ~HidDevice();

        bool Open(const std::wstring& devicePath, bool writeOnly = false);
        void Close();

        bool WriteReport(const unsigned char* data, size_t length);
        bool ReadReport(unsigned char* buffer, size_t length);

        bool IsOpen() const { return handle_ != INVALID_HANDLE_VALUE; }
        std::wstring GetPath() const { return devicePath_; }

        void PrintCaps();

    private:
        HANDLE handle_ = INVALID_HANDLE_VALUE;
        std::wstring devicePath_;
    };

}
