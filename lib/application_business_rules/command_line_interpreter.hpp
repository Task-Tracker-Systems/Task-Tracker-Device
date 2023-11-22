#include <functional>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string_helpers.hpp>
#include <tuple>

namespace command_line_interpreter
{

/**
 * Combines a command option with an argument.
 * \tparam T is the type of the argument
 */
template <class T, typename CharType = char>
struct Option
{
    typedef CharType CharT;
    typedef T ArgumentType;

    /**
     * Accepts a variety of notations for a command option.
     */
    std::vector<const CharT *> labels;

    /**
     * Checks if the name matches one of the labels valid for this option.
     * @param optionName the string to compare against
     * @return true in case it matches one of the valid labels
     * @return false else
     */
    bool doesMatchName(const CharT *const optionName) const
    {
        return std::find_if(std::begin(labels), std::end(labels), [&optionName](const auto candidate) {
                   return strcmp_g(candidate, optionName) == 0;
               }) != labels.end();
    }

    ArgumentType extractArgument(std::vector<std::basic_string<CharT>> &labelValuePairs) const
    {
        const auto itAllOptions = std::begin(labelValuePairs); // Skip the command name

        // Find the first matching argument in the command line
        const auto argIt = std::find_if(itAllOptions, std::end(labelValuePairs), [this](const auto &arg) {
            return doesMatchName(arg.c_str());
        });

        if (argIt != std::end(labelValuePairs))
        {
            // If a match is found, set the corresponding argument value
            const auto itArgValueString = std::next(argIt);
            const auto &argValueString = *itArgValueString;
            ArgumentType argument;
            if constexpr (std::is_same_v<decltype(argument), std::basic_string<CharT>>)
            {
                argument = argValueString;
            }
            else
            {
                std::basic_istringstream<CharT> iss(argValueString);
                iss >> argument;
                if (iss.fail())
                {
                    throw std::runtime_error("argument to option " + std::basic_string<CharT>(labels[0]) + " could not be parsed: '" + argValueString + "'");
                }
            }
            labelValuePairs.erase(argIt, std::next(itArgValueString));
            return argument;
        }
        else
        {
            // If no match is found, set the default value
            return defaultValue;
        }
    }

    T defaultValue;
};

/**
 * Combines a command with a function.
 * 
 * The order of the argument parsers must match the order of the 
 * parameters of the handler.
 * 
 * \tparam ReturnType return type of the handler
 * \tparam ArgTypes parameter types of the handler
 */
template <typename CharType, typename ReturnType, typename... ArgTypes>
struct Command
{
    typedef CharType CharT;
    const CharT *commandName;
    // TODO check if we can use objects or reference_wrapper instead of pointers
    std::tuple<const Option<ArgTypes, CharT> *...> options;
    std::function<ReturnType(ArgTypes...)> handler;

    // TODO add function to generate helper message.
    // This message can print the labels and tell the user which default value is used instead

    /**
     * Executes the command with the provided arguments.
     * 
     * The order of the arguments in the string does not matter.
     * 
     * \retval true in case command was found and handler called
     * @return false in case command was not found
     */
    bool execute(const CharT *const commandLine, ReturnType *const pRetVal = nullptr) const
    {
        auto tokens = tokenizeQuoted(std::basic_string<CharT>(commandLine));
        if (tokens.empty())
        {
            throw std::runtime_error("Invalid command line format.");
            return false;
        }
        else if (tokens[0].compare(commandName) != 0)
        {
            return false;
        }
        tokens.erase(std::begin(tokens)); // remove command name token, as not necessary any more

        // Iterate over each option and compare it against the full range of args (except the first one)
        const auto arguments = std::apply(
            [&](const auto &...option) {
                return std::make_tuple(option->extractArgument(tokens)...);
            },
            options);

        if (tokens.size() != 0) // some tokens have not been evaluated
        {
            std::ostringstream oss;
            std::copy(std::begin(tokens), std::prev(std::end(tokens)), std::ostream_iterator<std::string>(oss, " "));
            oss << tokens.back();
            throw std::runtime_error("Not all tokens could be evaluated; invalid command line. Remainder: '" + oss.str() + "'");
        }

        // Call the command handler with the extracted arguments
        const auto function = [this, &arguments]() { return std::apply(
                                                         [this](const auto &...argument) {
                                                             return handler(argument...);
                                                         },
                                                         arguments); };
        if constexpr (!std::is_same_v<ReturnType, void>)
        {
            if (pRetVal)
            {
                *pRetVal = function();
            }
            else
            {
                function();
            }
        }
        else
        {
            function();
        }
        return true;
    }
};

/**
 * Creates \ref Command object while deducing template arguments for its type.
 * 
 * This is a helper function to deduce the type of the created object using the
 * types of the arguments to this function.
 * 
 * \tparam ReturnType is the return type of the handler to be called
 * \tparam ArgTypes  are the parameters of the handler to be called
 * \param commandName is a string representing the name of the command
 * \param options is a collection of parsers for extracting the arguments
 * \param handler is the handler to be called
 * \returns an object of \ref Command
 */
template <typename CharType, typename ReturnType, typename... ArgTypes>
Command<CharType, ReturnType, ArgTypes...> makeCommand(
    const CharType *const commandName,
    const std::tuple<const Option<ArgTypes, CharType> *...> &options,
    const std::function<ReturnType(ArgTypes...)> &handler)
{
    Command<CharType, ReturnType, ArgTypes...> command = {
        .commandName = commandName,
        .options = options,
        .handler = handler,
    };
    return command;
}
} // namespace command_line_interpreter
