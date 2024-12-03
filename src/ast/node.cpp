#include <ast/node.hpp>
#include <renderer.hpp>

Node::Node() : posX(0), posY(0), width(0), height(0) {};

Node::Node(unsigned int col) : posX(col), posY(0), width(0), height(0) {};

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

    size_t childrenSize = children.size();
    NodePtr appendedChild = children.at(childrenSize - 1);

    appendedChild->setPosX(getPosX());
    appendedChild->setPosY(getPosY());

    if (childrenSize > 1) {
        NodePtr prevSibling = children.at(childrenSize - 2);

        switch (appendedChild->nodeRenderStyle()) {
            case NodeRenderStyle::BLOCK: {
                appendedChild->setPosY(prevSibling->getPosY() +
                                       prevSibling->getHeight());
                setHeight(getHeight() + appendedChild->getHeight());
            }; break;
            case NodeRenderStyle::INLINE: {
                appendedChild->setPosX(prevSibling->getPosX() +
                                       prevSibling->getWidth());

                if (getHeight() < appendedChild->getHeight()) {
                    setHeight(getHeight() +
                              (appendedChild->getHeight() - getHeight()));
                }
            }; break;
        }
    } else {
        setHeight(appendedChild->getHeight());
    }

    unsigned int appendedChildRealWidth =
        appendedChild->getPosX() + appendedChild->getWidth();

    // TODO: Have this wrap to next line if this exceeds
    // screen size.
    if (getWidth() < appendedChildRealWidth) {
        if (appendedChildRealWidth > getScreen().getWidth()) {
            appendedChild->setPosX(getPosX());
            appendedChild->setPosY(getHeight());
            setHeight(getHeight() + appendedChild->getHeight());
        } else {
            setWidth(appendedChildRealWidth);
        }
    }

    appendedChild->updateChildrenDimensionsOnChange();

    /** === Update the parents' and children's dimensions or something */
    NodePtr prnt = getParent();

    if (prnt) {
        prnt->updateParentDimensionsOnChildChange(shared_from_this());
    }
}

void Node::updateChildrenDimensionsOnChange() {
    /**
     *
     * TODO: All these adjustment logic should be done every time that there's
     * a change in dimensions in any node that's related to the affected
     * node AND make it responsive if possible.
     */
    const unsigned int cachedInitialPosX = getPosX();
    unsigned int currPosY = getPosY();
    unsigned int currPosX = getPosX();

    for (size_t i = 0, l = children.size(); i < l; ++i) {
        auto& c = children.at(i);

        c->setPosY(currPosY);
        c->setPosX(currPosX);

        if (i < l - 1) {
            auto& nextC = children.at(i + 1);

            // shit (doesnt take order and relativity into account, but
            // whatever) and other stuff
            switch (nextC->nodeRenderStyle()) {
                case NodeRenderStyle::BLOCK: {
                    currPosY += c->getHeight();
                }; break;
                case NodeRenderStyle::INLINE: {
                    currPosX += c->getWidth();
                }; break;
            }

            if (currPosX >= getWidth() + cachedInitialPosX) {
                currPosX = cachedInitialPosX;
                currPosY += c->getHeight();
            }
        } else {
            switch (c->nodeRenderStyle()) {
                case NodeRenderStyle::BLOCK: {
                    currPosY += c->getHeight();
                }; break;
                case NodeRenderStyle::INLINE: {
                    currPosX += c->getWidth();
                }; break;
            }

            if (currPosX >= getWidth() + cachedInitialPosX) {
                currPosX = cachedInitialPosX;
                currPosY += c->getHeight();
            }
        }

        c->updateChildrenDimensionsOnChange();
    }
}

