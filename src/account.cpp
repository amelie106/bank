#include "account.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>

// Account Methods

Account::Account(int n, int b, int l, std::shared_ptr<Person> p)
{

    (n >= 0) ? nr = n : throw std::runtime_error("Account number below 0.");
    (l >= 0) ? limit = l : throw std::runtime_error("Limit below 0.");
    (b >= (limit - 2 * limit)) ? balance = b : throw std::runtime_error("Balance below limit.");
    std::weak_ptr<Person> w_ptr = p;
    persons[p->get_name()] = w_ptr;
    fee = 0;
}

std::vector<std::string> Account::get_authorized_persons() const
{
    std::vector<std::string> authorized_persons;

    for (auto &i : persons)
    {
        if (!i.second.expired())
        {
            authorized_persons.push_back(i.second.lock()->get_name());
        }
    }

    return authorized_persons;
}

size_t Account::number_of_owners() const
{
    size_t owners{0};

    for (auto itr{this->persons.begin()}; itr != this->persons.end(); itr++)
    {
        if (!itr->second.expired())
        {
            owners++;
        }
    }
    return owners;
}

bool Account::add_authorized_person(std::shared_ptr<Person> p)
{
    bool not_yet_authorized{true};

    for (auto itr{persons.begin()}; itr != persons.end(); itr++)
    {
        std::shared_ptr<Person> shared = itr->second.lock();
        if (shared->get_name() == p->get_name())
            not_yet_authorized = false;
    }

    if (not_yet_authorized)
    {
        persons[p->get_name()] = p;
    }

    return not_yet_authorized;
}

void Account::remove_authorized_person(std::string name)
{

    std::map<std::string, std::weak_ptr<Person>> old_persons;

    for (const auto &i : persons)
        old_persons[i.second.lock()->get_name()] = i.second;

    for (auto &i : old_persons)
    {
        if (i.second.lock()->get_name() == name)
            persons.erase(name);
    }
}

bool Account::transfer(std::shared_ptr<Account> target, int x)
{
    int potential_new_balance{balance - x};
    bool transfer_successful{false};

    if (potential_new_balance >= (limit - 2 * limit))
    {
        // transfer amount
        balance = potential_new_balance;
        target->balance += x;
        // add fees
        this->fee += calc_fee(x);
        target->fee += target->calc_fee(x);

        transfer_successful = true;
    }
    return transfer_successful;
}

std::ostream &Account::print_for_person(std::ostream &o) const
{

    bool first_acc{true};

    o << "Nr.: " << nr;
    o << ", [" << balance << '/' << limit << "],";
    o << " Fees: " << fee << ',';
    o << " Owners: {";

    for (const auto &i : persons)
    {
        bool first_nr{true};
        if (!i.second.expired())
        {
            std::shared_ptr<Person> shared = i.second.lock();

            if (first_acc)
                first_acc = false;
            else
                o << ", ";

            o << shared->get_name();
        }
    }

    o << "}";

    return o;
}

std::ostream &Account::print(std::ostream &o) const
{

    bool first_acc{true};

    o << "Nr.: " << nr;
    o << ", [" << balance << '/' << limit << "],";
    o << " Fees: " << fee << ',';
    o << " Owners: {";

    for (const auto &i : persons)
    {
        bool first_nr{true};
        if (!i.second.expired())
        {
            std::shared_ptr<Person> shared = i.second.lock();

            if (first_acc)
                first_acc = false;
            else
                o << ", ";

            o << "[Name: " << shared->get_name();
            o << ", {";

            for (const auto &num : shared->get_account_nrs())
            {
                if (first_nr)
                {
                    o << num;
                    first_nr = false;
                }
                else
                {
                    o << ", " << num;
                }
            }
            o << "}]";
        }
    }

    o << '}';

    return o;
}

// GiroAccount Constructor

GiroAccount::GiroAccount(int nr, int balance, int limit, std::shared_ptr<Person> p, double ff) : Account(nr, balance, limit, p)
{

    (ff >= 0) ? fee_factor = ff : throw std::runtime_error("Fee Factor below 0.");
}

// BusinessAccount Constructor

BusinessAccount::BusinessAccount(int nr, int balance, int limit, std::shared_ptr<Person> p, double ff) : Account(nr, balance, limit, p)
{

    (ff >= 0) ? this->fee_factor = ff : throw std::runtime_error("Fee Factor below 0.");
}