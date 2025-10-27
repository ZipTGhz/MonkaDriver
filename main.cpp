#include <iostream>
#include "core/DeviceManager.h"
#include "drivers/led/LedController.h"
#include "utils/Logger.h"

using namespace monkaDriver;

int main() {
    Logger::Init();
    Logger::Info("MonkaDriver starting...");

    DeviceManager devManager;
    if (!devManager.ScanDevices()){
        Logger::Error("Error when scan devices.");
        return -1;
    }

    std::wstring MONKA_HWID_PART = L"vid_05ac&pid_024f";
    std::vector<std::wstring> monkaPaths = devManager.GetDevices(MONKA_HWID_PART);
    for (const std::wstring p : monkaPaths){
        
    }

    LedController ledController;
    ledController.SetPreset(1); // Example preset

    Logger::Info("MonkaDriver exited successfully.");
    return 0;
}
