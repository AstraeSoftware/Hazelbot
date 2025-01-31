#ifndef BLACKLIST_COUNTING_USER_H
#define BLACKLIST_COUNTING_USER_H

#include "../../Common.h"

class BlacklistCountingUser {
public:
  void InitializeCommand(const dpp::ready_t& event);
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
