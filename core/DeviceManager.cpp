#include "DeviceManager.h"
#include "../utils/Logger.h"
#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <hidclass.h>

using namespace monkaDriver;

bool DeviceManager::ScanDevices()
{
    // TODO: detect all HID devices
    Logger::Info("Initializing DeviceManager...");
    HDEVINFO hDevIntfs = SetupDiGetClassDevsW(
        &GUID_DEVINTERFACE_HID,
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    DWORD mIndex = 0;
    SP_DEVICE_INTERFACE_DATA devIntfData;
    ZeroMemory(&devIntfData, sizeof(SP_DEVICE_INTERFACE_DATA));
    devIntfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    while (SetupDiEnumDeviceInterfaces(
        hDevIntfs,
        NULL,
        &GUID_DEVINTERFACE_HID,
        mIndex,
        &devIntfData))
    {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetailW(hDevIntfs, &devIntfData, NULL, 0, &requiredSize, NULL);
        if (requiredSize > 0)
        {
            std::vector<BYTE> buffer(requiredSize);
            auto pDevIntfDeltailData = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>(buffer.data());
            pDevIntfDeltailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
            if (SetupDiGetDeviceInterfaceDetailW(hDevIntfs, &devIntfData, pDevIntfDeltailData, requiredSize, NULL, NULL))
            {
                devicePaths_.push_back(pDevIntfDeltailData->DevicePath);
            }
            else
            {
                Logger::Error(std::to_string(GetLastError()));
            }
        }

        mIndex++;
    }

    if (hDevIntfs)
        SetupDiDestroyDeviceInfoList(hDevIntfs);

    return true;
}

std::vector<std::wstring> DeviceManager::GetDevices(const std::wstring &hwidPart)
{
    // Logger::Info("Enumerating HID devices...");
    std::vector<std::wstring> paths;
    for (const std::wstring &p : devicePaths_)
    {
        if (p.find(hwidPart))
            paths.push_back(p);
    }
    return paths;
}
