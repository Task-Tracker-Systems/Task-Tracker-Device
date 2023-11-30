#include "Protocol.hpp"
#include "serial_port.hpp"
#include <array>

namespace cli = command_line_interpreter;

// --------------------------
// --- define commands ------
// --------------------------
#include <string>

// command for list
static const auto list = []() { serial_port::cout << "this is a list: a, b, c, ..." << std::endl; };
static const auto listCmd = cli::makeCommand("list", std::function(list));

// command for edit
static const auto edit = [](const int id, const std::string label, const int duration) {
    serial_port::cout << "Edit id(" << id << ") label('" << label << "') duration(" << duration << ")" << std::endl;
};
static const cli::Option<int> id = {.labels = {"--id"}, .defaultValue = 0};
static const cli::Option<std::string> label = {.labels = {"--name"}, .defaultValue = "foo"};
static const cli::Option<int> duration = {.labels = {"--duration"}, .defaultValue = 0};
static const auto editCmd = cli::makeCommand("edit", std::function(edit), std::make_tuple(&id, &label, &duration));
static const std::array<const cli::BaseCommand<char> *, 2> commands = {&listCmd, &editCmd};

bool ProtocolHandler::execute(const CharType *const commandLine)
{
    for (const auto command : commands)
    {
        try
        {
            const bool executed = command->execute(commandLine);
            if (executed)
            {
                return executed;
            }
        }
        catch (const std::runtime_error &e)
        {
            serial_port::cout << e.what() << std::endl;
            serial_port::cout << command->generateHelpMessage() << std::endl;
        }
    }

    // if we arrive here, no command has been executed
    if (!commands.empty())
    {
        serial_port::cout << "No command has been executed!" << std::endl
                          << "Possible commands are:" << std::endl;
        for (const auto command : commands)
        {
            serial_port::cout << "\t " << command->commandName << std::endl;
        }
    }
    return false;
}
