#include "HidDevice.h"
#include "../../utils/Logger.h"
#include <hidsdi.h>
#include <hidpi.h>

using namespace monkaDriver;

HidDevice::HidDevice(const std::wstring &devicePath, bool writeOnly = false)
{
    DWORD access = writeOnly ? GENERIC_WRITE : (GENERIC_READ | GENERIC_WRITE);
    DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

    handle_ = CreateFileW(
        devicePath.c_str(),
        access,
        shareMode,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (handle_ == INVALID_HANDLE_VALUE)
    {
        DWORD err = GetLastError();
        Logger::Error("Failed to open HID device: " + std::to_string(err));
    }

    devicePath_ = devicePath;
    Logger::Info("Opened HID device successfully.");
}

HidDevice::~HidDevice()
{
    if (handle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle_);
        handle_ = INVALID_HANDLE_VALUE;
        Logger::Info("Closed HID device.");
    }
}

bool HidDevice::WriteReport(const unsigned char *data, size_t length)
{
    if (!IsOpen())
        return false;

    DWORD bytesWritten = 0;
    BOOL result = WriteFile(handle_, data, (DWORD)length, &bytesWritten, NULL);

    if (!result)
    {
        Logger::Error("Failed to write HID report. Error code: " + std::to_string(GetLastError()));
        return false;
    }

    Logger::Debug("Wrote " + std::to_string(bytesWritten) + " bytes to HID device.");
    return true;
}

bool HidDevice::ReadReport(unsigned char *buffer, size_t length)
{
    if (!IsOpen())
        return false;

    DWORD bytesRead = 0;
    BOOL result = ReadFile(handle_, buffer, (DWORD)length, &bytesRead, NULL);

    if (!result)
    {
        Logger::Error("Failed to read HID report. Error code: " + std::to_string(GetLastError()));
        return false;
    }

    Logger::Debug("Read " + std::to_string(bytesRead) + " bytes from HID device.");
    return true;
}

void HidDevice::LogCaps()
{
    PHIDP_PREPARSED_DATA preparsedData;
    if (!HidD_GetPreparsedData(handle_, &preparsedData))
    {
        return;
    }

    HIDP_CAPS caps;
    HidP_GetCaps(preparsedData, &caps);

    Logger::Debug("Usage: 0x" + )

            std::wcout
        << "Usage: 0x" << std::hex << caps.Usage << std::endl;
    std::wcout << "UsagePage: 0x" << caps.UsagePage << std::dec << std::endl;
    std::wcout << "InputReportByteLength: " << caps.InputReportByteLength << std::endl;
    std::wcout << "OutputReportByteLength: " << caps.OutputReportByteLength << std::endl;
    std::wcout << "FeatureReportByteLength: " << caps.FeatureReportByteLength << std::endl;
    std::wcout << "Reserved: ";
    for (int i = 0; i < 17; ++i)
        std::wcout << caps.Reserved[i];
    std::wcout << std::endl;
    std::wcout << "NumberLinkCollectionNodes: " << caps.NumberLinkCollectionNodes << std::endl;
    std::wcout << "NumberInputButtonCaps: " << caps.NumberInputButtonCaps << std::endl;
    std::wcout << "NumberInputValueCaps: " << caps.NumberInputValueCaps << std::endl;
    std::wcout << "NumberInputDataIndices: " << caps.NumberInputDataIndices << std::endl;
    std::wcout << "NumberOutputButtonCaps: " << caps.NumberOutputButtonCaps << std::endl;
    std::wcout << "NumberOutputValueCaps: " << caps.NumberOutputValueCaps << std::endl;
    std::wcout << "NumberOutputDataIndices: " << caps.NumberOutputDataIndices << std::endl;
    std::wcout << "NumberFeatureButtonCaps: " << caps.NumberFeatureButtonCaps << std::endl;
    std::wcout << "NumberFeatureValueCaps: " << caps.NumberFeatureValueCaps << std::endl;
    std::wcout << "NumberFeatureDataIndices: " << caps.NumberFeatureDataIndices << std::endl;

    HidD_FreePreparsedData(preparsedData);
}