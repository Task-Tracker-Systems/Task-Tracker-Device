#pragma once
#include <functional>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string_helpers.hpp>
#include <tuple>
#include <utility>

/**
 * Provides class templates to define command line commands.
 *
 * \ref Command allows to define a command line command and attach a function to it.
 * Objects will be able to interpret a command line string and call the function ("handler") with the appropriate arguments.
 *
 * A command consists of a name ("label") and a set of options.
 *
 * \ref Option defines options for \ref Command objects.
 * An option consists of a set of alternative labels (strings) and a default value.
 * It can be used to extract arguments from a set of strings.
 *
 * Options to commands must be given as two tokens: The label and the actual argument.
 *
 * In total a command line looks as follows:
 *
 * ```
 * command name   value of 1st option      value of 2nd option
 * ┏━━━━┷━━━━┓         ┏┷━┓         ┏━━━━━━━┷━┓
 * PRINT_FILES --PAGES 1337 --TITLE "ALL FILES"
 *             ┗━━┯━━┛      ┗━━━━┯┛
 *  a label of 1st option       a label of 2nd option
 * ```
 *
 * The options may be given in any order on the command line.
 * Each option must have a label.
 * The order of the options given to the \ref Command must be the same as the parameters of the function.
 */
namespace command_line_interpreter
{

/**
 * Extracts data from string options.
 * 
 * Has the definition of variants of option labels.
 * Provides a way to extract data from strings.
 * Contains the default value in case option was not part of strings.
 * 
 * \tparam T is the type of the argument
 * \tparam CharType is the character type to work with
 */
template <class T, typename CharType = char>
struct Option
{
    typedef CharType CharT;
    typedef T ArgumentType;

    /**
     * A variety of notations for an option label.
     * 
     * For example "tasks", "--tasks", or "-t".
     */
    std::vector<const CharT *> labels;

    /**
     * Checks if the name matches one of the labels valid for this option.
     * 
     * \param optionName the string to compare against
     * \retval true in case it matches one of the valid labels
     * \retval false else
     */
    bool doesMatchName(const CharT *const optionName) const
    {
        return std::find_if(std::begin(labels), std::end(labels), [&optionName](const auto candidate) {
                   return strcmp_g(candidate, optionName) == 0;
               }) != labels.end();
    }

    /**
     * Searches for the definition of this option and extracts it.
     * 
     * The pair of label and value is removed from the sequence.
     * 
     * Interpretation of non-string output data is done via `std::basic_istringstream::operator>>()`.
     * 
     * \param labelValuePairs a sequence of (option labels and option values)
     * \returns either the value found in the sequence or the default value for this option
     * \throws std::runtime_error in case a label for this option is found but interpreting the value failed.
     *         This can happen in case the value is given in an incompatible format.
     */
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
 * Interface for all \ref Command.
 * \tparam CharType character type to be used
 */
template <typename CharType>
struct BaseCommand
{
    /**
     * Character type to be used.
     */
    typedef CharType CharT;

    /**
     * Stores configuration.
     * \param commandName is the label for the command
     */
    BaseCommand(const CharT *const commandName)
        : commandName(commandName)
    {
    }

    /**
     * Executes the command with the provided arguments.
     *
     * The order of the arguments in the string does not matter.
     * Does not attempt to extract arguments in case the command was not identified.
     *
     * In case the command line does contain more than white spaces
     * but the first word does not identify the command, `false` is returned
     * and no handler is called.
     *
     * In case the first word in the command line identifies the command but
     * interpreting the options fails, an exception is thrown.
     *
     * Else the handler is called.
     *
     * \retval true in case command was found and handler called
     * \retval false in case command was not found
     * \param commandLine is the command line to be interpreted
     * \throws `std::runtime_error` in case
     *         - the command line contains no data
     *         - not all options could be interpreted
     *         - extracting data from an option failed
     */
    virtual bool execute(const CharT *const commandLine) const = 0;

    /**
     * Generates a message explaining how to use this command.
     *
     * This message is intended to be read by human users.
     * \returns a short list of the possible options
     */
    virtual std::basic_string<CharT> generateHelpMessage() const = 0;

    /**
     * Identifier for the command.
     */
    const CharT *commandName;

