#pragma once

#include <algorithm>
#include <ast/node.hpp>
#include <cassert>
#include <constants/metadata.hpp>
#include <contrib.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <screen.hpp>
#include <sstream>
#include <state.hpp>
#include <utils.hpp>

using namespace terminal;
using namespace std;

void onMenuSelectUpdated(optional<string>);

enum RendererState { MENU, ORDER_CONFIRMATION, ORDER_RESULTS };

class Renderer {
   private:
    RendererState viewState;

    shared_ptr<ContainerNode> rootNode;
    shared_ptr<ContainerNode> header;
    shared_ptr<ContainerNode> body;

   public:
    ostringstream buf;
    Renderer();

   private:
    void createMenuView(bool);
    void createOrderConfirmationView();
    void createOrderResultsView();

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
