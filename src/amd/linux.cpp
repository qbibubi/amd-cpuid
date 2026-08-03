#include "cpuid.hpp"

#include <assert.h>
#include <stdio.h>

namespace
{} //

namespace cpuid
{
    void initialize(cpu_ctx* ctx)
    {
        assert(ctx != nullptr);

        __cpuid(0x80000000, ctx->eax, ctx->ebx, ctx->ecx, ctx->edx);

        printf("Max Extended Leaf: 0x%08X\n", ctx->eax);
    }

    void get_brand_str(cpu_ctx* ctx)
    {
        assert(ctx != nullptr);

        if (ctx->eax >= CPUID_FN8000_0004)
        {
            unsigned int brand[12];

            __cpuid(CPUID_FN8000_0002, brand[0], brand[1], brand[2], brand[3]);
            __cpuid(CPUID_FN8000_0003, brand[4], brand[5], brand[6], brand[7]);
            __cpuid(CPUID_FN8000_0004, brand[8], brand[9], brand[10], brand[11]);

            printf("%s\n", (char*)brand);
        }
        else
        {
            printf("Processor does not support brand string.\n");
        }
    }
} // namespace cpuid
