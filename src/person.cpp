#include "person.h"
#include "account.h"
#include <string>
#include <stdexcept>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

Person::Person(std::string n)
{

    (n.length() > 0) ? name = n : throw std::runtime_error("Person's name invalid.");
}

std::shared_ptr<Account> Person::create_account(int nr, int balance, int limit, int mode, double fee_factor)
{

    if (mode != 0 && mode != 1)
        throw std::runtime_error("Mode invalid.");

    if (mode == 0)
    {
        auto sh_ptr = std::make_shared<GiroAccount>(nr, balance, limit, get_shared(), fee_factor);
        accounts[nr] = sh_ptr;
        return sh_ptr;
    }
    else
    {
        auto sh_ptr = std::make_shared<BusinessAccount>(nr, balance, limit, get_shared(), fee_factor);
        accounts[nr] = sh_ptr;
        return sh_ptr;
    }
}

// do not use directly
void Person::add_account(std::shared_ptr<Account> acc_ptr)
{

    accounts[acc_ptr->get_nr()] = acc_ptr;
}

//do not use directly
void Person::remove_account(int n)
{

    std::map<int, std::weak_ptr<Account>> old_accounts;

    for (const auto &i : accounts)
        old_accounts[i.second.lock()->get_nr()] = i.second;

    for (auto &i : old_accounts)
    {
        if (i.second.lock()->get_nr() == n)
            accounts.erase(n);
    }
}

std::size_t Person::number_of_accounts() const
{
    size_t num{0};

    for (auto itr{this->accounts.begin()}; itr != this->accounts.end(); itr++)
    {
        if (!itr->second.expired())
        {
            num++;
        }
    }

    return num;
}

std::vector<int> Person::get_account_nrs() const
{

    std::vector<int> acc_nrs;

    for (auto itr{this->accounts.begin()}; itr != this->accounts.end(); itr++)
    {
        std::shared_ptr<Account> shared = itr->second.lock();
        acc_nrs.push_back(shared->get_nr());
    }

    return acc_nrs;
}

bool Person::share_account(std::shared_ptr<Person> other, int nr)
{

    bool nr_is_there{false};

    for (auto itr{accounts.begin()}; itr != accounts.end(); itr++)
    {
        if (itr->first == nr)
            nr_is_there = true;
    }

    if (!nr_is_there)
    {
        return false;
    }

    std::shared_ptr<Account> shared = accounts[nr].lock();
    auto success = shared->add_authorized_person(other);
    other->add_account(shared);
    return success;
}

std::ostream &Person::print(std::ostream &o) const
{
    bool first_acc{true};
    o << "Name: " << name;
    o << ", {";

    for (auto itr{this->accounts.begin()}; itr != this->accounts.end(); itr++)

    {
        if (!itr->second.expired())
        {
            std::shared_ptr<Account> shared = itr->second.lock();
            if (first_acc)
            {
                o << '[';
                shared->print_for_person(o);
                first_acc = false;
            }
            else
            {
                o << ", [";
                shared->print_for_person(o);
            }
        }
        o << ']';
    }

    o << '}';

    return o;
}