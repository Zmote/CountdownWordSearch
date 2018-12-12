#include <iostream>
#include "headers/countdown/Trie.h"

int main(int argc, char **argv) {
    std::cout << R"(
    ***********************************************************
    * Welcome to the Countdown Word Search Tool. Follow the   *
    * instructions below to find words from random set of     *
    * letters. To quit, press: Ctrl + Z.                      *
    ***********************************************************
    )" << "\n";

    std::cout << "Initializing dictionary... please be patient...\n";
    zmote::countdown::Trie dictionary{zmote::countdown::DICTIONARY_TYPE::EN};
    std::cout << "Dictionary initiliazed, let's find some words!\n";
    std::cout << "Type a number to show only results up to and above: ";
    int minLetterCount{};
    std::cin >> minLetterCount;
    std::string input{};
    std::getline(std::cin, input);
    std::cout << "Your letters:\n";
    while (std::getline(std::cin, input)) {
        std::cout << "Does '" << input << "' exists? " << (dictionary.exists(input) ? "Yes." : "No.") << "\n";
        std::vector<std::string> found_words = dictionary.find_words(input);
        if (!found_words.empty()) {
            std::cout << "Words found with " << std::to_string(minLetterCount) << " or more letters:\n";
            for (auto const &word : found_words) {
                if (word.length() >= minLetterCount) {
                    std::cout << word << ", " << word.length() << " letter word." << std::endl;
                }
            }
        } else {
            std::cout << "Nothing found!\n";
        }
        std::cout << "Your letters:\n";
    }
}