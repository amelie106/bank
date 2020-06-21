#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "person.h"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <iostream>

class Account
{

    int nr;
    int balance;
    int limit;
    std::map<std::string, std::weak_ptr<Person>> persons;
    double fee;

public:
    Account(int, int, int, std::shared_ptr<Person>);
    virtual ~Account() = default;
    int get_nr() const { return nr; }
    std::vector<std::string> get_authorized_persons() const;
    size_t number_of_owners() const;
    bool add_authorized_person(std::shared_ptr<Person>);
    void remove_authorized_person(std::string);
    bool transfer(std::shared_ptr<Account>, int);
    virtual double calc_fee(int) const = 0;
    std::ostream &print_for_person(std::ostream &) const;
    std::ostream &print(std::ostream &) const;
};

class GiroAccount : public Account
{
    double fee_factor;

public:
    GiroAccount(int, int, int, std::shared_ptr<Person>, double);
    double calc_fee(int) const
    {
        return fee_factor;
    };
};

class BusinessAccount : public Account
{

    double fee_factor;

public:
    BusinessAccount(int, int, int, std::shared_ptr<Person>, double);
    double calc_fee(int x) const
    {
        return fee_factor * x;
    };
};

#endif