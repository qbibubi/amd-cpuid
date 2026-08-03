#include "amd/cpuid.hpp"

int main()
{
    CpuId::Initialize();
    CpuId::GetBrandStr();
    CpuId::GetVendor();
    CpuId::GetVersionInfo();
    CpuId::GetFeatures();
    CpuId::Print();

    return 0;
}
