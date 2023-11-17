#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

struct Option
{
    std::string name;
    std::string argument;
};

struct CommandLine
{
    std::string command;
    std::vector<Option> options;
};

CommandLine parseCommandLine(const std::string &commandLine)
{
    CommandLine result;

    std::istringstream iss(commandLine);
    std::string token;

    // First token is the command
    iss >> result.command;

    while (iss >> std::quoted(token))
    {
        // The entire token is considered as the option name
        Option option;
        option.name = token;

        // Check if there is an argument for the option
        if (iss >> std::quoted(option.argument))
        {
            result.options.push_back(option);
        }
        else
        {
            // Missing argument for the option
            std::cerr << "Missing argument for option: " << option.name << std::endl;
            std::exit(1); // Exit with an error code
        }
    }

    return result;
}

int main()
{
    // Example of receiving the command line as a single string
    std::string commandLine = "command ---option1 arg1 ---option2 \"arg with spaces\"";

    CommandLine cmdLine = parseCommandLine(commandLine);

    // Print the parsed command and options
    std::cout << "Command: " << cmdLine.command << std::endl;
    std::cout << "Options:" << std::endl;
    for (const auto &option : cmdLine.options)
    {
        std::cout << "  Name: " << option.name << ", Argument: " << option.argument << std::endl;
    }

    return 0;
}
