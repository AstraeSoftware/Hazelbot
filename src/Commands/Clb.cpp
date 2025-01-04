#include "Clb.h"

void Clb::InitializeCommand(const dpp::ready_t& event) {
  dpp::slashcommand command;  
  dpp::command_option options(dpp::co_string, "stat", "The statistic you would like to get the leaderboard for", true);

  options.add_choice(dpp::command_option_choice("Counts", std::string("lb_counts")));
  options.add_choice(dpp::command_option_choice("Failures", std::string("lb_failures")));

  command.set_name("clb");
  command.set_description("Shows the counting leaderboard for the selected stat");
  command.add_option(options);
  command.set_application_id(event.from->creator->me.id);
  
  RegisterCommand(command);
}

void Clb::OnCommandRun(const dpp::slashcommand_t& event) {
  if(event.command.get_command_name() != "clb") return;  

  std::string lb = std::get<std::string>(event.get_parameter("stat"));
  event.reply("nya getting leaderboard for " + lb);
}
