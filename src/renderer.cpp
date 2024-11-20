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
        case RendererState::MENU_ITEM: {
            createMenuItemHeader(isNew);
            createMenuItemView(isNew);
            createMenuItemFooter(isNew);
        }; break;
        case RendererState::MENU_ITEM_SIZES: {
        };
        case RendererState::MENU_ITEM_ADDONS: {
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
        make_shared<GridNode>(screen.getWidth(), 0, 2, 1);

    navHeader->setIsFlexible(false);

    shared_ptr<ButtonNode> shopBtn =
        make_shared<ButtonNode>("s", "shop", make_tuple(KEY_s, KEY_S), true);
    shared_ptr<ButtonNode> adminBtn =
        make_shared<ButtonNode>("a", "admin", make_tuple(KEY_a, KEY_A));
    shared_ptr<ButtonNode> checkoutBtn =
        make_shared<ButtonNode>("c", "checkout", make_tuple(KEY_c, KEY_C));

    shopBtn->subscribe(onShopBtnClicked);
    adminBtn->subscribe(onAdminBtnClicked);
    checkoutBtn->subscribe(onCheckoutBtnClicked);

    navHeader->appendChild(shopBtn);
    navHeader->appendChild(adminBtn);
    navHeader->appendChild(checkoutBtn);

    header->appendChild(navHeader);
}

void Renderer::createMenuItemHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0);

    navHeader->setIsFlexible(false);
    navHeader->setColGap(2);
    navHeader->setRowGap(1);

    shared_ptr<ButtonNode> backBtn =
        make_shared<ButtonNode>("←", "esc", make_tuple(KEY_ESC, KEY_ESC));
    shared_ptr<ButtonNode> sizesBtn =
        make_shared<ButtonNode>("s", "sizes", make_tuple(KEY_S, KEY_s));
    shared_ptr<ButtonNode> addonsBtn =
        make_shared<ButtonNode>("a", "addons", make_tuple(KEY_A, KEY_a));

    backBtn->subscribe(onEscBtnClickedOnMenuItem);
    sizesBtn->subscribe(onSizesBtnClicked);
    addonsBtn->subscribe(onAddonsBtnClicked);

    navHeader->appendChild(backBtn);
    navHeader->appendChild(sizesBtn);
    navHeader->appendChild(addonsBtn);

    header->appendChild(navHeader);
}

void Renderer::createMenuItemSizesHeader(bool isNew) {}

void Renderer::createMenuItemAddonsHeader(bool isNew) {}

void Renderer::createOrderConfirmationHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0, 2);

    navHeader->setIsFlexible(false);

    shared_ptr<ButtonNode> shopBtn =
        make_shared<ButtonNode>("s", "shop", make_tuple(KEY_S, KEY_s));
    shared_ptr<ButtonNode> checkoutBtn = make_shared<ButtonNode>(
        "c", "checkout", make_tuple(KEY_c, KEY_C), true);

    shopBtn->subscribe(onShopBtnClicked);
    checkoutBtn->subscribe(onCheckoutBtnClicked);

    navHeader->appendChild(shopBtn);
    navHeader->appendChild(checkoutBtn);

    header->appendChild(navHeader);
}

void Renderer::createOrderResultsHeader(bool isNew) {}

void Renderer::createAdminMenuHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0, 2);

    navHeader->setIsFlexible(false);

    shared_ptr<ButtonNode> shopBtn =
        make_shared<ButtonNode>("s", "shop", make_tuple(KEY_s, KEY_S));
    shared_ptr<ButtonNode> adminBtn =
        make_shared<ButtonNode>("a", "admin", make_tuple(KEY_a, KEY_A), true);

    shopBtn->subscribe(onShopBtnClicked);
    adminBtn->subscribe(onAdminBtnClicked);

    navHeader->appendChild(shopBtn);
    navHeader->appendChild(adminBtn);

    header->appendChild(navHeader);
}

