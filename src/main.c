#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
# include "amd/windows.h"
#else
# include "amd/linux.h"
#endif // _WIN32

int main() {
  cpuid_ctx ctx = { 0 };

  amd_initialize(&ctx);
  amd_get_brand_str(&ctx);

  return 0;
}
