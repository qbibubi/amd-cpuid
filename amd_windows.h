#pragma once
#include "amd_cpuid.h"

typedef struct _cpuid_ctx {
  int registers[4];
} cpuid_ctx, *pcpuid_ctx;

void amd_initialize(cpuid_ctx *ctx);
void amd_get_brand_str(cpuid_ctx *ctx);
