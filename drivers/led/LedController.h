#include "../hid/HidDevice.h"

namespace monkaDriver {
    class LedController {
    public:
        void Init(const std::wstring& devicePath);
        void SetPreset(int presetId);

    private:
        HidDevice hid_;
    };
}
