#include <memory>
#include <renderer.hpp>


static unique_ptr<Renderer> renderer;

Renderer& getRenderer() noexcept { return *renderer; }

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

    switch (viewState) {
        case RendererState::MENU: {
        }; break;
        case RendererState::ORDER_CONFIRMATION: {
        }; break;
        case RendererState::ORDER_RESULTS: {
        }; break;
    }

    auto br = make_shared<LineBreakNode>(2);

    rootNode->appendChild(header);
    rootNode->appendChild(br);
    rootNode->appendChild(body);
}

void Renderer::renderBuffer() noexcept {
    rootNode->render(&buf);

    cout << buf.str();
    buf.str("");
    buf.clear();
}
