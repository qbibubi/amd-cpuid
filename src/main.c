#include "amd/cpuid.h"

int main() {
  cpuid_ctx ctx = { 0 };

  amd_initialize(&ctx);
  amd_get_brand_str(&ctx);
  amd_get_vendor(&ctx);

  return 0;
}
