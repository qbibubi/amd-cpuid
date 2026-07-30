#include "cpuid.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void amd_initialize(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    __cpuid(AMD_EXTENDED_LEAF, ctx->eax, ctx->ebx, ctx->ecx, ctx->edx);
    printf("Max Extended Leaf: 0x%08X\n", ctx->eax);
}

void amd_get_brand_str(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    if (ctx->eax >= AMD_EXTENDED_LEAF_4)
    {
        unsigned int brand[12];
        __cpuid(AMD_EXTENDED_LEAF_2, brand[0], brand[1], brand[2], brand[3]);
        __cpuid(AMD_EXTENDED_LEAF_3, brand[4], brand[5], brand[6], brand[7]);
        __cpuid(AMD_EXTENDED_LEAF_4, brand[8], brand[9], brand[10], brand[11]);

        printf("%s\n", (char*)brand);
    }
    else
    {
        printf("Processor does not support brand string.\n");
    }
}
