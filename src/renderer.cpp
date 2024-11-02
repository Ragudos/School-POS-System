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
    bool isNew = true;

    // if exists, remove
    if (rootNode) {
        moveCursorTo(static_cast<unsigned int>(0), rootNode->getHeight());
        clearLinesFromCursorToEndOfLine(rootNode->getHeight());

        rootNode.reset();
        header.reset();
        body.reset();

        isNew = false;
    }

    rootNode = make_shared<ContainerNode>();
    header = make_shared<ContainerNode>();
    body = make_shared<ContainerNode>();

    auto title = make_shared<TextNode>(TITLE.data());

    title->setWidth(getScreen().getWidth());
    title->setColor(255, 255, 0);
    title->setBold();

    auto titleBr = make_shared<LineBreakNode>();

    header->appendChild(title);
    header->appendChild(titleBr);

    switch (viewState) {
        case RendererState::MENU: {
            createMenuHeader(isNew);
            createMenuView(isNew);
        }; break;
        case RendererState::ORDER_CONFIRMATION: {
            createOrderConfirmationHeader(isNew);
            createOrderConfirmationView(isNew);
        }; break;
        case RendererState::ORDER_RESULTS: {
            createOrderResultsHeader(isNew);
            createOrderResultsView(isNew);
        }; break;
    }

    auto br = make_shared<LineBreakNode>(2);

    rootNode->appendChild(header);
    rootNode->appendChild(br);
    rootNode->appendChild(body);

    rootNode->render(&buf);
}

void Renderer::createMenuHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0, 2);

    navHeader->setIsFlexible(false);

    shared_ptr<ButtonNode> shopBtn = make_shared<ButtonNode>(
        's', "shop", tuple<unsigned int, unsigned int>({KEY_s, KEY_S}), true);
    shared_ptr<ButtonNode> adminBtn = make_shared<ButtonNode>(
        'a', "admin", tuple<unsigned int, unsigned int>({KEY_a, KEY_A}));
    shared_ptr<ButtonNode> checkoutBtn = make_shared<ButtonNode>(
        'c', "checkout", tuple<unsigned int, unsigned int>({KEY_c, KEY_C}));

    navHeader->appendChild(shopBtn);
    navHeader->appendChild(adminBtn);
    navHeader->appendChild(checkoutBtn);

    unsigned int pos = 20;

    saveCursorPosition(&buf);
    moveCursorTo(&buf, 0, pos++);
    buf << "navHeader: " << navHeader->getPosX() << " " << navHeader->getPosY()
        << " " << navHeader->getWidth() << " " << navHeader->getHeight();
    moveCursorTo(&buf, 0, pos++);
    buf << "header: " << header->getPosX() << " " << header->getPosY() << " "
        << header->getWidth() << " " << header->getHeight();
    moveCursorTo(&buf, 0, pos++);
    buf << "s: " << shopBtn->getPosX() << " " << shopBtn->getPosY() << " "
        << shopBtn->getWidth() << " " << shopBtn->getHeight();
    moveCursorTo(&buf, 0, pos++);
    buf << "a: " << adminBtn->getPosX() << " " << adminBtn->getPosY() << " "
        << adminBtn->getWidth() << " " << adminBtn->getHeight();
    moveCursorTo(&buf, 0, pos++);
    buf << "a: " << checkoutBtn->getPosX() << " " << checkoutBtn->getPosY()
        << " " << checkoutBtn->getWidth() << " " << checkoutBtn->getHeight();
    restoreSavedCursorPosition(&buf);

    header->appendChild(navHeader);
}

void Renderer::createOrderConfirmationHeader(bool isNew) {}

void Renderer::createOrderResultsHeader(bool isNew) {}

void Renderer::createAdminHeader(bool isNew) {}

