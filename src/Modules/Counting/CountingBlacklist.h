#ifndef COUNTING_BLACKLIST_H
#define COUNTING_BLACKLIST_H

#include "../../Common.h"

void InitializeBlacklist();
bool IsBlacklisted(dpp::snowflake user);

#endif