void Node::updateParentDimensionsOnChildChange(NodePtr childCaller) {
    unsigned int childCallerRealHeight =
        childCaller->getPosY() + childCaller->getHeight();

    if (getHeight() < childCallerRealHeight) {
        setHeight(childCallerRealHeight);
    }

    unsigned int childCallerRealWidth =
        childCaller->getPosX() + childCaller->getWidth();

    if (getWidth() < childCallerRealWidth) {
        setWidth(childCallerRealWidth);
    }

    updateChildrenDimensionsOnChange();

    NodePtr parent = getParent();

    // Base case
    if (parent == nullptr) {
        return;
    }

    parent->updateParentDimensionsOnChildChange(shared_from_this());
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

void ContainerNode::render(ostringstream* buf) const {
    for (auto node : children) {
        node->render(buf);
    }
}

NodeTypes ContainerNode::nodeType() const noexcept {
    return NodeTypes::CONTAINER;
}

GridNode::GridNode() : colGap(4), rowGap(2), childWidth(0) {
    setWidth(getScreen().getWidth());
}

GridNode::GridNode(unsigned int w) : colGap(4), rowGap(2), childWidth(0) {
    setWidth(w);
}

GridNode::GridNode(unsigned int w, unsigned int childW)
    : colGap(4), rowGap(2), childWidth(childW) {
    setWidth(w);
    assert(childW <= getWidth() ||
           !"child width should be <= grid container's width.");
}

GridNode::GridNode(unsigned int w, unsigned int childW, unsigned int colGap)
    : colGap(colGap), rowGap(2), childWidth(childW) {
    setWidth(w);
    assert(childW <= getWidth() ||
           !"child width should be <= grid container's width.");
}

GridNode::GridNode(unsigned int w, unsigned int childW, unsigned int colGap,
                   unsigned int rowGap)
    : colGap(colGap), rowGap(rowGap), childWidth(childW) {
    setWidth(w);
    assert(childW <= getWidth() ||
           !"child width should be <= grid container's width.");
}

void GridNode::removeAllChildren() noexcept {
    children.clear();

    setHeight(0);
}

unsigned int GridNode::getColGap() const noexcept { return colGap; }

void GridNode::setColGap(unsigned int c) {
    assert(c >= 0 || !"column gap must be >= 0");
    colGap = c;
}

unsigned int GridNode::getRowGap() const noexcept { return rowGap; }

void GridNode::setRowGap(unsigned int r) {
    assert(r >= 0 || !"row gap must be >= 0");
    rowGap = r;
}

bool GridNode::isFlexible() const noexcept { return flexible; }

void GridNode::setIsFlexible(bool f) noexcept { flexible = f; }

void GridNode::onChildAppended() {
    assert(!children.empty() ||
           !"GridNode::onChildAppended() called when children is empty().");

    size_t childrenSize = children.size();
    NodePtr appendedChild = children.at(childrenSize - 1);

    if (!flexible) {
        unsigned int w = appendedChild->getWidth() == 0
                             ? getWidth()
                             : appendedChild->getWidth();

        appendedChild->setWidth(childWidth == 0 ? w : childWidth);
    }

    appendedChild->setPosX(getPosX());
    appendedChild->setPosY(getPosY());

    if (childrenSize == 1) {
        if (appendedChild->getWidth() > getWidth()) {
            appendedChild->setWidth(getWidth());
        }

        setHeight(appendedChild->getHeight());
    } else {
        NodePtr prevSiblingOfAppendedChild = children.at(childrenSize - 2);
        unsigned int prevSiblingRealWidth =
            prevSiblingOfAppendedChild->getPosX() +
            prevSiblingOfAppendedChild->getWidth() + colGap;

        // have the appended child take the remaining width if
        // it has no defined width
        if (flexible && prevSiblingRealWidth < getWidth()) {
            if (appendedChild->getWidth() == 0 ||
                prevSiblingRealWidth + appendedChild->getWidth() > getWidth()) {
                appendedChild->setWidth(getWidth() - prevSiblingRealWidth);
            }
        }

        // if overflow
        if (prevSiblingRealWidth + appendedChild->getWidth() > getWidth()) {
            appendedChild->setPosX(getPosX());
            appendedChild->setPosY(prevSiblingOfAppendedChild->getPosY() +
                                   prevSiblingOfAppendedChild->getHeight() +
                                   rowGap);

            setHeight(appendedChild->getPosY() + appendedChild->getHeight());
        } else {
            unsigned int prevSiblingHeight =
                prevSiblingOfAppendedChild->getPosY() +
                prevSiblingOfAppendedChild->getHeight();
            unsigned int appendedChildHeight =
                appendedChild->getPosY() + appendedChild->getHeight();
            unsigned int tallestHeight = prevSiblingHeight > appendedChildHeight
                                             ? prevSiblingHeight
                                             : appendedChildHeight;

            for (size_t i = 0, l = childrenSize - 1; i < l; ++i) {
                NodePtr child = children.at(i);
                unsigned int childHeight =
                    child->getPosY() + child->getHeight();

                if (childHeight > tallestHeight) {
                    tallestHeight = childHeight;
                }
            }

            setHeight(tallestHeight);

            appendedChild->setPosX(prevSiblingRealWidth);
            appendedChild->setPosY(prevSiblingOfAppendedChild->getPosY());
        }
    }

    appendedChild->updateChildrenDimensionsOnChange();

    /** === Update the parents' and children's dimensions or something */
    NodePtr prnt = getParent();

    if (prnt) {
        prnt->updateParentDimensionsOnChildChange(shared_from_this());
    }
}

void GridNode::updateParentDimensionsOnChildChange(NodePtr childCaller) {
    unsigned int childCallerRealWidth =
        childCaller->getPosX() + childCaller->getWidth();

    if (getWidth() < childCallerRealWidth) {
        if (children.size() == 1 || !flexible) {
            childCaller->setWidth(childWidth == 0
                                      ? (childCallerRealWidth - getWidth())
                                      : childWidth);
        } else if (flexible && children.size() > 1) {
            childCaller->setWidth(childCallerRealWidth - getWidth());
        }
    }

    unsigned int childCallerRealHeight =
        childCaller->getPosY() + childCaller->getHeight();

    if (getHeight() < childCallerRealHeight) {
        setHeight(childCallerRealHeight);
    }

    updateChildrenDimensionsOnChange();

    NodePtr parent = getParent();

    // Base case
    if (parent == nullptr) {
        return;
    }

    parent->updateParentDimensionsOnChildChange(shared_from_this());
}

void GridNode::updateChildrenDimensionsOnChange() {
    /**
     *
     * TODO: All these adjustment logic should be done every time that there's
     * a change in dimensions in any node that's related to the affected
     * node AND make it responsive if possible.
     */
    const unsigned int cachedInitialPosX = getPosX();
    unsigned int currPosY = getPosY();
    unsigned int currPosX = getPosX();

    for (size_t i = 0, l = children.size(); i < l; ++i) {
        auto& c = children.at(i);

        c->setPosY(currPosY);
        c->setPosX(currPosX);

        if (i == l - 1) {
            if (!flexible) {
                currPosX += childWidth == 0 ? c->getWidth() : childWidth;
            } else if (currPosX < getWidth() + cachedInitialPosX) {
                currPosX += c->getWidth();
            }
        } else {
            auto& nextC = children.at(i + 1);

            if (!flexible) {
                currPosX += childWidth == 0 ? c->getWidth() + colGap
                                            : childWidth + colGap;
            } else if (currPosX < getWidth() + cachedInitialPosX) {
                currPosX += c->getWidth() + colGap;
            }

            if (currPosX + nextC->getWidth() > getWidth() + cachedInitialPosX) {
                currPosX = cachedInitialPosX;
                currPosY += c->getHeight() + rowGap;
            }
        }

        c->updateChildrenDimensionsOnChange();
    }
}

void GridNode::onChildRemoved(size_t idx, NodePtr removedChild) {
    throw logic_error("unimplemented");
}

bool LeafNode::canHaveChildren() const noexcept { return false; }

NodeTypes LeafNode::nodeType() const noexcept { return NodeTypes::LEAF; }

TextNode::TextNode(string text) : text(text) {
    setPosX(0);
    setPosY(0);
    setWidth(static_cast<unsigned int>(text.size()));
    setHeight(1);
}

TextNode::TextNode(string text, unsigned int posX) : text(text) {
    setPosX(posX);
    setPosY(0);
    setWidth(static_cast<unsigned int>(text.size()));
    setHeight(1);
}

TextNode::TextNode(string text, unsigned int posX, unsigned int posY)
    : text(text) {
    setPosX(posX);
    setPosY(posY);
    setWidth(static_cast<unsigned int>(text.size()));
    setHeight(1);
}

TextNode::TextNode(string text, unsigned int posX, unsigned int posY,
                   unsigned int width)
    : text(text) {
    setPosX(posX);
    setPosY(posY);
    setWidth(width);
    setHeight(1);
}

TextNode::TextNode(string text, unsigned int posX, unsigned int posY,
                   unsigned int width, unsigned int height)
    : text(text) {
    setPosX(posX);
    setPosY(posY);
    setWidth(width);
    setHeight(height);
}

NodeRenderStyle TextNode::nodeRenderStyle() const noexcept {
    return NodeRenderStyle::INLINE;
}

void TextNode::render(ostringstream* buf) const {
    moveCursorTo(buf, getPosX(), getPosY());

    if (color != nullopt) {
        textForeground(buf, get<0>(color.value()), get<1>(color.value()),
                       get<2>(color.value()));
    }

    if (backgroundColor != nullopt) {
        textBackground(buf, get<0>(backgroundColor.value()),
                       get<1>(backgroundColor.value()),
                       get<2>(backgroundColor.value()));
    }

    for (const auto& format : formats) {
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
    size_t currWidth = static_cast<size_t>(getWidth());
    size_t currHeight = static_cast<size_t>(getHeight());

    if (len > currWidth) {
        size_t currLine = 0;

        while (currLine < currHeight) {
            size_t start = currWidth * currLine;

            if (start >= len) {
                break;
            }

            string str = text.substr(start, currWidth);

            *buf << str;

            moveCursorTo(buf, getPosX(),
                         getPosY() + static_cast<unsigned int>(++currLine));
        }
    } else {
        *buf << text;

        /**
         *
         * COMMENTED OUT
         *
         * FOR SOME REASON, MY VSCODE'S TERMINAL DOESNT LIKE IT
         * AND WRAPS THE TEXT TO NEXT LINE, SO OTHER TEXTNODES
         * DONT GET RENDERED IN SOME WAY.
         */
        /*if (len < currWidth) {
            for (size_t i = 0, l = currWidth - len - 1; i < l; ++i) {
                *buf << ' ';
            }
        }

        unsigned int currLine = 1;
        while (currLine < currHeight) {
            *buf << endl;
            for (size_t i = 0, l = currWidth; i < l; ++i) {
                *buf << ' ';
            }

            ++currLine;
        }*/
    }

    textReset(buf);
}

void TextNode::setWidth(unsigned int w) {
    assert(w >= 0 || !"TextNode::setWidth() received a width that's not > 0");

    unsigned int textSize = static_cast<unsigned int>(text.size());

    if (w < textSize) {
        height = static_cast<unsigned int>(
            ceil(static_cast<float>(textSize) / static_cast<float>(w)));

        if (height == 0) {
            height = 1;
        }
    }

    width = w;
}

void TextNode::setHeight(unsigned int h) {
    assert(h >= 0 || !"TextNode::setHeight() received a height that's not > 0");

    size_t textSize = text.size();

    if (width < textSize) {
        assert(h < static_cast<unsigned int>(ceil(static_cast<float>(textSize) / static_cast<float>(width))) || !"TextNode::setHeight() received insufficient height to accommodate the wrapping of its text contents.");
    }

    height = h;
}

void TextNode::setRedColor(uint8_t r) {
    if (color == nullopt) {
        color = {r, 0, 0};
    } else {
        get<0>(color.value()) = r;
    }
}

void TextNode::setGreenColor(uint8_t g) {
    if (color == nullopt) {
        color = {0, g, 0};
    } else {
        get<1>(color.value()) = g;
    }
}

void TextNode::setBlueColor(uint8_t b) {
    if (color == nullopt) {
        color = {0, 0, b};
    } else {
        get<2>(color.value()) = b;
    }
}

void TextNode::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (color == nullopt) {
        color = {r, g, b};
    } else {
        get<0>(color.value()) = r;
        get<1>(color.value()) = g;
        get<2>(color.value()) = b;
    }
}

void TextNode::resetColor() noexcept { color.reset(); }

void TextNode::setRedBackgroundColor(uint8_t r) {
    if (backgroundColor == nullopt) {
        backgroundColor = {r, 0, 0};
    } else {
        get<0>(backgroundColor.value()) = r;
    }
}

void TextNode::setGreenBackgroundColor(uint8_t g) {
    if (backgroundColor == nullopt) {
        backgroundColor = {0, g, 0};
    } else {
        get<1>(backgroundColor.value()) = g;
    }
}

void TextNode::setBlueBackgroundColor(uint8_t b) {
    if (backgroundColor == nullopt) {
        backgroundColor = {0, 0, b};
    } else {
        get<2>(backgroundColor.value()) = b;
    }
}

void TextNode::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b) {
    if (backgroundColor == nullopt) {
        backgroundColor = {r, g, b};
    } else {
        get<0>(backgroundColor.value()) = r;
        get<1>(backgroundColor.value()) = g;
        get<2>(backgroundColor.value()) = b;
    }
}

