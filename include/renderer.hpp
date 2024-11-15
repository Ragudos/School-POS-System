#pragma once

#include <algorithm>
#include <ast/node.hpp>
#include <cassert>
#include <constants/metadata.hpp>
#include <contrib/menu.hpp>
#include <contrib/state.hpp>
#include <contrib/utils.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <screen.hpp>
#include <sstream>
#include <utils.hpp>

using namespace terminal;
using namespace std;

void onMenuSelectUpdated(optional<string>);
void onShopBtnClicked();
void onSizesBtnClicked();
void onAddonsBtnClicked();
void onAdminBtnClicked();
void onCheckoutBtnClicked();
void onIncrementBtnClicked();
void onDecrementBtnClicked();

enum RendererState {
    MENU,
    MENU_SIZES,
    MENU_ADDONS,
    ORDER_CONFIRMATION,
    ORDER_RESULTS,
    ADMIN_MENU
};

class Renderer {
   private:
    shared_ptr<ContainerNode> rootNode;
    shared_ptr<ContainerNode> header;
    shared_ptr<ContainerNode> body;
    shared_ptr<ContainerNode> footer;

   public:
    ostringstream buf;
    RendererState viewState;
    Renderer();

   private:
    // header
    void createMenuHeader(bool);
    void createMenuSizesHeader(bool);
    void createMenuAddonsHeader(bool);
    void createOrderConfirmationHeader(bool);
    void createOrderResultsHeader(bool);
    void createAdminMenuHeader(bool);
    // body
    void createMenuView(bool);
    void createMenuSizesView(bool);
    void createMenuAddonsView(bool);
    void createOrderConfirmationView(bool);
    void createOrderResultsView(bool);
    void createAdminMenuView(bool);
    // footer
    void createMenuFooter(bool);
    void createMenuSizesFooter(bool);
    void createMenuAddonsFooter(bool);
    void createOrderConfirmationFooter(bool);
    void createOrderResultsFooter(bool);
    void createAdminMenuFooter(bool);

   public:
    /**
     *
     * Outputs the buffer's contents and
     * reset it.
     */
    void renderBuffer() noexcept;
    void createView();

   public:
    void onKeyPressed(unsigned int);

   private:
    void onKeyPressed(unsigned int, Node::NodePtr);
};

Renderer& getRenderer() noexcept;
void initializeRenderer();
