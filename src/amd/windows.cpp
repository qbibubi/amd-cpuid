#include "cpuid.hpp"

#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <format>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <intrin.h>

#undef max
#undef min

namespace
{
    CpuId::Context Ctx = { 0 };

    std::vector<std::pair<std::string, std::string>> CpuInfo;
    std::vector<std::string> Features;

    std::unordered_map<uint32_t, std::string> EcxFeatureStrings = {
        { 0,  "SSE3 supported"                             },
        { 1,  "PCLMULQDQ instruction support"              },
        { 3,  "MONITOR/MWAIT instructions"                 },
        { 9,  "Supplemental SSE3 instruction support"      },
        { 12, "FMA instruction support"                    },
        { 13, "CMPXCHG16B instruction support"             },
        { 19, "SSE4.1 instruction support"                 },
        { 20, "SSE4.2 instruction support"                 },
        { 21, "x2APIC support"                             },
        { 22, "MOVBE instruction support"                  },
        { 23, "POPCNT instruction"                         },
        { 25, "AES instruction support"                    },
        { 26, "XSAVE instructions supported by hardware"   },
        { 27, "OSXSAVE supported"                          },
        { 28, "AVX instruction support"                    },
        { 29, "Half-precision convert instruction support" },
        { 30, "RDRAND instruction support"                 }
    };

    std::unordered_map<uint32_t, std::string> EdxFeatureStrings = {
        { 0,  "x87 floating point unit on-chip"            },
        { 1,  "Virtual-mode enhancements"                  },
        { 3,  "Page-size extensions"                       },
        { 2,  "Debugging extensions"                       },
        { 4,  "Time stamp counter"                         },
        { 5,  "AMD model-specific registers"               },
        { 6,  "Physical-address extensions"                },
        { 7,  "Machine check exception"                    },
        { 8,  "CMPXCHG8B instruction"                      },
        { 9,  "Advanced programmable interrupt controller" },
        { 11, "SYSENTER and SYSEXIT instructions"          },
        { 12, "Memory-type range registers"                },
        { 13, "Page global extension"                      },
        { 14, "Machine check architecture"                 },
        { 15, "Conditional move instructions"              },
        { 16, "Page attribute table"                       },
        { 17, "Page-size extensions"                       },
        { 19, "CLFLUSH instruction support"                },
        { 23, "MMX instructions"                           },
        { 24, "FXSAVE and FXRSTOR instructions"            },
        { 25, "SSE instruction support"                    },
        { 26, "SSE2 instruction support"                   },
        { 28, "Hyper-threading technology"                 },
    };
} // namespace

namespace CpuId
{
    void Initialize()
    {
        __cpuid(Ctx.Registers, CPUID_FN8000_0000);

        CpuInfo.push_back(std::make_pair("CPU max function", std::format("{:#010x}", Ctx.Eax)));
    }

    void GetBrandStr()
    {
        if (Ctx.Eax >= CPUID_FN8000_0004)
        {
            std::array<int32_t, 13> brand;

            __cpuid(&brand[0], CPUID_FN8000_0002);
            __cpuid(&brand[4], CPUID_FN8000_0003);
            __cpuid(&brand[8], CPUID_FN8000_0004);
            brand[12] = '\0';

            CpuInfo.push_back(std::make_pair("CPU brand", reinterpret_cast<char*>(brand.data())));
        }
        else
        {
            CpuInfo.push_back(std::make_pair("CPU brand", "not supported"));
        }
    }

    void GetVendor()
    {
        std::array<int32_t, 4> vendor;

        __cpuid(Ctx.Registers, CPUID_FN0000_0000);

        vendor[0] = Ctx.Ebx;
        vendor[1] = Ctx.Edx;
        vendor[2] = Ctx.Ecx;
        vendor[3] = '\0';

        CpuInfo.push_back(std::make_pair("CPU Vendor", reinterpret_cast<char*>(vendor.data())));
    }

    void GetVersionInfo()
    {
        // The value returned by CPUID Fn8000_0001_EAX is equivalent to CPUID Fn0000_0001_EAX.
        __cpuid(Ctx.Registers, CPUID_FN0000_0001 /* or CPUID_FN8000_0001 */);

        // Bits [15:12] and [31:28] are reserved
        uint8_t steppingId = Ctx.Ecx & 0xF;
        uint8_t baseModel = (Ctx.Ecx >> 4) & 0xF;
        uint8_t baseFamily = (Ctx.Ecx >> 8) & 0xF;
        uint8_t extendedModel = (Ctx.Ecx >> 16) & 0xF;
        uint8_t extendedFamily = (Ctx.Ecx >> 20) & 0xFF;

        uint8_t family = 0x0;
        uint8_t model = (extendedModel << 4) | baseModel;

        if (extendedModel == 0xE && baseModel == 0x8)
        {
            model = 0xE8;
        }

        if (baseFamily == 0xF)
        {
            if (extendedFamily == 0x01)
            {
                family = 0x10;
            }
            else
            {
                family = baseFamily + extendedFamily;
            }
        }
        else if (baseFamily < 0xF)
        {
            family = baseFamily;
            model = baseModel;
        }
    }

    void GetFeatures()
    {
        // CPUID Fn0000_0001 fills out ECX and EDX with feature bits
        __cpuid(Ctx.Registers, CPUID_FN0000_0001 /* or CPUID_FN8000_0001 */);

        std::cout << std::format("Features\n");

        for (auto const& [bit, feature] : EcxFeatureStrings)
        {
            if ((Ctx.Ecx >> bit) & 1)
            {
                Features.push_back(feature);
            }
        }

        for (auto const& [bit, feature] : EdxFeatureStrings)
        {
            if ((Ctx.Edx >> bit) & 1)
            {
                Features.push_back(feature);
            }
        }
    }

    void Print()
    {
        size_t maxLength = 0;
        for (auto const& [key, value] : CpuInfo)
        {
            maxLength = std::max(maxLength, key.length());
        }

        auto const width = maxLength + 4;

        for (auto const& [key, value] : CpuInfo)
        {
            std::cout << std::format("{:<{}}: {}\n", key, width, value);
        }

        std::cout << "CPU features:\n";

        for (auto const& feature : Features)
        {
            std::cout << std::format("        {}\n", feature.c_str());
        }
    }
} // namespace cpuid