void TextNode::resetBackgroundColor() noexcept { backgroundColor.reset(); }

void TextNode::setBold() { formats.insert(TextNodeFormats::BOLD); }

void TextNode::removeBold() { formats.erase(TextNodeFormats::BOLD); }

void TextNode::setItalic() { formats.insert(TextNodeFormats::ITALIC); }

void TextNode::removeItalic() { formats.erase(TextNodeFormats::ITALIC); }

void TextNode::setStrikethrough() {
    formats.insert(TextNodeFormats::STRIKETHROUGH);
}

void TextNode::removeStrikethrough() {
    formats.erase(TextNodeFormats::STRIKETHROUGH);
}

void TextNode::setDim() { formats.insert(TextNodeFormats::DIM); }

void TextNode::removeDim() { formats.erase(TextNodeFormats::DIM); }

void TextNode::setUnderline() { formats.insert(TextNodeFormats::UNDERLINE); }

void TextNode::removeUnderline() { formats.erase(TextNodeFormats::UNDERLINE); }

void TextNode::resetFormatting() { formats.clear(); }

LineBreakNode::LineBreakNode() { setHeight(1); }

LineBreakNode::LineBreakNode(unsigned int height) {
    assert(height > 0 || !"LineBreakNode() height must be > 0");
    setHeight(height);
}

