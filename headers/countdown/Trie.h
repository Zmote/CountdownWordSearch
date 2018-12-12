#ifndef COUNTDOWNWORDSEARCH_TRIE_H
#define COUNTDOWNWORDSEARCH_TRIE_H

#include <map>
#include <set>
#include "TrieNode.h"

namespace zmote::countdown {

    enum class DICTIONARY_TYPE{EN};

    using StringVector = std::vector<std::string>;

    class Trie {
        TrieNodeSharedPtr _root{std::make_shared<TrieNode>()};

    private:
        std::string extract_around_letter(char letter, std::string const &input);

        // Traverses tree by choosing trie paths according to available characters, collect
        // nodes that mark an end of a word to later retrieve the word by traveling upwards to root.
        void traverse_and_collect(TrieNodeSharedPtr const &p_node,
                                  TrieNodeVector &found_nodes,
                                  char const &active_char,
                                  std::string const &extracted);

        std::string retrieve_word_from_node(TrieNodeSharedPtr &p_node);

        StringVector calculate_words_from_nodes(TrieNodeVector &found_nodes);

    public:

        Trie() = default;

        explicit Trie(DICTIONARY_TYPE type);
        explicit Trie(std::string const &file_path);

        void build_from(std::string const &file_path);

        void add_word(std::string const &p_word);

        StringVector find_words(std::string &input);

        bool exists(std::string const &p_word);
    };
}

#endif //COUNTDOWNWORDSEARCH_TRIE_H
