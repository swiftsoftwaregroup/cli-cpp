#include <iostream>
#include <filesystem>
#include <set>
#include <argparse/argparse.hpp>
#include "greeting.hpp"

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("greeting-cli");
    program.add_description("CLI tool for greeting users in different languages.");

    // Create greet subcommand
    argparse::ArgumentParser greet_cmd("greet");
    greet_cmd.add_description("Greet a user based on a name in a file.");
    
    // Add file argument to greet subcommand
    greet_cmd.add_argument("file")
        .help("Path to the file containing the name")
        .action([](const std::string& value) {
            if (!std::filesystem::exists(value)) {
                throw std::runtime_error("File does not exist: " + value);
            }
            return value;
        });
    
    // Add language option to greet subcommand
    greet_cmd.add_argument("-l", "--language")
        .default_value(std::string("en"))
        .help("Language for the greeting (en: English, es: Spanish, bg: Bulgarian)")
        .action([](const std::string& value) {
            std::set<std::string> valid_languages = {"en", "es", "bg"};
            if (valid_languages.find(value) == valid_languages.end()) {
                throw std::runtime_error("Invalid language code. Must be one of: en, es, bg");
            }
            return value;
        });

    // Add greet subcommand to main program
    program.add_subparser(greet_cmd);

    try {
        program.parse_args(argc, argv);
        
        // Handle greet subcommand
        if (program.is_subcommand_used("greet")) {
            std::string file_path = greet_cmd.get<std::string>("file");
            std::string language = greet_cmd.get<std::string>("--language");
            
            std::string name = read_name_from_file(file_path);
            std::string greeting = generate_greeting(name, language);
            
            std::cout << greeting << std::endl;
        } else {
            // No subcommand specified, show help
            std::cout << program;
        }
        
        return 0;
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
}