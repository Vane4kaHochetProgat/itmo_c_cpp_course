//
// Created by vane4ka on 20.06.2022.
//

#ifndef CPP_2_VANE4KAHOCHETPROGAT_INT_VECTOR_H
#define CPP_2_VANE4KAHOCHETPROGAT_INT_VECTOR_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

class int_vector
{
  public:
	int_vector();
	int_vector(std::size_t size);
	int_vector(std::size_t size, int32_t value);
	int_vector(const int_vector& intVector);
	int_vector(int_vector&& intVector) noexcept;

	void resize(std::size_t new_size);
	void push_back(const int32_t& item);
	[[nodiscard]] std::size_t size() const;

	int_vector& operator=(const int_vector& intVector);
	int_vector& operator=(int_vector&& intVector) noexcept;

	int32_t& operator[](std::size_t index);
	int32_t operator[](std::size_t index) const;
	//	int32_t& operator[](std::size_t index) const;

	int32_t back();

	void pop_back();

	void clear();

	~int_vector();

  private:
	int32_t* vector{ nullptr };
	std::size_t capacity{ 0 };
	std::size_t length{ 0 };
};

#endif	  // CPP_2_VANE4KAHOCHETPROGAT_INT_VECTOR_H
