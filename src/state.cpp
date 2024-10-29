#include <memory>
#include <state.hpp>

static unique_ptr<State> state;

State& getState() noexcept { return *state; }

void initializeState() {
    assert(!state || !"State must only be initialized once");

    state = make_unique<State>();

    MenuItem item1("cafe-americano");
    MenuItem item2("cafe-latte");
    MenuItem item3("cappucino");
    MenuItem item4("iced-americano");
    MenuItem item5("iced-cafe-latte");
    MenuItem item6("iced-spanish-latte");
    MenuItem item7("coffee-jelly");
    MenuItem item8("caramel-bliss");
    MenuItem item9("mocha-frappe");
    MenuItem item10("java-chip");

    state->appendMenuItem(item1);
    state->appendMenuItem(item2);
    state->appendMenuItem(item3);
    state->appendMenuItem(item4);
    state->appendMenuItem(item5);
    state->appendMenuItem(item6);
    state->appendMenuItem(item7);
    state->appendMenuItem(item8);
    state->appendMenuItem(item9);
    state->appendMenuItem(item10);
}

/** === Decided to put the definitions in @file contrib.cpp === */
