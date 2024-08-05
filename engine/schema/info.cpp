#include "info.h"
#include "../utils/utils.h"

void SchemaSystemTypeScope::FindDeclaredClass(SchemaClassInfoData** pReturnClass, const char* szClassName)
{
	return g_pUtils->m_VMT.CallVMT<void,2>(this, pReturnClass, szClassName);
}
