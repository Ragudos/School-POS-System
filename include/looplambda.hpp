#ifndef LOOPLAMBDA_MODULE
#define LOOPLAMBDA_MODULE

#include <functional>

#include "./utils.hpp"

using namespace std;
using namespace miscellaneous;

class LoopLambda {
   private:
    int waitTimeInMilliseconds;
    function<void(LoopLambda*)> lambda;
    bool shouldLoop;

   public:
    LoopLambda(int waitTimeInMilliseconds, function<void(LoopLambda*)> lambda)
        : waitTimeInMilliseconds(waitTimeInMilliseconds),
          lambda(lambda),
          shouldLoop(false){};

    void start() {
        if (shouldLoop) {
            return;
        }

        shouldLoop = true;

        while (shouldLoop) {
            lambda(this);
            wait(waitTimeInMilliseconds);
        }
    }

    void stop() { shouldLoop = false; }
};

#endif
