#include "LedController.h"
#include "../../utils/Logger.h"

using namespace monkaDriver;

void LedController::Init(const std::wstring& devicePath) {
    if (!hid_.Open(devicePath, true)) {
        Logger::Error("Failed to open HID for LED controller.");
    }
}

void LedController::SetPreset(int presetId) {
    unsigned char payload[64] = { 0 };
    payload[0] = static_cast<unsigned char>(presetId);
    payload[14] = 0xAA;
    payload[15] = 0x55;

    hid_.WriteReport(payload, sizeof(payload));
}
