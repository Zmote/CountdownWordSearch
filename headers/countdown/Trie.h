#ifndef COUNTDOWNWORDSEARCH_TRIE_H
#define COUNTDOWNWORDSEARCH_TRIE_H

#include <map>
#include <set>
#include "TrieNode.h"

namespace zmote::countdown {
    using StringVector = std::vector<std::string>;
    class Trie {
        TrieNodeSharedPtr _root{std::make_shared<TrieNode>()};

    private:
        std::string extract_around_letter(char letter, const std::string &input);

        // Traverses tree by choosing trie paths according to available characters, collect
        // nodes that mark an end of a word to later retrieve the word by traveling upwards to root.
        void traverse_and_collect(const TrieNodeSharedPtr &p_node,
                                  TrieNodeVector &found_nodes,
                                  const char &active_char, const std::string &extracted);

        std::string retrieve_word_from_node(TrieNodeSharedPtr &p_node);

        StringVector calculate_words_from_nodes(TrieNodeVector &found_nodes);

    public:

        Trie() = default;

        explicit Trie(const std::string &file_path);

        void build_from(const std::string &file_path);

        void add_word(const std::string &p_word);

        StringVector find_words(std::string &input);

        bool exists(const std::string &p_word);
    };
}

#endif //COUNTDOWNWORDSEARCH_TRIE_H
