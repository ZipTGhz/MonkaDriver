#include <iostream>
#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <hidclass.h>
#include <hidsdi.h>
#include <hidusage.h>
#include <hidpi.h>
#include <vector>

void PrintInfos(HANDLE handle)
{
    if (handle == INVALID_HANDLE_VALUE)
    {
        return;
    }
    HIDD_ATTRIBUTES attributes;
    if (!HidD_GetAttributes(handle, &attributes))
    {
        return;
    }
    std::wcout << L"ATTRIBUTES:" << std::endl;
    std::wcout << L"\tSize: " << attributes.Size << std::endl;
    std::wcout << L"\tVID: " << attributes.VendorID << std::endl;
    std::wcout << L"\tPID: " << attributes.ProductID << std::endl;
    std::wcout << L"\tVersion Number: " << attributes.VersionNumber << std::endl;

    GUID hidGuid;
    HidD_GetHidGuid(&hidGuid);
    std::wcout << L"HID GUID: " << std::hex << hidGuid.Data1 << "-" << hidGuid.Data2 << "-" << hidGuid.Data3 << "-" << hidGuid.Data4 << std::dec << std::endl;

    WCHAR buffer[256];
    HidD_GetManufacturerString(handle, buffer, sizeof(buffer));
    std::wcout << L"Manufacturer: " << buffer << std::endl;
    HidD_GetPhysicalDescriptor(handle, buffer, sizeof(buffer));
    std::wcout << L"Physical descriptor: " << buffer << std::endl;
    HidD_GetProductString(handle, buffer, sizeof(buffer));
    std::wcout << L"Product: " << buffer << std::endl;
    HidD_GetSerialNumberString(handle, buffer, sizeof(buffer));
    std::wcout << L"Serial number: " << buffer << std::endl;
    ULONG numberBuffers;
    HidD_GetNumInputBuffers(handle, &numberBuffers);
    std::wcout << L"Input buffers: " << numberBuffers << std::endl;
}

void PrintCaps(HIDP_CAPS &caps)
{
    std::wcout << "Usage: 0x" << std::hex << caps.Usage << std::endl;
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
}

void PrintButtonCaps(PHIDP_PREPARSED_DATA preparsedData)
{
    // HIDP_BUTTON_CAPS btnCaps[32];
    // USHORT btnCapsLength = 32;

    // NTSTATUS statusCode = HidP_GetButtonCaps(HidP_Input, btnCaps, &btnCapsLength, preparsedData);
    // if (statusCode == HIDP_STATUS_SUCCESS)
    // {
    //     for (int i = 0; i < btnCapsLength; ++i)
    //     {
    //         std::wcout << L"UsagePage: " << btnCaps[i].UsagePage << L", ";
    //         if (btnCaps[i].IsRange)
    //         {
    //             std::wcout << L"UsageMin: " << btnCaps[i].Range.UsageMin << L", UsageMax: " << btnCaps->Range.UsageMax << std::endl;
    //         }
    //     }
    // }
    // else
    // {
    //     // std::wcout << "Actual length: " << btnCapsLength << std::endl;
    //     std::wcout << "Error code: " << statusCode << std::endl;
    // }
}

HANDLE OpenHandle(WCHAR *devicePath)
{
    HANDLE handle = CreateFileW(
        devicePath,
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    // if (handle == INVALID_HANDLE_VALUE)
    // {
    //     std::wcout << L"Error Code: " << GetLastError() << " " << devicePath << std::endl;
    //     return;
    // }

    // std::wcout << L"Device path: " << devicePath << std::endl;
    // GetInfos(handle);
    // GetParsedData(handle);
    // std::wcout << L"Open handle successfully" << std::endl;
    // CloseHandle(handle);
    return handle;
}

std::vector<std::wstring> GetDeviceInterfaces(std::wstring hwidPart = L"")
{
    std::vector<std::wstring> devicePaths;
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
                if (hwidPart.empty() || wcsstr(pDevIntfDeltailData->DevicePath, hwidPart.data()) != nullptr)
                    devicePaths.push_back(pDevIntfDeltailData->DevicePath);
            }
            else
            {
                std::wcout << L"Error code: " << GetLastError() << std::endl;
            }
        }

        mIndex++;
    }

    if (hDevIntfs)
        SetupDiDestroyDeviceInfoList(hDevIntfs);

    return devicePaths;
}

int main()
{
    /*
    Mục tiêu: Mở được HANDLE điều khiển Monka 3075
    */
    std::wstring MONKA_HWID_PART = L"vid_05ac&pid_024f";
    std::wstring DEFAULT_KEYBOARD_HWID_PART = L"vid_048d&pid_c108";

    auto devicePaths = GetDeviceInterfaces(DEFAULT_KEYBOARD_HWID_PART);
    for (int i = 0; i < devicePaths.size(); ++i)
    {
        std::wcout << devicePaths[i] << std::endl;
        HANDLE handle = OpenHandle(devicePaths[i].data());
        if (handle == INVALID_HANDLE_VALUE)
        {
            std::wcout << L"INVALID HANDLE VALUE" << std::endl;
        }

        PHIDP_PREPARSED_DATA preparsedData;
        HidD_GetPreparsedData(handle, &preparsedData);

        HIDP_CAPS caps;
        HidP_GetCaps(preparsedData, &caps);
        PrintCaps(caps);
        if (caps.NumberInputButtonCaps != 0)
        {
            PrintButtonCaps();
        }
        HidD_FreePreparsedData(preparsedData);

        CloseHandle(handle);
    }
    return 0;
}