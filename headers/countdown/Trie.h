#ifndef COUNTDOWNWORDSEARCH_TRIE_H
#define COUNTDOWNWORDSEARCH_TRIE_H

#include <map>
#include <set>
#include "TrieNode.h"

namespace zmote::countdown {
    class Trie {
        TrieNodeSharedPtr _root{std::make_shared<TrieNode>()};
        std::map<std::string, std::string> _replacementMap{};

    private:
        std::string extract_around_letter(char letter, const std::string &input);

        // Traverses tree by choosing trie paths according to available characters, collect
        // nodes that mark an end of a word to later retrieve the word by traveling upwards to root.
        void traverse_and_collect(const TrieNodeSharedPtr &p_node,
                                  TrieNodeVector &found_nodes,
                                  char active_char, const std::string &extracted);

        std::string retrieve_word_from_node(TrieNodeSharedPtr &p_node);

        std::vector<std::string> calculate_words_from_nodes(TrieNodeVector &found_nodes);

        void init_replacement_map();

        std::string calculate_gephi_node(const TrieNodeSharedPtr &p_node);

        std::string calculate_gephi_edge(const TrieNodeSharedPtr &p_node, const TrieNodeSharedPtr &p_child);

        std::pair<std::string,std::string> construct_gephi_pair(const TrieNodeSharedPtr &p_node);

        void calculate_trie_recursive(const TrieNodeSharedPtr &p_node, std::set<std::pair<std::string,std::string>> & nodes);

        std::string get_replacement(const std::string &letter);

        std::set<std::pair<std::string, std::string>> calculate_trie_representation();

    public:

        Trie() = default;

        explicit Trie(const std::string &file_path);

        void build_from(const std::string &file_path);

        void add_word(const std::string &p_word);

        std::vector<std::string> find_words(std::string &input);

        bool exists(const std::string &p_word);

        void writeTrieToCSV(const std::string &file_name);
    };
}

#endif //COUNTDOWNWORDSEARCH_TRIE_H
