#include "lui_cod.h"
#include "game_inc.h"
#include "addr_utils.hpp"
void LUI_CoD_RegisterDvars_Detour()
{
	printf("registering lui dvars\n");
	player_name = Dvar_RegisterString("player_name", "Player1", 0, "Sets the player name.");
	sv_cheats = Dvar_RegisterBool("sv_cheats", false, 0, "Enables cheats to be used on a server");
	spawn_br_gas = Dvar_RegisterBool("spawn_br_gas", true, 0, "Disables gas in battle royale maps");
	show_watermark = Dvar_RegisterBool("show_watermark", true, 0, "Shows the watermark for codUPLOADER");

	player_sustainammo = Dvar_RegisterBool("player_sustainAmmo", false, 0, "Firing weapon will not decrease clip ammo.");

	print_debug = Dvar_RegisterBool("print_debug", false, 0, "Print debug output to the external console");

	weap_impactType = Dvar_RegisterInt("weap_impactType", -1, -1, 1000, 0, "Forces an weapon impact type on every weapon. -1 = default");
	weap_dismembermentAlwaysEnabled = Dvar_RegisterBool("weap_dismembermentAlwaysEnabled", false, 0, "Enables dismemberment effect for all weapons");

	g_dumpScripts = Dvar_RegisterBool("g_dumpScripts", false, 0, "Dump GSC scripts");

	unlockAllItems = Dvar_RegisterBool("unlockAllItems", true, 0, "Unlocks all items");

	lui_cod_registerdvars.stub<void>();
}

int LuaShared_LuaCall_IsDemoBuild_Detour(uintptr_t luaVM)
{
	lua_pushboolean(luaVM, 1);
	return 1;
}

void LUI_CoD_LuaCall_EngineNotifyServer_Detour(uintptr_t luaVM) {
	static std::unordered_map<std::string, std::function<void()>> handlerMap{
		{"class_edit", SaveInventory},
		{"loadout_showcase_entered", SaveInventory}
	};
	if (lua_isstring(luaVM, 1)) {
		size_t strLen = 0;
		const char* rawStr = lua_tolstring(luaVM, 1, &strLen);
		std::string str(rawStr, strLen);
		if (handlerMap.find(str) != handlerMap.cend())
		{
			handlerMap.at(str)();
		}
	}
	lui_cod_luacall_enginenotifyserver_detour_impl.stub<void>(luaVM);
}

void lua_createtable(uintptr_t L, int narray, int nrec) {
	auto func = reinterpret_cast<void (*)(uintptr_t, int, int)>(0x1420833B0_g);
	func(L, narray, nrec);
}

float lua_tonumber32(uintptr_t luaVM, int index) {
	auto func = reinterpret_cast<float(*)(uintptr_t, int)>(0x1419CE9C0_g);
	return func(luaVM, index);
}

void LUI_BeginTable1(const char* key, uintptr_t luaVM)
{
	auto func = reinterpret_cast<void(*)(const char*, uintptr_t)>(0x1419BCF00_g);
	func(key, luaVM);
}

void LuaShared_SetTableString(const char* key, const char* value, uintptr_t luaVM)
{
	auto func = reinterpret_cast<void(*)(const char*, const char*, uintptr_t)>(0x1419CE5A0_g);
	func(key, value, luaVM);
}

void LuaShared_SetTableInt(const char* key, __int64 value, uintptr_t luaVM)
{
	auto func = reinterpret_cast<void(*)(const char*, __int64, uintptr_t)>(0x1419CE4F0_g);
	func(key, value, luaVM);
}

void LUI_EndTable(lua_State* luaVM) {
	auto func = reinterpret_cast<void(*)(lua_State*)>(0x1419BDAC0_g);
	func(luaVM);
}

void lua_pushnil(uintptr_t L) {
	auto func = reinterpret_cast<void(*)(uintptr_t)>(0x1420840E0_g);
	func(L);
}

void lua_pushnumber(uintptr_t L, long double n) {
	auto func = reinterpret_cast<void(*)(uintptr_t, long double n)>(0x142084100_g);
	func(L, n);
}

void LUI_OpenMenu(const char* menu) {
	auto func = reinterpret_cast<void(*)(int localClientNum, const char* menuName, int isPopup, int isModal, int isExclusive)>(0x141B9BDB0_g);
	func(0, menu, 0, 0, 0);
}

unsigned int GetMessageToDisplayCount_hk(MarketingCommsManager* a1, int messageType) {
	unsigned int temp = getmessagetodisplay.stub<unsigned int>(a1, messageType);
	if (messageType == 8) return 1;
	return temp;
}

