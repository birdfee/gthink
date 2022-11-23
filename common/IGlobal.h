#pragma once
//#include <vector>
#include <map>

typedef unsigned __int64 u_int64;
typedef u_int64 uid;

//static uid guid = 10000;	//static / extern
extern uid guid;

//static int genGuid()	//static / extern
//{ 
//	return ++guid;
//}
extern int genGuid();

static std::map<int, int> mapGates;
//static std::vector<int> vecGates;