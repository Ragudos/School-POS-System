#include <ast/node.hpp>
#include <cassert>
#include <cmath>
#include <memory>
#include <optional>
#include <utils.hpp>

using namespace std;

Node::Node() : posX(0), posY(0), width(0), height(0){};

Node::Node(unsigned int col) : posX(col), posY(0), width(0), height(0){};

Node::Node(unsigned int col, unsigned int row)
    : posX(col), posY(row), width(0), height(0) {}

Node::~Node() = default;

NodeRenderStyle Node::nodeRenderStyle() const noexcept {
    return NodeRenderStyle::BLOCK;
}

bool Node::canHaveChildren() const noexcept { return true; }

unsigned int Node::getWidth() const noexcept { return width; }

void Node::setWidth(unsigned int w) { width = w; }

unsigned int Node::getHeight() const noexcept { return height; }

void Node::setHeight(unsigned int h) { height = h; }

unsigned int Node::getPosX() const noexcept { return posX; }

void Node::setPosX(unsigned int col) { posX = col; }

unsigned int Node::getPosY() const noexcept { return posY; }

void Node::setPosY(unsigned int row) { posY = row; }

Node::NodePtr Node::getParent() const noexcept { return parent; }

const vector<Node::NodePtr>& Node::getChildren() const noexcept {
    return children;
}

void Node::assertChildIsValid(NodePtr) const {}

void Node::setAsParentOfChild(NodePtr child) {
    child->parent = shared_from_this();
}

void Node::onChildRemoved(size_t idx, NodePtr child) {
    if (children.empty()) {
        setWidth(0);
        setHeight(0);

        return;
    }

    unsigned int heightOfRemovedChild = child->getHeight();
    unsigned int largestWidth = 0;

    for (NodePtr node : children) {
        unsigned nodeRealWidth = node->getPosX() + node->getWidth();

        if (largestWidth < nodeRealWidth) {
            largestWidth = nodeRealWidth;
        }
    }

    for (size_t i = idx, l = children.size(); i < l; ++i) {
        NodePtr node = children.at(i);

        node->setPosX(node->getPosX() - heightOfRemovedChild);
    }

    setWidth(largestWidth);
    setHeight(getHeight() - heightOfRemovedChild);
}

void Node::onChildAppended() {
    assert(!children.empty() ||
           !"Node::onChildAppended() called when children is empty().");

    unsigned int posX = getPosX();
    unsigned int posY = getPosY();
    size_t childrenSize = children.size();
    NodePtr appendedChild = children.at(childrenSize - 1);

    if (childrenSize > 1) {
        NodePtr prevSibling = children.at(childrenSize - 2);

        switch (appendedChild->nodeRenderStyle()) {
            case NodeRenderStyle::BLOCK: {
                appendedChild->setPosY(prevSibling->getPosY() +
                                       prevSibling->getHeight());
            }; break;
            case NodeRenderStyle::INLINE: {
                appendedChild->setPosX(prevSibling->getPosX() +
                                       prevSibling->getWidth());
            }; break;
        }
    } else {
        appendedChild->setPosX(posX);
        appendedChild->setPosY(posY);
    }

    unsigned int appendedChildRealWidth =
        appendedChild->getPosX() + appendedChild->getWidth();

    // TODO: Have this wrap to next line if this exceeds
    // screen size.
    if (getWidth() < appendedChildRealWidth) {
        setWidth(appendedChildRealWidth);
    }

    if (appendedChild->nodeRenderStyle() == NodeRenderStyle::BLOCK) {
        setHeight(getHeight() + appendedChild->getHeight());
    }
}

void Node::appendChild(NodePtr child) {
    assert(canHaveChildren() || !"Node cannot have children");
    assertChildIsValid(child);

    setAsParentOfChild(child);

    children.push_back(move(child));

    onChildAppended();
}

void Node::removeChildAt(size_t idx) {
    assert(!children.empty() ||
           !"Called Node::removeChildAt() with no children.");
    assert(
        idx < children.size() ||
        !"Passed in a size_t idx > children.size() at Node::removeChildAt().");

    NodePtr removedChild = children.at(idx);

    children.erase(children.begin() + idx);

    onChildRemoved(idx, removedChild);
}

void Node::removeAllChildren() noexcept {
    assert(canHaveChildren() ||
           !"Called Node::removeAllChildren() on a node that cannot have "
           "children.");

    children.clear();

    setWidth(0);
    setHeight(0);
}

void InteractableNode::render(ostringstream* buf) const {
    for (NodePtr node : children) {
        node->render(buf);
    }
}

NodeTypes InteractableNode::nodeType() const noexcept {
    return NodeTypes::INTERACTABLE;
}

void ContainerNode::render(ostringstream* buf) const {
    for (NodePtr node : children) {
        node->render(buf);
    }
}

NodeTypes ContainerNode::nodeType() const noexcept {
    return NodeTypes::CONTAINER;
}

GridNode::GridNode() : colGap(4), rowGap(2) {}

NodeTypes LeafNode::nodeType() const noexcept { return NodeTypes::LEAF; }
