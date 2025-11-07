#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <algorithm>
#include <cctype>

// trim helpers
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        [](unsigned char ch){ return !std::isspace(ch); }));
}
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
        [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
}
static inline void trim(std::string &s) { ltrim(s); rtrim(s); }

int main() {
    std::cout << "Simple C++ Chatbot (type 'exit' or 'quit' to stop)\n\n";

    // Map of patterns -> responses. Patterns are regex strings.
    std::map<std::string, std::string> responses = {
        {R"(\bhello\b|\bhi\b|\bhey\b)", "Hi there! How can I help you today?"},
        {R"(\bhow are you\b|\bhow's it going\b)", "I'm a program, so I'm always running—thanks for asking!"},
        {R"(\bwhat(?:'s| is) your name\b|\bwho are you\b)", "I'm a simple C++ chatbot. You can extend my responses in the source code."},
        {R"(\bbye\b|\bgoodbye\b|\bsee you\b)", "Goodbye! Take care."},
        {R"(\bhelp\b|\bcommands\b)", "You can say hello, ask how I am, ask my name, or say bye. Type 'exit' to quit."},
        {R"(\bthank(s| you)?\b)", "You're welcome! Anything else I can help with?"}
    };

    std::string input;
    while (true) {
        std::cout << "\nYou: ";
        if (!std::getline(std::cin, input)) break; // EOF or error
        trim(input);
        if (input.empty()) continue;

        // check exit first
        std::string lower = input;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c); });
        if (lower == "exit" || lower == "quit") {
            std::cout << "Bot: Bye — ending chat.\n";
            break;
        }

        bool matched = false;
        for (const auto &pair : responses) {
            try {
                std::regex pattern(pair.first, std::regex_constants::icase | std::regex_constants::ECMAScript);
                if (std::regex_search(input, pattern)) {
                    std::cout << "Bot: " << pair.second << '\n';
                    matched = true;
                    break;
                }
            } catch (const std::regex_error &e) {
                // If a bad regex was supplied, skip it (useful while developing)
                continue;
            }
        }

        if (!matched) {
            std::cout << "Bot: Sorry, I don't understand. Try asking something else or type 'help'.\n";
        }
    }

    return 0;
}

