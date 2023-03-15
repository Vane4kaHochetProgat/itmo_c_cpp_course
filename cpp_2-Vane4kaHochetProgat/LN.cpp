//
// Created by vane4ka on 02.06.2022.
//

#include "LN.h"

LN::LN(std::string_view number)
{
	if (number.empty() || number == "NAN" || number == "NaN")
	{
		this->NOT_NAN = false;
		this->sign = true;
		this->number = int_vector(0);
		return;
	}
	this->NOT_NAN = true;
	int from = 0;

	if (number[0] == '-')
	{
		this->sign = false;
		from = 1;
	}
	else
	{
		this->sign = true;
	}
	uint32_t x = 0;
	uint32_t pow = 1;
	for (long long i = (long long)number.length() - 1; i >= from;)
	{
		while (pow < DIGIT_SIZE)
		{
			if ((number[i] - '0') < 0 || 9 < (number[i] - '0'))
			{
				throw std::invalid_argument("Constructor argument should be a decimal number");
			}
			x += pow * (number[i] - '0');
			pow *= 10;
			i--;
			if (i < from)
			{
				break;
			}
		}
		this->number.push_back((int32_t)x % DIGIT_SIZE);
		x /= DIGIT_SIZE;
		pow = 1;
	}
}

LN::LN(LN&& long_number) noexcept :
	number(std::move(long_number.number)), sign(long_number.sign), NOT_NAN(long_number.NOT_NAN)
{
}

LN& LN::operator=(const LN& long_number)
{
	if (this != &long_number)
	{
		this->sign = long_number.sign;
		this->number = long_number.number;
		this->NOT_NAN = long_number.NOT_NAN;
	}
	return *this;
}
LN& LN::operator=(LN&& long_number) noexcept
{
	if (this != &long_number)
	{
		this->sign = long_number.sign;
		this->number = std::move(long_number.number);
		this->NOT_NAN = long_number.NOT_NAN;
	}
	return *this;
}

// Comparison operators

bool LN::operator==(const LN& rhs) const
{
	if (!this->NOT_NAN || !rhs.NOT_NAN)
	{
		return false;
	}
	if (this->sign != rhs.sign)
	{
		return false;
	}
	if (this->number.size() != rhs.number.size())
	{
		return false;
	}

	for (std::size_t i = this->number.size(); i > 0; i--)
	{
		if (this->number[i - 1] != rhs.number[i - 1])
		{
			return false;
		}
	}
	return true;
}
bool LN::operator!=(const LN& rhs) const
{
	if (!this->NOT_NAN || !rhs.NOT_NAN)
	{
		return true;
	}
	return !(rhs == *this);
}
bool LN::operator<(const LN& rhs) const
{
	if (!this->NOT_NAN || !rhs.NOT_NAN)
	{
		return false;
	}
	if (this->sign && !rhs.sign)
	{
		return false;
	}

	if (!this->sign && rhs.sign)
	{
		return true;
	}

	if (this->number.size() > rhs.number.size())
	{
		return !this->sign;
	}

	if (this->number.size() < rhs.number.size())
	{
		return this->sign;
	}

	for (std::size_t i = this->number.size(); i > 0; i--)
	{
		if (this->number[i - 1] > rhs.number[i - 1])
		{
			return !this->sign;
		}

		if (this->number[i - 1] < rhs.number[i - 1])
		{
			return this->sign;
		}
	}
	return false;
}

bool LN::operator<=(const LN& rhs) const
{
	if (!this->NOT_NAN || !rhs.NOT_NAN)
	{
		return false;
	}
	if (!this->NOT_NAN || !rhs.NOT_NAN)
	{
		return false;
	}
	if (*this == rhs)
	{
		return true;
	}

	if (*this < rhs)
	{
		return true;
	}

	return false;
}

bool LN::operator>(const LN& rhs) const
{
	if (!this->NOT_NAN || !rhs.NOT_NAN)
	{
		return false;
	}
	return !(*this <= rhs);
}

bool LN::operator>=(const LN& rhs) const
{
	if (!this->NOT_NAN || !rhs.NOT_NAN)
	{
		return false;
	}
	return !(*this < rhs);
}