__int64 LUI_CoD_LuaCall_CRMGetMessageContent_impl_hk(uintptr_t luaVM) {

	int controllerIndex = (int)lua_tonumber32(luaVM, 1);
	int locationID = (int)lua_tonumber32(luaVM, 2);
	int messageIndex = (int)lua_tonumber32(luaVM, 3);

	//printf("Controller: %d, location: %d, messageIndex: %d\n", controllerIndex, locationID, messageIndex);

	// might move to json in future idk
	if (locationID == 8 && !messageIndex) {
		// General Info (version etc)
		lua_createtable(luaVM, 0, 0);
		LUI_BeginTable1("message", luaVM);
		LuaShared_SetTableString("message_id", "1", luaVM);
		LuaShared_SetTableString("title", "MRON IV Info", luaVM);
		LuaShared_SetTableString("content_short", "^5Welcome to MRON!\n^3Website: www.mronmod.fun", luaVM);
		LuaShared_SetTableString("action", "goto_ingame", luaVM);
		LuaShared_SetTableString("content_long", "Welcome to MRON", luaVM);
		LuaShared_SetTableString("layout_type", "0", luaVM);

		//LuaShared_SetTableString("popup_image", "mw_store_billboard_bronze_knight", (uintptr_t)L);
		//LuaShared_SetTableString("image", "mw_store_billboard_bronze_knight", (uintptr_t)L);
		//LuaShared_SetTableString("action_location", "barracks", (uintptr_t)L);

		LuaShared_SetTableString("checksum", "12345678", luaVM);
		LuaShared_SetTableInt("location_id", 1, luaVM);
		LuaShared_SetTableInt("message_index", 0, luaVM);
		LUI_EndTable((lua_State*)luaVM);
	}
	else if (locationID == 8 && messageIndex == 1) {
		// Patch Notes section
		lua_createtable(luaVM, 0, 0);
		LUI_BeginTable1("message", luaVM);
		LuaShared_SetTableString("message_id", "1", luaVM);
		LuaShared_SetTableString("title", "MRON Patch Notes", luaVM);
		LuaShared_SetTableString("content_short", "^3MRON 4.0 June XX\n^1MRON Season 2 Update\n^2- NEW: Drones Attack\n^3Appears when anyone uses Directional UAV\n^3Has 100% chance to appear\n^5- Added automatization for settings saving\n^5- Added new localization strings\n^5- Opened BETA for BR Classic Mode\n^5- Bugfixes for Circles Randomization\n^2- NEW: Custom Dynamic Zones\n^3Select 4 circle zones in F1 menu\n^3Press Enable\n^5- Fixed a lot of in-code bugs\n^5- Reworked kill-rewards\n^5- Added ability to select kill count for DMZ Royale Nuke\n^5- Added lui_dev_features to be enabled by default\n^5- Added Armor, DMG Display and StimBoostV2 to MRONMultiplayer Menu\n\n^3MRON 0.5 Launch February 17\n^1- [CORRUPTED]\n^1- [REDACTED]\n", luaVM);
		LuaShared_SetTableString("action", "goto_ingame", luaVM);
		LuaShared_SetTableString("content_long", "Welcome to MRON", luaVM);
		LuaShared_SetTableString("layout_type", "0", luaVM);

		//LuaShared_SetTableString("popup_image", "mw_store_billboard_bronze_knight", (uintptr_t)L);
		//LuaShared_SetTableString("image", "mw_store_billboard_bronze_knight", (uintptr_t)L);
		//LuaShared_SetTableString("action_location", "barracks", (uintptr_t)L);

		LuaShared_SetTableString("checksum", "12345679", luaVM);
		LuaShared_SetTableInt("location_id", 1, luaVM);
		LuaShared_SetTableInt("message_index", 1, luaVM);
		LUI_EndTable((lua_State*)luaVM);
	}
	else {
		lua_pushnil(luaVM);
	}
	return 1;
}

__int64 lua_tointeger32(uintptr_t luaVM, int index) {
	auto func = reinterpret_cast<__int64(*)(uintptr_t, int)>(0x1419B78C0_g);
	return func(luaVM, index);
}

// function is only used for getting the map name from server table, shouldn't cause any issues
__int64 LUI_CoD_LuaCall_GetServerData_hk(uintptr_t luaVM) { // fixes map image not showing for servers
	int controller;
	int index;
	int column;

	controller = lua_tointeger32(luaVM, 1);
	index = lua_tointeger32(luaVM, 2);
	column = lua_tointeger32(luaVM, 3);
	int numlocalservers = *(int*)(0x14EEB1220_g);
	ClServerInfo* localServers = (ClServerInfo*)(0x14EEB1224_g);
	int* indexes = (int*)(0x152C46A8C_g);

	if (index > numlocalservers)
	{
		printf("[%s] Index exceeded number of local servers %d/%d\n", __FUNCTION__, index, numlocalservers);
		lua_pushstring(luaVM, "");
		return 1;
	}
	auto data = &localServers[indexes[index]];
	lua_pushstring(luaVM, data->mapName);

	return 1;
}

int LUI_CoD_LuaCall_GetSeasonRank(uintptr_t luaVM) {
	lua_pushnumber(luaVM, 155);
	return 1;
}

int LUI_CoD_LuaCall_GetPlayerXP(uintptr_t luaVM) {
	lua_pushnumber(luaVM, 999999999);
	return 1;
}

int LUI_CoD_LuaCall_GetSeasonXP(uintptr_t luaVM) {
	lua_pushnumber(luaVM, 999999999);
	return 1;
}
