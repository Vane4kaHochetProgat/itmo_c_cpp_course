//
// Created by vane4ka on 02.06.2022.
//

#ifndef CPP_2_VANE4KAHOCHETPROGAT_LN_H
#define CPP_2_VANE4KAHOCHETPROGAT_LN_H

#include "int_vector.h"
#include <string_view>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <valarray>
#include <climits>

#define DIGIT_SIZE (100000000)

class LN
{
  public:
	// Constructors

	LN() = default;
	LN(long long number);
	LN(std::string_view number);
	LN(const LN& long_number);
	LN(LN&& long_number) noexcept;
	LN(const char* s);
	// Destructor

	~LN();

	// Special operators

	LN& operator=(const LN& long_number);
	LN& operator=(LN&& long_number) noexcept;

	// Cast operators

	explicit operator bool() const;
	explicit operator std::string();
	explicit operator long long();

	// Comparison operators

	bool operator==(const LN& rhs) const;
	bool operator!=(const LN& rhs) const;
	bool operator<(const LN& rhs) const;
	bool operator>(const LN& rhs) const;
	bool operator<=(const LN& rhs) const;
	bool operator>=(const LN& rhs) const;

	// Arithmetic operators

	// unary minus
	LN operator-();

	// sum
	LN operator+=(const LN& a);
	LN operator-=(const LN& a);
	friend LN operator+(const LN& lhs, const LN& rhs);
	friend LN operator-(const LN& lhs, const LN& rhs);

	// mult

	LN operator*=(const LN& a);
	friend LN operator*(const LN& lhs, const LN& rhs);

	// div

	LN operator/=(const LN& a);
	friend LN operator/(const LN& lhs, const LN& rhs);
	LN operator%=(const LN& a);
	friend LN operator%(const LN& lhs, const LN& rhs);

	// sqrt

	LN operator~() const;

  private:
	int_vector number;
	bool sign{ true };
	bool NOT_NAN{ true };
};

LN operator""_ln(const char* s);

#endif	  // CPP_2_VANE4KAHOCHETPROGAT_LN_H
