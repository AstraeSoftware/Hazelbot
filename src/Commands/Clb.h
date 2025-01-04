#ifndef CLB_H
#define CLB_H

#include "../Common.h"

class Clb {
private:
  // nyaa
public:
  void InitializeCommand(const dpp::ready_t& event);
  void OnCommandRun(const dpp::slashcommand_t& event);
};

#endif
