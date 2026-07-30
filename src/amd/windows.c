#include "cpuid.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <memory.h>

void amd_initialize(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    __cpuid(ctx->registers, AMD_EXTENDED_LEAF);
    printf("Max Extended Leaf: 0x%08X\n", ctx->eax);
}

void amd_get_brand_str(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    if (ctx->eax >= AMD_EXTENDED_LEAF_4)
    {
        int brand[12];
        __cpuid(&brand[0], AMD_EXTENDED_LEAF_2);
        __cpuid(&brand[4], AMD_EXTENDED_LEAF_3);
        __cpuid(&brand[8], AMD_EXTENDED_LEAF_4);

        printf("%s\n", (char*)brand);
    }
    else
    {
        printf("Processor does not support brand string.\n");
    }
}

void amd_get_vendor(cpuid_ctx *ctx) {
    int vendor[4];

    __cpuid(ctx->registers, 0x00000000);

    vendor[0] = ctx->ebx;
    vendor[1] = ctx->edx;
    vendor[2] = ctx->ecx;
    vendor[3] = '\0';

    printf("%s\n", (char *)vendor);
}