LineBreakNode::LineBreakNode(unsigned int height, unsigned int posX) {
    assert(height > 0 || !"LineBreakNode() height must be > 0");
    setHeight(height);
    setPosX(posX);
}

LineBreakNode::LineBreakNode(unsigned int height, unsigned int posX,
                             unsigned int posY) {
    assert(height > 0 || !"LineBreakNode() height must be > 0");
    setHeight(height);
    setPosX(posX);
    setPosY(posY);
}

void LineBreakNode::render(ostringstream* buf) const {}

SelectOptionNode::SelectOptionNode(string value)
    : TextNode(kebabToPascal(value)), value(value) {}

SelectOptionNode::SelectOptionNode(string value, unsigned int posX)
    : TextNode(kebabToPascal(value), posX), value(value) {}

SelectOptionNode::SelectOptionNode(string value, unsigned int posX,
                                   unsigned int posY)
    : TextNode(kebabToPascal(value), posX, posY), value(value) {}

SelectOptionNode::SelectOptionNode(string value, unsigned int posX,
                                   unsigned int posY, unsigned int width)
    : TextNode(kebabToPascal(value), posX, posY, width), value(value) {}

SelectOptionNode::SelectOptionNode(string value, unsigned int posX,
                                   unsigned int posY, unsigned int width,
                                   unsigned int height)
    : TextNode(kebabToPascal(value), posX, posY, width, height), value(value) {}

