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

    auto titleBr = make_shared<LineBreakNode>(2);

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
        case RendererState::MENU_ITEM_CONFIRM: {
            createMenuItemConfirmHeader(isNew);
            createMenuItemConfirmView(isNew);
            createMenuItemConfirmFooter(isNew);
        }; break;
        case RendererState::MENU_ITEM_SIZES: {
            createMenuItemSizesHeader(isNew);
            createMenuItemSizesView(isNew);
            createMenuItemSizesFooter(isNew);
        }; break;
        case RendererState::MENU_ITEM_ADDONS: {
            createMenuItemAddonsHeader(isNew);
            createMenuItemAddonsView(isNew);
            createMenuItemAddonsFooter(isNew);
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
        make_shared<ButtonNode>("←(esc)", "BACK", make_tuple(KEY_ESC, KEY_ESC));
    shared_ptr<ButtonNode> sizesBtn =
        make_shared<ButtonNode>("s", "SIZES", make_tuple(KEY_S, KEY_s));
    shared_ptr<ButtonNode> addonsBtn =
        make_shared<ButtonNode>("a", "ADD-ONS", make_tuple(KEY_A, KEY_a));

    backBtn->subscribe(onEscBtnClickedOnMenuItem);
    sizesBtn->subscribe(onSizesBtnClicked);
    addonsBtn->subscribe(onAddonsBtnClicked);

    navHeader->appendChild(backBtn);
    navHeader->appendChild(sizesBtn);
    navHeader->appendChild(addonsBtn);

    header->appendChild(navHeader);
}

void Renderer::createMenuItemConfirmHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0);

    navHeader->setIsFlexible(false);
    navHeader->setColGap(2);
    navHeader->setRowGap(1);

    shared_ptr<ButtonNode> backBtn =
        make_shared<ButtonNode>("←(esc)", "BACK", make_tuple(KEY_ESC, KEY_ESC));

    backBtn->subscribe(onEscBtnClickedOnMenuItem);

    navHeader->appendChild(backBtn);

    header->appendChild(navHeader);
}

void Renderer::createMenuItemSizesHeader(bool isNew) {
	Screen& screen = getScreen();
	State& state = getState();
	shared_ptr<GridNode> navHeader = make_shared<GridNode>(screen.getWidth(), 0);

    navHeader->setIsFlexible(false);
    navHeader->setColGap(2);
    navHeader->setRowGap(1);

    shared_ptr<ButtonNode> backBtn =
        make_shared<ButtonNode>("←(esc)", "BACK", make_tuple(KEY_ESC, KEY_ESC));
    shared_ptr<ButtonNode> sizesBtn =
        make_shared<ButtonNode>("s", "SIZES", make_tuple(KEY_S, KEY_s), true);
    shared_ptr<ButtonNode> addonsBtn =
        make_shared<ButtonNode>("a", "ADD-ONS", make_tuple(KEY_A, KEY_a));

    backBtn->subscribe(onEscBtnClickedOnMenuItem);
    addonsBtn->subscribe(onAddonsBtnClicked);

    navHeader->appendChild(backBtn);
    navHeader->appendChild(sizesBtn);
    navHeader->appendChild(addonsBtn);

    header->appendChild(navHeader);
}

void Renderer::createMenuItemAddonsHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0);

    navHeader->setIsFlexible(false);
    navHeader->setColGap(2);
    navHeader->setRowGap(1);

    shared_ptr<ButtonNode> backBtn =
        make_shared<ButtonNode>("←(esc)", "BACK", make_tuple(KEY_ESC, KEY_ESC));
    shared_ptr<ButtonNode> sizesBtn =
        make_shared<ButtonNode>("s", "SIZES", make_tuple(KEY_S, KEY_s));
    shared_ptr<ButtonNode> addonsBtn =
        make_shared<ButtonNode>("a", "ADD-ONS", make_tuple(KEY_A, KEY_a), true);

    backBtn->subscribe(onEscBtnClickedOnMenuItem);
    sizesBtn->subscribe(onSizesBtnClicked);

    navHeader->appendChild(backBtn);
    navHeader->appendChild(sizesBtn);
    navHeader->appendChild(addonsBtn);

    header->appendChild(navHeader);
}

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

