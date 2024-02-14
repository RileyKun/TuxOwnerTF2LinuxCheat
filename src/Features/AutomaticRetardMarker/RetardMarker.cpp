#include "RetardMarker.h"

CRetard gRetard;

void CRetard::Run()
{
    for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;
		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;
        FindNiggers(pEntity);
    }
}

bool CRetard::isWordInList(const std::string& word, const std::string wordsList[]) {
    for (int i = 0; !wordsList[i].empty(); ++i) {
        if (word == wordsList[i]) {
            return true;
        }
    }
    return false;
}


void CRetard::FindNiggers(CBaseEntity* pEntity)
{
    float flCurTime = gInts.Engine->Time();
	static float flNextSend = 0.0f;
	static float thesleeptime = 80.0f;

    player_info_t pInfo;
    if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
        return;

    std::string tranny[] = {"trans", "MtF", "FtM", "Trans", "Girl", "Transgender", "Queen", "queen", "Slvt", "catgirl" , "CatGirl", "Catgirl", "Maid", "maid", "lesbian", "UwU", "OwO", "⚧️", "xe", "xe/him", "they", "them", "estro", "estrogen", "she", "her", "she/her", "transrights", "rights", "#transrights", "submissive", "sub", "transgirl", "catboy", "CatBoy", "Catboy", "non-binary", "pride", "binary", "fempyro", "bussy", "daddy", "neko", "Neko", "Fox", "Foxxy", "Meow", "meow", "thighs", "thigh", "cute", "lives", "matter"}; // everyone who calls himself a "girl" espically who make it their name is either trans or just some man lol so ya
    std::string pedo[]   = {"Mimi Sentry", "mimi", "cunny", "loli", "mlp", "Dashie", "Pony", "lil", "little", "children"}; // idk
    std::string furry[]  = {"uwu", "owo", "proto", "protogen", "furry", "Fuwwy", ":3", "omo", ">w>",">w<","Fluffy","Fluff","Fur"};
    /*
    * Todo: more strings to mark retards as
    */
    std::string chatprintftranny = "\x6[\x3Tux\x6] \x4" + std::string(pInfo.name) + "is possibly a tranny";
    std::string chatprintffurry  = "\x6[\x3Tux\x6] \x4" + std::string(pInfo.name) + "is possibly a furfag";
    std::string chatprintfpedo  = "\x6[\x3Tux\x6] \x4" + std::string(pInfo.name) + "is possibly a pedophile";

    if(isWordInList(pInfo.name, tranny) && flCurTime > flNextSend) 
    {
        gInts.ClientMode->m_pChat->ChatPrintf(0, chatprintftranny.c_str());	   
        flNextSend = (flCurTime + thesleeptime);
    }
    if(isWordInList(pInfo.name, pedo) && flCurTime > flNextSend) 
    {
        gInts.ClientMode->m_pChat->ChatPrintf(0, chatprintfpedo.c_str());	   
        flNextSend = (flCurTime + thesleeptime);
    }
    if(isWordInList(pInfo.name, furry) && flCurTime > flNextSend) 
    {
        gInts.ClientMode->m_pChat->ChatPrintf(0, chatprintffurry.c_str());	   
        flNextSend = (flCurTime + thesleeptime);
    }
}