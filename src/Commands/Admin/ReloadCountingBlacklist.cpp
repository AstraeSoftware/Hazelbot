#include "ReloadCountingBlacklist.h"
#include "../../Modules/Counting/CountingBlacklist.h"

const std::string COMMAND_NAME = "reloadcountingblacklist";

void ReloadCountingBlacklist::InitializeCommand(const dpp::ready_t& event) {
  dpp::slashcommand command;

  command.set_name(COMMAND_NAME);
  command.set_description("[ADMIN ONLY] Reloads the blacklist file without restarting the bot or counting system.");
  command.set_default_permissions(0);
  command.set_application_id(event.from->creator->me.id);

  RegisterCommand(command);
}

void ReloadCountingBlacklist::OnCommandRun(const dpp::slashcommand_t& event) {
  if(event.command.get_command_name() != COMMAND_NAME) return;
  InitializeBlacklist();
  event.reply("Reloaded counting blacklist");
}
