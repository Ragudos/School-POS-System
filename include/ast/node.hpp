#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <screen.hpp>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utils.hpp>
#include <vector>

using namespace std;
using namespace string_utils;
using namespace terminal;

enum NodeTypes { CONTAINER, INTERACTABLE, LEAF };

enum NodeRenderStyle { BLOCK, INLINE };

enum TextNodeFormats { BOLD, ITALIC, UNDERLINE, STRIKETHROUGH, DIM };

class Node : public enable_shared_from_this<Node> {
   public:
    using NodePtr = shared_ptr<Node>;

   private:
    NodePtr parent;
    unsigned int posX;
    unsigned int posY;

   protected:
    vector<NodePtr> children;
    unsigned int width;
    unsigned int height;

   public:
    Node();
    Node(unsigned int);
    Node(unsigned int, unsigned int);

   public:
    virtual void render(ostringstream *) const = 0;
    virtual NodeTypes nodeType() const noexcept = 0;

    /**
     *
     * Default is `NodeRenderStyle::BLOCK`
     */
    virtual NodeRenderStyle nodeRenderStyle() const noexcept;
    virtual bool canHaveChildren() const noexcept;

    virtual unsigned int getWidth() const noexcept;
    virtual void setWidth(unsigned int);

    virtual unsigned int getHeight() const noexcept;
    virtual void setHeight(unsigned int);

    virtual unsigned int getPosX() const noexcept;
    virtual void setPosX(unsigned int);

    virtual unsigned int getPosY() const noexcept;
    virtual void setPosY(unsigned int);

    virtual NodePtr getParent() const noexcept;
    virtual const vector<NodePtr> &getChildren() const noexcept;

    virtual ~Node();

   private:
    /**
     *
     * Default behavior:
     * Adjusts width and height based on child with largest width
     * and latest child's height (to be inserted).
     * Basically depends on child to define its dimensions
     * unlike a grid container.
     */
    virtual void onChildRemoved(size_t, NodePtr);
    /**
     *
     * Default behavior:
     * Adjusts width and height based on child with largest width
     * and latest child's height (to be inserted).
     * Basically depends on child to define its dimensions
     * unlike a grid container.
     */
    virtual void onChildAppended();

   public:
    virtual void updateParentDimensionsOnChildChange(NodePtr);
    virtual void updateChildrenDimensionsOnChange();

   public:
    /**
     *
     * Useful for nodes that only accept a certain type of
     * Node type
     */
    virtual void assertChildIsValid(NodePtr) const;
    virtual void setAsParentOfChild(NodePtr);
    virtual void appendChild(NodePtr);
    virtual void removeChildAt(size_t);
    virtual void removeAllChildren() noexcept;
};

class ContainerNode : public Node {
    public:
        virtual void render(ostringstream*) const override;

        NodeTypes nodeType() const noexcept override;
};

class GridNode : public ContainerNode {
   private:
    unsigned int colGap;
    unsigned int rowGap;

    unsigned int childWidth;

    /**
     *
     * Whether to have its children take remaining width
     */
    bool flexible;

   public:
    GridNode();
    GridNode(unsigned int);
    GridNode(unsigned int, unsigned int);
    GridNode(unsigned int, unsigned int, unsigned int);
    GridNode(unsigned int, unsigned int, unsigned int, unsigned int);

   public:
    virtual void removeAllChildren() noexcept override;
    virtual void onChildAppended() override;
    virtual void onChildRemoved(size_t, NodePtr) override;

    virtual void updateChildrenDimensionsOnChange() override;
    virtual void updateParentDimensionsOnChildChange(
        NodePtr childCaller) override;

    unsigned int getColGap() const noexcept;
    void setColGap(unsigned int);

    unsigned int getRowGap() const noexcept;
    void setRowGap(unsigned int);

    bool isFlexible() const noexcept;
    void setIsFlexible(bool) noexcept;
};

class LeafNode : public Node {
   public:
    bool canHaveChildren() const noexcept override;
    NodeTypes nodeType() const noexcept override;
};

class TextNode : public LeafNode {
   private:
    string text;
    set<TextNodeFormats> formats;
    optional<tuple<uint8_t, uint8_t, uint8_t>> color;
    optional<tuple<uint8_t, uint8_t, uint8_t>> backgroundColor;

   public:
    TextNode(string);
    TextNode(string, unsigned int);
    TextNode(string, unsigned int, unsigned int);
    TextNode(string, unsigned int, unsigned int, unsigned int);
    TextNode(string, unsigned int, unsigned int, unsigned int, unsigned int);

   public:
    virtual NodeRenderStyle nodeRenderStyle() const noexcept override;

   public:
    virtual void render(ostringstream *) const override;

   public:
    virtual void setWidth(unsigned int w) override;
    virtual void setHeight(unsigned int h) override;

    void setRedColor(uint8_t);
    void setGreenColor(uint8_t);
    void setBlueColor(uint8_t);
    void setColor(uint8_t, uint8_t, uint8_t);
    void resetColor() noexcept;

    void setRedBackgroundColor(uint8_t);
    void setGreenBackgroundColor(uint8_t);
    void setBlueBackgroundColor(uint8_t);
    void setBackgroundColor(uint8_t, uint8_t, uint8_t);
    void resetBackgroundColor() noexcept;

    void setBold();
    void removeBold();

    void setItalic();
    void removeItalic();

    void setStrikethrough();
    void removeStrikethrough();

    void setDim();
    void removeDim();

    void setUnderline();
    void removeUnderline();

    void resetFormatting();
};

class LineBreakNode : public LeafNode {
   public:
    LineBreakNode();
    LineBreakNode(unsigned int);
    LineBreakNode(unsigned int, unsigned int);
    LineBreakNode(unsigned int, unsigned int, unsigned int);

   public:
    virtual void render(ostringstream *) const override;
};

class SelectOptionNode : public TextNode {
   private:
    string value;

   public:
    SelectOptionNode(string);
    SelectOptionNode(string, unsigned int);
    SelectOptionNode(string, unsigned int, unsigned int);
    SelectOptionNode(string, unsigned int, unsigned int, unsigned int);
    SelectOptionNode(string, unsigned int, unsigned int, unsigned int,
                     unsigned int);

   public:
    NodeRenderStyle nodeRenderStyle() const noexcept override;

   public:
    string getValue() const noexcept;
};

class InteractableNode : public Node {
   public:
    virtual void render(ostringstream *) const override;

   public:
    /**
     *
     * Returns false if `keyCode` is not this node's
     * trigger.
     */
    virtual bool onKeyPressed(unsigned int);
    NodeTypes nodeType() const noexcept override;
};

class SelectNode : public InteractableNode {
   public:
    using SubscriberCallback = function<void(optional<string>)>;

   private:
    size_t activeOptionIdx;

    vector<SubscriberCallback> subscribers;

   private:
    void selectNext() noexcept;
    void selectPrevious() noexcept;

    void notify();

   public:
    SelectNode();
    SelectNode(size_t);
    ~SelectNode();

   public:
    void render(ostringstream *) const override;
    void subscribe(SubscriberCallback);
    void unsubscribe(SubscriberCallback);

   public:
    void onChildRemoved(size_t, NodePtr) override;

   public:
    void assertChildIsValid(NodePtr) const override;

   public:
    void resetActiveIdx() noexcept;
    virtual bool onKeyPressed(unsigned int) override;

   public:
    optional<string> getValueOfSelectedOption() const;
    size_t getActiveOptionIdx() const noexcept;
    /**
     *
     * Set the active idx based on provided value.
     * Throws an error if not found as child.
     */
    void setActiveChildWithValue(string);
};