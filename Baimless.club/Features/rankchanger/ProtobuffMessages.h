#pragma once

#define k_EMsgGCCStrike15_v2_MatchmakingGC2ClientReserve 9107
#define k_EMsgGCClientWelcome 4004
#define k_EMsgGCClientHello 4006
#define k_EMsgGCAdjustItemEquippedState 1059
#define k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello 9109
#define k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello 9110

// ProfileChanger
struct MatchmakingGC2ClientHello {
	static const unsigned int commendation = 8;
	static const unsigned int ranking = 7;
	static const unsigned int player_level = 17;
};
struct PlayerCommendationInfo {
	static const unsigned int cmd_friendly = 1;
	static const unsigned int cmd_teaching = 2;
	static const unsigned int cmd_leader = 4;
};
struct PlayerRankingInfo {
	static const unsigned int rank_id = 2;
	static const unsigned int wins = 3;
};

// InvChanger
struct SubscribedType {
	static const unsigned int type_id = 1;
	static const unsigned int object_data = 2;
};

struct CMsgSOCacheSubscribed {
	static const unsigned int objects = 2;
};

struct CMsgClientWelcome {
	static const unsigned int outofdate_subscribed_caches = 3;
};

struct CSOEconItem {
	static const unsigned int id = 1;
	static const unsigned int account_id = 2;
	static const unsigned int inventory = 3;
	static const unsigned int def_index = 4;
	static const unsigned int quantity = 5;
	static const unsigned int level = 6;
	static const unsigned int quality = 7;
	static const unsigned int flags = 8;
	static const unsigned int origin = 9;
	static const unsigned int custom_name = 10;
	static const unsigned int attribute = 12;
	static const unsigned int in_use = 14;
	static const unsigned int style = 15;
	static const unsigned int original_id = 16;
	static const unsigned int equipped_state = 18;
	static const unsigned int rarity = 19;
};

struct CMsgAdjustItemEquippedState {
	static const unsigned int item_id = 1;
	static const unsigned int new_class = 2;
	static const unsigned int new_slot = 3;
};

struct CSOEconItemEquipped {
	static const unsigned int new_class = 1;
	static const unsigned int new_slot = 2;
};

struct CSOEconItemAttribute {
	static const unsigned int def_index = 1;
	static const unsigned int value = 2;
	static const unsigned int value_bytes = 3;
};