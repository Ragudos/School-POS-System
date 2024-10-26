#include <cassert>
#include <iostream>
#include <memory>
#include <renderer.hpp>


using namespace std;

static unique_ptr<Renderer> renderer;

Renderer& getRenderer() { return *renderer; }

void initializeRenderer() {
    assert(!renderer || !"Renderer must only be initialized once.");

    renderer = make_unique<Renderer>();
}

void Renderer::render() noexcept {
    cout << buf.str();
    buf.str("");
    buf.clear();
}
