#ifndef BANK_H
#define BANK_H
#include <string>
#include <map>
#include <iostream>
#include <memory>

class Person;
class Account;

class Bank
{

    std::map<std::string, std::shared_ptr<Person>> customers;
    std::map<int, std::shared_ptr<Account>> accounts;

public:
    Bank() {}
    void create_person(std::string, int, int, int, int, double);
    std::shared_ptr<Account> create_account(std::shared_ptr<Person>, int, int, int, int, double);
    std::shared_ptr<Person> get_person(std::string);
    std::shared_ptr<Account> get_account(int);
    bool remove_person(std::string);
    bool remove_account(int);
    std::ostream &print_customers(std::ostream &) const;
    std::ostream &print_accounts(std::ostream &) const;
};

#endif