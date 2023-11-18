#include <cstring>
#include <cwchar>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>

template <typename CharType>
int strcmp_g(const CharType *const lhs, const CharType *const rhs);

template <>
int strcmp_g(const char *const lhs, const char *const rhs)
{
    return std::strcmp(lhs, rhs);
}

template <>
int strcmp_g(const wchar_t *const lhs, const wchar_t *const rhs)
{
    return std::wcscmp(lhs, rhs);
}

/**
 * Combines a command option with an argument.
 * \tparam T is the type of the argument
 */
template <class T, typename CharType = char>
struct Option
{
    typedef CharType CharT;

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

    T defaultValue;
};

// Tokenize a string using std::quoted
template <typename CharType>
std::vector<std::basic_string<CharType>> tokenizeQuoted(const std::basic_string<CharType> &input)
{
    std::basic_stringstream<CharType> iss(input);
    std::vector<std::basic_string<CharType>> tokens;

    std::basic_string<CharType> token;
    while (!iss.eof())
    {
        iss >> std::quoted(token);
        if (iss.fail())
        {
            throw std::runtime_error("failed to tokenize string: '" + input + "'");
        }
        tokens.push_back(token);
    }

    return tokens;
}

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
    // TODO check if we can use objects or references instead of pointers
    std::tuple<const Option<ArgTypes, CharT> *...> options;
    std::function<ReturnType(ArgTypes...)> handler;

    template <class ArgumentType>
    static ArgumentType getArgument(const Option<ArgumentType, CharT> &option,
                                    const std::vector<std::basic_string<CharT>> &args)
    {
        const auto itAllOptions = std::next(std::begin(args)); // Skip the command name

        // Find the first matching argument in the command line
        const auto argIt = std::find_if(itAllOptions, std::end(args), [&option](const auto &arg) {
            return option.matches(arg);
        });

        if (argIt != std::end(args))
        {
            // If a match is found, set the corresponding argument value
            const auto &argValueString = *std::next(argIt);
            std::basic_istringstream<CharT> iss(argValueString);
            ArgumentType argument;
            iss >> argument;
            if (iss.fail())
            {
                throw std::runtime_error("argument to option " + option.name + " could not be parsed: '" + argValueString + "'");
            }
            return argument;
        }
        else
        {
            // If no match is found, set the default value
            return option.defaultValue;
        }
    }

    /**
     * Executes the command with the provided arguments.
     * 
     * The order of the arguments in the string does not matter.
     * 
     * \retval true in case command was found and handler called
     * @return false in case command was not found
     */
    // TODO add bool as return value to indicate success
    // TODO add pointer as parameter to return the return value of the handler is appropriate
    bool execute(const CharT *const commandLine, ReturnType *const pRetVal = nullptr) const
    {
        const auto args = tokenizeQuoted(std::basic_string<CharT>(commandLine));
        if (args.empty())
        {
            throw std::runtime_error("Invalid command line format.");
            return false;
        }
        else if (args[0].compare(commandName) != 0)
        {
            return false;
        }

        // Iterate over each option and compare it against the full range of args (except the first one)
        const auto arguments = std::apply(
            [&](const auto &...option) {
                return std::make_tuple(getArgument(option, args)...);
            },
            options);

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
