#include <iostream>
#include "headers/countdown/Trie.h"

void init_continous_mode();

void process_commands(int argc, char **argv);

void process_help();

void process_exists(int argc, char *const *argv);

void process_find(int argc, char *const *argv);

int main(int argc, char **argv) {
    if (argc > 1 && argc < 5) {
        process_commands(argc, argv);
    } else {
        if (argc > 1) {
            std::cout << R"(
Wrong amount of arguments. Type -help to get more info on available commands.
            )" << "\n";
        } else {
            init_continous_mode();
        }
    }
}

void process_commands(int argc, char **argv) {
    std::string flag{argv[1]};
    if (flag == "-find") {
        process_find(argc, argv);
    } else if (flag == "-help") {
        process_help();
    } else if (flag == "-exists") {
        process_exists(argc, argv);
    } else {
        std::cout << R"(Flag not found. Type -help to see all available flags.)" << "\n";
    }
}

void process_find(int argc, char *const *argv) {
    if (argc > 4) {
            std::cout << "Illformed command. Checkout -help to see how to use this command.\n";
        } else {
            zmote::countdown::Trie dictionary{zmote::countdown::DICTIONARY_TYPE::EN};
            std::string input{argv[2]};
            int limit{-1};
            try {
                limit = std::stoi(argv[3]);
            } catch (std::exception const &ex) {
                std::cout
                        << "Min. Limit conversion failed. Did you type a number? Check out the usage of this command with -help\n";
                std::cout << "Continuing with default of min 3 letter words.\n";
            }
            std::vector<std::string> found_words = dictionary.find_words(input);
            if (!found_words.empty()) {
                int hiddenCount{0};
                for (auto const &word : found_words) {
                    if (limit < 0 || word.length() >= limit) {
                        std::cout << word << ", " << word.length() << " letter word.\n";
                    } else {
                        ++hiddenCount;
                    }
                }
                if (hiddenCount > 0) {
                    std::cout << std::to_string(hiddenCount) << " Results were hidden due to limit.\n";
                }
            } else {
                std::cout << "No words found!\n";
            }
        }
}

void process_exists(int argc, char *const *argv) {
    if (argc != 3) {
            std::cout << "Illformed command. Checkout -help to see correct usage.\n";
        } else {
            zmote::countdown::Trie dictionary{zmote::countdown::DICTIONARY_TYPE::EN};
            std::cout << "Does '" << argv[2] << "' exists? " << (dictionary.exists(argv[2]) ? "Yes." : "No.")
                      << "\n";
        }
}

void process_help() {
    std::cout << R"(
------------------------------------------------------------------------------------------------------
Available commands:
[Empty]     If you start the tool without any flag or arguments, the tool will initiate a loop
            Where you can continiously type in letters to be searched in the dictionary.
-find       Finds words according to inputted letters. Usage: -find [letters] [min-size of found words]
            Example: -find satulyxl 6
-exists     Check if a word exists in the library. Usage: -exists [word],
            Example: -exists Vinegar
-help       Print commands list. Ie. what you're seeing now.
------------------------------------------------------------------------------------------------------
)" << "\n";
}

void init_continous_mode() {
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
    std::__cxx11::string input{};
    getline(std::cin, input);
    std::cout << "Your letters:\n";
    while (getline(std::cin, input)) {
        std::cout << "Does '" << input << "' exists? " << (dictionary.exists(input) ? "Yes." : "No.") << "\n";
        std::vector<std::__cxx11::string> found_words = dictionary.find_words(input);
        if (!found_words.empty()) {
            std::cout << "Words found with " << std::__cxx11::to_string(minLetterCount) << " or more letters:\n";
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