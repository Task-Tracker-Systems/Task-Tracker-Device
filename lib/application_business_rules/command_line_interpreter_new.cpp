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

std::string unescapeString(const std::string &str)
{
    std::string result;
    bool escape = false;

    for (char ch : str)
    {
        if (escape)
        {
            // Handle escaped characters
            if (ch == '\\' || ch == '\"')
            {
                result += ch;
            }
            else
            {
                // Invalid escape sequence, treat as a regular character
                result += '\\';
                result += ch;
            }
            escape = false;
        }
        else
        {
            if (ch == '\\')
            {
                escape = true;
            }
            else
            {
                result += ch;
            }
        }
    }

    return result;
}

CommandLine parseCommandLine(const std::string &commandLine)
{
    CommandLine result;

    std::istringstream iss(commandLine);
    std::string token;
    bool inQuotedString = false;

    while (iss >> token)
    {
        if (token.size() >= 3 && token.substr(0, 3) == "---")
        {
            // Found an option
            Option option;
            option.name = token.substr(3); // Extract option name

            // Check if there is an argument for the option
            if (iss >> token)
            {
                // Handle quoted string
                if (token.front() == '\"')
                {
                    inQuotedString = true;
                    option.argument = token.substr(1); // Skip the opening quote
                }
                else
                {
                    option.argument = unescapeString(token);
                }

                // Continue reading tokens until the closing quote is found
                while (inQuotedString && iss >> token)
                {
                    option.argument += " " + unescapeString(token);
                    if (token.back() == '\"')
                    {
                        inQuotedString = false;
                        option.argument.pop_back(); // Remove the closing quote
                        break;
                    }
                }
            }

            result.options.push_back(option);
        }
        else
        {
            // Invalid argument format
            std::cerr << "Invalid argument format: " << token << std::endl;
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
