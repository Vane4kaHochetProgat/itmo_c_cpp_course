//
// Created by vane4ka on 20.06.2022.
//

#include "int_vector.h"

int_vector::int_vector() = default;
int_vector::int_vector(std::size_t size)
{
	this->clear();
	length = size;
	capacity = 2 * size + 1;
	vector = (int32_t*)malloc(capacity * sizeof(int32_t));
	for (std::size_t i = 0; i < size; i++)
	{
		vector[i] = 0;
	}
}
int_vector::int_vector(std::size_t size, int32_t value)
{
	this->clear();
	length = size;
	capacity = 2 * size + 1;
	vector = (int32_t*)malloc(capacity * sizeof(int32_t));
	if (vector == nullptr)
	{
		throw std::bad_alloc();
	}
	for (std::size_t i = 0; i < size; i++)
	{
		vector[i] = value;
	}
}
int_vector::int_vector(const int_vector& intVector)
{
	this->clear();
	length = intVector.length;
	capacity = intVector.capacity;
	this->vector = (int32_t*)malloc(capacity * sizeof(int32_t));
	if (this->vector == nullptr)
	{
		throw std::bad_alloc();
	}
	std::memcpy(this->vector, intVector.vector, length * sizeof(int32_t));
}
int_vector::int_vector(int_vector&& intVector) noexcept
{
	this->clear();
	length = intVector.length;
	capacity = intVector.capacity;
	vector = intVector.vector;
	intVector.vector = nullptr;
	intVector.capacity = 0;
	intVector.length = 0;
}
std::size_t int_vector::size() const
{
	return this->length;
}
int_vector& int_vector::operator=(const int_vector& intVector)
{
	this->clear();
	if (this != &intVector)
	{
		this->length = intVector.length;
		this->capacity = intVector.capacity;
		this->vector = (int32_t*)malloc(capacity * sizeof(int32_t));
		if (this->vector == nullptr)
		{
			throw std::bad_alloc();
		}
		std::memcpy(this->vector, intVector.vector, length * sizeof(int32_t));
	}
	return *this;
}
int_vector& int_vector::operator=(int_vector&& intVector) noexcept
{
	this->clear();
	if (this != &intVector)
	{
		this->length = intVector.length;
		this->capacity = intVector.capacity;
		this->vector = intVector.vector;
		intVector.vector = nullptr;
		intVector.capacity = 0;
		intVector.length = 0;
	}
	return *this;
}
int32_t& int_vector::operator[](std::size_t index)
{
	return this->vector[index];
}
int32_t int_vector::operator[](std::size_t index) const
{
	return this->vector[index];
}
int_vector::~int_vector()
{
	capacity = 0;
	length = 0;
	if (vector != nullptr)
	{
		free(vector);
	}
	vector = nullptr;
}
void int_vector::clear()
{
	capacity = 0;
	length = 0;
	if (vector != nullptr)
	{
		free(vector);
	}
	vector = nullptr;
}
int32_t int_vector::back()
{
	if (length == 0)
	{
		throw std::length_error("Index -1 is out of range");
	}
	return vector[length - 1];
}
void int_vector::resize(std::size_t new_size)
{
	if (this->vector == nullptr)
	{
		*this = int_vector(0);
	}
	if (2 * new_size <= capacity && length <= new_size)
	{
		this->length = new_size;
		return;
	}
	this->capacity = 2 * new_size;
	auto* temp = (int32_t*)realloc(this->vector, capacity * sizeof(int32_t));
	if (this->vector == nullptr)
	{
		throw std::bad_alloc();
	}
	this->vector = temp;
	for (std::size_t i = length; i < new_size; i++)
	{
		vector[i] = 0;
	}
	this->length = new_size;
}
void int_vector::pop_back()
{
	if (length == 0)
	{
		return;
	}
	this->length--;
}
void int_vector::push_back(const int32_t& item)
{
	if (length == 0)
	{
		*this = int_vector(1);
	}
	else
	{
		this->resize(length + 1);
	}
	this->vector[length - 1] = item;
}
