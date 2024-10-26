#include <looplambda.hpp>
#include <utils.hpp>

using namespace miscellaneous;

LoopLambda::LoopLambda(const unsigned int waitTimeInMs,
                       function<void(LoopLambda*)> lambda)
    : lambda(lambda), waitTimeInMs(waitTimeInMs), shouldLoop(false){};

void LoopLambda::start() {
    if (shouldLoop) {
        return;
    }

    shouldLoop = true;

    while (shouldLoop) {
        lambda(this);

        if (!shouldLoop) {
            break;
        }

        wait(waitTimeInMs);
    }
}

void LoopLambda::stop() noexcept { shouldLoop = false; }
