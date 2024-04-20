#pragma once
#include <string>
#include <vector>
using namespace std;

#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Action.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        ~WareHouse(); //Destructor
        WareHouse(const WareHouse &other); //Copy constructor
        WareHouse& operator=(const WareHouse& other); //Assignment operator
        WareHouse(WareHouse&& other) noexcept; //Move constructor
        WareHouse& operator=(WareHouse&& other) noexcept; //Move assignment operator
        void start();
        void addOrder(Order* order);
        void addCustomer(Customer* customer); //Our function
        void addVolunteer(Volunteer* volunteer); //Our function
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        vector<Order*> &getPendingOrders();
        vector<Order*> &getInProcessOrders();
        vector<Order*> &getCompletedOrders();
        vector<Volunteer*> &getvolunteers();
        const vector<Customer*> &getCustomers() const;
        const int getOrderCounter();
        const int getCustomerCounter();
        void increaseOrderCounter();
        void close();
        void open();

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //Our member
        void parsing(const string &configFilePath);
};