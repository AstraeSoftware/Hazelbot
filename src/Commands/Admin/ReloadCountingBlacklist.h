#ifndef RELOAD_COUNTING_BLACKLIST_H
#define RELOAD_COUNTING_BLACKLIST_H

#include "../../Common.h"

class ReloadCountingBlacklist {
public:
  void InitializeCommand(const dpp::ready_t& event);
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
