#ifndef RENDERER_MODULE
#define RENDERER_MODULE

#include <iostream>
#include <sstream>

#include "./ast/node.hpp"

using namespace std;

enum RendererState { MENU, ORDER_CONFIRMATION, ORDER_RESULTS };

const string TITLE = "Comffee Point-Of-Sale System";
const string menuItems[6] = {"caffe-macchiato", "java-chip",
                             "decaf",           "cold-brew",
                             "caffe-americano", "blonde-roast"};

class Renderer {
   public:
    RendererState viewState;
    shared_ptr<ContainerNode> rootNode;
    shared_ptr<GridNode> bodyMenu;
    shared_ptr<SelectNode> menuSelectNode;
    shared_ptr<ContainerNode> navigationContainer;
    ostringstream buf;

   private:
    Renderer()
        : rootNode(make_shared<ContainerNode>()),
          bodyMenu(make_shared<GridNode>()),
          menuSelectNode(make_shared<SelectNode>()),
          navigationContainer(make_shared<ContainerNode>()) {
        auto title = make_shared<TextNode>(TITLE);

        title->setColor(255, 255, 0);
        title->setBold();

        auto titleLineBreak = make_shared<LineBreakNode>(2);

        rootNode->appendChild(title);
        rootNode->appendChild(titleLineBreak);
        rootNode->appendChild(bodyMenu);

        bodyMenu->appendChild(menuSelectNode);

        for (int i = 0; i < 6; ++i) {
            auto menuSelectOption = make_shared<SelectOptionNode>(menuItems[i]);
            menuSelectNode->appendChild(menuSelectOption);
        }
    }
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

   public:
    static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }

   public:
    void render() {
        cout << buf.str();
        buf.str("");
        buf.clear();
    }

    void onKeyPressed(int keyCode) {
        const vector<Node::NodePtr>& children = rootNode->getChildren();

        for (size_t i = 0, l = children.size(); i < l; ++i) {
            Node::NodePtr node = children.at(i);

            if (node->nodeType() == NodeTypes::INTERACTABLE) {
                shared_ptr<InteractableNode> castNode =
                    static_pointer_cast<InteractableNode>(node);
                if (castNode->onKeyPressed(keyCode)) {
                    castNode->render(&buf);
                }
            }

            onKeyPressed(keyCode, node);
        }
    }

    void onKeyPressed(int keyCode, Node::NodePtr currNode) {
        if (!currNode->canHaveChildren()) {
            return;
        }

        const vector<Node::NodePtr>& children = currNode->getChildren();

        for (size_t i = 0, l = children.size(); i < l; ++i) {
            Node::NodePtr node = children.at(i);

            if (node->nodeType() == NodeTypes::INTERACTABLE) {
                shared_ptr<InteractableNode> castNode =
                    static_pointer_cast<InteractableNode>(node);
                if (castNode->onKeyPressed(keyCode)) {
                    castNode->render(&buf);
                }
            }

            onKeyPressed(keyCode, node);
        }
    }
};

#endif