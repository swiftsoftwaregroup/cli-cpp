#pragma once

#include <string>

/// Read a name from a specified file
///
/// This function opens a file and reads a single line containing a name.
/// Leading and trailing whitespace is automatically trimmed from the name.
///
/// @param file_path Path to the file containing the name
/// @return The name read from the file with whitespace trimmed
/// @throws std::runtime_error if the file cannot be opened
///
/// Example:
/// ```cpp
/// try {
///     std::string name = read_name_from_file("name.txt");
///     // Use the name...
/// } catch (const std::runtime_error& e) {
///     // Handle error...
/// }
/// ```
std::string read_name_from_file(const std::string& file_path);

/// Generate a greeting in the specified language
///
/// Supported languages:
/// - "en": English ("Hello, {name}!")
/// - "es": Spanish ("¡Hola, {name}!")
/// - "bg": Bulgarian ("Здравей, {name}!")
///
/// @param name The name to include in the greeting
/// @param language The language code for the greeting ('en', 'es', or 'bg')
/// @return The formatted greeting in the specified language
/// @note Returns "Unsupported language: {language}" if the language code is not supported
///
/// Example:
/// ```cpp
/// auto greeting = generate_greeting("John", "en"); // Returns "Hello, John!"
/// auto spanish = generate_greeting("Maria", "es"); // Returns "¡Hola, Maria!"
/// ```
std::string generate_greeting(const std::string& name, const std::string& language);