LN LN::operator+=(const LN& a)
{
	if (!this->NOT_NAN || !a.NOT_NAN)
	{
		*this = LN("NAN");
		return *this;
	}

	if (a.sign == this->sign)
	{
		if (a.number.size() > this->number.size())
		{
			this->number.resize(a.number.size());
		}
		uint64_t temp = 0;
		for (std::size_t i = 0; i < this->number.size(); i++)
		{
			this->number[i] = this->number[i] + (int32_t)temp;
			if (a.number.size() <= i)
			{
				break;
			}
			temp = this->number[i] + a.number[i];
			if (temp >= DIGIT_SIZE)
			{
				this->number[i] = (int32_t)(temp % DIGIT_SIZE);
				temp /= DIGIT_SIZE;
			}
			else
			{
				this->number[i] = (int32_t)temp;
				temp = 0;
			}
			if (i == this->number.size() - 1 && temp != 0)
			{
				this->number.push_back((int32_t)temp);
			}
		}
	}
	else
	{
		bool flag = this->sign;
		this->sign = a.sign;
		if (*this == a)
		{
			this->number.clear();
			this->number.push_back(0);
			this->sign = true;
			return *this;
		}
		LN a_copy = a;
		if (!flag && *this > a || flag && *this < a)
		{
			this->sign = flag;
		}
		else
		{
			this->sign = !flag;
			std::swap(a_copy.number, this->number);
		}

		for (std::size_t i = 0; i < this->number.size(); i++)
		{
			if (i >= a_copy.number.size())
			{
				break;
			}
			int32_t temp = this->number[i] - a_copy.number[i];
			if (temp < 0)
			{
				temp += DIGIT_SIZE;
				this->number[i + 1]--;
			}
			this->number[i] = temp;
		}
		while (this->number.size() > 1 && this->number.back() == 0)
		{
			this->number.pop_back();
		}
	}

	return *this;
}

LN LN::operator-=(const LN& a)
{
	if (!this->NOT_NAN || !a.NOT_NAN)
	{
		*this = LN("NAN");
		return *this;
	}
	LN a_copy(a);
	a_copy.sign = !a.sign;
	*this += a_copy;
	return *this;
}
LN operator+(const LN& lhs, const LN& rhs)
{
	if (!lhs.NOT_NAN || !rhs.NOT_NAN)
	{
		LN temp("NAN");
		return temp;
	}
	LN temp(lhs);
	return temp += rhs;
}

LN operator-(const LN& lhs, const LN& rhs)
{
	if (!lhs.NOT_NAN || !rhs.NOT_NAN)
	{
		LN temp("NAN");
		return temp;
	}
	LN temp(lhs);
	return temp -= rhs;
}

LN LN::operator-()
{
	if (!this->NOT_NAN)
	{
		return *this;
	}
	if (!*this)
	{
		return *this;
	}
	LN temp(*this);
	temp.sign = !this->sign;
	return temp;
}
LN::operator bool() const
{
	if (!this->NOT_NAN)
	{
		return true;
	}
	return !(/*this->sign && */ this->number.size() == 1 && this->number[0] == 0);
}
LN::operator long long()
{
	// return std::stoll((std::string)(*this));
	if (*this > LN(LLONG_MAX) || *this < LN(LLONG_MIN))
	{
		throw std::out_of_range("Invalid cast to long long - LN value is out of range");
	}
	long long a = 0;
	long long pow = 1;
	for (std::size_t i = 0; i < this->number.size(); i++)
	{
		a += this->number[i] * pow;
		pow *= DIGIT_SIZE;
	}
	a *= (this->sign ? 1 : -1);
	return a;
}
LN::operator std::string()
{
	if (!this->NOT_NAN)
	{
		return "NaN";
	}
	std::string s = (this->sign ? "" : "-");
	for (std::size_t i = this->number.size(); i > 0; i--)
	{
		std::string a = std::to_string(this->number[i - 1]);
		if (i != this->number.size() && a.length() <= 8)
		{
			const std::size_t length = 8 - a.length();
			s += std::string(length, '0');
		}
		s += a;
	}
	return s;
}

LN LN::operator*=(const LN& a)
{
	if (!this->NOT_NAN || !a.NOT_NAN)
	{
		*this = LN("NAN");
		return *this;
	}
	if (this->sign || a.sign)
	{
		this->sign &= a.sign;
	}
	else
	{
		this->sign = true;
	}
	int_vector temp_v(this->number.size() + a.number.size(), 0);

	for (std::size_t j = 0; j < this->number.size(); j++)
	{
		uint64_t temp = 0;
		for (std::size_t i = 0; i < a.number.size() || temp; i++)
		{
			uint64_t cur = uint64_t(this->number[j]) * (i < a.number.size() ? a.number[i] : 0);
			cur += temp_v[i + j] + temp;

			temp_v[i + j] = int32_t(cur % DIGIT_SIZE);
			temp = cur / DIGIT_SIZE;
		}
	}

	while (temp_v.size() > 1 && temp_v.back() == 0)
	{
		temp_v.pop_back();
	}
	this->number = temp_v;
	return *this;
}

