#ifndef COUNTDOWNWORDSEARCH_TRIENODE_H
#define COUNTDOWNWORDSEARCH_TRIENODE_H

#include <memory>
#include <vector>

namespace zmote::countdown {
    class TrieNode;

    using TrieNodeSharedPtr = std::shared_ptr<TrieNode>;
    using TrieNodeWeakPtr = std::weak_ptr<TrieNode>;
    using TrieNodeVector = std::vector<TrieNodeSharedPtr>;

    class TrieNode : public std::enable_shared_from_this<TrieNode> {
        TrieNodeWeakPtr _parent;
        TrieNodeVector _children;
        bool _isEndNode = false;
        char _value;

    private:
        auto find_element(char letter);

    public:
        TrieNode();

        explicit TrieNode(char value);

        TrieNodeSharedPtr &add_child(char letter);

        void mark_word_end();

        bool is_word_end() const;

        void set_parent(TrieNodeSharedPtr const &pParent);

        TrieNodeSharedPtr get_parent() const;

        bool has_next(char letter);

        TrieNodeSharedPtr &get_next(char letter);

        char val() const;
    };
}
#endif //COUNTDOWNWORDSEARCH_TRIENODE_H
