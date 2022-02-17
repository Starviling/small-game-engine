/**
* String ID creation pulled from textbook: Page 461
* Game Engine Architecture Third Edition
*/
#pragma once
#include <boost/crc.hpp>  // for boost::crc_32_type
#include <unordered_map>

typedef unsigned int StringId;
extern StringId internString(const char* str);