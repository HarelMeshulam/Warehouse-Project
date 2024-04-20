#include <string>
#include <vector>
#include "../include/Customer.h"
using std::string;
using std::vector;

    Customer::Customer(int otherid, const string &othername, int otherlocationDistance, int othermaxOrders):
        id(otherid),name(othername),locationDistance(otherlocationDistance),maxOrders(othermaxOrders), ordersId()
        {
        }
        const string &Customer::getName() const {
            return name;
        }
        int Customer::getId() const{
            return id;
        }
        int Customer::getCustomerDistance() const {
            return locationDistance;
        }
        int Customer::getMaxOrders() const {
            return maxOrders;
        }
        int Customer::getNumOrders() const {
            return ordersId.size();
        }
        bool Customer::canMakeOrder() const {
            return maxOrders>getNumOrders();
        }
        int Customer::getNumOrdersLeft() const {
            return maxOrders-getNumOrders();
        }
        const vector<int> &Customer::getOrdersIds() const {
            return ordersId;
        }
        int Customer::addOrder(int orderId) {
            if(canMakeOrder() == true) {
                ordersId.push_back(orderId);
                return orderId;
            }
            else {
                return -1;
            }
        }


        Customer::~Customer() = default;

    //Soldier customer
    SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders):
    Customer(id,name,locationDistance,maxOrders)
    {
    }
    SoldierCustomer *SoldierCustomer::clone() const {
        SoldierCustomer *s= new SoldierCustomer(getId(),getName(), getCustomerDistance(),getMaxOrders());
        for(int i:this->getOrdersIds()) {
            s->addOrder(i);
        }
        return s;
    }

    //Civilian customer
    CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders):
    Customer(id,name,locationDistance,maxOrders)
    {
    }
    CivilianCustomer *CivilianCustomer::clone() const{
        CivilianCustomer *s = new CivilianCustomer(getId(),getName(), getCustomerDistance(),getMaxOrders());
        for(int i:this->getOrdersIds()) {
            s->addOrder(i);
        }
        return s;
    }