NodeRenderStyle SelectOptionNode::nodeRenderStyle() const noexcept {
    return NodeRenderStyle::BLOCK;
}

string SelectOptionNode::getValue() const noexcept { return value; }

void InteractableNode::render(ostringstream* buf) const {
    for (NodePtr node : children) {
        node->render(buf);
    }
}

bool InteractableNode::onKeyPressed(unsigned int keyCode) { return false; }

NodeTypes InteractableNode::nodeType() const noexcept {
    return NodeTypes::INTERACTABLE;
}

SelectNode::SelectNode() : activeOptionIdx(0) {}
SelectNode::SelectNode(size_t activeOptionIdx)
    : activeOptionIdx(activeOptionIdx) {}
SelectNode::~SelectNode() { subscribers.clear(); }

void SelectNode::selectNext() noexcept {
    activeOptionIdx = (activeOptionIdx + 1) % children.size();

    notify();
}

void SelectNode::selectPrevious() noexcept {
    if (activeOptionIdx == 0) {
        activeOptionIdx = children.size() - 1;
    } else {
        activeOptionIdx -= 1;
    }

    notify();
}

void SelectNode::notify() {
    optional<string> activeValId = getValueOfSelectedOption();

    for (SubscriberCallback subscriber : subscribers) {
        subscriber(activeValId);
    }
}

