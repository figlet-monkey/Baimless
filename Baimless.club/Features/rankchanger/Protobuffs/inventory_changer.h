#pragma once
#include "ProtoWriter.h"
#include "ProtobuffMessages.h"




struct wskin
{
	int wId;
	int paintkit;
};

std::vector<wskin> g_skins = {
	{ 508,622 },
	{ 5027,10007 },
	{ 1,527 },
	{ 2,544 },
	{ 3,387 },
	{ 4,586 },
	{ 32,338 },
	{ 36,404 },
	{ 61,653 },
	{ 63,270 },
	{ 30,652 },
	{ 7,656 },
	{ 8,305 },
	{ 10,260 },
	{ 13,629 },
	{ 14,401 },
	{ 16,632 },
	{ 60,383 },
	{ 17,433 },
	{ 19,283 },
	{ 24,688 },
	{ 25,616 },
	{ 26,306 },
	{ 27,198 },
	{ 28,483 },
	{ 29,434 },
	{ 33,481 },
	{ 34,262 },
	{ 35,450 },
	{ 39,311 },
	{ 38,232 },
	{ 11,628 },
	{ 9,344 },
	{ 40,538 }
};

#define START_MUSICKIT_INDEX 1500000
#define START_ITEM_INDEX     2000000


static void fix_null_inventory(ProtoWriter& cache);
static void clear_equip_state(ProtoWriter& object);
static void apply_medals(ProtoWriter& object);
static void apply_music_kits(ProtoWriter& object);
static void add_all_items(ProtoWriter& object);
static void add_item(ProtoWriter& object, int index, int itemIndex, int rarity, int paintKit, int seed, float wear, std::string name);
static int GetAvailableClassID(int definition_index);
static int GetSlotID(int definition_index);
static std::vector<uint32_t> music_kits = { 3, 4, 5, 6, 7, 8 };

static std::string inventory_changer(void *pubDest, uint32_t *pcubMsgSize) {
	ProtoWriter msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8, 11);
	if (msg.getAll(CMsgClientWelcome::outofdate_subscribed_caches).empty())
		return msg.serialize();

	ProtoWriter cache(msg.get(CMsgClientWelcome::outofdate_subscribed_caches).String(), 4);
	// If not have items in inventory, Create null inventory
	fix_null_inventory(cache);
	// Add custom items
	auto objects = cache.getAll(CMsgSOCacheSubscribed::objects);
	for (int i = 0; i < objects.size(); i++)
	{
		ProtoWriter object(objects[i].String(), 2);

		if (!object.has(SubscribedType::type_id))
			continue;

		switch (object.get(SubscribedType::type_id).Int32())
		{
		case 1: // Inventory
		{
			if (true) //g_Options.skins_packet_clear_default_items
				object.clear(SubscribedType::object_data);

			clear_equip_state(object);

			//deleted

			cache.replace(Field(CMsgSOCacheSubscribed::objects, TYPE_STRING, object.serialize()), i);
		}
		break;
		}
	}
	msg.replace(Field(CMsgClientWelcome::outofdate_subscribed_caches, TYPE_STRING, cache.serialize()), 0);

	return msg.serialize();
}

static bool inventory_changer_presend(void* pubData, uint32_t &cubData)
{
	ProtoWriter msg((void*)((DWORD)pubData + 8), cubData - 8, 19);
	// Change music kit check
	if (msg.has(CMsgAdjustItemEquippedState::item_id)
		&& msg.get(CMsgAdjustItemEquippedState::new_class).UInt32() == 0
		|| msg.get(CMsgAdjustItemEquippedState::new_slot).UInt32() == 54)
	{
		int ItemIndex = /*(uint32_t)*/msg.get(CMsgAdjustItemEquippedState::item_id).UInt64() - START_MUSICKIT_INDEX;

		if (ItemIndex > 38 || ItemIndex < 3)
			return true;

		/*g_Options.skins_packets_musci_kit*/auto skins_packets_musci_kit = msg.get(CMsgAdjustItemEquippedState::new_slot).UInt32() == 65535 ? 0 : ItemIndex - 2;

		return false;
	}
	// Change weapon skin check
	if (!msg.has(CMsgAdjustItemEquippedState::item_id)
		|| !msg.has(CMsgAdjustItemEquippedState::new_class)
		|| !msg.has(CMsgAdjustItemEquippedState::new_slot))
		return true;

	return false;
}

