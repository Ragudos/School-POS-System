#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cstdint>
#include <functional>

using namespace std;

class LoopLambda {
   private:
    function<void(LoopLambda*)> lambda;
    const unsigned int waitTimeInMs;
    bool shouldLoop;

   public:
    LoopLambda(const unsigned int waitTimeInMs, function<void(LoopLambda*)> lambda);

    void start();
    void stop() noexcept;
};
