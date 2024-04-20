#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#include "../include/WareHouse.h"
extern WareHouse* backup;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

        WareHouse::WareHouse(const string &configFilePath): //Constructor
        isOpen(true),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(0),volunteerCounter(0),orderCounter(0)
        {  
            parsing(configFilePath);
        }
        WareHouse::~WareHouse() {  //Destructor
            for (BaseAction* ba : actionsLog) {
                delete ba;
            }
            for (Volunteer* volunteer : volunteers) {
                delete volunteer;
            }
            for (Order* order : pendingOrders) {
                delete order;
            }
            for (Order* order : inProcessOrders) {
                delete order;
            }
            for (Order* order : completedOrders) {
                delete order;
            }
            for (Customer* customer : customers) {
                delete customer;
            }
            actionsLog.clear();
            volunteers.clear();
            pendingOrders.clear();
            inProcessOrders.clear();
            completedOrders.clear();
            customers.clear();
        }
        WareHouse::WareHouse(const WareHouse &other): //Copy constructor
        isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter) {
            for (BaseAction* ba : other.actionsLog) {
                actionsLog.push_back(ba->clone());
            }
            for (Volunteer* volunteer : other.volunteers) {
                
                volunteers.push_back(volunteer->clone());
            }
            for (Order* order : other.pendingOrders) {
                pendingOrders.push_back(order->clone());
            }
            for (Order* order : other.inProcessOrders) {
                inProcessOrders.push_back(order->clone());
            }
            for (Order* order : other.completedOrders) {
                completedOrders.push_back(order->clone());
            }
            for (Customer* customer : other.customers) {
                customers.push_back(customer->clone());
            }
        }
        WareHouse& WareHouse::operator=(const WareHouse& other) {  //Assignment operator
            
            if(this != &other) {
                //Clean my data
                for (BaseAction* ba : actionsLog) {
                    delete ba;
                }
                for (Volunteer* volunteer : volunteers) {
                    delete volunteer;
                }   
                for (Order* order : pendingOrders) {
                    delete order;
                }
                for (Order* order : inProcessOrders) {
                    delete order;
                }
                for (Order* order : completedOrders) {
                    delete order;
                }
                for (Customer* customer : customers) {
                    delete customer;
                }
                
                actionsLog.clear();
                volunteers.clear();
                pendingOrders.clear();
                inProcessOrders.clear();
                completedOrders.clear();
                customers.clear();

                //Deep copy other data
                isOpen = other.isOpen;
                customerCounter = other.customerCounter;
                volunteerCounter = other.volunteerCounter;
                orderCounter = other.orderCounter;
                for (BaseAction* ba : other.actionsLog) {
                    actionsLog.push_back(ba->clone());
                }
                for (Volunteer* volunteer : other.volunteers) {
                    volunteers.push_back(volunteer->clone());
                }
                for (Order* order : other.pendingOrders) {
                    pendingOrders.push_back(order->clone());
                }
                for (Order* order : other.inProcessOrders) {
                    inProcessOrders.push_back(order->clone());
                }
                for (Order* order : other.completedOrders) {
                    completedOrders.push_back(order->clone());
                }
                for (Customer* customer : other.customers) {
                    customers.push_back(customer->clone());
                }
            }
            return *this;
        }
        WareHouse::WareHouse(WareHouse&& other) noexcept:  //Move constructor
        isOpen(other.isOpen),actionsLog(other.actionsLog),volunteers(other.volunteers),pendingOrders(other.pendingOrders),inProcessOrders(other.inProcessOrders),completedOrders(other.completedOrders),customers(other.customers),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter) {
            other.actionsLog.clear();
            other.volunteers.clear();
            other.pendingOrders.clear();
            other.inProcessOrders.clear();
            other.completedOrders.clear();
            other.customers.clear();
        }
        WareHouse& WareHouse::operator=(WareHouse&& other) noexcept { //Move assignment operator
            if (this != &other) {
                //Clean my data
                for (BaseAction* ba : actionsLog) {
                    delete ba;
                }
                for (Volunteer* volunteer : volunteers) {
                    delete volunteer;
                }
                for (Order* order : pendingOrders) {
                    delete order;
                }
                for (Order* order : inProcessOrders) {
                    delete order;
                }
                for (Order* order : completedOrders) {
                    delete order;
                }
                for (Customer* customer : customers) {
                    delete customer;
                }
                actionsLog.clear();
                volunteers.clear();
                pendingOrders.clear();
                inProcessOrders.clear();
                completedOrders.clear();
                customers.clear();

                //Shallow copy other data
                isOpen = other.isOpen;
                actionsLog = other.actionsLog;
                volunteers = other.volunteers;
                pendingOrders = other.pendingOrders;
                inProcessOrders = other.inProcessOrders;
                completedOrders = other.completedOrders;
                customers = other.customers;
                customerCounter = other.customerCounter;
                volunteerCounter = other.volunteerCounter;
                orderCounter = other.orderCounter;
                
                //Clean other data
                other.actionsLog.clear();
                other.volunteers.clear();
                other.pendingOrders.clear();
                other.inProcessOrders.clear();
                other.completedOrders.clear();
                other.customers.clear();
                
            }
            return *this;
            
        }        
        void WareHouse::start(){
            std::cout << "Warehouse is open!" << std::endl;  //Print welcome message
            BaseAction *action; //BaseAction pointer that will do all the actions.
            while(isOpen) {
                string userInput;
                int num;
                getline(cin, userInput);  //The user input is now saved at the string userInput
                int pos =  userInput.find(" ");  //Index of the space char
                string command = userInput.substr(0, pos); //The first world of the use input. For exapmle: order, orderStatus, step and etc.
                if (pos != -1 && command!="customer") {
                    num = stoi(userInput.substr(pos + 1, userInput.size()));  //The number that follows the first word
                }
                if (command == "order") {
                    action = new AddOrder(num);
                    action->act(*this);  //Preforom the action
                    delete action;
                }
                else if (command == "orderStatus") {
                     action = new PrintOrderStatus(num);  
                     action->act(*this);  //Preforom the action
                     delete action;
                }
                else if (command == "customer") {
                    int pos2 = userInput.find(" ", pos+1);
                    string name = userInput.substr(pos + 1, pos2 - pos - 1);
                    pos = pos2;
                    pos2 = userInput.find(" ", pos+1);
                    string type = userInput.substr(pos + 1, pos2 - pos - 1);
                    pos = pos2;
                    pos2 = userInput.find(" ", pos+1);
                    num = stoi(userInput.substr(pos + 1, pos2-pos-1));
                    pos = pos2;
                    pos2 = userInput.find(" ", pos+1); 
                    int num2 = stoi(userInput.substr(pos + 1, pos2-pos-1));                    
                    action = new AddCustomer(name, type, num, num2);
                    action->act(*this);  //Preforom the action
                    delete action; 
                }
                else if (command == "step") {
                    action = new SimulateStep(num);
                    action->act(*this);  //Preforom the action  
                    delete action;
                }
                else if (command == "customerStatus") {
                    action = new PrintCustomerStatus(num);
                    action->act(*this);  //Preforom the action  
                    delete action;
                }
                else if (command == "volunteerStatus") {
                    action = new PrintVolunteerStatus(num);
                    action->act(*this);  //Preforom the action  
                    delete action;
                }
                else if (command == "log") {
                    action = new PrintActionsLog();
                    action->act(*this);  //Preforom the action
                    delete action;
                }
                else if (command == "close") {
                    action = new Close();
                    action->act(*this);  //Preforom the action
                    delete action;
                }
                else if (command == "backup") {
                    action = new BackupWareHouse();
                    action->act(*this);  //Preforom the action
                    delete action;
                }
                else if (command == "restore") {
                    action = new RestoreWareHouse();
                    action->act(*this);  //Preforom the action
                    delete action;
                }
            }
            
        }
        void WareHouse::addOrder(Order* order) {
            pendingOrders.push_back(order);
            orderCounter++;
        }
        void WareHouse::addCustomer(Customer* customer) {  //Our function
            customers.push_back(customer);
            customerCounter++;
        }
        void WareHouse::addVolunteer(Volunteer* volunteer) {  //Our function
            volunteers.push_back(volunteer);
            volunteerCounter++;
        }
        void WareHouse::addAction(BaseAction* action){
            actionsLog.push_back(action);
        }
        Customer &WareHouse::getCustomer(int customerId) const{
            for(Customer *c : customers){
                if(c->getId() == customerId) {
                    return *c;
                }
            }
            Customer *c = new CivilianCustomer(customerId,"null",-1,-1); //Dummy
            return *c;
        }
        Volunteer &WareHouse::getVolunteer(int volunteerId) const{
            for(Volunteer *v : volunteers){
                if(v->getId() == volunteerId) {
                    return *v;
                }
            }
            Volunteer *v = new CollectorVolunteer(-1,"null",-1); //Dummy
            return *v;
        }
        Order &WareHouse::getOrder(int orderId) const{
            for(Order *o : pendingOrders){
                if(o->getId() == orderId) {
                    return *o;
                }
            }
            for(Order *o : inProcessOrders){
                if(o->getId() == orderId) {
                    return *o;
                }
            }
            for(Order *o : completedOrders){
                if(o->getId() == orderId) {
                    return *o;
                }
            }
            Order *o = new Order(orderId,-1,-1); //Dummy
            return *o;
        }
        const vector<BaseAction*> &WareHouse::getActions() const {
            return actionsLog;
        }
        vector<Order*> &WareHouse::getPendingOrders() {
            return pendingOrders;
        }
        vector<Order*> &WareHouse::getInProcessOrders() {
            return inProcessOrders;
        }
        vector<Order*> &WareHouse::getCompletedOrders() {
            return completedOrders;
        }
        vector<Volunteer*> &WareHouse::getvolunteers() {
            return volunteers;
        }
        const vector<Customer*> &WareHouse::getCustomers() const{ 
            return customers;
        }
        const int WareHouse::getOrderCounter() {
            return orderCounter;
        }
        const int WareHouse::getCustomerCounter() {
            return customerCounter;
        }
        void WareHouse::close() {
            isOpen = false;
        }
        void WareHouse::open() {
            isOpen = true;
        }
        void WareHouse::parsing(const string &configFilePath){
            fstream configFile;
            configFile.open(configFilePath, ios::in);  //Read mode of config file
            if (configFile.is_open()) {  //If we managed to open the file
                string line;
                while (getline(configFile, line)) {  //Iterate through the lines
                    string firstWord = line.substr(0, line.find(" ")); //First world of each line
                    if (firstWord == "customer" || firstWord == "volunteer") { //Only if the line starts by the words customer or volunteer
                        stringstream lineToSplit(line);  //The line we will split
                        string s; //Variable to store token obtained from the line
                        vector<string> v; //Declaring vector to store the string after split
                        while (getline(lineToSplit, s, ' ') && s != "#") {  //Split each word and insert the word into a vector
                            v.push_back(s);
                        }
                        if (firstWord == "customer") {  //Handle customer
                            string customerName = v[1];
                            int customerDistance = stoi(v[3]);
                            int customerMaxOrder = stoi(v[4]);
                            if (v[2] == "soldier") {
                                Customer *c = new SoldierCustomer(customerCounter, customerName, customerDistance, customerMaxOrder);
                                addCustomer(c->clone());
                                delete c;
                            }
                            else {
                                Customer *c = new CivilianCustomer(customerCounter, customerName, customerDistance, customerMaxOrder);
                                addCustomer(c->clone());
                                delete c;
                            }
                        }
                        else {  //Handle volunteer
                            string volunteerrName = v[1];
                            int volunteerCoolDownOrMaxDistance = stoi(v[3]);
                            if (v[2] == "collector") {
                                Volunteer *v = new CollectorVolunteer(volunteerCounter, volunteerrName, volunteerCoolDownOrMaxDistance);
                                addVolunteer(v->clone());
                                delete v;
                            }
                            else if (v[2] == "limited_collector") {
                                int volunteerMaxOrders = stoi(v[4]);
                                Volunteer *v = new LimitedCollectorVolunteer(volunteerCounter, volunteerrName, volunteerCoolDownOrMaxDistance, volunteerMaxOrders);
                                addVolunteer(v->clone());
                                delete v;
                            }
                            else if (v[2] == "driver") {
                                int volunteerDistancePerStep = stoi(v[4]);
                                Volunteer *v = new DriverVolunteer(volunteerCounter, volunteerrName, volunteerCoolDownOrMaxDistance, volunteerDistancePerStep);
                                addVolunteer(v->clone());
                                delete v;
                            }
                            else {
                                int volunteerDistancePerStep = stoi(v[4]);
                                int volunteerMaxOrders = stoi(v[5]);
                                Volunteer *v = new LimitedDriverVolunteer(volunteerCounter, volunteerrName, volunteerCoolDownOrMaxDistance, volunteerDistancePerStep, volunteerMaxOrders);
                                addVolunteer(v->clone());
                                delete v;
                            }
                        }
                    }
                }
                configFile.close(); //Close the file
            }
        }