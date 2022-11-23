#pragma once
#include "IGlobal.h"

uid guid = 10000;

int genGuid()
{
	return ++guid;
}