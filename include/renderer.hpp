#ifndef RENDERER_MODULE
#define RENDERER_MODULE

#include <algorithm>
#include <iostream>
#include <sstream>

#include "./ast/node.hpp"

using namespace std;

enum RendererState { MENU, ORDER_CONFIRMATION, ORDER_RESULTS };

struct MenuItem {
    string id;
    string description;
    double price;
};

const string TITLE = "Comffee Point-Of-Sale System";
const vector<MenuItem> menuItems = {
    {"caffe-macchiato", "Coffee and caramel", 120},
    {"java-chip", "Sweet and chippy", 120},
    {"decaf", "Coffee taste without a kick", 40},
    {"cold-brew",
     "Shiver me timbers! This will truly wake you up djweqior jiwejif "
     "wjerjwioe jriowejif jweiof jiowejf iweifjiwejf wefi jweiofj "
     "iwoejfiowjefio wjeiof jwiof jiowjeiof jwieofw",
     60},
    {"caffe-americano", "Coffee; The American style", 80},
    {"blonde-roast", "A hot roasted coffee from our pure beans.", 80}};

void onMenuSelectUpdate(const string&);

class Renderer {
   public:
    RendererState viewState;
    shared_ptr<ContainerNode> rootNode;
    // Header
    shared_ptr<ContainerNode> navigationContainer;

    // Body
    shared_ptr<GridNode> body;
    // Menu
    shared_ptr<SelectNode> menuSelectNode;
    shared_ptr<GridNode> menuSelectedItemInfo;

    ostringstream buf;

   public:
    static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }

   private:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

   public:
    void initializeComponents() {
        rootNode = make_shared<ContainerNode>();
        navigationContainer = make_shared<ContainerNode>();
        body = make_shared<GridNode>();
        menuSelectNode = make_shared<SelectNode>();
        menuSelectedItemInfo = make_shared<GridNode>();

        auto title = make_shared<TextNode>(TITLE);

        title->setColor(255, 255, 0);
        title->setBold();

        auto titleLineBreak = make_shared<LineBreakNode>(2);

        rootNode->appendChild(title);
        rootNode->appendChild(titleLineBreak);
        rootNode->appendChild(body);

        menuSelectNode->observer.subscribe(onMenuSelectUpdate);

        body->appendChild(menuSelectNode);

        for (size_t i = 0, l = menuItems.size(); i < l; ++i) {
            auto menuSelectOption =
                make_shared<SelectOptionNode>(menuItems.at(i).id);
            menuSelectNode->appendChild(menuSelectOption);
        }

        body->appendChild(menuSelectedItemInfo);
        onMenuSelectUpdate(menuSelectNode->getValueOfSelectedOption().value());
    }

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

void onMenuSelectUpdate(const string& choiceId) {
    Renderer& renderer = Renderer::getInstance();

    auto item = find_if(
        menuItems.begin(), menuItems.end(),
        [choiceId](const MenuItem& item) { return item.id == choiceId; });

    if (item == menuItems.end()) {
        throw logic_error("There is no menuItem with choice id of " + choiceId);
    }

    if (!renderer.menuSelectedItemInfo->getChildren().empty()) {
        size_t menuSelectedItemInfoHeight =
            renderer.menuSelectedItemInfo->getHeight();

        saveCursorPosition();

        moveCursorTo(22, 22);

        cout << menuSelectedItemInfoHeight;

        restoreSavedCursorPosition();

        moveCursorTo(&renderer.buf, renderer.menuSelectedItemInfo->getCol(),
                     renderer.menuSelectedItemInfo->getRow() +
                         menuSelectedItemInfoHeight);
        clearLinesFromCursorToEndOfLine(&renderer.buf,
                                        menuSelectedItemInfoHeight);
    }

    renderer.menuSelectedItemInfo->emptyChildren();

    auto desc = make_shared<TextNode>(item->description);

    renderer.menuSelectedItemInfo->appendChild(desc);
    renderer.menuSelectedItemInfo->render(&renderer.buf);
    renderer.render();
}

#endif