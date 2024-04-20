#include <string>
#include <vector>
#include <iostream>
#include "../include/Volunteer.h"
using std::string;
using std::vector;
#define NO_ORDER -1

    Volunteer::Volunteer(int otherid, const string &othername):
    completedOrderId(NO_ORDER),activeOrderId(NO_ORDER),id(otherid),name(othername)
    {
    }
    Volunteer::~Volunteer() = default;
    int Volunteer::getId() const {
       return id;
    }
    const string &Volunteer::getName() const {
        return name;
    }
    int Volunteer::getActiveOrderId() const {
        return activeOrderId;
    }
    int Volunteer::getCompletedOrderId() const {
        return completedOrderId;
    }
    bool Volunteer::isBusy() const
    {
        return activeOrderId != NO_ORDER;
    }  




    //collector volunteer
    CollectorVolunteer::CollectorVolunteer(int id, string name, int othercoolDown):
    Volunteer{id,name}, coolDown(othercoolDown), timeLeft(-1)
    {
    }
    CollectorVolunteer::~CollectorVolunteer() = default;
    CollectorVolunteer *CollectorVolunteer::clone() const { //Deep copy
            CollectorVolunteer *v = new CollectorVolunteer(getId() ,getName(), getCoolDown());
            v->activeOrderId = activeOrderId;
            v->completedOrderId = completedOrderId;
            v->setTimeLeft(getTimeLeft());
            return v;
    }
    int CollectorVolunteer::getTimeLeft() const {
            return timeLeft;
    }
    void CollectorVolunteer::setTimeLeft(int otehrtimeleft) {
            timeLeft = otehrtimeleft;
    }
    bool CollectorVolunteer::decreaseCoolDown() {
        setTimeLeft(getTimeLeft()-1);
        if(timeLeft != 0) {
            return false;
        }
        else {
            return true;
        }

    }
    void CollectorVolunteer::step() {
        if(isBusy() == true)
        {
            if(decreaseCoolDown()) {
                completedOrderId = activeOrderId;
                activeOrderId =- 1;
            }
        }
    }
    int CollectorVolunteer::getCoolDown() const {
        return coolDown;
    }
    bool CollectorVolunteer::hasOrdersLeft() const {
        return true;
    }
    bool CollectorVolunteer::canTakeOrder(const Order &order) const {
        return isBusy() != true && hasOrdersLeft() == true;
    }
    void CollectorVolunteer::acceptOrder(const Order &order) {
        timeLeft = coolDown;
        activeOrderId = order.getId();
    }
    string CollectorVolunteer::toString() const{
            string output = "VolunteerID: " + std::to_string(getId())+ "\n";
            output += "isBusy: ";
            if(isBusy()) {
                output += "true\n";
                output += "OrderID: "+ std::to_string(activeOrderId) +"\n";
                output += "time left:"+std::to_string(getTimeLeft()) +"\n";
            }
            else {
                output += "false\n";
                output += "OrderID: None\n";
                output += "time left: None\n";
            }
            output += "OrdersLeft: No Limit";
            return output;
    }
    string CollectorVolunteer::getType() const {
        return "CollectorVolunteer";
    } 


    //limitedcollector volunteer
    LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int othermaxOrders):
    CollectorVolunteer(id,name,coolDown), maxOrders(othermaxOrders),ordersLeft(maxOrders)
    {
    }
    LimitedCollectorVolunteer::~LimitedCollectorVolunteer() = default;
    LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const { //Deep copy
        LimitedCollectorVolunteer *v = new LimitedCollectorVolunteer(getId() ,getName(), getCoolDown(), maxOrders);
        v->activeOrderId = activeOrderId;
        v->completedOrderId = completedOrderId;
        v->ordersLeft = ordersLeft;
        v->setTimeLeft(getTimeLeft());
        return v;
    }
    bool LimitedCollectorVolunteer::hasOrdersLeft() const
    {
        if(ordersLeft > 0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
        return isBusy() != true && hasOrdersLeft() == true;
    }
    void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
        setTimeLeft(getCoolDown());
        activeOrderId = order.getId();
        ordersLeft--;
    }
    int LimitedCollectorVolunteer::getMaxOrders() const {
        return maxOrders;
    }
    int LimitedCollectorVolunteer::getNumOrdersLeft() const{ 
        return ordersLeft;
    }
    string LimitedCollectorVolunteer::toString() const {
        string output="VolunteerID: "+ std::to_string(getId())+ "\n";
        output += "isBusy: ";
        if(isBusy()) {
            output += "true\n";
            output += "OrderID: "+ std::to_string(activeOrderId) +"\n";
            output += "time left:"+std::to_string(getTimeLeft()) +"\n";
        }
        else {
            output += "false\n";
            output += "OrderID: None\n";
            output += "time left: None\n";
        }
        output += "OrdersLeft: " + std::to_string(getNumOrdersLeft());
        return output;
    }
    string LimitedCollectorVolunteer::getType() const {
        return "LimitedCollectorVolunteer";
    }


    //driver volunteer
    DriverVolunteer::DriverVolunteer(int id, string name, int othermaxDistance, int otherdistancePerStep):
    Volunteer(id,name), maxDistance(othermaxDistance), distancePerStep(otherdistancePerStep),distanceLeft(NO_ORDER)
    {
    }
    DriverVolunteer::~DriverVolunteer() = default;
    DriverVolunteer *DriverVolunteer::clone() const { //Deep copy
        DriverVolunteer *v = new DriverVolunteer(getId() ,getName(), getMaxDistance(), getDistancePerStep());
        v->activeOrderId = activeOrderId;
        v->completedOrderId = completedOrderId;
        v->setDistanceLeft(distanceLeft);
        return v;
    }
    int DriverVolunteer::getDistanceLeft() const {
        return distanceLeft;
    }
    void DriverVolunteer::setDistanceLeft(int otherdistance) {
        distanceLeft=otherdistance;
    }
    int DriverVolunteer::getMaxDistance() const {
        return maxDistance;
    }
    int DriverVolunteer::getDistancePerStep() const {
        return distancePerStep;
    }
    bool DriverVolunteer::decreaseDistanceLeft(){ 
        distanceLeft -= distancePerStep;
        if(distanceLeft <= 0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool DriverVolunteer::hasOrdersLeft() const {
        return true;
    }
    bool DriverVolunteer::canTakeOrder(const Order &order) const {
        return isBusy() == false && order.getDisance() <= maxDistance && hasOrdersLeft();
    }
    void DriverVolunteer::acceptOrder(const Order &order) {
        distanceLeft = order.getDisance();
        activeOrderId = order.getId();
    }
    void DriverVolunteer::step() {
        if(isBusy() == true) {
            if(decreaseDistanceLeft()) {
                completedOrderId = activeOrderId;
                activeOrderId = -1;
            }
        }
    } 
    string DriverVolunteer::toString() const {
        string output="VolunteerID: " +std::to_string(getId())+ "\n";
        output += "isBusy: ";
        if(isBusy()){
            output += "true\n";
            output += "OrderID: "+ std::to_string(activeOrderId)+"\n";
            output += "time left:"+std::to_string(getDistanceLeft())+"\n";
        }
        else{
            output += "false\n";
            output += "OrderID: None\n";
            output += "time left: None\n";
        }
        output += "OrdersLeft: No Limit";
        return output;
    }

    string DriverVolunteer::getType() const {
        return "DriverVolunteer";
    }

    //limitedDriverVolunteer
    LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int othermaxOrders):
    DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(othermaxOrders),ordersLeft(othermaxOrders)
    {
    }
    LimitedDriverVolunteer::~LimitedDriverVolunteer() = default;
    LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
        LimitedDriverVolunteer *v = new LimitedDriverVolunteer(getId() ,getName(), getMaxDistance(), getDistancePerStep(), getMaxOrders());
        v->activeOrderId = activeOrderId;
        v->completedOrderId = completedOrderId;
        v->ordersLeft = ordersLeft;
        v->setDistanceLeft(getDistanceLeft());
        return v;
    }
    int LimitedDriverVolunteer::getMaxOrders() const {
        return maxOrders;
    }
    int LimitedDriverVolunteer::getNumOrdersLeft() const {
        return ordersLeft;
    }
    bool LimitedDriverVolunteer::hasOrdersLeft() const {
        if(ordersLeft>0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
        return isBusy() == false && order.getDisance()<=getMaxDistance() && hasOrdersLeft();
    }
    void LimitedDriverVolunteer::acceptOrder(const Order &order) {
        setDistanceLeft(order.getDisance());
        activeOrderId = order.getId();
        ordersLeft--;
    }
    string LimitedDriverVolunteer::toString() const {
        string output = "VolunteerID: "+std::to_string(getId())+ "\n";
        output += "isBusy: ";
        if(isBusy()) {
            output += "true\n";
            output += "OrderID: "+ std::to_string(activeOrderId)+"\n";
            output += "time left:"+std::to_string(getDistanceLeft())+"\n";
        }
        else {
            output += "false\n";
            output += "OrderID: None\n";
            output += "time left: None\n";
        }
        output += "OrdersLeft: "+ std::to_string(getNumOrdersLeft());
        return output;
    }
    string LimitedDriverVolunteer::getType() const {
        return "LimitedDriverVolunteer";
    }