void SelectNode::render(ostringstream* buf) const {
    for (size_t i = 0, l = children.size(); i < l; ++i) {
        shared_ptr<SelectOptionNode> node =
            static_pointer_cast<SelectOptionNode>(children.at(i));

        if (i == activeOptionIdx) {
            node->setColor(0, 0, 0);
            node->setBackgroundColor(255, 255, 255);
        } else {
            node->resetColor();
            node->resetBackgroundColor();
        }

        node->render(buf);
    }
}

void SelectNode::subscribe(SelectNode::SubscriberCallback cb) {
    subscribers.push_back(cb);
}

void SelectNode::unsubscribe(SelectNode::SubscriberCallback cb) {
    subscribers.erase(
        remove_if(subscribers.begin(), subscribers.end(),
                  [&cb](const SelectNode::SubscriberCallback currCb) {
                      return currCb.target_type() == cb.target_type();
                  }),
        subscribers.end());
}

void SelectNode::onChildRemoved(size_t idx, NodePtr removedChild) {
    if (children.empty()) {
        setWidth(0);
        setHeight(0);

        activeOptionIdx = 0;

        return;
    }

    unsigned int heightOfRemovedChild = removedChild->getHeight();
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

    // ==== Update option if active is removed  ==== //

    size_t childrenSize = children.size();

    if (activeOptionIdx == childrenSize) {
        activeOptionIdx = childrenSize - 1;
    } else if (idx < activeOptionIdx) {
        activeOptionIdx -= 1;
    }
}

void SelectNode::assertChildIsValid(NodePtr child) const {
    assert(dynamic_pointer_cast<SelectOptionNode>(child) ||
           !"SelectNode only accepts SelectOptionNode as its children");
}

void SelectNode::resetActiveIdx() noexcept {
    activeOptionIdx = 0;
    notify();
}

optional<string> SelectNode::getValueOfSelectedOption() const {
    if (children.empty()) {
        return nullopt;
    }

    assert(activeOptionIdx < children.size() || !"SelectNode::getValueOfSelectedOption() is called when activeOptionIdx is < children.size()");

    const shared_ptr<const SelectOptionNode> selectedOptionNode =
        static_pointer_cast<const SelectOptionNode>(
            children.at(activeOptionIdx));

    return selectedOptionNode->getValue();
}

