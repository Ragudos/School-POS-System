#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <vector>


using namespace std;

enum NodeTypes { CONTAINER, INTERACTABLE, LEAF };

enum NodeRenderStyle { BLOCK, INLINE };

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

class InteractableNode : public Node {
   public:
    virtual void render(ostringstream*) const override;
    /**
     *
     * Returns false if `keyCode` is not this node's trigger
     */
    virtual bool onKeyPressed(unsigned int) = 0;

    NodeTypes nodeType() const noexcept override;
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

   public:
    GridNode();
    GridNode(unsigned int);
    GridNode(unsigned int, unsigned int);
    GridNode(unsigned int, unsigned int, unsigned int);
    GridNode(unsigned int, unsigned int, unsigned int, unsigned int);
};

class LeafNode : public Node {
   public:
    NodeTypes nodeType() const noexcept override;
};

class TextNode : public LeafNode {};
