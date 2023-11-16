#include <cstring>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <trim.hpp>
#include <tuple>
#include <vector>

/**
 * Reads and removes an object from a string.
 * 
 * \tparam DataType is the type of the object in question
 * \param sentence is the string to be inspected
 * \returns a container optionally holding the searched object
 */
template <typename DataType>
std::optional<DataType> extractData(std::string &sentence)
{
    std::istringstream istream(sentence);
    std::ostringstream remainder;
    DataType data;
    // read object from string
    istream >> data;

    // check if extraction was successful
    if (!istream.fail())
    {
        // copy the rest of the string
        remainder << istream.rdbuf();
        // replace the given string with the remainder
        sentence = remainder.str();
        return data;
    }

    return std::nullopt;
}

/**
 * Combines a command option with an argument.
 * \tparam T is the type of the argument
 */
template <class T>
struct Argument
{
    /**
     * Accepts a variety of notations for a command option.
     */
    std::vector<const char *> labels;

    /**
     * Extracts an option from the string matching the argument.
     * 
     * Separator characters are allowed between the label and the actual value.
     * 
     * \param argumentsToExtract the string to be parsed; found option will be removed from string
     * \return the found data from the option or the default value if not found
     */
    T extract(std::string &argumentsToExtract) const
    {
        // find an option label
        auto endLabel = std::string::npos; // points to the character next to the label
        std::string::size_type startLabel;
        for (const auto label : labels)
        {
            startLabel = argumentsToExtract.find(label);
            if (startLabel != std::string::npos)
            {
                endLabel = startLabel + std::strlen(label);
                break;
            }
        }
        if (endLabel == std::string::npos)
        {
            return defaultValue;
        }

        // search for beginning of argument
        constexpr const char *const separator = " =";
        const auto beginArgument = argumentsToExtract.find_first_not_of(separator, endLabel);
        if (beginArgument == std::string::npos)
        {
            return defaultValue;
        }

        // extract data
        // everything behind the label and separator
        auto remainder = trim(argumentsToExtract.substr(beginArgument));
        // extracts an object of the given type from the beginning of the remaining string
        const auto foundData = extractData<T>(remainder);
        if (foundData)
        {
            const auto preamble = argumentsToExtract.substr(0, startLabel);
            // the given string will be replaced by the substring before and after the extracted option
            argumentsToExtract = preamble + remainder;
            return foundData.value();
        }
        else
        {
            return defaultValue;
        }
    }

    T defaultValue;
};

/**
 * Combines a command with a function.
 * 
 * The order of the argument parsers must match the order of the 
 * parameters of the function.
 * 
 * \tparam ReturnType return type of the function
 * \tparam Arguments parameter types of the function
 */
template <typename ReturnType, typename... Arguments>
struct Command
{
    const char *commandName;
    std::tuple<const Argument<Arguments> *...> argumentParsers;
    std::function<ReturnType(Arguments...)> function;

    /**
     * Parses a string and calls the function if command was found.
     * 
     * The order of the arguments in the string does not matter.
     * 
     * \param commandToParse Is the string to be interpreted.
     * \retval true in case command was found and function called
     * @return false in case command was not found
     */
    bool parse(std::string commandToParse, ReturnType *const pReturnValue = nullptr) const
    {
        std::cout << "Command to parse: '" << commandToParse << "'" << std::endl;
        const auto someCommandNameFound = extractData<std::string>(commandToParse);
        if (!someCommandNameFound)
        {
            return false;
        }

        if (someCommandNameFound.value().compare(commandName) != 0)
        {
            return false; // OK: apparently this command was not meant to be called
        }

        // unpack the collection of parsers
        return std::apply(
            [&](auto &...argumentParser) {
                const std::tuple<const Arguments...> extractedArguments = {argumentParser->extract(commandToParse)...};
                std::cout << "Rest of command = '" << commandToParse << "'" << std::endl;

                // check if some part of the command string was invalid and thus not extracted
                if (trim(commandToParse).length() > 0)
                {
                    throw std::runtime_error("invalid arguments to command");
                    return false;
                }

                // call function after passing command through all parsers
                const auto handler = [&]() { return std::apply(function, extractedArguments); };
                if constexpr (!std::is_same_v<ReturnType, void>)
                {
                    if (pReturnValue)
                    {
                        *pReturnValue = handler();
                    }
                    else
                    {
                        handler();
                    }
                }
                else
                {
                    handler();
                }
                return true;
            },
            argumentParsers);
    }
};

/**
 * Creates \ref Command object while deducing template arguments for its type.
 * 
 * This is a helper function to deduce the type of the created object using the
 * types of the arguments to this function.
 * 
 * \tparam ReturnType is the return type of the function to be called
 * \tparam Arguments  are the parameters of the function to be called
 * \param commandName is a string representing the name of the command
 * \param argumentParsers is a collection of parsers for extracting the arguments
 * \param function is the function to be called
 * \returns an object of \ref Command
 */
template <typename ReturnType, typename... Arguments>
Command<ReturnType, Arguments...> makeCommand(
    const char *const commandName,
    const std::tuple<const Argument<Arguments> *...> &argumentParsers,
    const std::function<ReturnType(Arguments...)> &function)
{
    Command<ReturnType, Arguments...> command = {
        .commandName = commandName,
        .argumentParsers = argumentParsers,
        .function = function,
    };
    return command;
}
