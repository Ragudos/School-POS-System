#include <ast/node.hpp>
#include <cassert>
#include <constants/metadata.hpp>
#include <contrib.hpp>
#include <iostream>
#include <memory>
#include <renderer.hpp>
#include <screen.hpp>
#include <utils.hpp>

using namespace std;
using namespace terminal;

static unique_ptr<Renderer> renderer;

Renderer& getRenderer() { return *renderer; }

void initializeRenderer() {
    assert(!renderer || !"Renderer must only be initialized once.");

    renderer = make_unique<Renderer>();
    renderer->createView();
}

void Renderer::createView() {
    // if exists, remove
    if (rootNode) {
        moveCursorTo(static_cast<unsigned int>(0), rootNode->getHeight());
        clearLinesFromCursorToEndOfLine(rootNode->getHeight());
    }

    rootNode = make_shared<ContainerNode>();
    header = make_shared<ContainerNode>();
    body = make_shared<ContainerNode>();

    auto title = make_shared<TextNode>(TITLE.data());

    title->setColor(255, 255, 0);
    title->setBold();

    header->appendChild(title);

    rootNode->appendChild(header);
    rootNode->appendChild(body);
}

void Renderer::renderBuffer() noexcept {
    rootNode->render(&buf);

    cout << buf.str();
    buf.str("");
    buf.clear();
}
