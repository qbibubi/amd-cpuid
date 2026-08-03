/**
 * @file cpuid.hpp
 * @brief Header containing universal knowledge of AMD CPUID architecture design
 *
 * @note Functionality based on OS is split between `windows.c` and `linux.c` source files
 */
#pragma once
#include <cstdint>

#define CPUID_FN0000_0000 0x00000000
#define CPUID_FN0000_0001 0x00000001
#define CPUID_FN0000_0007 0x00000007
#define CPUID_FN0000_000D 0x0000000D
#define CPUID_FN0000_000F 0x0000000F

#define CPUID_FN8000_0000 0x80000000
#define CPUID_FN8000_0001 0x80000001
#define CPUID_FN8000_0002 0x80000002
#define CPUID_FN8000_0003 0x80000003
#define CPUID_FN8000_0004 0x80000004

#ifdef __cplusplus
namespace CpuId
{
#endif //  __cplusplus

    struct Context
    {
        union
        {
            int32_t Registers[4];

            struct
            {
                int32_t Eax, Ebx, Ecx, Edx;
            };
        };
    };

    void Initialize();
    void GetBrandStr();
    void GetVendor();
    void GetVersionInfo();
    void GetFeatures();
    void Print();

#ifdef __cplusplus
} // namespace cpuid
#endif //  __cplusplus
