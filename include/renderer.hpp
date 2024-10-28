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
    /**
     *
     * Outputs the buffer's contents and
     * reset it.
     */
    void renderBuffer() noexcept;
    void createView();
};

Renderer& getRenderer();
void initializeRenderer();
