#ifndef RANDOM_H_
#define RANDOM_H_

	#include <random>

	class Random {
		private:
			std::mt19937_64 random;
		public:
			Random();
			unsigned long long next(unsigned long long max);
	};

#endif /* RANDOM_H_ */
