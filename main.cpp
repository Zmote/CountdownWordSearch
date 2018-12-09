#include <iostream>
#include <fstream>
#include "headers/countdown/Trie.h"


//TODO: Implement input with console arguments, so that the tool can be used standalone
//TODO: Compile word library so command can be used independent of txt-file

int main() {
    std::cout << "Initializing dictionary... please be patient...\n";
    zmote::countdown::Trie dictionary{"./resources/words.txt"};

    std::string input{};
    std::cout << "Check if a word exists in the dictionary:\n";
    std::cout << "Your word:\n";
    while (std::getline(std::cin, input)) {
        std::cout << "Does " << input << " exists? " << (dictionary.exists(input) ? "Yes" : "No") << "\n";
        std::vector<std::string> found_words = dictionary.find_words(input);
        if (!found_words.empty()) {
            std::cout << "Words found bigger than 7 letters:\n";
            for (const auto &word : found_words) {
                if (word.length() >= 7) {
                    std::cout << word << ", " << word.length() << " letter word." << std::endl;
                }
            }
        } else {
            std::cout << "Nothing found!\n";
        }
        std::cout << "Your word:\n";
    }
}