void Renderer::createOrderResultsHeader(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    optional<Order> maybeOrder = state.getOrderInfo();

    assert(maybeOrder.has_value());

    Order order = maybeOrder.value();

    shared_ptr<GridNode> navHeader =
        make_shared<GridNode>(screen.getWidth(), 0, 2);

    navHeader->setIsFlexible(false);

    shared_ptr<TextNode> thankYouText = make_shared<TextNode>(
        "The order has been confirmed. Its ID is " + order.getOrderUid());

    navHeader->appendChild(thankYouText);

    header->appendChild(navHeader);
}

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
    shared_ptr<TextNode> itemPrice = make_shared<TextNode>(
        "Base price: ₱" + formatNumber(item.getBasePrice()));

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

    // TODO: Add add-ons to subtotal
    shared_ptr<TextNode> menuItemPrice = make_shared<TextNode>(
        "Base price: ₱" + formatNumber(currMenuItem->getBasePrice()) +
        ", Subtotal: ₱" + formatNumber(currMenuItem->calculateSubtotal()));

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

void Renderer::createMenuItemConfirmView(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();

    optional<MenuItem*> maybeMenuItem =
        state.getMenuItemWithUid(state.getSelectedMenuItemInCartUid());

    assert(maybeMenuItem.has_value());

    MenuItem* menuItem = maybeMenuItem.value();

    shared_ptr<GridNode> textContainer =
        make_shared<GridNode>(screen.getWidth(), screen.getWidth());

    textContainer->setRowGap(0);
    textContainer->setIsFlexible(false);

    shared_ptr<TextNode> menuItemName = make_shared<TextNode>(
        menuItem->getName() + " will be added to your cart.");
    shared_ptr<LineBreakNode> br = make_shared<LineBreakNode>(1);
    shared_ptr<TextNode> sizeText =
        make_shared<TextNode>("Size: " + toString(menuItem->getSize()));
    shared_ptr<TextNode> qtyText =
        make_shared<TextNode>("Quantity: " + formatNumber(menuItem->getQty()));
    shared_ptr<TextNode> subtotalText = make_shared<TextNode>(
        "Subtotal: ₱" + formatNumber(menuItem->calculateSubtotal()));
    shared_ptr<LineBreakNode> secondBr = make_shared<LineBreakNode>(2);
    shared_ptr<TextNode> confirmationText =
        make_shared<TextNode>("Press enter to confirm...");

    textContainer->appendChild(menuItemName);
    textContainer->appendChild(br);
    textContainer->appendChild(sizeText);
    textContainer->appendChild(qtyText);
    textContainer->appendChild(subtotalText);
    textContainer->appendChild(secondBr);
    textContainer->appendChild(confirmationText);

    body->appendChild(textContainer);
}

