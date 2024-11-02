#include <memory>
#include <state.hpp>

static unique_ptr<State> state;

State& getState() noexcept { return *state; }

void initializeState() {
    assert(!state || !"State must only be initialized once");

    state = make_unique<State>();

    MenuItem item1("cafe-americano", 90);
    MenuItem item2("cafe-latte", 110);
    MenuItem item3("cappucino", 110);
    MenuItem item4("iced-americano", 90);
    MenuItem item5("iced-cafe-latte", 110);
    MenuItem item6("iced-spanish-latte", 130);
    MenuItem item7("coffee-jelly", 130);
    MenuItem item8("caramel-bliss", 130);
    MenuItem item9("mocha-frappe", 130);
    MenuItem item10("java-chip", 130);

    item1.setDescription(
        "Amazing What Ha Hello There Human How Are You? HaHaha heheHe kAkosa "
        "Aarpn YumMy");
    item2.setDescription("Amazing");
    item3.setDescription("Amazing");
    item4.setDescription("Amazing");
    item5.setDescription("Amazing");
    item6.setDescription("Amazing");
    item7.setDescription("Amazing");
    item8.setDescription("Amazing");
    item9.setDescription("Amazing");
    item10.setDescription("Amazing");

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
