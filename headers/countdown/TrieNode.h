#ifndef COUNTDOWNWORDSEARCH_TRIENODE_H
#define COUNTDOWNWORDSEARCH_TRIENODE_H

#include <memory>
#include <vector>

namespace zmote::countdown {

    static int GLOBAL_ID{0};

    using TrieNodeSharedPtr = std::shared_ptr<class TrieNode>;
    using TrieNodeWeakPtr = std::weak_ptr<class TrieNode>;
    using TrieNodeVector = std::vector<TrieNodeSharedPtr>;

    class TrieNode : public std::enable_shared_from_this<TrieNode> {
        TrieNodeWeakPtr _parent;
        TrieNodeVector _children;
        bool _isEndNode = false;
        char _value;
        int _id = zmote::countdown::GLOBAL_ID++;

    private:
        auto find_element(char letter);

    public:
        TrieNode();

        explicit TrieNode(char value);

        TrieNodeSharedPtr &add_child(char letter);

        void mark_word_end();

        bool is_word_end() const;

        void set_parent(const TrieNodeSharedPtr &pParent);

        TrieNodeSharedPtr get_parent() const;

        bool has_next(char letter);

        TrieNodeSharedPtr &get_next(char letter);

        char val() const;

        int get_id() const;

        const TrieNodeVector &get_children() const;
    };
}
#endif //COUNTDOWNWORDSEARCH_TRIENODE_H
