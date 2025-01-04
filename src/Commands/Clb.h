#ifndef CLB_H
#define CLB_H

#include "../Common.h"
#include "../Modules/Counting/Counting.h"

class Clb {
private:
  void getCountsLeaderboard(const dpp::slashcommand_t& event);
  void getFailuresLeaderboard(const dpp::slashcommand_t& event); 
public:
  void InitializeCommand(const dpp::ready_t& event, Counting* countingInstance);
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