static void fix_null_inventory(ProtoWriter& cache)
{
	bool inventory_exist = false;
	auto objects = cache.getAll(CMsgSOCacheSubscribed::objects);
	for (int i = 0; i < objects.size(); i++)
	{
		ProtoWriter object(objects[i].String(), 2);
		if (!object.has(SubscribedType::type_id))
			continue;
		if (object.get(SubscribedType::type_id).Int32() != 1)
			continue;
		inventory_exist = true;
		break;
	}
	if (!inventory_exist)
	{
		int cache_size = objects.size();
		ProtoWriter null_object(2);
		null_object.add(Field(SubscribedType::type_id, TYPE_INT32, (int64_t)1));

		cache.add(Field(CMsgSOCacheSubscribed::objects, TYPE_STRING, null_object.serialize()));
	}
}

static void clear_equip_state(ProtoWriter& object)
{
	auto object_data = object.getAll(SubscribedType::object_data);
	for (int j = 0; j < object_data.size(); j++)
	{
		ProtoWriter item(object_data[j].String(), 19);

		if (item.getAll(CSOEconItem::equipped_state).empty())
			continue;

		// create NOT equiped state for item
		ProtoWriter null_equipped_state(2);
		null_equipped_state.replace(Field(CSOEconItemEquipped::new_class, TYPE_UINT32, (int64_t)0));
		null_equipped_state.replace(Field(CSOEconItemEquipped::new_slot, TYPE_UINT32, (int64_t)0));
		// unequip all
		auto equipped_state = item.getAll(CSOEconItem::equipped_state);
		for (int k = 0; k < equipped_state.size(); k++)
			item.replace(Field(CSOEconItem::equipped_state, TYPE_STRING, null_equipped_state.serialize()), k);

		object.replace(Field(SubscribedType::object_data, TYPE_STRING, item.serialize()), j);
	}
}

std::vector<uint32_t> packets_medals = { 1372, 958, 957, 956, 955 };
int packets_equipped_medal = 874;



static void apply_medals(ProtoWriter& object)
{

	/*if (!g_Options.medals.enable) 
		return;
	
	int c = 10001;
	for (uint32_t MedalIndex : g_Options.medals.medals)
	{
		CSOEconItem Medal;
		Medal.account_id;
		Medal->account_id = g_SteamUser->GetSteamID().GetAccountID();
		Medal.set_origin(9);
		Medal.set_rarity(6);
		Medal.set_quantity(1);
		Medal.set_quality(4);
		Medal.set_level(1);
		CSOEconItemAttribute* TimeAcquiredAttribute = Medal.attribute();
		uint32_t TimeAcquiredAttributeValue = 0;
		TimeAcquiredAttribute->set_def_index(222);
		TimeAcquiredAttribute->set_value_bytes(&TimeAcquiredAttributeValue, 4);
		Medal.set_def_index(MedalIndex);
		Medal.set_inventory(c);
		Medal.set_id(c);
		if (Settings::MedalChanger::equipped_medal_override && Settings::MedalChanger::equipped_medal == MedalIndex)
		{
			CSOEconItemEquipped* EquippedState = Medal.add_equipped_state();
			EquippedState->set_new_class(0);
			EquippedState->set_new_slot(55);
		}
		pInventoryCacheObject->add_object_data(Medal.SerializeAsString());
		c++;
	}*/
	
}

static void apply_music_kits(ProtoWriter& object)
{

}

static void add_all_items(ProtoWriter& object)
{


}

static void add_item(ProtoWriter& object, int index, int itemIndex, int rarity, int paintKit, int seed, float wear, std::string name)
{

}

static int GetAvailableClassID(int definition_index)
{

}

static int GetSlotID(int definition_index)
{

}