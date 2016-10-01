#include <cstdint>
#include <cstring>
#include <memory>
#include <cmath>

namespace Sieves {
	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

	constexpr size_t sizeOfElementInBytes = sizeof(uint32_t);
	constexpr size_t sizeOfElementInBits = 8 * sizeOfElementInBytes;
	constexpr size_t logSizeOfElementInBits = compileTimeLogBase2(sizeOfElementInBits);

	class PrimeSieve {
		std::unique_ptr<uint32_t[]> m_sieve;
	public:
		PrimeSieve(const size_t minSieveSize) {
			const size_t numOfElementsInArray = 1 + minSieveSize / sizeOfElementInBits;
			m_sieve = std::make_unique<uint32_t[]>(numOfElementsInArray);
			const size_t limit = numOfElementsInArray*sizeOfElementInBytes;

			uint32_t* const sieve = m_sieve.get();
			std::memset(sieve, 0xAA, limit);
			sieve[0] ^= 6;

			const size_t sqrtLimit = std::sqrt(numOfElementsInArray*sizeOfElementInBits);
			for (size_t i = 3; i <= sqrtLimit; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1))))
					for (size_t j = i + i; j <= minSieveSize; j += i)
						sieve[j >> logSizeOfElementInBits] &= ~(1 << (j&(sizeOfElementInBits - 1)));
		}

		inline bool isPrime(size_t number) const {
			return m_sieve[number >> logSizeOfElementInBits] & (1 << (number&(sizeOfElementInBits - 1)));
		}
	};

	class FactorCountSieve {
		std::unique_ptr<uint8_t[]> m_sieve;
	public:
		FactorCountSieve(const size_t limit) : m_sieve(new uint8_t[limit + 1]) {
			uint8_t* const sieve = m_sieve.get();
			std::memset(sieve, 0, limit + 1);
			const size_t sqrtLimit = std::sqrt(limit + 1);
			for (size_t i = 2; i <= limit; ++i)
				if (!sieve[i])
					for (size_t j = i + i; j <= limit; j += i)
						sieve[j]++;
		}

		inline uint8_t numOfFactors(size_t number) const {
			return m_sieve[number];
		}

		inline uint8_t* get() const {
			return m_sieve.get();
		}
	};
}