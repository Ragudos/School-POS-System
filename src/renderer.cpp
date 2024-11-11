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
        footer.reset();

        isNew = false;
    }

    rootNode = make_shared<ContainerNode>();
    header = make_shared<ContainerNode>();
    body = make_shared<ContainerNode>();
    footer = make_shared<ContainerNode>();

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
            createMenuFooter(isNew);
        }; break;
        case RendererState::ORDER_CONFIRMATION: {
            createOrderConfirmationHeader(isNew);
            createOrderConfirmationView(isNew);
            createOrderConfirmationFooter(isNew);
        }; break;
        case RendererState::ORDER_RESULTS: {
            createOrderResultsHeader(isNew);
            createOrderResultsView(isNew);
            createOrderResultsFooter(isNew);
        }; break;
        case RendererState::ADMIN_MENU: {
            createAdminMenuHeader(isNew);
            createAdminMenuView(isNew);
            createAdminMenuFooter(isNew);
        }; break;
    }

    shared_ptr<LineBreakNode> headerBr = make_shared<LineBreakNode>(2);
    shared_ptr<LineBreakNode> bodyBr = make_shared<LineBreakNode>(3);

    rootNode->appendChild(header);
    rootNode->appendChild(headerBr);
    rootNode->appendChild(body);
    rootNode->appendChild(bodyBr);
    rootNode->appendChild(footer);

    rootNode->render(&buf);
}

void Renderer::createMenuHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0, 2);

    navHeader->setIsFlexible(false);

    shared_ptr<ButtonNode> shopBtn =
        make_shared<ButtonNode>('s', "shop", make_tuple(KEY_s, KEY_S), true);
    shared_ptr<ButtonNode> adminBtn =
        make_shared<ButtonNode>('a', "admin", make_tuple(KEY_a, KEY_A));
    shared_ptr<ButtonNode> checkoutBtn =
        make_shared<ButtonNode>('c', "checkout", make_tuple(KEY_c, KEY_C));

    shopBtn->subscribe(onShopBtnClicked);
    adminBtn->subscribe(onAdminBtnClicked);
    checkoutBtn->subscribe(onCheckoutBtnClicked);

    navHeader->appendChild(shopBtn);
    navHeader->appendChild(adminBtn);
    navHeader->appendChild(checkoutBtn);

    /*unsigned int pos = 20;

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
    restoreSavedCursorPosition(&buf);*/

    header->appendChild(navHeader);
}

void Renderer::createOrderConfirmationHeader(bool isNew) {}

void Renderer::createOrderResultsHeader(bool isNew) {}

void Renderer::createAdminMenuHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0, 2);

    navHeader->setIsFlexible(false);

    shared_ptr<ButtonNode> shopBtn =
        make_shared<ButtonNode>('s', "shop", make_tuple(KEY_s, KEY_S));
    shared_ptr<ButtonNode> adminBtn =
        make_shared<ButtonNode>('a', "admin", make_tuple(KEY_a, KEY_A), true);

    shopBtn->subscribe(onShopBtnClicked);
    adminBtn->subscribe(onAdminBtnClicked);

    navHeader->appendChild(shopBtn);
    navHeader->appendChild(adminBtn);

    /*unsigned int pos = 20;

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
    restoreSavedCursorPosition(&buf);*/

    header->appendChild(navHeader);
}

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

    optional<MenuItem*> maybeItem =
        state.getMenuItemWithId(menuSelect->getValueOfSelectedOption().value());

    assert(
        maybeItem != nullopt ||
        !"Received nothing from State::getMenuItemWithId() where it shouldn't!");

    shared_ptr<ContainerNode> activeMenuItemNode = make_shared<ContainerNode>();

    menuSelect->subscribe(onMenuSelectUpdated);

    const MenuItem* item = maybeItem.value();

    shared_ptr<GridNode> itemDisplay = make_shared<GridNode>();
    shared_ptr<TextNode> itemDescription =
        make_shared<TextNode>(item->getDescription());
    shared_ptr<TextNode> itemPrice =
        make_shared<TextNode>(formatNumber(item->getPrice()));
    shared_ptr<TextNode> itemQty =
        make_shared<TextNode>("quantity: " + formatNumber(item->getQty()));

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
}

void Renderer::createOrderConfirmationView(bool isNew) {}

void Renderer::createOrderResultsView(bool isNew) {}

void Renderer::createAdminMenuView(bool isNew) {}

void Renderer::createMenuFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    shared_ptr<ButtonNode> incrementBtn =
        make_shared<ButtonNode>('+', "add", make_tuple(KEY_PLUS, KEY_PLUS));
    shared_ptr<ButtonNode> decrementBtn = make_shared<ButtonNode>(
        '-', "subtract", make_tuple(KEY_HYPHEN_MINUS, KEY_HYPHEN_MINUS));

    toolTipsContainer->appendChild(incrementBtn);
    toolTipsContainer->appendChild(decrementBtn);

    incrementBtn->subscribe(onIncrementBtnClicked);
    decrementBtn->subscribe(onDecrementBtnClicked);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createOrderConfirmationFooter(bool isNew) {}

void Renderer::createOrderResultsFooter(bool isNew) {}

void Renderer::createAdminMenuFooter(bool isNew) {}

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

void onShopBtnClicked() {
    Renderer& renderer = getRenderer();
    renderer.viewState = RendererState::MENU;

    renderer.createView();
    renderer.renderBuffer();
}

void onAdminBtnClicked() {
    Renderer& renderer = getRenderer();
    renderer.viewState = RendererState::ADMIN_MENU;

    renderer.createView();
    renderer.renderBuffer();
}

void onCheckoutBtnClicked() {}

void onIncrementBtnClicked() {
    State& state = getState();
    Renderer& renderer = getRenderer();

    optional<MenuItem*> maybeSelectedMenuItem =
        state.getMenuItemWithId(state.getSelectedMenuItemId());

    assert(maybeSelectedMenuItem.has_value() || !"onIncrementBtnClicked() called despite State not having a selectedMenuItem");

    MenuItem* selectedMenuItem = maybeSelectedMenuItem.value();

    selectedMenuItem->increaseQty(1);

    renderer.createView();
    renderer.renderBuffer();
}

void onDecrementBtnClicked() {
    State& state = getState();
    Renderer& renderer = getRenderer();

    optional<MenuItem*> maybeSelectedMenuItem =
        state.getMenuItemWithId(state.getSelectedMenuItemId());

    assert(maybeSelectedMenuItem.has_value() || !"onIncrementBtnClicked() called despite State not having a selectedMenuItem");

    MenuItem* selectedMenuItem = maybeSelectedMenuItem.value();

    selectedMenuItem->decreaseQty(1);

    renderer.createView();
    renderer.renderBuffer();
}
