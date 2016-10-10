#pragma once

#ifdef _DEBUG 
#define ASSERT(condition, msg) if ((condition)) { throw std::exception(msg); } // msg should be a char array
#else 
#define ASSERT(condition, msg) (condition);
#endif