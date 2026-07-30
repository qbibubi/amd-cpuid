/**
 * @file cpuid.h
 * @brief Header containing universal knowledge of AMD CPUID architecture design
 */
#pragma once

#define AMD_EXTENDED_LEAF   0x80000000
#define AMD_EXTENDED_LEAF_2 0x80000002
#define AMD_EXTENDED_LEAF_3 0x80000003
#define AMD_EXTENDED_LEAF_4 0x80000004

typedef struct _cpuid_ctx {
    union {
        int registers[4];
        struct {
            int eax, ebx, ecx, edx;
        };
    };
} cpuid_ctx, *pcpuid_ctx;

void amd_initialize(cpuid_ctx *ctx);
void amd_get_brand_str(cpuid_ctx *ctx);