//
// Created by vane4ka on 21.04.2022.
//

#ifndef CPP_1_VANE4KAHOCHETPROGAT_PHONEBOOK_H
#define CPP_1_VANE4KAHOCHETPROGAT_PHONEBOOK_H

#include <istream>
#include <ostream>
#include <string>
#include <utility>

class Phonebook {
public:
    struct Name {
        Name() = default;

        bool operator==(const Name &rhs) const;

        bool operator!=(const Name &rhs) const;

        bool operator<(const Name &rhs) const;

        bool operator>(const Name &rhs) const;

        bool operator<=(const Name &rhs) const;

        bool operator>=(const Name &rhs) const;

        std::string first_name;
        std::string second_name;
        std::string last_name;
    };

    Phonebook() = default;

    Phonebook(Name name, uint64_t phoneNumber);

    void setName(Name name);

    void setPhoneNumber(uint64_t phoneNumber);

    bool operator<(const Phonebook &rhs) const;

    bool operator>(const Phonebook &rhs) const;

    bool operator<=(const Phonebook &rhs) const;

    bool operator>=(const Phonebook &rhs) const;

    bool operator==(const Phonebook &rhs) const;

    bool operator!=(const Phonebook &rhs) const;

    friend std::istream &operator>>(std::istream &in, Phonebook &phonebook);

    friend std::ostream &operator<<(std::ostream &os, const Phonebook &phonebook);

private:
    Name name;
    std::uint64_t phone_number;
};

#endif      // CPP_1_VANE4KAHOCHETPROGAT_PHONEBOOK_H