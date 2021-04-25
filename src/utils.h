#ifndef UTILS_H_
#define UTILS_H_

	class Utils {
		private:
			Utils();
		public:
			//logaritmo en base 2
			//hacer version con la intruccion _tzcnt_u64
			static unsigned int log2(unsigned long long val);
	};

#endif /* UTILS_H_ */
