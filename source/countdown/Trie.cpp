#include <string>
#include <set>
#include <fstream>
#include <algorithm>
#include "../../headers/countdown/Trie.h"

namespace zmote::countdown {

    Trie::Trie(const std::string &filePath) {
        build_from(filePath);
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

    void Trie::add_word(const std::string &pWord) {
        TrieNodeSharedPtr node = _root;
        std::for_each(pWord.cbegin(), pWord.cend(), [&node](char letter) {
            node = node->add_child(letter);
        });
        node->mark_word_end();
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
    };
}