#include <memory>
#include <renderer.hpp>


static unique_ptr<Renderer> renderer;

Renderer& getRenderer() noexcept { return *renderer; }

void initializeRenderer() {
    assert(!renderer || !"Renderer must only be initialized once.");

    renderer = make_unique<Renderer>();
    renderer->createView();
}

Renderer::Renderer() : viewState(RendererState::MENU) {}

void Renderer::createView() {
    // if exists, remove
    if (rootNode) {
        moveCursorTo(static_cast<unsigned int>(0), rootNode->getHeight());
        clearLinesFromCursorToEndOfLine(rootNode->getHeight());

        rootNode.reset();
        header.reset();
        body.reset();
    }

    rootNode = make_shared<ContainerNode>();
    header = make_shared<ContainerNode>();
    body = make_shared<ContainerNode>();

    auto title = make_shared<TextNode>(TITLE.data());

    title->setWidth(getScreen().getWidth());
    title->setColor(255, 255, 0);
    title->setBold();

    header->appendChild(title);

    switch (viewState) {
        case RendererState::MENU: {
            createMenuView();
        }; break;
        case RendererState::ORDER_CONFIRMATION: {
            createOrderConfirmationView();
        }; break;
        case RendererState::ORDER_RESULTS: {
            createOrderResultsView();
        }; break;
    }

    auto br = make_shared<LineBreakNode>(2);

    rootNode->appendChild(header);
    rootNode->appendChild(br);
    rootNode->appendChild(body);

    rootNode->render(&buf);
}

void Renderer::createMenuView() {
    State& state = getState();
    shared_ptr<GridNode> menuGrid = make_shared<GridNode>();
    shared_ptr<SelectNode> menuSelect = make_shared<SelectNode>();

    menuGrid->setIsFlexible(true);

    for (const auto item : state.getMenuItems()) {
        shared_ptr<SelectOptionNode> optionNode =
            make_shared<SelectOptionNode>(item.getId());

        menuSelect->appendChild(optionNode);
    }

    state.setSelectedMenuItemId(
        state.getMenuItems().at(menuSelect->getActiveOptionIdx()).getId());

    shared_ptr<ContainerNode> activeMenuItemNode = make_shared<ContainerNode>();
    optional<const MenuItem*> maybeItem =
        state.getMenuItemWithId(menuSelect->getValueOfSelectedOption().value());

    assert(
        maybeItem != nullopt ||
        "Received nothing from State::getMenuItemWithId() where it shouldn't!");

    const MenuItem* item = maybeItem.value();

    shared_ptr<GridNode> itemDisplay = make_shared<GridNode>();
    shared_ptr<TextNode> itemDescription =
        make_shared<TextNode>(item->getDescription());
    shared_ptr<TextNode> itemPrice =
        make_shared<TextNode>(formatNumber(item->getPrice()));
    shared_ptr<TextNode> itemQty =
        make_shared<TextNode>(formatNumber(item->getQty()));

    itemDisplay->setRowGap(1);
    itemDisplay->setIsFlexible(false);
    itemDisplay->appendChild(itemDescription);
    itemDisplay->appendChild(itemPrice);
    itemDisplay->appendChild(itemQty);

    int pos = 1;

    /**
     * TODO: fix bug when order of appending is
     * reversed below.
     */
    menuGrid->appendChild(menuSelect);
    menuGrid->appendChild(itemDisplay);

    moveCursorTo(10, (15 + (pos++)));
    cout << "item display (posX, posY): "
         << "(" << itemDisplay->getPosX() << ", " << itemDisplay->getPosY()
         << ")";
    moveCursorTo(10, (15 + (pos++)));
    cout << "item display width: " << itemDisplay->getWidth();

    body->appendChild(menuGrid);

    saveCursorPosition();

    for (const auto child : menuSelect->getChildren()) {
        auto c = static_pointer_cast<SelectOptionNode>(child);

        moveCursorTo(10, (15 + (pos++)));
        cout << "c " << c->getValue() << " (posX, posY, width, height): "
             << "(" << c->getPosX() << ", " << c->getPosY() << ", "
             << c->getWidth() << ", " << c->getHeight() << ")";
    }

    moveCursorTo(10, (15 + (pos++)));
    cout << "item qty (posX, posY, width, height): "
         << "(" << itemQty->getPosX() << ", " << itemQty->getPosY() << ", "
         << itemQty->getWidth() << ", " << itemQty->getHeight() << ")";
    moveCursorTo(10, (15 + (pos++)));
    cout << "item price (posX, posY, width, height): "
         << "(" << itemPrice->getPosX() << ", " << itemPrice->getPosY() << ", "
         << itemPrice->getWidth() << ", " << itemPrice->getHeight() << ")";
    moveCursorTo(10, (15 + (pos++)));
    cout << "item desc (posX, posY, width, height): "
         << "(" << itemDescription->getPosX() << ", "
         << itemDescription->getPosY() << ", " << itemDescription->getWidth()
         << ", " << itemDescription->getHeight() << ")";
    moveCursorTo(10, (15 + (pos++)));
    cout << "menu select width: " << menuSelect->getWidth();
    moveCursorTo(10, (15 + (pos++)));
    cout << "menu select (posX, posY): "
         << "(" << menuSelect->getPosX() << ", " << menuSelect->getPosY()
         << ")";
    moveCursorTo(10, (15 + (pos++)));
    cout << "menu grid width: " << menuGrid->getWidth();
    moveCursorTo(10, (15 + (pos++)));
    cout << "screen width: " << getScreen().getWidth();
    restoreSavedCursorPosition();
}

void Renderer::createOrderConfirmationView() {}

void Renderer::createOrderResultsView() {}

void Renderer::renderBuffer() noexcept {
    cout << buf.str();
    buf.str("");
    buf.clear();
}
