#pragma once

#include <algorithm>
#include <ast/node.hpp>
#include <cassert>
#include <constants/metadata.hpp>
#include <contrib/menu.hpp>
#include <contrib/state.hpp>
#include <contrib/storage.hpp>
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
void onEnterBtnClickedMenuSelect(unsigned int);

void onShopBtnClicked(unsigned int);

void onEscBtnClickedOnMenuItem(unsigned int);
void onSizesBtnClicked(unsigned int);
void onAddonsBtnClicked(unsigned int);

void onEditBtnClicked(unsigned int);

void onAddMinusBtnClicked(unsigned int);

void onAdminBtnClicked(unsigned int);
void onCheckoutBtnClicked(unsigned int);

enum RendererState {
    MENU,
    MENU_ITEM,
    MENU_ITEM_CONFIRM,
    MENU_ITEM_SIZES,
    MENU_ITEM_ADDONS,
    ORDER_CONFIRMATION,
    ORDER_RESULTS,
    ADMIN_MENU,
    ADMIN_MENU_REVIEW_ORDER_INPUT_READONLY,
    ADMIN_MENU_REVIEW_ORDER_INPUT_EDIT,
    ADMIN_MENU_DAILY_SALES,
    ADMIN_MENU_MONTHLY_SALES,
    ADMIN_MENU_YEARLY_SALES,
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
    void createMenuItemHeader(bool);
    void createMenuItemConfirmHeader(bool);
    void createMenuItemSizesHeader(bool);
    void createMenuItemAddonsHeader(bool);
    void createOrderConfirmationHeader(bool);
    void createOrderResultsHeader(bool);
    void createAdminMenuHeader(bool);
    void createAdminMenuReviewOrderInputReadonlyHeader(bool);
    void createAdminMenuReviewOrderInputEditHeader(bool);
    void createAdminMenuDailySalesHeader(bool);
    void createAdminMenuMonthlySalesHeader(bool);
    void createAdminMenuYearlySalesHeader(bool);
    // body
    void createMenuView(bool);
    void createMenuItemView(bool);
    void createMenuItemConfirmView(bool);
    void createMenuItemSizesView(bool);
    void createMenuItemAddonsView(bool);
    void createOrderConfirmationView(bool);
    void createOrderResultsView(bool);
    void createAdminMenuView(bool);
    void createAdminMenuReviewOrderInputReadonlyView(bool);
    void createAdminMenuReviewOrderInputEditView(bool);
    void createAdminMenuDailySalesView(bool);
    void createAdminMenuMonthlySalesView(bool);
    void createAdminMenuYearlySalesView(bool);
    // footer
    void createMenuFooter(bool);
    void createMenuItemFooter(bool);
    void createMenuItemConfirmFooter(bool);
    void createMenuItemSizesFooter(bool);
    void createMenuItemAddonsFooter(bool);
    void createOrderConfirmationFooter(bool);
    void createOrderResultsFooter(bool);
    void createAdminMenuFooter(bool);
    void createAdminMenuReviewOrderInputReadonlyFooter(bool);
    void createAdminMenuReviewOrderInputEditFooter(bool);
    void createAdminMenuDailySalesFooter(bool);
    void createAdminMenuMonthlySalesFooter(bool);
    void createAdminMenuYearlySalesFooter(bool);

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
