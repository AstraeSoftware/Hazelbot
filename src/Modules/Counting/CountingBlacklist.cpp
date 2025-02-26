#include "CountingBlacklist.h"

std::vector<dpp::snowflake> _blacklistedUsers;
 
void InitializeBlacklist() {
  Log("Initializing counting blacklist..", INFO, "Counting");

  _blacklistedUsers = std::vector<dpp::snowflake>();
  std::ifstream file(filePath);
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::vector<std::string> tokens = StringUtils::split_string_on_nl(buffer.str());
  
  for(std::string token : tokens) {
    if (token == "") continue;

    Log("Blacklisting user with ID " + token + " from interacting with the counting system", DEBUG, "Counting");
    _blacklistedUsers.push_back(dpp::snowflake(token));
  }

  Log("Counting blacklist initialized!", INFO, "Counting");
  file.close();
}

bool IsBlacklisted(dpp::snowflake user) {
  return std::count(_blacklistedUsers.begin(), _blacklistedUsers.end(), user) == 1;
}
