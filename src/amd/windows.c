#include "cpuid.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <memory.h>

void amd_initialize(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    __cpuid(ctx->registers, CPUID_FN8000_0000);

    printf("Max Extended Leaf: 0x%08X\n", ctx->eax);
}

void amd_get_brand_str(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    if (ctx->eax >= CPUID_FN8000_0004)
    {
        int brand[12];
        __cpuid(&brand[0], CPUID_FN8000_0002);
        __cpuid(&brand[4], CPUID_FN8000_0003);
        __cpuid(&brand[8], CPUID_FN8000_0004);

        printf("CPU Brand: %s\n", (char*)brand);
    }
    else
    {
        printf("Processor does not support brand string.\n");
    }
}

void amd_get_vendor(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    int vendor[4];

    __cpuid(ctx->registers, CPUID_FN0000_0000);

    vendor[0] = ctx->ebx;
    vendor[1] = ctx->edx;
    vendor[2] = ctx->ecx;
    vendor[3] = '\0';

    printf("%s\n", (char *)vendor);
}