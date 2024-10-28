#include <memory>
#include <state.hpp>

static unique_ptr<State> state;

State& getState() noexcept { return *state; }

void initializeState() {
    assert(!state || !"State must only be initialized once");

    state = make_unique<State>();
}

/** === Decided to put the definitions in @file contrib.cpp === */
