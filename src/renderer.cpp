#include <ast/node.hpp>
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

Renderer::Renderer() {
    initializeRenderer();

    rootNode = make_shared<ContainerNode>();
    header = make_shared<ContainerNode>();
    body = make_shared<ContainerNode>();

    rootNode->appendChild(header);
    rootNode->appendChild(body);
}

void Renderer::renderBuffer() noexcept {
    cout << buf.str();
    buf.str("");
    buf.clear();
}
