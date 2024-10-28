#pragma once

#include <ast/node.hpp>
#include <constants/metadata.hpp>
#include <memory>
#include <sstream>


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

   public:
    /**
     *
     * Outputs the buffer's contents and
     * reset it.
     */
    void renderBuffer() noexcept;

   public:
    shared_ptr<ContainerNode> getRootNode();
    shared_ptr<ContainerNode> getHeaderNode();
    shared_ptr<ContainerNode> getBodyNode();
};

Renderer& getRenderer();
void initializeRenderer();
