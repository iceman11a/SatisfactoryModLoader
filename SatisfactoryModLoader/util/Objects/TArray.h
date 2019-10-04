#pragma once

#include <stdexcept>
#include <Lib.h>
#include "FMemory.h"

namespace SML {
	namespace Objects {
		template<class T>
		class TArray {
		protected:
			T* data;
			int count;
			int max;

		public:
			inline TArray() {
				data = nullptr;
				count = max = 0;
			};

			inline TArray(const TArray<T>& other) {
				if (other.max) {
					data = (T*)FMemory::malloc(other.max * sizeof(T));
					max = other.max;
					count = other.count;
					for (int i = 0; i < count; ++i) {
						new (&data[i]) T(other.data[i]);
					}
				} else {
					data = nullptr;
					count = max = 0;
				}
			}

			inline TArray(TArray<T>&& other) {
				if (other.max) {
					data = other.data;
					other.data = nullptr;
					max = other.max;
					count = other.count;
					other.max = other.count = 0;
				} else {
					data = nullptr;
					count = max = 0;
				}
			}

			inline ~TArray() {
				for (int i = 0; i < count; ++i) {
					((T&) data[i]).~T();
				}
				if (max) FMemory::free(data);
			}

			inline TArray<T>& operator=(const TArray<T>& other) {
				if (max) {
					if (other.max) {
						data = (T*)FMemory::realloc(data, other.max * sizeof(T));
						data = (T*)std::memset(data, 0, other.count * sizeof(T));
						count = other.count;
						max = other.max;
						for (int i = 0; i < count; ++i) {
							new (&data[i]) T(other.data[i]);
						}
					} else {
						FMemory::free(data);
						data = nullptr;
						count = max = 0;
					}
				} else {
					if (other.max) {
						data = (T*)FMemory::malloc(other.max * sizeof(T));
						count = other.count;
						max = other.max;
						for (int i = 0; i < count; ++i) {
							new (&data[i]) T(other.data[i]);
						}
					} else {
						data = nullptr;
						max = count = 0;
					}
				}

				return *this;
			}

			inline TArray<T>& operator=(TArray<T>&& other) {
				if (data) FMemory::free(data);
				data = nullptr;
				count = max = 0;
				if (other.Max) {
					data = other.data;
					other.data = nullptr;
					max = other.max;
					count = other.count;
					other.max = other.count = 0;
				}

				return *this;
			}

			inline int num() const {
				return count;
			};

			inline T& operator[](int i) {
				if (i < 0 || i >= count) throw std::out_of_range(std::string("TArray out of range exception for index ").append(std::to_string(i)));
				return data[i];
			};

			inline const T& operator[](int i) const {
				if (i >= count || i < 0) throw std::out_of_range(std::string("TArray out of range exception for index ").append(std::to_string(i)));
				return data[i];
			};

			inline bool isValid(int i) const {
				return i < max && i > 0;
			}

			inline void* getData() const {
				return data;
			}

			inline void add(const T& item) {
				if (!max) {
					data = (T*) FMemory::malloc(1);
					count = 0;
					max = 1;
				} else if (count + 1 > max) {
					max = count + 1;
					data = (T*) FMemory::realloc(data, max * sizeof(T));
				}
				++count;
				new (&data[count - 1]) T(item);
			}

			inline void clear() {
				this->~TArray();
				count = max = 0;
				data = nullptr;
			}

			/* TODO: 
			- Add reallocation on subscript operator access
			- Add insert and erase functions
			*/
		};
	}
}