void Renderer::createMenuView(bool isNew) {
    State& state = getState();
    shared_ptr<GridNode> menuGrid = make_shared<GridNode>();
    shared_ptr<SelectNode> menuSelect = make_shared<SelectNode>();

    menuGrid->setIsFlexible(true);

    for (const auto item : state.getMenuItemsData()) {
        shared_ptr<SelectOptionNode> optionNode =
            make_shared<SelectOptionNode>(item.getName());

        menuSelect->appendChild(optionNode);
    }

    if (!isNew) {
        menuSelect->setActiveChildWithValue(
            state.getSelectedMenuItemDataName());
    } else {
        state.setSelectedMenuItemDataName(
            state.getMenuItemsData()
                .at(menuSelect->getActiveOptionIdx())
                .getName());
    }

    optional<MenuItemData> maybeItem = state.getMenuItemDataWithName(
        menuSelect->getValueOfSelectedOption().value());

    assert(
        maybeItem != nullopt ||
        !"Received nothing from State::getMenuItemWithId() where it shouldn't!");

    shared_ptr<ContainerNode> activeMenuItemNode = make_shared<ContainerNode>();

    menuSelect->subscribe(onMenuSelectUpdated);

    const MenuItemData item = maybeItem.value();

    shared_ptr<GridNode> itemDisplay = make_shared<GridNode>();
    shared_ptr<TextNode> itemDescription =
        make_shared<TextNode>(item.getDescription());
    shared_ptr<TextNode> itemPrice =
        make_shared<TextNode>(formatNumber(item.getBasePrice()));

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

    body->appendChild(menuGrid);
}

void Renderer::createMenuItemView(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> menuItemMetadataContainer =
        make_shared<GridNode>(screen.getWidth(), screen.getWidth());
    shared_ptr<GridNode> menuItemDynamicMetadataContainer =
        make_shared<GridNode>(screen.getWidth(), 0);

    menuItemMetadataContainer->setRowGap(1);
    menuItemDynamicMetadataContainer->setRowGap(1);

    menuItemMetadataContainer->setIsFlexible(false);
    menuItemDynamicMetadataContainer->setIsFlexible(false);

    optional<MenuItem*> maybeCurrMenuItem =
        state.getMenuItemWithUid(state.getSelectedMenuItemInCartUid());

    assert(maybeCurrMenuItem.has_value());

    MenuItem* currMenuItem = maybeCurrMenuItem.value();

    shared_ptr<TextNode> menuItemName =
        make_shared<TextNode>(currMenuItem->getName());
    shared_ptr<TextNode> menuItemDescription = make_shared<TextNode>(
        state.getMenuItemDataWithName(currMenuItem->getName())
            .value()
            .getDescription());
    shared_ptr<TextNode> menuItemPrice =
        make_shared<TextNode>(formatNumber(currMenuItem->getBasePrice()));

    menuItemMetadataContainer->appendChild(menuItemName);
    menuItemMetadataContainer->appendChild(menuItemDescription);
    menuItemMetadataContainer->appendChild(menuItemPrice);

    shared_ptr<TextNode> size =
        make_shared<TextNode>("size: " + toString(currMenuItem->getSize()));
    shared_ptr<TextNode> qty = make_shared<TextNode>(
        "- " + formatNumber(currMenuItem->getQty()) + " +");

    menuItemDynamicMetadataContainer->appendChild(size);
    menuItemDynamicMetadataContainer->appendChild(qty);

    auto br = make_shared<LineBreakNode>(2);

    body->appendChild(menuItemMetadataContainer);
    body->appendChild(br);
    body->appendChild(menuItemDynamicMetadataContainer);
}

void Renderer::createMenuItemSizesView(bool isNew) {}

void Renderer::createMenuItemAddonsView(bool isNew) {}

void Renderer::createOrderConfirmationView(bool isNew) {}

void Renderer::createOrderResultsView(bool isNew) {}

void Renderer::createAdminMenuView(bool isNew) {}

