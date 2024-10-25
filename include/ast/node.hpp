#ifndef NODE_MODULE
#define NODE_MODULE

#include <math.h>

#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <vector>

#include "../keyboard.hpp"
#include "../observer.hpp"
#include "../screen.hpp"
#include "../utils.hpp"

using namespace std;
using namespace terminal;
using namespace miscellaneous;

enum NodeTypes {
    /**
     *
     * To contain nodes
     */
    CONTAINER,
    /**
     *
     * An interactable node must have a corresponding key code.
     * It can only have select children specified by the node itself.
     */
    INTERACTABLE,
    /**
     *
     * A leaf cannot have children
     */
    LEAF
};

enum NodeRenderStyle {
    /**
     *
     * Renders in the next line of previous node.
     */
    BLOCK,
    /**
     *
     * Renders in the same line as the previous node.
     */
    INLINE
};

enum TextNodeFormats { BOLD, ITALIC, UNDERLINE, STRIKETHROUGH, DIM };

/**
 *
 * Base node of Abstract-Tree syntax
 * for the frontend
 */
class Node : public enable_shared_from_this<Node> {
   public:
    using NodePtr = shared_ptr<Node>;

   protected:
    vector<NodePtr> children;
    size_t width = 0;
    size_t height = 0;

   private:
    NodePtr parent = nullptr;
    size_t row = 0;
    size_t col = 0;

   protected:
    virtual void adjustDimensionsOnBeforeAppendChild(NodePtr child) {
        // Default behavior:

        // Adjusts width and height based on child with largest width
        // and latest child's height (to be inserted).
        // Basically depends on child to define its dimensions
        // unlike a grid container.
        size_t childRow = getRow();
        size_t childCol = getCol();

        if (!children.empty()) {
            NodePtr latestNode = children.at(children.size() - 1);

            switch (child->nodeRenderStyle()) {
                case NodeRenderStyle::BLOCK: {
                    childRow = latestNode->getRow() + latestNode->getHeight();
                }; break;
                case NodeRenderStyle::INLINE: {
                    childCol = latestNode->getCol() + latestNode->getWidth();
                }; break;
            }
        }

        size_t childCurrWidth = child->getWidth();

        if (getWidth() < childCol + childCurrWidth) {
            setWidth(childCol + childCurrWidth);
        }

        child->setRow(childRow);
        child->setCol(childCol);

        setHeight(getHeight() + child->getHeight());
    }

    virtual void adjustDimensionsOnRemoveChild(size_t idx, NodePtr child) {
        if (children.empty()) {
            setHeight(0);
            setWidth(0);

            return;
        }

        size_t heightOfRemovedChild = child->getHeight();
        size_t largestWidth = 0;

        for (size_t i = 0, l = children.size(); i < l; ++i) {
            NodePtr node = children.at(i);
            size_t nodeWidth = node->getCol() + node->getWidth();

            if (largestWidth < nodeWidth) {
                largestWidth = nodeWidth;
            }
        }

        for (size_t i = idx, l = children.size(); i < l; ++i) {
            NodePtr node = children.at(i);
            node->setRow(node->getRow() - heightOfRemovedChild);
        }

        NodePtr lastChild = children.at(children.size() - 1);

        setWidth(largestWidth);
        setHeight(lastChild->getRow() + lastChild->getHeight());
    }

   public:
    /** Render to a passed buffer */
    virtual void render(ostringstream *buf) const = 0;
    virtual NodeTypes nodeType() const = 0;

    virtual bool canHaveChildren() const { return true; }
    virtual NodeRenderStyle nodeRenderStyle() const {
        return NodeRenderStyle::BLOCK;
    }

    virtual void onChildRemoved(optional<size_t> idx){};
    virtual void onChildAppended(){};

    virtual void setAsParentOfChild(NodePtr child) {
        child->parent = shared_from_this();
    }
    virtual void assertChildIsValid(NodePtr child) const {};

    virtual void appendChild(NodePtr child) {
        if (canHaveChildren()) {
            assertChildIsValid(child);
            setAsParentOfChild(child);
            adjustDimensionsOnBeforeAppendChild(child);
            children.push_back(move(child));
            onChildAppended();
        } else {
            throw logic_error("This node cannot have children.");
        }
    }

    virtual void removeChildAt(size_t idx) {
        if (children.empty()) {
            return;
        }

        if (idx < children.size()) {
            NodePtr removedChild = children.at(idx);

            children.erase(children.begin() + idx);

            // TODO: ALso adjust the dimensions of
            // parents recursively
            adjustDimensionsOnRemoveChild(idx, removedChild);
            onChildRemoved(idx);
        }
    }

