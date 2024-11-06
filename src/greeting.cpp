#include "greeting.hpp"
#include <fstream>
#include <map>
#include <stdexcept>

std::string read_name_from_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + file_path);
    }
    
    std::string name;
    std::getline(file, name);
    
    // Trim whitespace
    name.erase(0, name.find_first_not_of(" \t\n\r"));
    name.erase(name.find_last_not_of(" \t\n\r") + 1);
    
    return name;
}

std::string generate_greeting(const std::string& name, const std::string& language) {
    std::map<std::string, std::string> greetings = {
        {"en", "Hello, " + name + "!"},
        {"es", "¡Hola, " + name + "!"},
        {"bg", "Здравей, " + name + "!"}
    };
    
    auto it = greetings.find(language);
    if (it != greetings.end()) {
        return it->second;
    }
    return "Unsupported language: " + language;
}