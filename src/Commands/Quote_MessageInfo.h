#ifndef QUOTE_MESSAGE_INFO_H
#define QUOTE_MESSAGE_INFO_H

#include "../Common.h"

struct Quote_MessageInfo{
private:
  dpp::snowflake _guildId;
  dpp::snowflake _message;
  dpp::snowflake _messageAuthor;
  std::string _messageContent;
  std::time_t _messageSent;
  std::string _authorName;
  std::string _authorAvatar;

public:
  Quote_MessageInfo(dpp::snowflake guildId, dpp::snowflake message, dpp::snowflake messageAuthor, std::string messageContent, std::time_t messageSent, std::string authorName, std::string authorAvatar);
  Quote_MessageInfo();
  dpp::snowflake GetGuildId();
  dpp::snowflake GetMessageId();
  std::string GetMessageContent();
  std::time_t GetMessageSent();
  dpp::snowflake GetMessageAuthorId();
  std::string GetMessageAuthorName();
  std::string GetMessageAuthorAvatar();
};

#endif
