#ifndef COUNTING_BLACKLIST_H
#define COUNTING_BLACKLIST_H

#include "../../Common.h"

const std::string filePath = "config/counting_blacklist.cfg";

void InitializeBlacklist();
bool IsBlacklisted(dpp::snowflake user);

#endif
