#include <algorithm>
#include "../../headers/countdown/TrieNode.h"

namespace zmote::countdown {

    TrieNode::TrieNode() : _value{'\0'} {}

    TrieNode::TrieNode(char value) : _value{value} {}

    auto TrieNode::find_element(char letter) {
        return std::find_if(_children.begin(), _children.end(),
                            [&letter](TrieNodeSharedPtr &shared_ptr) {
                                return tolower(shared_ptr->val()) == tolower(letter);
                            });
    }

    TrieNodeSharedPtr &TrieNode::add_child(char letter) {
        auto it = find_element(letter);
        if (it == _children.end()) {
            _children.emplace_back(std::make_shared<TrieNode>(tolower(letter)));
            _children.back()->set_parent(shared_from_this());
            return _children.back();
        }
        return *it;
    }

    void TrieNode::mark_word_end() {
        _isEndNode = true;
    }

    bool TrieNode::is_word_end() const {
        return _isEndNode;
    }

    void TrieNode::set_parent(const TrieNodeSharedPtr &pParent) {
        _parent = pParent;
    }

    TrieNodeSharedPtr TrieNode::get_parent() const {
        return _parent.lock();
    }

    bool TrieNode::has_next(char letter) {
        auto it = find_element(letter);
        return it != _children.end();
    }

    TrieNodeSharedPtr &TrieNode::get_next(char letter) {
        if (has_next(letter)) {
            return *find_element(letter);
        }
        throw std::out_of_range{"No next node found."};
    }

    char TrieNode::val() const {
        return _value;
    }

    const TrieNodeVector &TrieNode::get_children() const {
        return _children;
    }

    int TrieNode::get_id() const {
        return _id;
    }
}