    virtual void emptyChildren() {
        children.clear();

        setWidth(0);
        setHeight(0);
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

    virtual const vector<NodePtr> &getChildren() const { return children; }

    // Destroy Node for proper cleanup
    virtual ~Node() = default;
};

class InteractableNode : public Node {
   public:
    InteractableNode() {}

   public:
    virtual bool onKeyPressed(int keyCode) { return false; }
    NodeTypes nodeType() const override { return NodeTypes::INTERACTABLE; }
};

class ContainerNode : public Node {
   public:
    ContainerNode() {}

   public:
    void render(ostringstream *buf) const override {
        for (size_t i = 0, l = children.size(); i < l; ++i) {
            children.at(i)->render(buf);
        }
    }

    NodeTypes nodeType() const override { return NodeTypes::CONTAINER; }
};

class GridNode : public ContainerNode {
   private:
    size_t colGap = 2;
    size_t rowGap = 1;

    size_t childWidth;

   public:
    GridNode() : childWidth(0) { setWidth(Screen::getInstance().getWidth()); }
    GridNode(size_t width) : childWidth(0) { setWidth(width); }
    GridNode(size_t width, size_t childWidth) : childWidth(childWidth) {
        setWidth(width);
    }

   public:
    virtual void emptyChildren() {
        children.clear();

        setHeight(0);
    }

    size_t getColGap() const { return colGap; }
    size_t getRowGap() const { return rowGap; }

   protected:
    void adjustDimensionsOnBeforeAppendChild(NodePtr child) override {
        if (childWidth != 0) {
            child->setWidth(childWidth);
        }

        if (children.empty()) {
            child->setRow(getRow());
            child->setCol(getCol());

            if (childWidth == 0) {
                shared_ptr<GridNode> prnt =
                    dynamic_pointer_cast<GridNode>(getParent());

                if (prnt) {
                    child->setWidth(getWidth());
                }
            }

            setHeight(child->getHeight());

            return;
        }

        NodePtr latestChild = children.at(children.size() - 1);
        size_t childCol =
            latestChild->getCol() + latestChild->getWidth() + colGap;
        size_t childRow = latestChild->getRow();

        if (childWidth == 0 && childCol < getWidth()) {
            child->setWidth(getWidth() - childCol);
        }

        // If overflow
        if (childCol + child->getWidth() > getWidth()) {
            childCol = getCol();
            childRow =
                latestChild->getRow() + latestChild->getHeight() + rowGap;

            setHeight(childRow + child->getHeight());
        } else {
            size_t tallestHeight =
                latestChild->getRow() + latestChild->getHeight();

            for (size_t i = 0, l = children.size() - 1; i < l; ++i) {
                NodePtr child = children.at(i);
                size_t childHeight = child->getRow() + child->getHeight();

                if (childHeight > tallestHeight) {
                    tallestHeight = childHeight;
                }
            }

            size_t childHeight = childRow + child->getHeight();

            setHeight(tallestHeight > childHeight ? tallestHeight
                                                  : childHeight);
        }

        child->setCol(childCol);
        child->setRow(childRow);
    }

    void adjustDimensionsOnRemoveChild(size_t idx, NodePtr child) override {
        // TODO:
        throw logic_error("unimplemented");
    }
};

class TextNode : public Node {
   private:
    string text;
    set<TextNodeFormats> formats;
    optional<tuple<int, int, int>> color = nullopt;
    optional<tuple<int, int, int>> backgroundColor = nullopt;

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
        setRow(row);
        setCol(col);
        setWidth(width);
    }
    TextNode(string text, size_t row, size_t col, size_t width, size_t height)
        : text(text) {
        size_t textSize = text.size();

        if (width < textSize && ceil(textSize / width) < height) {
            throw logic_error(
                "The provided height is not enough to accommodate for the "
                "wrapping of TextNode.");
        }

        setRow(row);
        setCol(col);
        setWidth(width);
        setHeight(height);
    }

   private:
    void assertColorValueIsValid(int val) {
        if (val < 0 || val > 255) {
            throw logic_error("Color value must be between 0 and 255");
        }
    }

   public:
    virtual void setWidth(size_t w) override {
        if (w == 0) {
            throw logic_error("Width must be > 0 for TextNode.");
        }

        size_t textSize = text.size();

        if (w < textSize) {
            width = w;
            height = ceil(textSize / w);

            if (height == 0) {
                height = 1;
            }

            saveCursorPosition();

            moveCursorTo(25, 25);
            cout << height;

            restoreSavedCursorPosition();
        } else {
            width = w;
        }
    }

