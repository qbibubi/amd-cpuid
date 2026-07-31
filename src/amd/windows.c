#include "cpuid.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <memory.h>

// This would have been so much cleaner if done with C++ templates (haha)
static void printb(int num)
{
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);

        if (i % 4 == 0)
        {
            printf(" ");
        }
    }

    printf("\n");
}

static void printb8(uint8_t num)
{
    for (int i = sizeof(uint8_t) * 8 - 1; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);

        if (i % 4 == 0)
        {
            printf(" ");
        }
    }

    printf("\n");
}

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

    printf("CPU Vendor: %s\n", (char*)vendor);
}

void amd_get_version_info(cpuid_ctx* ctx)
{
    assert(ctx != NULL);

    // The value returned by CPUID Fn8000_0001_EAX is equivalent to CPUID Fn0000_0001_EAX.
    __cpuid(ctx->registers, CPUID_FN0000_0001 /* or CPUID_FN8000_0001 */);

    printf("[%s] CPUID_FN0000_0001_EAX: ", __FUNCTION__);
    printb(ctx->eax);

    uint8_t steppingId = ctx->eax & 0xF;
    uint8_t baseModel = (ctx->eax >> 4) & 0xF;
    uint8_t baseFamily = (ctx->eax >> 8) & 0xF;

    // Bits [15:12] and [31:28] are Reserved

    uint8_t extendedModel = (ctx->eax >> 16) & 0xF;
    uint8_t extendedFamily = (ctx->eax >> 20) & 0xFF;

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
