#ifndef COUNTDOWNWORDSEARCH_TRIENODE_H
#define COUNTDOWNWORDSEARCH_TRIENODE_H

#include <memory>
#include <algorithm>
#include <vector>

// TODO: Make parent access more secure, raw pointer is a temporary solution
namespace zmote::countdown {
    class TrieNode {
        TrieNode *_parent;
        std::vector<std::shared_ptr<TrieNode>> _children;
        bool _isEndNode = false;
        char _value;
    private:
        auto find_element(char letter) {
            return std::find_if(_children.begin(), _children.end(),
                                [&letter](std::shared_ptr<TrieNode> &shared_ptr) {
                                    return tolower(shared_ptr->val()) == tolower(letter);
                                });
        }

    public:
        TrieNode() : _value{'\0'}, _parent{nullptr} {}

        explicit TrieNode(char value) : _value{value}, _parent{nullptr} {}

        std::shared_ptr<TrieNode> &add_child(char letter) {
            auto it = find_element(letter);
            if (it == _children.end()) {
                _children.emplace_back(std::make_shared<TrieNode>(tolower(letter)));
                _children.back()->set_parent(this);
                return _children.back();
            }
            return *it;
        }

        void mark_word_end() {
            _isEndNode = true;
        }

        bool is_word_end() const {
            return _isEndNode;
        }

        void set_parent(TrieNode *pParent) {
            _parent = pParent;
        }

        TrieNode *get_parent() const {
            return _parent;
        }

        bool has_next(char letter) {
            auto it = find_element(letter);
            return it != _children.end();
        }

        std::shared_ptr<TrieNode> &get_next(char letter) {
            if (has_next(letter)) {
                return *find_element(letter);
            }
            throw std::out_of_range{"No next node found."};
        }

        char val() const {
            return _value;
        }
    };
}
#endif //COUNTDOWNWORDSEARCH_TRIENODE_H