void Renderer::createMenuView(bool isNew) {
    State& state = getState();
    shared_ptr<GridNode> menuGrid = make_shared<GridNode>();
    shared_ptr<SelectNode> menuSelect = make_shared<SelectNode>();

    menuGrid->setIsFlexible(true);

    for (const auto item : state.getMenuItems()) {
        shared_ptr<SelectOptionNode> optionNode =
            make_shared<SelectOptionNode>(item.getId());

        menuSelect->appendChild(optionNode);
    }

    if (!isNew) {
        menuSelect->setActiveChildWithValue(state.getSelectedMenuItemId());
    } else {
        state.setSelectedMenuItemId(
            state.getMenuItems().at(menuSelect->getActiveOptionIdx()).getId());
    }

    shared_ptr<ContainerNode> activeMenuItemNode = make_shared<ContainerNode>();
    optional<const MenuItem*> maybeItem =
        state.getMenuItemWithId(menuSelect->getValueOfSelectedOption().value());

    menuSelect->subscribe(onMenuSelectUpdated);

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

    /**
     * TODO: fix bug when order of appending is
     * reversed below.
     */
    menuGrid->appendChild(menuSelect);
    menuGrid->appendChild(itemDisplay);

    /**
     *
     * DO NOT MOVE THIS.
     */
    itemDisplay->appendChild(itemDescription);
    itemDisplay->appendChild(itemPrice);
    itemDisplay->appendChild(itemQty);

    body->appendChild(menuGrid);

    /*
    int pos = 1;
    saveCursorPosition();

    moveCursorTo(1, (20 + (pos++)));
    cout << "body: "
         << "(" << body->getWidth() << ", " << body->getHeight() << ")";

    moveCursorTo(1, (20 + (pos++)));
    cout << "item qty: "
         << "(" << itemQty->getPosX() << ", " << itemQty->getPosY() << ", "
         << itemQty->getWidth() << ", " << itemQty->getHeight() << ")";
    moveCursorTo(1, (20 + (pos++)));
    cout << "item price: "
         << "(" << itemPrice->getPosX() << ", " << itemPrice->getPosY() << ", "
         << itemPrice->getWidth() << ", " << itemPrice->getHeight() << ")";
    moveCursorTo(1, (20 + (pos++)));
    cout << "item desc: "
         << "(" << itemDescription->getPosX() << ", "
         << itemDescription->getPosY() << ", " << itemDescription->getWidth()
         << ", " << itemDescription->getHeight() << ")";
    moveCursorTo(1, (20 + (pos++)));
    cout << "item display: "
         << "(" << itemDisplay->getPosX() << ", " << itemDisplay->getPosY()
         << ", " << itemDisplay->getWidth() << ", " << itemDisplay->getHeight()
         << ")";
    moveCursorTo(1, (20 + (pos++)));
    cout << "menu select width: " << menuSelect->getWidth();
    moveCursorTo(1, (20 + (pos++)));
    cout << "menu select (posX, posY): "
         << "(" << menuSelect->getPosX() << ", " << menuSelect->getPosY()
         << ")";
    moveCursorTo(1, (20 + (pos++)));
    cout << "menu grid width: " << menuGrid->getWidth();
    moveCursorTo(1, (20 + (pos++)));
    cout << "screen width: " << getScreen().getWidth();
    restoreSavedCursorPosition();*/
}

void Renderer::createOrderConfirmationView(bool isNew) {}

void Renderer::createOrderResultsView(bool isNew) {}

void Renderer::renderBuffer() noexcept {
    cout << buf.str();
    buf.str("");
    buf.clear();
}

void Renderer::onKeyPressed(unsigned int keyCode) {
    onKeyPressed(keyCode, rootNode);
}

void Renderer::onKeyPressed(unsigned int keyCode, Node::NodePtr currNode) {
    // base case
    if (!currNode->canHaveChildren()) {
        return;
    }

    const vector<Node::NodePtr>& children = currNode->getChildren();

    for (auto& c : children) {
        if (c->nodeType() == NodeTypes::INTERACTABLE) {
            static_pointer_cast<InteractableNode>(c)->onKeyPressed(keyCode);
        }

        onKeyPressed(keyCode, c);
    }
}

void onMenuSelectUpdated(optional<string> selectedMenuItemId) {
    if (!selectedMenuItemId.has_value()) {
        return;
    }

    State& state = getState();
    Renderer& renderer = getRenderer();

    state.setSelectedMenuItemId(selectedMenuItemId.value());

    /**
     *
     * TODO:
     *
     * Only update the changed components
     */
    renderer.createView();
    renderer.renderBuffer();
}
