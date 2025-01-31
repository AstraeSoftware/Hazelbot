#include "BlacklistCountingUser.h"
#include "../../Modules/Counting/CountingBlacklist.h"

const std::string COMMAND_NAME = "blacklistcountinguser";

void BlacklistCountingUser::InitializeCommand(const dpp::ready_t& event) {
  dpp::slashcommand command;
  
  command.set_name(COMMAND_NAME);
  command.set_description("[MODERATOR ONLY] Adds a user to the counting blacklist text file.");
  command.set_default_permissions(dpp::p_manage_messages);
  command.set_application_id(event.from->creator->me.id);

  dpp::command_option input(dpp::co_user, "user", "The user you would like to blacklist", true);
  command.add_option(input);

  RegisterCommand(command);
}

void BlacklistCountingUser::OnCommandRun(const dpp::slashcommand_t& event) {
  if (event.command.get_command_name() != COMMAND_NAME) return;

  dpp::snowflake userId = std::get<dpp::snowflake>(event.get_parameter("user"));
  
  std::ofstream blacklistFile = std::ofstream(filePath, std::ios_base::app);
  blacklistFile << std::endl << userId.str();
  blacklistFile.close();

  event.reply("Blacklisted user " + userId.str());
}
