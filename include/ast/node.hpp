#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using namespace std;

/**
 *
 * Base node of Abstract-Tree syntax
 * for the frontend
 */
class Node : public enable_shared_from_this<Node> {
   private:
    using NodePtr = shared_ptr<Node>;

    vector<NodePtr> children;
    NodePtr parent = nullptr;

   public:
    virtual void render(ostringstream *buf) const = 0;
    virtual bool canHaveChildren() const { return true; }

    void appendChild(NodePtr child) {
        if (canHaveChildren()) {
            child->parent = shared_from_this();
            children.push_back(move(child));
        } else {
            throw logic_error("This node cannot have children.");
        }
    }

    void removeChildAt(size_t idx) {
        if (idx < children.size()) {
            children.erase(children.begin() + idx);
        }
    }

    // Destroy Node for proper cleanup
    virtual ~Node() = default;
};

class TextNode : public Node, public enable_shared_from_this<Node> {
   public:
    void render(ostringstream *buf) const override { *buf << ""; }

    bool canHaveChildren() const override { return false; }
};
