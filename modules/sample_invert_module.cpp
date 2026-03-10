#include "module_api.h"

namespace {
bool applyInvert(VortexImage* image) {
    if (!image || !image->data || image->channels < 3) {
        return false;
    }

    const int pixels = image->width * image->height;
    for (int i = 0; i < pixels; ++i) {
        const int base = i * image->channels;
        image->data[base] = static_cast<std::uint8_t>(255 - image->data[base]);
        image->data[base + 1] = static_cast<std::uint8_t>(255 - image->data[base + 1]);
        image->data[base + 2] = static_cast<std::uint8_t>(255 - image->data[base + 2]);
    }
    return true;
}

const ModuleDescriptor kDescriptor{
    "sample.invert",
    "Invert Colors",
    "0.1.0",
    &applyInvert
};
}

extern "C" VORTEX_EXPORT const ModuleDescriptor* register_module() {
    return &kDescriptor;
}
