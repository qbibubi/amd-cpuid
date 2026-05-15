#include <assert.h>
#include <cpuid.h>
#include <stdbool.h>
#include <stdio.h>

#define AMD_EXTENDED_LEAF 0x80000000
#define AMD_EXTENDED_LEAF_2 0x80000002
#define AMD_EXTENDED_LEAF_3 0x80000003
#define AMD_EXTENDED_LEAF_4 0x80000004

/**
 * 1. Create a printer structure that will be logging into the console
 * 2. Create macros for getting specific bits or just learn how to do it
 * manually
 * 3. Create defines for AMD_* specific leafs, values and what not.
 */

typedef struct cpuid_ctx {
  int eax, ebx, ecx, edx;
} cpu_ctx;

void amd_initialize(cpu_ctx *ctx);
void amd_get_brand_str(cpu_ctx *ctx);

int main() {
  struct cpuid_ctx ctx = {};

  amd_initialize(&ctx);
  amd_get_brand_str(&ctx);

  return 0;
}

void amd_initialize(cpu_ctx *ctx) {
  assert(ctx != NULL);

  __cpuid(AMD_EXTENDED_LEAF, ctx->eax, ctx->ebx, ctx->ecx, ctx->edx);
  printf("Max Extended Leaf: 0x%08X\n", ctx->eax);
}

void amd_get_brand_str(cpu_ctx *ctx) {
  assert(ctx != NULL);

  if (ctx->eax >= AMD_EXTENDED_LEAF_4) {
    unsigned int brand[12];
    __cpuid(AMD_EXTENDED_LEAF_2, brand[0], brand[1], brand[2], brand[3]);
    __cpuid(AMD_EXTENDED_LEAF_3, brand[4], brand[5], brand[6], brand[7]);
    __cpuid(AMD_EXTENDED_LEAF_4, brand[8], brand[9], brand[10], brand[11]);

    printf("%s\n", (char *)brand);
  }
}