size_t SelectNode::getActiveOptionIdx() const noexcept {
    return activeOptionIdx;
}

void SelectNode::setActiveChildWithValue(string val) {
    assert(
        !children.empty() ||
        !"SelectNode::setActiveChildWithValue() is called with no children.");

    for (size_t i = 0, l = children.size(); i < l; ++i) {
        const shared_ptr<const SelectOptionNode> child =
            static_pointer_cast<SelectOptionNode>(children.at(i));

        if (child->getValue() == val) {
            activeOptionIdx = i;
            notify();
            return;
        }
    }

    throw runtime_error(
        "Reached unreachable code at SelectNode::setActiveChildWithValue()");
}

bool SelectNode::onKeyPressed(unsigned int keyCode) {
    switch (keyCode) {
        case KEY_DOWN: {
            selectNext();
        };
            return true;
        case KEY_UP: {
            selectPrevious();
        };
            return true;
        default:
            return false;
    }
}
int pos = 26;

ButtonNode::ButtonNode(string icon, string text,
                       tuple<unsigned int, unsigned int> keyCode)
    : icon(icon), text(text), keyCode(keyCode), isPressed(false) {
    height = 1;
    setWidth(static_cast<unsigned int>(numOfUtf8Chars(icon)) + 1 +
             static_cast<unsigned int>(text.size()));
}

ButtonNode::ButtonNode(string icon, string text,
                       tuple<unsigned int, unsigned int> keyCode,
                       bool isPressed)
    : icon(icon), text(text), keyCode(keyCode), isPressed(isPressed) {
    height = 1;
    setWidth(static_cast<unsigned int>(numOfUtf8Chars(icon)) + 1 +
             static_cast<unsigned int>(text.size()));
}

void ButtonNode::setWidth(unsigned int w) {
    assert(w > 0 || !"TextNode::setWidth() received a width that's not > 0");

    unsigned int textSize = static_cast<unsigned int>(text.size());

    if (w < textSize) {
        height = static_cast<unsigned int>(
            ceil(static_cast<float>(textSize) / static_cast<float>(w)));

        if (height == 0) {
            height = 1;
        }
    }

    width = w;
}

void ButtonNode::setHeight(unsigned int h) {
    assert(h > 0 || !"TextNode::setHeight() received a height that's not > 0");

    size_t textSize = text.size();

    if (width < textSize) {
        assert(h < static_cast<unsigned int>(ceil(static_cast<float>(textSize) / static_cast<float>(width))) || !"TextNode::setHeight() received insufficient height to accommodate the wrapping of its text contents.");
    }

    height = h;
}

void ButtonNode::render(ostringstream* buf) const {
    moveCursorTo(buf, getPosX(), getPosY());

    textBold(buf);

    *buf << icon << " ";

    if (!isPressed) {
        textNormal(buf);
        textDim(buf);
    }

    *buf << text;

    textReset(buf);
}

void ButtonNode::notify(unsigned int keyCode) {
    for (SubscriberCallback subscriber : subscribers) {
        subscriber(keyCode);
    }
}

void ButtonNode::subscribe(SubscriberCallback cb) { subscribers.push_back(cb); }

void ButtonNode::unsubscribe(SubscriberCallback cb) {
    subscribers.erase(
        remove_if(subscribers.begin(), subscribers.end(),
                  [&cb](const ButtonNode::SubscriberCallback currCb) {
                      return currCb.target_type() == cb.target_type();
                  }),
        subscribers.end());
}

bool ButtonNode::onKeyPressed(unsigned int pressedKeyCode) {
    if (get<0>(keyCode) == pressedKeyCode ||
        get<1>(keyCode) == pressedKeyCode) {
        isPressed = true;

        notify(pressedKeyCode);

        return true;
    }

    isPressed = false;

    return false;
}

bool ButtonNode::canHaveChildren() const noexcept { return false; }