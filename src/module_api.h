#pragma once

#include <cstdint>

#ifdef _WIN32
#define VORTEX_EXPORT __declspec(dllexport)
#else
#define VORTEX_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

struct VortexImage {
    int width;
    int height;
    int channels;
    std::uint8_t* data;
};

using ModuleApplyFn = bool (*)(VortexImage* image);

struct ModuleDescriptor {
    const char* id;
    const char* name;
    const char* version;
    ModuleApplyFn apply;
};

using RegisterModuleFn = const ModuleDescriptor* (*)();

}
