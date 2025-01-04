#include "Clb.h"
#include <dpp/colors.h>

const Counting* _cInstance; 

struct LeaderboardUser {
public:
  dpp::snowflake userId;
  int value;
  LeaderboardUser(dpp::snowflake id, int v) {
    userId = id;
    value = v;
  }
};

int qsPartition(std::vector<LeaderboardUser>& arr, int low, int high) {
  // Choose the pivot
  LeaderboardUser pivot = arr[high];

  // Index of smaller element and indicates the right position of pivot found so far
  int i = low - 1;

  // Traverse arr[low..high] and move all smaller elements on the left side
  // Elements from low to i are smaller after every iteration
  
  for(int j = low; j <= high - 1; j++) {
    if(arr[j].value < pivot.value) {
      i++;
      std::swap(arr[i], arr[j]);
    }
  }

  // Move pivot after smaller elements and return its position
  std::swap(arr[i + 1], arr[high]);
  return i + 1;
}

void quickSort(std::vector<LeaderboardUser>& arr, int low, int high) {
  if(low < high) {
    // pi is the partition return index of pivot
    int pi = qsPartition(arr, low, high);

    // Recursion calls for smaller elements
    // and greater or equals elements
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

void Clb::InitializeCommand(const dpp::ready_t& event, Counting* countingInstance) {
  _cInstance = countingInstance;

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

  if(lb == "lb_counts") getCountsLeaderboard(event);
  else if(lb == "lb_failures") getFailuresLeaderboard(event);
}


void Clb::getCountsLeaderboard(const dpp::slashcommand_t& event) {
  std::vector<LeaderboardUser> lb;
  for(auto user : _cInstance->State.user_stats) {
    lb.push_back(LeaderboardUser(user.first, user.second.total_counts));
  }
  quickSort(lb, 0, lb.size() - 1);
  std::reverse(std::begin(lb), std::end(lb));

  std::string message = "";
  for(int i = 0; i < lb.size() && i < 10; i++) {
    std::string entry = "**#" + std::to_string(i + 1) + "**: <@" + lb[i].userId.str() + "> - " + std::to_string(lb[i].value) + " counts\n";
    message += entry;
  }

  dpp::embed content = dpp::embed();
  content.set_title("Counting Leaderboard - Counts");
  content.set_description(message);
  content.set_thumbnail(event.command.get_guild().get_icon_url());
  content.set_footer(dpp::embed_footer().set_text(std::to_string(_cInstance->State.total_counts) + " total counts."));
  content.set_color(dpp::colors::aqua);
  
  event.reply(content);
}

void Clb::getFailuresLeaderboard(const dpp::slashcommand_t& event) {
  std::vector<LeaderboardUser> lb;
  for(auto user : _cInstance->State.user_stats) {
    lb.push_back(LeaderboardUser(user.first, user.second.total_failures));
  }
  quickSort(lb, 0, lb.size() - 1);
  std::reverse(std::begin(lb), std::end(lb));

  std::string message = "";
  for(int i = 0; i < lb.size() && i < 10; i++) {
    std::string entry = "**#" + std::to_string(i + 1) + "**: <@" + lb[i].userId.str() + "> - " + std::to_string(lb[i].value) + " failures\n";
    message += entry;
  }

  dpp::embed content = dpp::embed();
  content.set_title("Counting Leaderboard - Failures");
  content.set_description(message);
  content.set_thumbnail(event.command.get_guild().get_icon_url());
  content.set_footer(dpp::embed_footer().set_text(std::to_string(_cInstance->State.total_failures) + " total failures."));
  content.set_color(dpp::colors::crimson_red);

  event.reply(content);
}
