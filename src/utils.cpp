#include "utils.h"

unsigned int Utils::log2(unsigned long long val){
	int ret=64;
	for(;val!=0;val>>=1) {
		ret--;
	}
	return 64-ret;
}

