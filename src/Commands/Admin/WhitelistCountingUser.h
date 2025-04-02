#ifndef WHITELIST_COUNTING_USER_H
#define WHITELIST_COUNTING_USER_H

#include "../../Common.h"

class WhitelistCountingUser {
public:
  void InitializeCommand(const dpp::ready_t& event);
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
