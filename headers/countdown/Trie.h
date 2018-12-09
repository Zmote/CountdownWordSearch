#ifndef COUNTDOWNWORDSEARCH_TRIE_H
#define COUNTDOWNWORDSEARCH_TRIE_H

#include <string>
#include <set>
#include "TrieNode.h"

namespace zmote::countdown {
    using TrieNode = zmote::countdown::TrieNode;

    class Trie {
        std::shared_ptr<TrieNode> _root = std::make_shared<TrieNode>();
    private:
        std::string extract_around_letter(char letter, std::string &input) {
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

    public:

        Trie() = default;

        explicit Trie(const std::string &filePath) {
            build_from(filePath);
        }

        void build_from(const std::string &file_path) {
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

        void add_word(const std::string &pWord) {
            TrieNode *node = _root.get();
            std::for_each(pWord.cbegin(), pWord.cend(), [&node](char letter) {
                node = node->add_child(letter).get();
            });
            node->mark_word_end();
        }

        // Traverses tree by choosing trie paths according to available characters, collect
        // nodes that mark an end of a word to later retrieve the word by traveling upwards to root.
        void traverse_and_collect(TrieNode *p_node, std::vector<std::shared_ptr<TrieNode>> &found_nodes,
                                  char active_char, std::string extracted) {
            if (p_node->has_next(active_char)) {
                if (p_node->is_word_end()) {
                    found_nodes.push_back(std::make_shared<TrieNode>(*p_node));
                }
                for (char letter : extracted) {
                    traverse_and_collect(p_node->get_next(active_char).get(), found_nodes, letter,
                                         extract_around_letter(letter, extracted));
                }
            } else {
                if (p_node->is_word_end()) {
                    found_nodes.push_back(std::make_shared<TrieNode>(*p_node));
                }
            }
        }

        std::string retrieve_word_from_node(TrieNode *p_node) {
            std::string word{};
            while (p_node->get_parent() != nullptr) {
                word += p_node->val();
                p_node = p_node->get_parent();
            }
            return std::string{word.rbegin(), word.rend()};
        }

        std::vector<std::string> calculate_words_from_nodes(std::vector<std::shared_ptr<TrieNode>> &found_nodes) {
            std::set<std::string> calculated_words{};
            for (std::shared_ptr<TrieNode> &node : found_nodes) {
                std::string word{};
                word += node->val();
                calculated_words.insert(retrieve_word_from_node(node.get()));
            }
            std::vector<std::string> words = std::vector<std::string>{calculated_words.begin(), calculated_words.end()};
            std::sort(words.begin(), words.end(), [](const std::string &i, const std::string &j) {
                return i.length() < j.length();
            });
            return words;
        }

        std::vector<std::string> find_words(std::string &input) {
            std::vector<std::shared_ptr<TrieNode>> found_nodes{};
            std::string origin{input};
            for (int i = 0; i < origin.length(); i++) {
                traverse_and_collect(_root.get(), found_nodes, origin[i], extract_around_letter(origin[i], origin));
            }
            return calculate_words_from_nodes(found_nodes);
        }

        bool exists(const std::string &p_word) {
            const std::string &input{p_word};
            bool _exists = true;
            TrieNode *node = _root.get();
            std::for_each(input.cbegin(), input.cend(), [&node, &_exists](char letter) {
                if (node->has_next(letter)) {
                    node = node->get_next(letter).get();
                } else {
                    _exists = false;
                    return;
                }
            });
            return _exists ? node->is_word_end() : _exists;
        };
    };
}

#endif //COUNTDOWNWORDSEARCH_TRIE_H
