#pragma once

typedef unsigned __int64 u_int64;
typedef u_int64 uid;

static uid guid = 10000;

static int genGuid()
{
	return ++guid;
}