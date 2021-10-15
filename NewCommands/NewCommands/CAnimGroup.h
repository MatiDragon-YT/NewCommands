#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct animDesc
{
	int32_t animId;
	int32_t flags;
};

class CAnimGroup
{
public:
	char *groupName;
	char *blockName;
	int32_t modelIndex;
	int32_t animsCount;
	char **animNames;
	animDesc *animDesc;
};

#pragma pack(pop)