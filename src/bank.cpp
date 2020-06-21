#include "bank.h"
#include "person.h"
#include "account.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

void Bank::create_person(std::string name, int nr, int balance, int limit, int mode, double fee_factor)
{

    for (auto it{customers.begin()}; it != customers.end(); it++)
    {
        if (it->first == name)
            throw std::runtime_error("Name already in Bank.");
    }

    for (auto it{accounts.begin()}; it != accounts.end(); it++)
    {
        if (it->first == nr)
            throw std::runtime_error("Account Nr already in Bank.");
    }

    std::shared_ptr<Person> per_ptr = std::make_shared<Person>(name);
    customers[name] = per_ptr;

    auto acc_ptr = per_ptr->create_account(nr, balance, limit, mode, fee_factor);
    accounts[nr] = acc_ptr;
}

std::shared_ptr<Account> Bank::create_account(std::shared_ptr<Person> ptr, int nr, int balance,
                                              int limit, int mode, double fee_factor)
{

    bool is_new{true};

    for (auto it{customers.begin()}; it != customers.end(); it++)
    {
        if (it->first == ptr->get_name())
            is_new = false;
    }

    if (is_new)
        throw std::runtime_error("Person not yet in Bank - please create person first");

    auto acc_ptr = customers[ptr->get_name()]->create_account(nr, balance, limit, mode, fee_factor);
    accounts[nr] = acc_ptr;

    return acc_ptr;
}

std::shared_ptr<Person> Bank::get_person(std::string n)
{

    std::map<std::string, std::shared_ptr<Person>>::iterator it = customers.find(n);

    if (it == customers.end())
        throw std::runtime_error("Person not in Bank");

    return it->second;
}

std::shared_ptr<Account> Bank::get_account(int n)
{

    std::map<int, std::shared_ptr<Account>>::iterator it = accounts.find(n);

    if (it == accounts.end())
        throw std::runtime_error("Account not in Bank");

    return it->second;
}

bool Bank::remove_person(std::string n)
{

    std::map<std::string, std::shared_ptr<Person>>::iterator item_ptr = customers.find(n);

    if (item_ptr == customers.end())
        return false;

    std::map<int, std::shared_ptr<Account>> old_accounts;

    for (const auto &i : accounts)
    {
        old_accounts[i.second->get_nr()] = i.second;
    }

    for (auto &i : old_accounts)
    {

        bool person_is_in{false};

        for (const auto &p : i.second->get_authorized_persons())
        {
            if (p == n)
                person_is_in = true;
        }

        for (auto &p : i.second->get_authorized_persons())
        {
            if (p == n && i.second->get_authorized_persons().size() <= 1) // Entire account deleted (only had this person)
            {
                accounts.erase(i.second->get_nr());
            }
            else if (p == n && i.second->get_authorized_persons().size() > 1)
            {
                accounts[i.second->get_nr()]->remove_authorized_person(n); // Account deleted from person
            }
        }
    }

    customers.erase(n);

    return true;
}

bool Bank::remove_account(int n)
{

    std::map<int, std::shared_ptr<Account>>::iterator item_ptr = accounts.find(n);

    if (item_ptr == accounts.end())
        return false;

    std::map<std::string, std::shared_ptr<Person>> old_persons;

    for (const auto &i : customers)
    {
        old_persons[i.second->get_name()] = i.second;
    }

    for (auto &i : old_persons)
    {

        bool account_is_in{false};

        for (const auto &p : i.second->get_account_nrs())
        {
            if (p == n)
                account_is_in = true;
        }

        for (auto &p : i.second->get_account_nrs())
        {
            if (p == n && i.second->get_account_nrs().size() <= 1) // Entire person deleted (only had this account)
            {
                customers.erase(i.second->get_name());
            }
            else if (p == n && i.second->get_account_nrs().size() > 1) // Account deleted from person
            {
                customers[i.second->get_name()]->remove_account(n);
            }
        }
    }

    accounts.erase(n);

    return true;
}

std::ostream &Bank::print_customers(std::ostream &o) const
{

    o << '[' << std::endl;

    for (const auto &i : customers)

    {
        i.second->print(o);
        o << std::endl;
    }

    if (customers.size() == 0)
        o << std::endl;
    o << ']';

    return o;
}

std::ostream &Bank::print_accounts(std::ostream &o) const
{

    o << '[' << std::endl;

    for (auto it{accounts.begin()}; it != accounts.end(); it++)
    {
        it->second->print(o);
        o << std::endl;
    }

    if (accounts.size() == 0)
        o << std::endl;
    o << ']';

    return o;
}