    virtual void setHeight(size_t h) override {
        if (h == 0) {
            throw logic_error("Height must be > 0 for TextNode.");
        }

        size_t textSize = text.size();

        if (width < textSize) {
            if (h < ceil(textSize / width)) {
                throw logic_error(
                    "Provided height is insufficient for TextNode to display "
                    "all its text contents by wrapping.");
            }
        }

        height = h;
    }

    void render(ostringstream *buf) const override {
        moveCursorTo(buf, getCol(), getRow());

        if (color != nullopt) {
            textForeground(buf, get<0>(color.value()), get<1>(color.value()),
                           get<2>(color.value()));
        }

        if (backgroundColor != nullopt) {
            textBackground(buf, get<0>(backgroundColor.value()),
                           get<1>(backgroundColor.value()),
                           get<2>(backgroundColor.value()));
        }

        for (const auto &format : formats) {
            switch (format) {
                case TextNodeFormats::BOLD:
                    textBold(buf);
                    break;
                case TextNodeFormats::STRIKETHROUGH:
                    textStrikethrough(buf);
                    break;
                case TextNodeFormats::DIM:
                    textDim(buf);
                    break;
                case TextNodeFormats::ITALIC:
                    textItalic(buf);
                    break;
                case TextNodeFormats::UNDERLINE:
                    textUnderline(buf);
                    break;
            }
        }

        size_t len = text.size();
        size_t currWidth = getWidth();
        size_t currHeight = getHeight();

        saveCursorPosition();

        moveCursorTo(20, 20);

        cout << currWidth << " " << currHeight;

        restoreSavedCursorPosition();

        // We need to wrap this text.
        if (len > currWidth) {
            size_t currLine = 0;

            while (currLine <= currHeight) {
                *buf << text.substr(currLine * currWidth, currWidth);
                moveCursorTo(buf, getCol(), getRow() + (++currLine));
            }
        } else {
            *buf << text;

            if (len < currWidth) {
                *buf << string(currWidth - len, ' ');
            }

            size_t currLine = 1;

            while (currLine < currHeight) {
                *buf << endl << string(len, ' ');
                ++currLine;
            }
        }

        textReset(buf);
    }

    bool canHaveChildren() const override { return false; }
    NodeTypes nodeType() const override { return NodeTypes::LEAF; }
    virtual NodeRenderStyle nodeRenderStyle() const override {
        return NodeRenderStyle::INLINE;
    }

    void setRedColor(int r) {
        assertColorValueIsValid(r);

        if (color == nullopt) {
            color = {r, 0, 0};
        } else {
            get<0>(color.value()) = r;
        }
    }
    void setGreenColor(int g) {
        assertColorValueIsValid(g);

        if (color == nullopt) {
            color = {0, g, 0};
        } else {
            get<1>(color.value()) = g;
        }
    }
    void setBlueColor(int b) {
        assertColorValueIsValid(b);

        if (color == nullopt) {
            color = {0, 0, b};
        } else {
            get<2>(color.value()) = b;
        }
    }
    void setColor(int r, int g, int b) {
        assertColorValueIsValid(r);
        assertColorValueIsValid(g);
        assertColorValueIsValid(b);

        if (color == nullopt) {
            color = {r, g, b};
        } else {
            get<0>(color.value()) = r;
            get<1>(color.value()) = g;
            get<2>(color.value()) = b;
        }
    }
    void resetColor() { color = nullopt; }

    void setRedBackgroundColor(int r) {
        assertColorValueIsValid(r);

        if (backgroundColor == nullopt) {
            backgroundColor = {r, 0, 0};
        } else {
            get<0>(backgroundColor.value()) = r;
        }
    }
    void setGreenBackgroundColor(int g) {
        assertColorValueIsValid(g);

        if (backgroundColor == nullopt) {
            backgroundColor = {0, g, 0};
        } else {
            get<1>(backgroundColor.value()) = g;
        }
    }
    void setBlueBackgroundColor(int b) {
        assertColorValueIsValid(b);

        if (backgroundColor == nullopt) {
            backgroundColor = {0, 0, b};
        } else {
            get<2>(backgroundColor.value()) = b;
        }
    }
    void setBackgroundColor(int r, int g, int b) {
        assertColorValueIsValid(r);
        assertColorValueIsValid(g);
        assertColorValueIsValid(b);

        if (backgroundColor == nullopt) {
            backgroundColor = {r, g, b};
        } else {
            get<0>(backgroundColor.value()) = r;
            get<1>(backgroundColor.value()) = g;
            get<2>(backgroundColor.value()) = b;
        }
    }
    void resetBackgroundColor() { backgroundColor = nullopt; }

    void setBold() { formats.insert(TextNodeFormats::BOLD); }
    void removeBold() { formats.erase(TextNodeFormats::BOLD); }

