#ifndef NODE_MODULE
#define NODE_MODULE

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "../utils.hpp"

using namespace std;
using namespace terminal;

enum NodeTypes {
    /**
     *
     * To contain nodes
     */
    CONTAINER,
    /**
     *
     * Used to format its children (color, bold, etc.)
     */
    ACCESSORY,
    /**
     *
     * An interactable node must have a corresponding key code
     * AND cannot have children. It can only have select
     * children specified by the node itself.
     */
    INTERACTABLE,
    /**
     *
     * A leaf cannot have children
     */
    LEAF
};

/**
 *
 * Base node of Abstract-Tree syntax
 * for the frontend
 */
class Node : public enable_shared_from_this<Node> {
   public:
    using NodePtr = shared_ptr<Node>;

   private:
    vector<NodePtr> children;
    NodePtr parent = nullptr;

    size_t width = 0;
    size_t height = 0;
    size_t row = 1;
    size_t col = 1;

   public:
    /** Render to a passed buffer */
    virtual void render(ostringstream *buf) const = 0;
    virtual bool canHaveChildren() const { return true; }
    virtual NodeTypes nodeType() const = 0;

    virtual void appendChild(
        NodePtr child,
        /**
         *
         * Whether the child will inherit this node's inheritable properties
         */
        bool inherit = true) {
        if (canHaveChildren()) {
            child->parent = shared_from_this();

            if (inherit) {
                child->setRow(row);
                child->setCol(col);
            }

            children.push_back(move(child));
        } else {
            throw logic_error("This node cannot have children.");
        }
    }

    virtual void removeChildAt(size_t idx) {
        if (idx < children.size()) {
            children.erase(children.begin() + idx);
        }
    }

    virtual size_t getWidth() const { return width; }
    virtual void setWidth(size_t w) { width = w; }

    virtual size_t getHeight() const { return height; }
    virtual void setHeight(size_t h) { height = h; }

    virtual size_t getRow() const { return row; }
    virtual void setRow(size_t r) { row = r; }

    virtual size_t getCol() const { return col; }
    virtual void setCol(size_t c) { col = c; }

    virtual NodePtr getParent() const { return parent; }

    // Destroy Node for proper cleanup
    virtual ~Node() = default;
};

class TextNode : public Node, public enable_shared_from_this<Node> {
   private:
    string text;

   public:
    TextNode(string text) : text(text) {
        setWidth(text.size());
        setHeight(1);
    }
    TextNode(string text, size_t row) : text(text) {
        setRow(row);
        setWidth(text.size());
        setHeight(1);
    }
    TextNode(string text, size_t row, size_t col) : text(text) {
        setRow(row);
        setCol(col);
        setWidth(text.size());
        setHeight(1);
    }
    TextNode(string text, size_t row, size_t col, size_t width) : text(text) {
        if (width < text.size()) {
            throw logic_error("Invalid width");
        }

        setRow(row);
        setCol(col);
        setWidth(width);
        setHeight(1);
    }
    TextNode(string text, size_t row, size_t col, size_t width, size_t height)
        : text(text) {
        if (width < text.size()) {
            throw logic_error("Invalid width or height");
        }

        setRow(row);
        setCol(col);
        setWidth(width);
        setHeight(height);
    }

    void render(ostringstream *buf) const override {
        moveCursorTo(buf, getRow(), getCol());

        *buf << text;

        size_t len = text.size();
        size_t currWidth = getWidth();
        size_t currHeight = getHeight();

        if (len < currWidth) {
            *buf << string(currWidth - len, ' ');
        }

        size_t currLines = 1;

        while (currLines < currHeight) {
            *buf << endl << string(len, ' ');
            ++currLines;
        }
    }

    bool canHaveChildren() const override { return false; }
    NodeTypes nodeType() const override { return NodeTypes::LEAF; }
};

class LineBreakNode : public Node, public enable_shared_from_this<Node> {
   public:
    LineBreakNode(size_t row) { setRow(row); }
    LineBreakNode(size_t row, size_t col) {
        setRow(row);
        setCol(col);
    }

    void render(ostringstream *buf) const override {
        moveCursorTo(buf, getRow(), getCol());

        *buf << endl;
    }

    bool canHaveChildren() const override { return false; }
    NodeTypes nodeType() const override { return NodeTypes::LEAF; }
};

#endif
