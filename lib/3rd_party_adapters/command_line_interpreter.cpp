#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

template <typename DataType>
std::optional<DataType> extractData(std::string &sentence)
{
    std::istringstream istream(sentence);
    std::ostringstream remainer;
    DataType data;
    istream >> data;

    if (!istream.fail())
    {
        remainer << istream.rdbuf();
        sentence = remainer.str();
        return data;
    }

    return std::nullopt;
}

template <class T>
struct Argument
{
    std::vector<const char *> labels;
    T extract(std::string &argumentsToExtract) const
    {
        // find argument label
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
                auto remainer = argumentsToExtract.substr(label_end + 1);
                const auto foundData = extractData<T>(remainer);
                if (foundData)
                {
                    const auto preamble = argumentsToExtract.substr(0, label_end);
                    argumentsToExtract = preamble + remainer;
                    return foundData.value();
                }
            }
        }

        return defaultValue;
    }
    T defaultValue;
};

template <typename ReturnType, typename... Arguments>
struct Command
{
    const char *commandName;
    std::tuple<const Argument<Arguments> *...> argumentParsers;
    std::function<ReturnType(Arguments...)> function;
    const bool parse(std::string commandToParse) const
    {
        const auto foundCommandName = extractData<std::string>(commandToParse);
        if (foundCommandName)
        {
            if (foundCommandName.value().compare(commandName) == 0)
            {
                std::apply(
                    [&](auto &...argumentParser) {
                        function(argumentParser->extract(commandToParse)...);
                    },
                    argumentParsers);
                return true;
            }
        }
        return false;
    }
};

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
