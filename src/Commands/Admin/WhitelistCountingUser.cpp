#include "WhitelistCountingUser.h"
#include "../../Modules/Counting/CountingBlacklist.h"

const std::string COMMAND_NAME = "whitelistcountinguser";

void WhitelistCountingUser::InitializeCommand(const dpp::ready_t& event) {
  dpp::slashcommand command;

  command.set_name(COMMAND_NAME);
  command.set_description("[MODERATOR ONLY] Removes a user from the counting blacklist");
  command.set_default_permissions(dpp::p_manage_messages);
  command.set_application_id(event.from->creator->me.id);

  dpp::command_option input(dpp::co_user, "user", "The user you would like to remove from the blacklist", true);
  command.add_option(input);

  RegisterCommand(command);
}

void WhitelistCountingUser::OnCommandRun(const dpp::slashcommand_t& event) {
  if (event.command.get_command_name() != COMMAND_NAME) return;

  dpp::snowflake userId = std::get<dpp::snowflake>(event.get_parameter("user"));

  // convert the file into an vector of strings
  std::ifstream readFile = std::ifstream(filePath);
  // loop through the list and find any matches
  std::string line;
  std::vector<std::string> users;
  while (std::getline(readFile, line)) {
    if (line == userId.str())
      continue;
    users.push_back(line);
  }
  readFile.close();
  // remove the matches
  // write the remaining strings back into the file
  std::ofstream writeFile = std::ofstream(filePath);
  
  for (std::string line : users) {
    writeFile << line << std::endl;
  }

  writeFile.close();

  event.reply("Removed user " + userId.str() + " from blacklist");
}
