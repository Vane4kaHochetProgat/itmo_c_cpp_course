//
// Created by vane4ka on 21.04.2022.
//

#include "phonebook.h"

bool Phonebook::Name::operator==(const Phonebook::Name &rhs) const {
    return first_name == rhs.first_name && second_name == rhs.second_name && last_name == rhs.last_name;
}

bool Phonebook::Name::operator!=(const Phonebook::Name &rhs) const {
    return !(*this == rhs);
}

bool Phonebook::Name::operator<(const Phonebook::Name &rhs) const {
    if (last_name == rhs.last_name) {
        if (first_name == rhs.first_name) {
            return second_name < rhs.second_name;
        }
        return first_name < rhs.first_name;
    }
    return last_name < rhs.last_name;
}

bool Phonebook::Name::operator>(const Phonebook::Name &rhs) const {
    return rhs < *this;
}

bool Phonebook::Name::operator<=(const Phonebook::Name &rhs) const {
    return !(*this > rhs);
}

bool Phonebook::Name::operator>=(const Phonebook::Name &rhs) const {
    return !(*this < rhs);
}

Phonebook::Phonebook(Name name, std::uint64_t phoneNumber) : name(std::move(name)), phone_number(phoneNumber) {}

void Phonebook::setName(Name name) {
    this->name = std::move(name);
}

void Phonebook::setPhoneNumber(std::uint64_t phoneNumber) {
    phone_number = phoneNumber;
}

bool Phonebook::operator<(const Phonebook &rhs) const {
    if (name == rhs.name) {
        return phone_number < rhs.phone_number;
    }
    return name < rhs.name;
}

bool Phonebook::operator>(const Phonebook &rhs) const {
    return rhs < *this;
}

bool Phonebook::operator<=(const Phonebook &rhs) const {
    return !(*this > rhs);
}

bool Phonebook::operator>=(const Phonebook &rhs) const {
    return !(*this < rhs);
}

bool Phonebook::operator==(const Phonebook &rhs) const {
    return name == rhs.name && phone_number == rhs.phone_number;
}

bool Phonebook::operator!=(const Phonebook &rhs) const {
    return !(*this == rhs);
}

std::istream &operator>>(std::istream &in, Phonebook &phonebook) {
    Phonebook::Name name;
    std::uint64_t phone_number;
    in >> name.last_name >> name.first_name >> name.second_name;
    in >> phone_number;
    phonebook.setName(std::move(name));
    phonebook.setPhoneNumber(phone_number);
    return in;
}

std::ostream &operator<<(std::ostream &os, const Phonebook &phonebook) {
    os << phonebook.name.last_name << " " << phonebook.name.first_name << " " << phonebook.name.second_name << " "
       << phonebook.phone_number;
    return os;
}
