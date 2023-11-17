#include <any>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>

// Class template for options
template <typename T>
class Option
{
  public:
    std::vector<std::string> names;
    T argument;
    T defaultValue;

    // Constructor for an option with multiple names and a default value
    Option(std::initializer_list<std::string> optionNames, const T &defaultVal)
        : names(optionNames), defaultValue(defaultVal)
    {
    }

    // Constructor for an option with a single name and a default value
    Option(const std::string &optionName, const T &defaultVal)
        : names({optionName}), defaultValue(defaultVal)
    {
    }

    // Check if a string matches one of the option names
    bool matches(const std::string &optionName) const
    {
        return std::find(names.begin(), names.end(), optionName) != names.end();
    }
};

// Class template for commands
template <typename ReturnType, typename... ArgTypes>
class Command
{
  private:
    std::string name;
    std::function<ReturnType(ArgTypes...)> handler;
    std::tuple<Option<ArgTypes>...> options;

  public:
    // Constructor for a command with a name and handler
    Command(const std::string &commandName, const std::function<ReturnType(ArgTypes...)> &commandHandler,
            const Option<ArgTypes> &...commandOptions)
        : name(commandName), handler(commandHandler), options(commandOptions...)
    {
    }

    // Execute the command with the provided arguments
    void execute(const std::vector<std::string> &args) const
    {
        if (args.empty() || args[0] != name)
        {
            std::cerr << "Invalid command line format." << std::endl;
            return;
        }

        // Iterate over each option and compare it against the full range of args (except the first one)
        for (auto &option : options)
        {
            auto it = std::next(args.begin()); // Skip the command name

            // Find the first matching argument in the command line
            auto argIt = std::find_if(it, args.end(), [&option](const std::string &arg) {
                return option.matches(arg);
            });

            if (argIt != args.end())
            {
                // If a match is found, set the corresponding argument value
                std::istringstream(*std::next(argIt)) >> option.argument;
            }
            else
            {
                // If no match is found, set the default value
                option.argument = option.defaultValue;
            }
        }

        // Call the command handler with the extracted arguments
        handler(std::get<ArgTypes>(options).argument...);
    }
};

// Tokenize a string using std::quoted
std::vector<std::string> tokenizeQuoted(const std::string &input)
{
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    std::string token;
    while (iss >> std::quoted(token))
    {
        tokens.push_back(token);
    }

    return tokens;
}

// Example usage
int main()
{
    // Define options with default values
    Option<std::string> targetOption{"-t", "--target", "default_target"};
    Option<int> countOption{"-c", "--count", 42};
    Option<bool> verboseOption{"-v", "--verbose", true};

    // Define a command with options
    Command<void, std::string, int, bool> myCommand(
        "myCommand", [](const std::string &target, int count, bool verbose) {
            // Command handler logic
            std::cout << "Command executed with arguments:" << std::endl;
            std::cout << "  Target: " << target << std::endl;
            std::cout << "  Count: " << count << std::endl;
            std::cout << "  Verbose: " << std::boolalpha << verbose << std::endl;
        },
        targetOption, countOption, verboseOption);

    // Example of receiving the command line as a single string
    std::string commandLine = "myCommand --verbose --target \"example\"";

    // Tokenize command line and execute the command
    std::vector<std::string> args = tokenizeQuoted(commandLine);
    myCommand.execute(args);

    return 0;
}