void Renderer::createMenuFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    shared_ptr<ButtonNode> enterBtn = make_shared<ButtonNode>(
        "\u23CE", "enter", make_tuple(KEY_ENTER, KEY_ENTER), true);
    // Just a text
    shared_ptr<ButtonNode> upDownBtn =
        make_shared<ButtonNode>("↑/↓", "up/down", make_tuple(0, 0), true);

    enterBtn->subscribe(onEnterBtnClickedMenuSelect);

    toolTipsContainer->appendChild(enterBtn);
    toolTipsContainer->appendChild(upDownBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createMenuItemFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    shared_ptr<ButtonNode> upDownBtn = make_shared<ButtonNode>(
        "-/+", "add/minus", make_tuple(KEY_HYPHEN_MINUS, KEY_PLUS), true);

    upDownBtn->subscribe(onAddMinusBtnClicked);

    toolTipsContainer->appendChild(upDownBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createMenuItemSizesFooter(bool isNew) {}

void Renderer::createMenuItemAddonsFooter(bool isNew) {}

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

void onMenuSelectUpdated(optional<string> selectedMenuItemDataName) {
    if (!selectedMenuItemDataName.has_value()) {
        return;
    }

    State& state = getState();
    Renderer& renderer = getRenderer();

    state.setSelectedMenuItemDataName(selectedMenuItemDataName.value());

    /**
     *
     * TODO:
     *
     * Only update the changed components
     */
    renderer.createView();
    renderer.renderBuffer();
}

void onEnterBtnClickedMenuSelect(unsigned int) {
    Renderer& renderer = getRenderer();
    State& state = getState();

    if (renderer.viewState == RendererState::MENU_ITEM) {
        return;
    }

    optional<MenuItemData> maybeSelectedMenuItemData =
        state.getMenuItemDataWithName(state.getSelectedMenuItemDataName());

    assert(maybeSelectedMenuItemData.has_value());

    MenuItem menuItem(maybeSelectedMenuItemData.value());

    state.appendMenuItemToCart(menuItem);
    state.setSelectedMenuItemInCartUid(menuItem.getUid());

    renderer.viewState = RendererState::MENU_ITEM;

    renderer.createView();
    renderer.renderBuffer();
}

void onShopBtnClicked(unsigned int) {
    Renderer& renderer = getRenderer();

    if (renderer.viewState == RendererState::MENU) {
        return;
    }

    renderer.viewState = RendererState::MENU;

    renderer.createView();
    renderer.renderBuffer();
}

void onEscBtnClickedOnMenuItem(unsigned int) {
    Renderer& renderer = getRenderer();

    if (renderer.viewState != RendererState::MENU_ITEM) {
        return;
    }

    State& state = getState();

    // just remove for now, and not ask
    // for confirmation
    state.removeMenuItemFromCartWithUid(state.getSelectedMenuItemInCartUid());
    state.resetSelectedMenuItemInCardUid();

    renderer.viewState = RendererState::MENU;

    renderer.createView();
    renderer.renderBuffer();
}

void onSizesBtnClicked(unsigned int) {
    Renderer& renderer = getRenderer();
    State& state = getState();

    if (renderer.viewState == RendererState::MENU_ITEM_SIZES) {
        return;
    }

    renderer.viewState = RendererState::MENU_ITEM_SIZES;

    renderer.createView();
    renderer.renderBuffer();
}

void onAddonsBtnClicked(unsigned int) {
    Renderer& renderer = getRenderer();
    State& state = getState();

    if (renderer.viewState == RendererState::MENU_ITEM_ADDONS) {
        return;
    }

    renderer.viewState = RendererState::MENU_ITEM_ADDONS;

    renderer.createView();
    renderer.renderBuffer();
}

void onAddMinusBtnClicked(unsigned int keyCode) {
    Renderer& renderer = getRenderer();
    State& state = getState();

    switch (renderer.viewState) {
        case RendererState::MENU_ITEM: {
            optional<MenuItem*> maybeCurrMenuItem =
                state.getMenuItemWithUid(state.getSelectedMenuItemInCartUid());

            assert(maybeCurrMenuItem.has_value());

            MenuItem* currMenuItem = maybeCurrMenuItem.value();

            switch (keyCode) {
                case KEY_HYPHEN_MINUS:
                    currMenuItem->decreaseQty(1);
                    break;
                case KEY_PLUS:
                    currMenuItem->increaseQty(1);
                    break;
            }
        }; break;
    };

    renderer.createView();
    renderer.renderBuffer();
}

void onAdminBtnClicked(unsigned int) {
    Renderer& renderer = getRenderer();

    if (renderer.viewState == RendererState::ADMIN_MENU) {
        return;
    }

    renderer.viewState = RendererState::ADMIN_MENU;

    renderer.createView();
    renderer.renderBuffer();
}

void onCheckoutBtnClicked(unsigned int) {
    State& state = getState();
    Renderer& renderer = getRenderer();

    renderer.viewState = RendererState::ORDER_CONFIRMATION;

    renderer.createView();
    renderer.renderBuffer();
}
