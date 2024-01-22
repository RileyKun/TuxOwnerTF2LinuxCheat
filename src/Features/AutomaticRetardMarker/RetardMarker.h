#include "../../SDK.h"

class CRetard 
{
public:
    void Run();
    void FindNiggers(CBaseEntity* pEntity);
    bool isWordInList(const std::string& word, const std::string wordsList[]);
};

extern CRetard gRetard;