LN operator*(const LN& lhs, const LN& rhs)
{
	if (!lhs.NOT_NAN || !rhs.NOT_NAN)
	{
		LN temp("NAN");
		return temp;
	}
	LN temp(lhs);
	return temp *= rhs;
}
LN LN::operator/=(const LN& a)
{
	if (!this->NOT_NAN || !a.NOT_NAN || !a)
	{
		*this = LN("NAN");
		return *this;
	}
	if (!(*this))
	{
		return *this;
	}
	LN a_copy(a);
	a_copy.sign = this->sign;
	if (this->sign && *this < a_copy || !this->sign && *this > a_copy)
	{
		this->number = int_vector(1, 0);
		this->sign = true;
		return *this;
	}

	if (this->sign || a.sign)
	{
		this->sign &= a.sign;
	}
	else
	{
		this->sign = true;
	}
	a_copy.sign = true;

	int_vector temp_v(this->number.size(), 0);

	LN current_value("0");

	for (std::size_t i = this->number.size(); i > 0; i--)
	{
		current_value = current_value * DIGIT_SIZE;
		current_value.number[0] = current_value.number[0] + this->number[i - 1];

		int32_t x = 0;
		int32_t left_index = 0;
		int32_t right_index = DIGIT_SIZE;

		while (left_index <= right_index)
		{
			int32_t middle = (left_index + right_index) >> 1;

			LN cur = a_copy * middle;
			if (cur <= current_value)
			{
				x = middle;
				left_index = middle + 1;
			}
			else
			{
				right_index = middle - 1;
			}
		}
		temp_v[i - 1] = x;
		current_value -= a_copy * x;
	}

	while (temp_v.size() > 1 && temp_v.back() == 0)
	{
		temp_v.pop_back();
	}
	this->number = temp_v;
	return *this;
}

LN operator/(const LN& lhs, const LN& rhs)
{
	if (!lhs.NOT_NAN || !rhs.NOT_NAN || !rhs)
	{
		LN temp("NAN");
		return temp;
	}
	LN temp(lhs);
	return temp /= rhs;
}
LN LN::operator%=(const LN& a)
{
	if (!this->NOT_NAN || !a.NOT_NAN || !a)
	{
		*this = LN("NAN");
		return *this;
	}
	*this -= (*this / a) * a;
	return *this;
}
LN operator%(const LN& lhs, const LN& rhs)
{
	if (!lhs.NOT_NAN || !rhs.NOT_NAN || !rhs)
	{
		LN temp("NAN");
		return temp;
	}
	LN temp(lhs);
	return temp %= rhs;
}

LN operator""_ln(const char* s)
{
	return { std::string_view(s) };
}

LN::LN(const char* s)
{
	*this = LN(std::string_view(s));
}

LN LN::operator~() const
{
	if (!this->sign || !this->NOT_NAN)
	{
		return { "NAN" };
	}

	LN low("0");
	LN high(*this);
	LN ONE("1");
	high += ONE;
	LN TWO("2");
	while (high - low > ONE)
	{
		//	std::cout << (std::string)(high - low) << "\n";
		LN mid = (low + high) / TWO;
		if (mid * mid <= *this)
		{
			low = mid;
		}
		else
		{
			high = mid;
		}
	}

	return low;
}
LN::~LN()
{
	number.clear();
}
LN::LN(const LN& long_number) : NOT_NAN(long_number.NOT_NAN), sign(long_number.sign)
{
	this->number = long_number.number;
}

LN::LN(long long number)
{
	bool minimum = false;
	if (number == LLONG_MIN)
	{
		minimum = true;
		number++;
	}
	this->NOT_NAN = true;
	this->number.resize(sizeof(long long) / sizeof(int32_t) + 1);
	if (number >= 0)
	{
		this->sign = true;
	}
	else
	{
		this->sign = false;
		number = -number;
	}
	int i = 0;
	while (number > 0)
	{
		this->number[i++] = (int32_t)(number % DIGIT_SIZE);
		number /= DIGIT_SIZE;
	}
	this->number[0] += minimum;
	while (this->number.back() == 0 && this->number.size() > 1)
	{
		this->number.pop_back();
	}
}