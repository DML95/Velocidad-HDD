#include "random.h"

Random::Random(){
	std::random_device randomDevice;
	this->random.seed(randomDevice());
}

unsigned long long Random::next(unsigned long long max){
	return this->random()%max;
}
