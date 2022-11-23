#pragma once
#include "../common/IGlobal.h"

uid guid = 10000;

int genGuid()
{
	return ++guid;
}