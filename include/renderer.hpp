#pragma once

#include <algorithm>
#include <ast/node.hpp>
#include <cassert>
#include <constants/metadata.hpp>
#include <contrib.hpp>
#include <iostream>
#include <memory>
#include <screen.hpp>
#include <sstream>
#include <state.hpp>
#include <utils.hpp>

using namespace terminal;
using namespace std;

enum RendererState { MENU, ORDER_CONFIRMATION, ORDER_RESULTS };

class Renderer {
   private:
    RendererState viewState;

    ostringstream buf;

    shared_ptr<ContainerNode> rootNode;
    shared_ptr<ContainerNode> header;
    shared_ptr<ContainerNode> body;

   public:
    Renderer();

   private:
    void createMenuView();
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
};

Renderer& getRenderer() noexcept;
void initializeRenderer();
