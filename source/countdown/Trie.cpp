#include <string>
#include <fstream>
#include <algorithm>
#include "../../headers/countdown/Trie.h"

namespace zmote::countdown {

    Trie::Trie(const std::string &file_path) {
        build_from(file_path);
    }

    void Trie::init_replacement_map() {
        _replacementMap.emplace("-", "HYPHEN");
        _replacementMap.emplace(",", "COMMA");
        _replacementMap.emplace("'", "QUOTATION");
        _replacementMap.emplace("&", "AND");
        _replacementMap.emplace(".", "DOT");
        _replacementMap.emplace("/", "SLASH");
        _replacementMap.emplace("!", "EXCLAMATION");
        _replacementMap.emplace("0", "ZERO");
        _replacementMap.emplace("1", "ONE");
        _replacementMap.emplace("2", "TWO");
        _replacementMap.emplace("3", "THREE");
        _replacementMap.emplace("4", "FOUR");
        _replacementMap.emplace("5", "FIVE");
        _replacementMap.emplace("6", "SIX");
        _replacementMap.emplace("7", "SEVEN");
        _replacementMap.emplace("8", "EIGHT");
        _replacementMap.emplace("9", "NINE");
        _replacementMap.emplace("10", "TEN");
    }

    std::string Trie::extract_around_letter(char letter, const std::string &input) {
        std::string extracted{};
        bool once_extracted = false;
        for (char i : input) {
            if (once_extracted || i != letter) {
                extracted += i;
            } else {
                once_extracted = true;
            }
        }
        return extracted;
    }

    void Trie::traverse_and_collect(const TrieNodeSharedPtr &p_node,
                                    TrieNodeVector &found_nodes,
                                    char active_char, const std::string &extracted) {
        if (p_node->has_next(active_char)) {
            if (p_node->is_word_end()) {
                found_nodes.emplace_back(p_node);
            }
            for (const char &letter : extracted) {
                traverse_and_collect(p_node->get_next(active_char), found_nodes, letter,
                                     extract_around_letter(letter, extracted));
            }
        } else {
            if (p_node->is_word_end()) {
                found_nodes.emplace_back(p_node);
            }
        }
    }

    std::string Trie::retrieve_word_from_node(TrieNodeSharedPtr &p_node) {
        std::string word{};
        while (p_node->get_parent()) {
            word += p_node->val();
            p_node = p_node->get_parent();
        }
        return std::string{word.rbegin(), word.rend()};
    }

    std::vector<std::string> Trie::calculate_words_from_nodes(TrieNodeVector &found_nodes) {
        std::set<std::string> calculated_words{};
        for (TrieNodeSharedPtr &node : found_nodes) {
            calculated_words.insert(retrieve_word_from_node(node));
        }
        std::vector<std::string> words = std::vector<std::string>{calculated_words.begin(), calculated_words.end()};
        std::sort(words.begin(), words.end(), [](const std::string &i, const std::string &j) {
            return i.length() < j.length();
        });
        return words;
    }

    void Trie::build_from(const std::string &file_path) {
        _root = std::make_shared<TrieNode>();
        std::ifstream file_stream{};
        file_stream.open(file_path, std::ifstream::in);
        if (file_stream.is_open()) {
            std::string line{};
            while (std::getline(file_stream, line)) {
                this->add_word(line);
            }
        }
    }

    void Trie::add_word(const std::string &p_word) {
        TrieNodeSharedPtr node = _root;
        std::for_each(p_word.cbegin(), p_word.cend(), [&node](char letter) {
            node = node->add_child(letter);
        });
        node->mark_word_end();
    }

    std::vector<std::string> Trie::find_words(std::string &input) {
        TrieNodeVector found_nodes{};
        std::string origin{input};
        for (int i = 0; i < origin.length(); i++) {
            traverse_and_collect(_root, found_nodes, origin[i], extract_around_letter(origin[i], origin));
        }
        return calculate_words_from_nodes(found_nodes);
    }

    bool Trie::exists(const std::string &p_word) {
        const std::string &input{p_word};
        bool _exists = true;
        TrieNodeVector found_nodes{};
        TrieNodeSharedPtr node = _root;
        std::for_each(input.cbegin(), input.cend(), [&](char letter) {
            if (node->has_next(letter)) {
                node = node->get_next(letter);
            } else {
                _exists = false;
                return;
            }
        });
        return _exists ? node->is_word_end() : _exists;
    }

    using VectorOfCharVectors = std::vector<std::vector<char>>;

    std::string Trie::get_replacement(const std::string &letter) {
        if (!letter.empty() && letter[0] == '\0') return "EMPTY";
        auto it = _replacementMap.find(letter);
        if (it != _replacementMap.end()) {
            return it->second;
        }
        return letter;
    };


    std::string Trie::calculate_gephi_edge(const TrieNodeSharedPtr &p_node, const TrieNodeSharedPtr &p_child) {
        std::string line{};
        line += std::to_string(p_node->get_id());
        line += ",";
        line += std::to_string(p_child->get_id());
        line += ",Directed,";
        line += std::to_string(++zmote::countdown::GLOBAL_ID);
        line += ",,,1\n";
        return line;
    }

    std::string Trie::calculate_gephi_node(const TrieNodeSharedPtr &p_node) {
        std::string line{};
        line += std::to_string(p_node->get_id());
        line += ",";
        line += get_replacement(std::string{p_node->val()});
        line += ",\n";
        return line;
    }

    std::pair<std::string, std::string> Trie::construct_gephi_pair(const TrieNodeSharedPtr &p_node, int level) {
        std::string node{calculate_gephi_node(p_node)};
        std::string edges{};
        for (const TrieNodeSharedPtr &p_child: p_node->get_children()) {
            edges += calculate_gephi_edge(p_node, p_child);
        }
        return std::pair<std::string, std::string>{node, edges};
    }

    void Trie::calculate_trie_recursive(const TrieNodeSharedPtr &p_node,
                                        std::set<std::pair<std::string, std::string>> &nodes, int level) {
        if (!p_node->get_children().empty()) {
            for (const TrieNodeSharedPtr &p_child:p_node->get_children()) {
                calculate_trie_recursive(p_child, nodes, level + 1);
            }
            nodes.insert(construct_gephi_pair(p_node, level));
        } else {
            nodes.insert(construct_gephi_pair(p_node, level));
        }
    }

    std::set<std::pair<std::string, std::string>> Trie::calculate_trie_representation() {
        init_replacement_map();
        zmote::countdown::GLOBAL_ID = 0;
        std::set<std::pair<std::string, std::string>> nodes{};
        calculate_trie_recursive(_root, nodes, 0);
        return nodes;
    }

    void Trie::writeTrieToCSV(const std::string &file_name) {
        auto nodes_and_edges = calculate_trie_representation();
        std::ofstream nodes{file_name + "_nodes.csv"};
        std::ofstream edges{file_name + "_edges.csv"};
        std::set<std::string> nodes_set{};
        std::set<std::string> edges_set{};
        for (const auto &pair: nodes_and_edges) {
            nodes_set.emplace(pair.first);
            edges_set.emplace(pair.second);
        }

        std::string nodes_all{"Id,Label,timeset\n"};
        for (const std::string &node_string: nodes_set) {
            nodes_all += node_string;
        }

        std::string edges_all{"Source,Target,Type,Id,Label,timeset,Weight\n"};
        for (const std::string &edge_string: edges_set) {
            edges_all += edge_string;
        }
        nodes << nodes_all;
        edges << edges_all;

        nodes.close();
        edges.close();
    }
}