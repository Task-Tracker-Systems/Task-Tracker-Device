#include <functional>
#include <optional>
#include <sstream>
#include <string>
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
    std::ostringstream remainer;
    DataType data;
    // read object from string
    istream >> data;

    // check if extraction was successful
    if (!istream.fail())
    {
        // copy the rest of the string
        remainer << istream.rdbuf();
        // replace the given string with the remainder
        sentence = remainer.str();
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
        auto startLabel = std::string::npos;
        for (const auto label : labels)
        {
            startLabel = argumentsToExtract.find(label);
            if (startLabel != std::string::npos)
            {
                break;
            }
        }

        // extract data
        if (startLabel != std::string::npos)
        {
            constexpr const char *const seperator = " =,;";
            const auto label_end = argumentsToExtract.find_first_of(seperator, startLabel + 1);
            if (label_end != std::string::npos)
            {
                // everything behind the label and separator
                auto remainer = argumentsToExtract.substr(label_end + 1);
                // extracts an object of the given type from the beginning of the remaining string
                const auto foundData = extractData<T>(remainer);
                if (foundData)
                {
                    const auto preamble = argumentsToExtract.substr(0, startLabel);
                    // the given string will be replaced by the substring before and after the extracted option
                    argumentsToExtract = preamble + remainer;
                    return foundData.value();
                }
            }
        }

        return defaultValue;
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
    const bool parse(std::string commandToParse) const
    {
        const auto foundCommandName = extractData<std::string>(commandToParse);
        if (foundCommandName)
        {
            if (foundCommandName.value().compare(commandName) == 0)
            {
                // unpack the collection of parsers
                std::apply(
                    [&](auto &...argumentParser) {
                        // call function after passing command through all parsers
                        function(argumentParser->extract(commandToParse)...);
                    },
                    argumentParsers);
                return true;
            }
        }
        return false;
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
