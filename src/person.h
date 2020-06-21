#ifndef PERSON_H
#define PERSON_H
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>

class Account;

class Person : public std::enable_shared_from_this<Person>
{

    std::string name;
    std::map<int, std::weak_ptr<Account>> accounts;

public:
    Person(std::string);
    std::string get_name() const
    {
        return name;
    };
    std::shared_ptr<Person> get_shared() { return shared_from_this(); };
    std::shared_ptr<Account> create_account(int, int, int, int, double);
    void remove_account(int);
    void add_account(std::shared_ptr<Account>);
    std::size_t number_of_accounts() const;
    std::vector<int> get_account_nrs() const;
    bool share_account(std::shared_ptr<Person>, int);
    std::ostream &print(std::ostream &) const;
};

#endif