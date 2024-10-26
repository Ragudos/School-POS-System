#include <iostream>
#include <renderer.hpp>

using namespace std;

Renderer& Renderer::getInstance() {
    static Renderer instance;

    return instance;
};

void Renderer::render() noexcept {
    cout << buf.str();
    buf.str("");
    buf.clear();
}