void Renderer::createMenuItemSizesView(bool isNew) {
	State& state = getState();
    shared_ptr<GridNode> menuGrid = make_shared<GridNode>();
    shared_ptr<SelectNode> menuSelect = make_shared<SelectNode>();

    menuGrid->setIsFlexible(true);

    for (const auto item : state.getMenuItemSizesData()) {
        shared_ptr<SelectOptionNode> optionNode =
            make_shared<SelectOptionNode>(toString(item.getSize()));

        menuSelect->appendChild(optionNode);
    }

    if (!state.getSelectedMenuItemSizeName().empty()) {
        menuSelect->setActiveChildWithValue(
            state.getSelectedMenuItemSizeName());
    } else {
        state.setSelectedMenuItemSizeName(
            toString(state.getMenuItemSizesData()
                         .at(menuSelect->getActiveOptionIdx())
                         .getSize()));
    }

    optional<MenuItemSizeData> maybeItem = state.getSelectedMenuItemSizeName(
        menuSelect->getValueOfSelectedOption().value());

    assert(
        maybeItem != nullopt);

    shared_ptr<ContainerNode> activeMenuItemNode = make_shared<ContainerNode>();

    menuSelect->subscribe(onMenuSelectUpdated);

    const MenuItemSizeData item = maybeItem.value();

    shared_ptr<GridNode> itemDisplay = make_shared<GridNode>();
    shared_ptr<TextNode> itemDescription =
        make_shared<TextNode>(item.getDescription());
    shared_ptr<TextNode> itemPrice = make_shared<TextNode>(
        "Additional price: ₱" + formatNumber(item.getAdditionalPrice()));

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

void Renderer::createMenuItemAddonsView(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();

    shared_ptr<GridNode> addonMetadataContainer =
        make_shared<GridNode>(screen.getWidth(), screen.getWidth());

    addonMetadataContainer->setRowGap(1);
    addonMetadataContainer->setIsFlexible(false);

    const auto& addons = state.getMenuItemAddonData();
    if (addons.empty()) {
        shared_ptr<TextNode> noAddons =
            make_shared<TextNode>("No add-ons available.");
        addonMetadataContainer->appendChild(noAddons);
    } else {
        shared_ptr<TextNode> title = make_shared<TextNode>("Available Add-ons:");
        addonMetadataContainer->appendChild(title);

        for (const auto& addon : addons) {
            shared_ptr<TextNode> addonName =
                make_shared<TextNode>(addon.getName() + ":");
            shared_ptr<TextNode> addonPrice =
                make_shared<TextNode>("- Additional Price: ₱" + formatNumber(addon.getPrice()));

            addonMetadataContainer->appendChild(addonName);
            addonMetadataContainer->appendChild(addonPrice);
        }
    }

    auto br = make_shared<LineBreakNode>(2);
    body->appendChild(addonMetadataContainer);
    body->appendChild(br);
}

void Renderer::createOrderConfirmationView(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();
    shared_ptr<GridNode> container =
        make_shared<GridNode>(screen.getWidth(), screen.getWidth());
    const vector<MenuItem> cartItems = state.getMenuItemsInCart();

    container->setIsFlexible(false);
    container->setRowGap(2);

    if (cartItems.empty()) {
        shared_ptr<TextNode> emptyText =
            make_shared<TextNode>("There are no items in cart yet :(.");

        container->appendChild(emptyText);
    } else {
        shared_ptr<GridNode> cartGrid =
            make_shared<GridNode>(screen.getWidth(), screen.getWidth() / 7);

        cartGrid->setColGap(1);
        cartGrid->setRowGap(1);

        shared_ptr<TextNode> title =
            make_shared<TextNode>("The ordered items are:");

        container->appendChild(title);

        double total = 0;

        for (auto& item : cartItems) {
            double subTotal = item.calculateSubtotal();
            total += subTotal;

            shared_ptr<GridNode> cartItemGrid = make_shared<GridNode>(
                screen.getWidth() / 7, screen.getWidth() / 7);

            cartItemGrid->setRowGap(0);
            cartItemGrid->setIsFlexible(false);

            shared_ptr<TextNode> itemName =
                make_shared<TextNode>(item.getName() + " " + item.getUid());
            shared_ptr<LineBreakNode> br = make_shared<LineBreakNode>(1);
            shared_ptr<TextNode> itemQty =
                make_shared<TextNode>("Quantity: " + to_string(item.getQty()));
            shared_ptr<TextNode> itemSize =
                make_shared<TextNode>("Size: " + toString(item.getSize()));
            shared_ptr<TextNode> itemPrice =
                make_shared<TextNode>("Subtotal: ₱" + formatNumber(subTotal));

            cartItemGrid->appendChild(itemName);
            cartItemGrid->appendChild(br);
            cartItemGrid->appendChild(itemQty);
            cartItemGrid->appendChild(itemSize);
            cartItemGrid->appendChild(itemPrice);

            cartGrid->appendChild(cartItemGrid);
        }

        container->appendChild(cartGrid);

        shared_ptr<TextNode> subMetadata =
            make_shared<TextNode>("Total: ₱" + formatNumber(total));

        container->appendChild(subMetadata);
    }

    body->appendChild(container);
}

void Renderer::createOrderResultsView(bool isNew) {
    Screen& screen = getScreen();
    State& state = getState();

    optional<Order> maybeOrder = state.getOrderInfo();

    assert(maybeOrder.has_value());

    Order order = maybeOrder.value();

    shared_ptr<GridNode> viewContainer =
        make_shared<GridNode>(screen.getWidth(), screen.getWidth() / 7);

    viewContainer->setIsFlexible(false);

    for (auto& item : order.getItems()) {
        shared_ptr<GridNode> itemCard = make_shared<GridNode>(
            screen.getWidth() / 7, screen.getWidth() / 7, 1, 0);

        itemCard->setIsFlexible(false);

        shared_ptr<TextNode> itemName =
            make_shared<TextNode>(item.getName() + " " + item.getUid());
        shared_ptr<TextNode> itemSubtotal =
            make_shared<TextNode>("₱" + formatNumber(item.calculateSubtotal()));

        itemCard->appendChild(itemName);
        itemCard->appendChild(itemSubtotal);

        viewContainer->appendChild(itemCard);
    }

    shared_ptr<LineBreakNode> br = make_shared<LineBreakNode>(1);
    shared_ptr<GridNode> orderMetadata =
        make_shared<GridNode>(screen.getWidth(), 0, 2, 1);

    shared_ptr<TextNode> vatNode =
        make_shared<TextNode>("VAT: " + formatNumber(order.getVAT()));
    shared_ptr<TextNode> totalPriceNode =
        make_shared<TextNode>("Total: " + formatNumber(order.getTotalPrice()));
    shared_ptr<TextNode> orderStateNode = make_shared<TextNode>(
        "Status: " + orderStateToString(order.getOrderState()));

    orderMetadata->setIsFlexible(false);

    orderMetadata->appendChild(vatNode);
    orderMetadata->appendChild(totalPriceNode);
    orderMetadata->appendChild(orderStateNode);

    body->appendChild(viewContainer);
    body->appendChild(br);
    body->appendChild(orderMetadata);
}

void Renderer::createAdminMenuView(bool isNew) {
    State& state = getState();
    shared_ptr<GridNode> menuGrid = make_shared<GridNode>();
    shared_ptr<SelectNode> menuSelect = make_shared<SelectNode>();

    menuGrid->setIsFlexible(true);

    for (const auto& option : state.getAdminMenuOptions()) {
        shared_ptr<SelectOptionNode> optionNode =
            make_shared<SelectOptionNode>(option.getName());

        menuSelect->appendChild(optionNode);
    }

    if (!state.getSelectedAdminMenuOptionName().empty()) {
        menuSelect->setActiveChildWithValue(
            state.getSelectedAdminMenuOptionName());
    } else {
        state.setSelectedAdminMenuOptionName(
            state.getAdminMenuOptions()
                .at(menuSelect->getActiveOptionIdx())
                .getName());
    }

    optional<AdminMenuOption> maybeItem = state.getSelectedAdminMenuOptionName(
        menuSelect->getValueOfSelectedOption().value());

    assert(
        maybeItem != nullopt ||
        !"Received nothing from State::getMenuItemWithId() where it shouldn't!");

    shared_ptr<ContainerNode> activeMenuItemNode = make_shared<ContainerNode>();

    menuSelect->subscribe(onMenuSelectUpdated);

    const AdminMenuOption item = maybeItem.value();

    shared_ptr<TextNode> itemDescription =
        make_shared<TextNode>(item.getDescription());

    /**
     * TODO: fix bug when order of appending is
     * reversed below.
     */
    menuGrid->appendChild(menuSelect);
    menuGrid->appendChild(itemDescription);

    body->appendChild(menuGrid);
}

void Renderer::createMenuFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    shared_ptr<ButtonNode> enterBtn = make_shared<ButtonNode>(
        "\u23CE", "enter", make_tuple(KEY_ENTER, KEY_ENTER_LINUX), true);
    // Just a text
    shared_ptr<ButtonNode> upDownBtn =
        make_shared<ButtonNode>("↑/↓", "up/down", make_tuple(0, 0), true);
    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    enterBtn->subscribe(onEnterBtnClickedMenuSelect);

    toolTipsContainer->appendChild(enterBtn);
    toolTipsContainer->appendChild(upDownBtn);
    toolTipsContainer->appendChild(quitBtn);

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

    shared_ptr<ButtonNode> enterBtn = make_shared<ButtonNode>(
        "\u23CE", "enter", make_tuple(KEY_ENTER, KEY_ENTER_LINUX), true);
    shared_ptr<ButtonNode> upDownBtn = make_shared<ButtonNode>(
        "-/+", "add/minus", make_tuple(KEY_HYPHEN_MINUS, KEY_PLUS), true);
    // just a text
    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    enterBtn->subscribe(onEnterBtnClickedMenuSelect);
    upDownBtn->subscribe(onAddMinusBtnClicked);

    toolTipsContainer->appendChild(enterBtn);
    toolTipsContainer->appendChild(upDownBtn);
    toolTipsContainer->appendChild(quitBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createMenuItemConfirmFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    shared_ptr<ButtonNode> enterBtn =
        make_shared<ButtonNode>("\u23CE(enter)", "confirm",
                                make_tuple(KEY_ENTER, KEY_ENTER_LINUX), true);
    // Just a text
    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    enterBtn->subscribe(onEnterBtnClickedMenuSelect);

    toolTipsContainer->appendChild(enterBtn);
    toolTipsContainer->appendChild(quitBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createMenuItemSizesFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    shared_ptr<ButtonNode> enterBtn = make_shared<ButtonNode>(
        "\u23CE", "enter", make_tuple(KEY_ENTER, KEY_ENTER_LINUX), true);
    // Just a text
    shared_ptr<ButtonNode> upDownBtn =
        make_shared<ButtonNode>("↑/↓", "up/down", make_tuple(0, 0), true);
    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    enterBtn->subscribe(onEnterBtnClickedMenuSelect);

    toolTipsContainer->appendChild(enterBtn);
    toolTipsContainer->appendChild(upDownBtn);
    toolTipsContainer->appendChild(quitBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createMenuItemAddonsFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    // Just a text
    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    toolTipsContainer->appendChild(quitBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createOrderConfirmationFooter(bool isNew) {
    State& state = getState();

    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    if (!state.getMenuItemsInCart().empty()) {
        shared_ptr<ButtonNode> backBtn = make_shared<ButtonNode>(
            "←(esc)", "clear", make_tuple(KEY_ESC, KEY_ESC), true);
        shared_ptr<ButtonNode> enterBtn = make_shared<ButtonNode>(
            "\u23CE(enter)", "confirm", make_tuple(KEY_ENTER, KEY_ENTER_LINUX),
            true);

        backBtn->subscribe(onEscBtnClickedOnMenuItem);
        enterBtn->subscribe(onEnterBtnClickedMenuSelect);

        toolTipsContainer->appendChild(backBtn);
        toolTipsContainer->appendChild(enterBtn);
    }

    // Just a text
    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    toolTipsContainer->appendChild(quitBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createOrderResultsFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    shared_ptr<ButtonNode> backBtn = make_shared<ButtonNode>(
        "←(esc)", "BACK", make_tuple(KEY_ESC, KEY_ESC), true);
    // Just a text
    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    backBtn->subscribe(onEscBtnClickedOnMenuItem);

    toolTipsContainer->appendChild(backBtn);
    toolTipsContainer->appendChild(quitBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

void Renderer::createAdminMenuFooter(bool isNew) {
    shared_ptr<GridNode> toolTipsContainer = make_shared<GridNode>();

    toolTipsContainer->setColGap(2);
    toolTipsContainer->setRowGap(1);

    shared_ptr<ButtonNode> quitBtn =
        make_shared<ButtonNode>("q", "quit", make_tuple(0, 0), true);

    toolTipsContainer->appendChild(quitBtn);

    shared_ptr<TextNode> lineSeparatorUp =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));
    shared_ptr<TextNode> lineSeparatorBottom =
        make_shared<TextNode>(string(toolTipsContainer->getWidth(), '-'));

    footer->appendChild(lineSeparatorUp);
    footer->appendChild(toolTipsContainer);
    footer->appendChild(lineSeparatorBottom);
}

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

void onMenuSelectUpdated(optional<string> selectedMenuSelectName) {
    if (!selectedMenuSelectName.has_value()) {
        return;
    }

    State& state = getState();
    Renderer& renderer = getRenderer();

    switch (renderer.viewState) {
        case RendererState::MENU: {
            state.setSelectedMenuItemDataName(selectedMenuSelectName.value());
        }; break;
        case RendererState::MENU_ITEM_SIZES: {
            state.setSelectedMenuItemSizeName(selectedMenuSelectName.value());
        }; break;
        case RendererState::ADMIN_MENU: {
            state.setSelectedAdminMenuOptionName(
                selectedMenuSelectName.value());
        }; break;
    }

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

    switch (renderer.viewState) {
        case RendererState::MENU: {
            optional<MenuItemData> maybeSelectedMenuItemData =
                state.getMenuItemDataWithName(
                    state.getSelectedMenuItemDataName());

            assert(maybeSelectedMenuItemData.has_value());

            MenuItem menuItem(maybeSelectedMenuItemData.value());

            state.appendMenuItemToCart(menuItem);
            state.setSelectedMenuItemInCartUid(menuItem.getUid());

            renderer.viewState = RendererState::MENU_ITEM;
        }; break;
        case RendererState::MENU_ITEM: {
            renderer.viewState = RendererState::MENU_ITEM_CONFIRM;
        }; break;
        case RendererState::MENU_ITEM_CONFIRM: {
            // TODO
            renderer.viewState = RendererState::MENU;
        }; break;
        case RendererState::MENU_ITEM_SIZES: {
            optional<MenuItemSizeData> maybeSelectedMenuItemSizeData =
                state.getSelectedMenuItemSizeName(
                    state.getSelectedMenuItemSizeName());

            assert(maybeSelectedMenuItemSizeData.has_value());

            MenuItemSizeData selectedMenuItemSizeData =
                maybeSelectedMenuItemSizeData.value();

            optional<MenuItem*> maybeSelectedMenuItemInCart =
                state.getMenuItemWithUid(state.getSelectedMenuItemInCartUid());

            assert(maybeSelectedMenuItemInCart.has_value());

            MenuItem* selectedMenuItemInCart =
                maybeSelectedMenuItemInCart.value();

            selectedMenuItemInCart->setSize(selectedMenuItemSizeData.getSize());

            renderer.viewState = RendererState::MENU_ITEM;
        }; break;
        case RendererState::ORDER_CONFIRMATION: {
            Order order(state.getMenuItemsInCart());

            saveOrder(order);
            state.setOrderInfo(order);

            renderer.viewState = ORDER_RESULTS;
        }; break;
    }

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
    State& state = getState();

    switch (renderer.viewState) {
        case RendererState::MENU_ITEM: {
            // just remove for now, and not ask
            // for confirmation
            state.removeMenuItemFromCartWithUid(
                state.getSelectedMenuItemInCartUid());
            state.resetSelectedMenuItemInCartUid();
            state.resetSelectedMenuItemSizeName();

            renderer.viewState = RendererState::MENU;

            renderer.createView();
            renderer.renderBuffer();
        }; break;
        case RendererState::MENU_ITEM_CONFIRM:
        case RendererState::MENU_ITEM_SIZES:
        case RendererState::MENU_ITEM_ADDONS: {
            renderer.viewState = RendererState::MENU_ITEM;

            renderer.createView();
            renderer.renderBuffer();
        }; break;
        case RendererState::ORDER_CONFIRMATION: {
            if (state.getMenuItemsInCart().empty()) {
                return;
            }

            state.clearMenuItemsInCart();

            renderer.createView();
            renderer.renderBuffer();
        }; break;
        case RendererState::ORDER_RESULTS: {
            state.clearMenuItemsInCart();
            state.resetOrderInfo();

            renderer.viewState = RendererState::MENU;

            renderer.createView();
            renderer.renderBuffer();
        }; break;
    }
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
