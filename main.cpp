#include <iostream>
#include <cstdint>
#include <fstream>
#include <iostream>

namespace fuzzing
{
uint64_t largestFrameAddress{0};
}


int LLVMFuzzerTestOneInput(uint8_t* input, size_t inputSize)
{
    volatile int a[4];
    return a[0];
}

int main(int argc, char* argv[])
{
    uint64_t frameAddress = reinterpret_cast<uint64_t>(__builtin_frame_address(0));
    std::cout << "frameAddress: " << frameAddress << std::endl;

    uint8_t* inp = 0;
    int ret = LLVMFuzzerTestOneInput(inp, 0);

    std::cout << "largestFrameAddress: " << fuzzing::largestFrameAddress << std::endl;

    uint64_t maxStackUsage = fuzzing::largestFrameAddress - frameAddress;

    // TODO: store in file? or how to transfer data?
    std::cout << "maxStackUsage: " << maxStackUsage << std::endl;

    std::ofstream stackUsageFile;
    stackUsageFile.open("/tmp/fuzzerStackUsage.txt");

    if(stackUsageFile.is_open())
    {
        stackUsageFile << maxStackUsage << std::endl;
        stackUsageFile.close();
    }
    else
    {
        std::cout << "Failed to open file /tmp/fuzzerStackUsage.txt" << std::endl;
    }

    // TODO: run for all seeds. Plot using e.g. seaborn
    // Point plot not amazing in case multiple seeds have same stack usage.
    // maybe violin plot https://seaborn.pydata.org/generated/seaborn.violinplot.html

    return ret;
}

extern "C" __attribute__((no_instrument_function))
void __cyg_profile_func_enter(void* this_fn, void* call_site)
{
    uint64_t frameAddress = reinterpret_cast<uint64_t>(__builtin_frame_address(0));
    if (frameAddress > fuzzing::largestFrameAddress)
    {
        fuzzing::largestFrameAddress = frameAddress;
    }
}

