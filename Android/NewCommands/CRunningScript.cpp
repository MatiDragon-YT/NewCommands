#include "CRunningScript.h"
#include "NewCommands.h"

void CRunningScript::UpdateCompareFlag(unsigned char flag)
{
	if (GameId != GTALCS)
	{
		return CArmCall::Method<void, CRunningScript*, unsigned char>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN14CRunningScript17UpdateCompareFlagEh")), this, flag);
	}
	else
	{
		LCS_Script* script = reinterpret_cast<LCS_Script*>(this);
		if (script->notFlag)
			flag = !flag;
		if (script->logicalOp == 0) {
			script->condResult = flag;
			return;
		}
		if (script->logicalOp >= 1 && script->logicalOp <= 8) {
			script->condResult &= flag;
			if (script->logicalOp == 1) {
				script->logicalOp = 0;
				return;
			}
		}
		else {
			script->condResult |= flag;
			if (script->logicalOp <= 21) {
				script->logicalOp = 0;
				return;
			}
		}
		script->logicalOp--;
	}
}

void CRunningScript::ReadTextLabelFromScript(char* pBuffer, unsigned char nBufferLength)
{
	return CArmCall::Method<void, CRunningScript*, char*, unsigned char>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN14CRunningScript23ReadTextLabelFromScriptEPch")), this, pBuffer, nBufferLength);
}
