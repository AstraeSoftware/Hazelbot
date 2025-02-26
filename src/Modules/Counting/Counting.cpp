#include "Counting.h"
#include "CountingBlacklist.h"
#include "../../EmojiHandler.h"

std::string getReaction(CountingState state){
  std::regex regex("[0,1,3-9]"); // matches any number other than 2
  if(!std::regex_search(std::to_string(state.current_number - 1), regex)){
    // number is made up of only '2'
    return GetEmoji("counting_continue_chain_2_emoji", false, u8"2️⃣");
  }

  if((state.current_number - 1) % 100 == 0){
    // divisible by 100
    return GetEmoji("counting_continue_chain_one-hundred_emoji", false, u8"💯");
  }

  if(state.current_number - 1 >= state.highest_count){
    // new high score
    return GetEmoji("counting_continue_chain_new-highest_emoji", false, u8"☑️");
  }

  return GetEmoji("counting_continue_chain_emoji", false, u8"✅");
}

void Counting::saveState(){
  Log("Saving state...", DEBUG, "Counting");
  CountingSavesystem::save(State);
  Log("Saved state!", INFO, "Counting");
}

bool Counting::isCountingMessage(const dpp::message_create_t& event){
  dpp::snowflake channelId = event.msg.channel_id;

  if(channelId != Counting::_countingChannelID){
    // message was sent outside of counting channel, so we want to ignore it
    return false;
  }

  if(event.msg.author == event.from->creator->me){
    // this is our own message, so we want to ignore it
    return false;
  }

  if(IsBlacklisted(event.msg.author.id)) {
    return false;
  }
  
  Log("Message recieved", DEBUG, "Counting");
  return true;
}

int Counting::getFirstNumberInString(const std::string& str){
  // matches any number, unless it is contained in angled brackets <>
  // this is so that it can ignore emojis, pings, channel links, etc.
  // doesn't ignore links, but that should be added. i just Suck at regex

  std::smatch match;
  std::regex regex("[0-9]+");
  std::regex_search(str, match, regex);

  if(match.size() == 0){
    // no numbers in the given string, return with value -1
    return -1;
  }

  // returns the first number found
  return std::stoi(match[0].str());
}

void Counting::onFailChain(const dpp::message_create_t& event, const CountingFailChainCondition& condition){
  Log("Counting chain failed at " + std::to_string(State.current_number - 1) + ".", INFO, "Counting");

  if(State.current_number > State.highest_count){
    // new longest chain 
    State.highest_count = State.current_number - 1;
    State.highest_count_sent = event.msg.sent;
    State.longest_chain_failed_by = event.msg.author.id;
  }

  CountingUserState user;
  if(State.user_stats.count(event.msg.author.id.str())){
    // if user is already in user_stats
    user = State.user_stats[event.msg.author.id.str()];
    // otherwise leave all user stats as the default, by just not doing anything
  }

  user.total_failures++;

  if(State.current_number > user.biggest_failure){
    // this is a new biggest failure for the user, so update the records
    user.biggest_failure = State.current_number - 1;
  }

  State.user_stats.insert_or_assign(event.msg.author.id.str(), user);

  State.current_number = 1;
  State.last_count_author = 0;

  State.total_failures++;

  event.from->creator->message_add_reaction(event.msg.id, event.msg.channel_id, GetEmoji("counting_fail_chain_emoji", false, u8"❌"));

  if(condition == WRONG_NUMBER){
    event.reply(GetResponse("counting_chain_fail_wrongnumber"));
  }
  else if(condition == DOUBLE_SEND){
    event.reply(GetResponse("counting_chain_fail_doublesend"));
  }

  saveState();
}

void Counting::onContinueChain(const dpp::message_create_t& event){
  Counting::State.last_count_author = event.msg.author.id;
  Counting::State.total_counts++;

  CountingUserState user;
  if(Counting::State.user_stats.count(event.msg.author.id.str())){
    // if the user is already in user_stats
    user = Counting::State.user_stats[event.msg.author.id.str()]; 
    // otherwise leave all the user stats as the default, by just not doing anything
  }

  user.total_counts++;
  if(Counting::State.current_number > user.highest_count){
    user.highest_count = Counting::State.current_number - 1;
    user.highest_count_sent = event.msg.sent;
  }

  State.user_stats.insert_or_assign(event.msg.author.id.str(), user);
  State.current_number++;

  std::string reaction = getReaction(State);
  event.from->creator->message_add_reaction(event.msg.id, event.msg.channel_id, reaction);

  Log("Counting chain has reached " + std::to_string(State.current_number - 1) + "!", DEBUG, "Counting");
  Counting::saveState();
}

Counting::Counting(){
  Log("Initializing counting...", DEBUG, "Counting");
  Counting::_countingChannelID = dpp::snowflake(ConfigParser::get_string("counting_channel_id", "0"));
  Log("Loading save file", DEBUG, "Counting");
  State = CountingSavesystem::load();
  Log("Save file loaded!", DEBUG, "Counting");
  InitializeBlacklist();
  Log("Counting initialized!", INFO, "Counting");
}

void Counting::OnMessageCreate(const dpp::message_create_t& event){
  if(!isCountingMessage(event))
    return;

  int sentNumber = getFirstNumberInString(event.msg.content);

  if(sentNumber == -1){
    // no number was found in the string, so -1 was returned
    Log("No number found in message.", DEBUG, "Counting");
    return;
  }

  Log("Recieved number " + std::to_string(sentNumber) + "!", DEBUG, "Counting");

  if(sentNumber != Counting::State.current_number){
    onFailChain(event, WRONG_NUMBER);
    return;
  }

  if(event.msg.author.id == Counting::State.last_count_author){
    onFailChain(event, DOUBLE_SEND);
    return;
  }

  onContinueChain(event);
}
