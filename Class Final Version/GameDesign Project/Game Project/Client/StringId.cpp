#include "StringId.h"


// static std::unordered_map<StringId, const char*> gStringIdTable;
StringId internString(const char* str)
{

	// Hash the string
	boost::crc_32_type result;
	result.process_bytes(str, strlen(str));
	StringId sid = result.checksum();
	return sid;
	/*
	std::unordered_map<StringId, const char*>::iterator it
		= gStringIdTable.find(sid);
	if (it == gStringIdTable.end())
	{
		// This string has not yet been added to the
		// table. Add it, being sure to copy it in case
		// the original was dynamically allocated and
		// might later be freed.
		gStringIdTable[sid] = strdup(str);
	}
	*/
}