    void setItalic() { formats.insert(TextNodeFormats::ITALIC); }
    void removeItalic() { formats.insert(TextNodeFormats::ITALIC); }

    void setStrikeThrough() { formats.insert(TextNodeFormats::STRIKETHROUGH); }
    void removeStrikeThrough() {
        formats.erase(TextNodeFormats::STRIKETHROUGH);
    }

    void setDim() { formats.insert(TextNodeFormats::DIM); }
    void removeDim() { formats.erase(TextNodeFormats::DIM); }

    void setUnderline() { formats.insert(TextNodeFormats::UNDERLINE); }
    void removeUnderline() { formats.insert(TextNodeFormats::UNDERLINE); }

    void resetFormatting() { formats.clear(); }
};

class LineBreakNode : public Node {
   public:
    LineBreakNode() { setHeight(1); }
    LineBreakNode(size_t height) { setHeight(height); }
    LineBreakNode(size_t height, size_t row) {
        setHeight(height);
        setRow(row);
    }
    LineBreakNode(size_t height, size_t row, size_t col) {
        setHeight(height);
        setRow(row);
        setCol(col);
    }

   public:
    void render(ostringstream *buf) const override {
        moveCursorTo(buf, getCol(), getRow());

        *buf << endl;
    }

    bool canHaveChildren() const override { return false; }
    NodeTypes nodeType() const override { return NodeTypes::LEAF; }
};

class SelectOptionNode : public TextNode {
   private:
    string value;

   public:
    SelectOptionNode(string value)
        : TextNode(kebabToPascal(value)), value(value) {}

    NodeRenderStyle nodeRenderStyle() const override {
        return NodeRenderStyle::BLOCK;
    }

    string getValue() const { return value; }
};

class SelectNode : public InteractableNode {
   public:
    Observer<string> observer;

   private:
    size_t activeSelectOptionIdx = 0;
    const tuple<int, int> keyCodes = {KEY_DOWN, KEY_UP};

   public:
    SelectNode() = default;
    ~SelectNode() { observer.clear(); }

   private:
    void selectNext() {
        activeSelectOptionIdx = (activeSelectOptionIdx + 1) % children.size();
        observer.setState(getValueOfSelectedOption().value());
    }
    void selectPrevious() {
        if (activeSelectOptionIdx == 0) {
            activeSelectOptionIdx = children.size() - 1;
        } else {
            activeSelectOptionIdx -= 1;
        }
        observer.setState(getValueOfSelectedOption().value());
    }

   public:
    void render(ostringstream *buf) const override {
        for (size_t i = 0, l = children.size(); i < l; ++i) {
            shared_ptr<SelectOptionNode> node =
                static_pointer_cast<SelectOptionNode>(children.at(i));

            if (i == activeSelectOptionIdx) {
                node->setBackgroundColor(255, 255, 255);
                node->setColor(0, 0, 0);
            } else {
                node->resetColor();
                node->resetBackgroundColor();
            }

            node->render(buf);
        }
    }

    void assertChildIsValid(NodePtr child) const override {
        shared_ptr<SelectOptionNode> ptr =
            dynamic_pointer_cast<SelectOptionNode>(child);

        if (ptr) {
            return;
        }

        throw logic_error(
            "SelectNode only accepts SelectOptionNode as its children.");
    }

    void onChildRemoved(optional<size_t> idx = nullopt) override {
        if (children.empty()) {
            return;
        }

        size_t childSize = children.size();

        if (activeSelectOptionIdx >= childSize) {
            activeSelectOptionIdx = childSize - 1;
            observer.setState(getValueOfSelectedOption().value());
        } else if (idx < activeSelectOptionIdx) {
            activeSelectOptionIdx -= 1;
            observer.setState(getValueOfSelectedOption().value());
        }
    }

    optional<string> getValueOfSelectedOption() const {
        if (children.empty()) {
            return nullopt;
        }

        auto selectedOptionNode = dynamic_pointer_cast<SelectOptionNode>(
            children.at(activeSelectOptionIdx));

        if (selectedOptionNode) {
            return selectedOptionNode->getValue();
        }

        return nullopt;
    }

    bool onKeyPressed(int keyCode) override {
        if (children.empty()) {
            return false;
        }

        if (keyCode == get<0>(keyCodes)) {
            selectNext();

            return true;
        } else if (keyCode == get<1>(keyCodes)) {
            selectPrevious();

            return true;
        }

        return false;
    }
};

/**
 *
 * More like a text that listens for keyPress
 * since implementing a cursor click listener
 * would require a library to lessen complexity.
 */
class ButtonNode : public InteractableNode {
   public:
    ButtonNode() {}
};

#endif