    virtual ~BaseCommand()
    {
    }
};

/**
 * Writes a description for an \ref Option to a string stream.
 *
 * \tparam Option template specialization of \ref Option
 * \tparam CharT the character type to be used
 * \param messageStream a stream to write to
 * \param option the option to be described
 */
template <class Option, typename CharT>
void writeOptionHelperToStream(std::basic_ostringstream<CharT> &messageStream,
                               const Option &option)
{
    messageStream << "\t ";
    for (const auto label : option->labels)
    {
        messageStream << label << " ";
    }
    messageStream << "\tdefault value: " << option->defaultValue << std::endl;
}

/**
 * Combines a command with a function.
 * 
 * The order of the argument parsers must match the order of the 
 * parameters of the handler.
 * 
 * \tparam CharType is the underlying character type of the command line
 * \tparam ReturnType return type of the handler
 * \tparam ArgTypes parameter types of the handler
 */
template <typename CharType, typename ReturnType, typename... ArgTypes>
struct Command : public BaseCommand<CharType>
{
    typedef typename BaseCommand<CharType>::CharT CharT;

    /**
     * A set of options used to retrieve the arguments for the handler.
     */
    // TODO check if we can use objects or reference_wrapper instead of pointers
    std::tuple<const Option<ArgTypes, CharT> *...> options;

    /**
     * The function to be called.
     */
    std::function<ReturnType(ArgTypes...)> handler;

    /**
     * \copydoc BaseCommand::execute()
     */
    bool execute(const CharT *const commandLine) const override
    {
        return execute(commandLine, nullptr);
    }

    /**
     * \copydoc BaseCommand::execute()
     * 
     * \param pRetVal is an optional pointer to an object where to store the return value of the handler
     *                if the pointer equals nullptr, the return value will be omitted
     */
    bool execute(const CharT *const commandLine, ReturnType *const pRetVal) const
    {
        auto tokens = tokenizeQuoted(std::basic_string<CharT>(commandLine));
        if (tokens.empty())
        {
            throw std::runtime_error("Invalid command line format.");
            return false;
        }
        else if (tokens[0].compare(this->commandName) != 0)
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

    /**
     * \copydoc BaseCommand::generateHelpMessage()
     */
    std::basic_string<CharT> generateHelpMessage() const override
    {
        std::basic_ostringstream<CharT> messageStream;
        messageStream << "Call: " << this->commandName << " [OPTION]..." << std::endl;
        if constexpr (std::tuple_size_v < decltype(options) >> 0)
        {
            messageStream << "Options:" << std::endl;
            std::apply([&messageStream](const auto &...option) {
                ((writeOptionHelperToStream(messageStream, option)), ...);
            },
                       options);
        }
        return messageStream.str();
    }

    /**
     * Stores its configuration.
     * \param commandName label for the command
     * \param handler the function to be called
     * \param options the \ref Option objects to the command arguments
     */
    Command(
        const CharType *const commandName,
        const std::function<ReturnType(ArgTypes...)> &handler,
        const std::tuple<const Option<ArgTypes, CharType> *...> &options = std::make_tuple())
        : BaseCommand<CharT>(commandName),
          options(options),
          handler(handler)
    {
    }
};

/**
 * Creates \ref Command object while deducing template arguments for its type.
 * 
 * This is a helper function to deduce the type of the created object using the
 * types of the arguments to this function.
 * 
 * \tparam CharType is the underlying type of the command line string
 * \tparam ReturnType is the return type of the handler to be called
 * \tparam ArgTypes are the parameters of the handler to be called
 * \param commandName is a string representing the name of the command
 * \param options is a collection of parsers for extracting the arguments
 * \param handler is the function to be called
 * \returns an object of \ref Command
 */
template <typename CharType, typename ReturnType, typename... ArgTypes>
Command<CharType, ReturnType, ArgTypes...> makeCommand(
    const CharType *const commandName,
    const std::function<ReturnType(ArgTypes...)> &handler,
    const std::tuple<const Option<ArgTypes, CharType> *...> &options = std::make_tuple())
{
    return Command<CharType, ReturnType, ArgTypes...>(commandName, handler, options);
}
} // namespace command_line_interpreter
