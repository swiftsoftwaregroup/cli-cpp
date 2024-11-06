#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <array>
#include <memory>
#include "greeting.hpp"

namespace fs = std::filesystem;

// Helper function to run the CLI and capture output
struct CliResult {
    int exit_code;
    std::string output;
};

CliResult run_cli(const std::vector<std::string>& args) {
    // Print current directory and its contents to understand where we are
    // std::cout << "\nWorking directory: " << fs::current_path().string() << "\n";
    // std::cout << "Directory contents:\n";
    // for (const auto& entry : fs::directory_iterator(fs::current_path())) {
    //     std::cout << "  " << entry.path().filename().string() << "\n";
    // }

    // Get absolute path to executable
    // fs::path project_root = fs::current_path();
    // fs::path exe_path = project_root / "cli-cpp";
    // std::string cmd = exe_path.string();

    // Use relative path to executable
    std::string cmd = "./cli-cpp";

    // Append arguments to command
    for (const auto& arg : args) {
        cmd += " " + arg;
    }
    cmd += " 2>&1";
    
    // Rest of the function remains the same
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    int status = pclose(pipe.release());
    auto exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    
    // Debug output
    // std::cout << "Command output: " << result << std::endl;
    // std::cout << "Exit code: " << exit_code << std::endl;
    
    return CliResult{exit_code, result};
}

class GreetingTest : public ::testing::Test {
protected:
    fs::path temp_dir;
    
    void SetUp() override {
        // Use /tmp on Unix systems or get from environment
        const char* tmp = std::getenv("TMPDIR");
        if (tmp == nullptr) {
            tmp = "/tmp";
        }
        temp_dir = fs::path(tmp) / "greeting_cli_test";
        fs::create_directories(temp_dir);
    }
    
    void TearDown() override {
        fs::remove_all(temp_dir);
    }
    
    fs::path create_test_file(const std::string& filename, const std::string& content) {
        fs::path file_path = temp_dir / filename;
        std::ofstream file(file_path);
        file << content;
        return file_path;
    }
};

// Direct function tests
TEST(GreetingFunctionTest, GenerateGreeting) {
    EXPECT_EQ(generate_greeting("Alice", "en"), "Hello, Alice!");
    EXPECT_EQ(generate_greeting("Carlos", "es"), "¡Hola, Carlos!");
    EXPECT_EQ(generate_greeting("Ivan", "bg"), "Здравей, Ivan!");
    EXPECT_TRUE(generate_greeting("Test", "fr").find("Unsupported language") != std::string::npos);
}

TEST_F(GreetingTest, ReadNameFromFile) {
    auto file_path = create_test_file("valid.txt", "John Smith");
    EXPECT_EQ(read_name_from_file(file_path.string()), "John Smith");
    
    // Test with whitespace
    file_path = create_test_file("whitespace.txt", "  Jane Doe  \n");
    EXPECT_EQ(read_name_from_file(file_path.string()), "Jane Doe");
    
    // Test empty file
    file_path = create_test_file("empty.txt", "");
    EXPECT_EQ(read_name_from_file(file_path.string()), "");
    
    // Test nonexistent file
    EXPECT_THROW(read_name_from_file("nonexistent.txt"), std::runtime_error);
    
    // Test unreadable file
    file_path = create_test_file("unreadable.txt", "Test");
    fs::permissions(file_path, fs::perms::none);
    EXPECT_THROW(read_name_from_file(file_path.string()), std::runtime_error);
    fs::permissions(file_path, fs::perms::owner_all); // Restore permissions for cleanup
}

// CLI integration tests
TEST_F(GreetingTest, CliBasicGreeting) {
    auto file_path = create_test_file("name.txt", "John");
    
    auto result = run_cli({"greet", file_path.string()});
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_TRUE(result.output.find("Hello, John!") != std::string::npos);
}

TEST_F(GreetingTest, CliSpanishGreeting) {
    auto file_path = create_test_file("name.txt", "Maria");
    
    auto result = run_cli({"greet", "-l", "es", file_path.string()});
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_TRUE(result.output.find("¡Hola, Maria!") != std::string::npos);
}

TEST_F(GreetingTest, CliBulgarianGreeting) {
    auto file_path = create_test_file("name.txt", "Ivan");
    
    auto result = run_cli({"greet", "--language", "bg", file_path.string()});
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_TRUE(result.output.find("Здравей, Ivan!") != std::string::npos);
}

TEST_F(GreetingTest, CliFileNotFound) {
    auto result = run_cli({"greet", "nonexistent.txt"});
    EXPECT_NE(result.exit_code, 0);
    EXPECT_TRUE(result.output.find("File does not exist") != std::string::npos);
}

TEST_F(GreetingTest, CliInvalidLanguage) {
    auto file_path = create_test_file("name.txt", "John");
    
    auto result = run_cli({"greet", "-l", "fr", file_path.string()});
    EXPECT_NE(result.exit_code, 0);
    EXPECT_TRUE(result.output.find("Invalid language code") != std::string::npos);
}

TEST_F(GreetingTest, CliHelpText) {
    auto result = run_cli({"--help"});
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_TRUE(result.output.find("CLI tool for greeting users") != std::string::npos);
}

TEST_F(GreetingTest, CliGreetHelpText) {
    auto result = run_cli({"greet", "--help"});
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_TRUE(result.output.find("greet") != std::string::npos);
    EXPECT_TRUE(result.output.find("language") != std::string::npos);
}