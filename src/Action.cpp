#include <vector>
#include "../include/Action.h"
using std::string;
using std::vector;
#include <iostream>
extern WareHouse* backup;

        BaseAction::BaseAction():  //Default constructor
        errorMsg(""),status(ActionStatus::ERROR)
        {
        }
        BaseAction::~BaseAction() = default;
        ActionStatus BaseAction::getStatus() const {
            return status;
        }
        void BaseAction::complete(){
            status = ActionStatus::COMPLETED;
        }
        void BaseAction::error(string othererrorMsg){
            status = ActionStatus::ERROR;
            errorMsg = othererrorMsg;
        }
        string BaseAction::getErrorMsg() const {
            return errorMsg;
        }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        SimulateStep::SimulateStep(int othernumOfSteps):
        BaseAction(), numOfSteps(othernumOfSteps)
        {
        }
        void SimulateStep::act(WareHouse &wareHouse){
            for (int k = 0; k < numOfSteps; k++) {
                std::vector<int> pendingOrdersToRemove; //Vector of orders indices that are pending
                int i = 0;
                for (Order* order : wareHouse.getPendingOrders()) {
                    bool gotDeleted = false;
                    if (order->getStatus() == OrderStatus::PENDING) {
                        for (Volunteer* volunteers : wareHouse.getvolunteers()) {
                            if (volunteers->getType() == "CollectorVolunteer" || volunteers->getType() == "LimitedCollectorVolunteer") {
                                if (volunteers->canTakeOrder(*order)) {
                                    volunteers->acceptOrder(*order);
                                    order->setCollectorId(volunteers->getId());
                                    order->setStatus(OrderStatus::COLLECTING);
                                    wareHouse.getInProcessOrders().push_back(order);
                                    pendingOrdersToRemove.push_back(i);
                                    gotDeleted = true;
                                    break;
                                }
                            }
                        }
                    }
                    else if (order->getStatus() == OrderStatus::COLLECTING) {
                        for (Volunteer* volunteers : wareHouse.getvolunteers()) {
                            if (volunteers->getType() == "DriverVolunteer" || volunteers->getType() == "LimitedDriverVolunteer") {
                                if (volunteers->canTakeOrder(*order)) {
                                volunteers->acceptOrder(*order);
                                order->setDriverId(volunteers->getId());
                                order->setStatus(OrderStatus::DELIVERING);
                                wareHouse.getInProcessOrders().push_back(order);
                                pendingOrdersToRemove.push_back(i);
                                gotDeleted = true;
                                break;
                                }
                            }
                        }
                    }
                    if (!gotDeleted) {
                        i++;
                    }
                }
                //Remove processed orders from the pendingOrders vector
                for (int index : pendingOrdersToRemove) {
                    wareHouse.getPendingOrders().erase(wareHouse.getPendingOrders().begin() + index);
                }

                //Preform step
                for (Volunteer *volunteers : wareHouse.getvolunteers()) {
                    volunteers->step();
                }

                std::vector<int> inProcessOrdersToRemove; //Vector of orders indices that are in process
                int j = 0;
                for (Order* order : wareHouse.getInProcessOrders()) {
                    bool gotDeleted = false;
                    if (order->getStatus() == OrderStatus::COLLECTING) {
                        if (wareHouse.getVolunteer(order->getCollectorId()).getCompletedOrderId() == order->getId()) {
                            wareHouse.getPendingOrders().push_back(order);
                            inProcessOrdersToRemove.push_back(j);
                            gotDeleted = true;
                        }
                    }
                    else if (order->getStatus() == OrderStatus::DELIVERING) {
                        if (wareHouse.getVolunteer(order->getDriverId()).getCompletedOrderId() == order->getId()) {
                        order->setStatus(OrderStatus::COMPLETED);
                        wareHouse.getCompletedOrders().push_back(order);
                        inProcessOrdersToRemove.push_back(j);
                        gotDeleted = true;
                        }
                    }   
                    if (!gotDeleted) {
                        j++;
                    }
                }
                //Remove processed orders from the inProcessOrders vector
                for (int index : inProcessOrdersToRemove) {
                    wareHouse.getInProcessOrders().erase(wareHouse.getInProcessOrders().begin() + index);
                }

                //Delete maxed out volunteers
                i = 0;
                for (Volunteer* volunteers : wareHouse.getvolunteers()) {
                    bool gotDeleted = false;
                    if (!volunteers->hasOrdersLeft() && !(volunteers->isBusy())) {
                        wareHouse.getvolunteers().erase(wareHouse.getvolunteers().begin() + i);
                        delete volunteers;
                        gotDeleted = true;
                    }
                    if (!gotDeleted) {
                        i++;
                    }
                }
            }
            complete();
            wareHouse.addAction(this->clone());
        }
        std::string SimulateStep::toString() const {
            std::string s = "simulateStep "+ std::to_string(numOfSteps);
            if(getStatus() == ActionStatus::COMPLETED) {
                s += " COMPLETED";
            }
            else{
                s += " ERROR";
            }
            return s;
        }
        SimulateStep *SimulateStep::clone() const {
            SimulateStep *b = new SimulateStep(numOfSteps);
            if(getStatus() == ActionStatus::COMPLETED) {  //Update the status in case simulate step was completed
                b->complete();
            }
            return b;
        }




//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    AddOrder::AddOrder(int id):
        BaseAction(), customerId(id) {
            error("Error: Cannot place this order");
        }
        void AddOrder::act(WareHouse &wareHouse) {
            Customer &customer = wareHouse.getCustomer(customerId);
            if(customer.getCustomerDistance() != -1 && customer.canMakeOrder()) { //If the customer is real
                Order *o = new Order(wareHouse.getOrderCounter(),customerId,customer.getCustomerDistance());
                o->setStatus(OrderStatus::PENDING);
                wareHouse.addOrder(o);
                customer.addOrder(o->getId());
                complete();
            }
            else { //If dummy customer
                std::cout << getErrorMsg()<<std::endl;
                if(customer.getCustomerDistance() == -1) {
                    delete &customer; //Delete dummy customer
                }
            }
            wareHouse.addAction(this->clone());
        }
        string AddOrder::toString() const {
            string s = "order "+ std::to_string(customerId);
            if(getStatus() == ActionStatus::COMPLETED) {
                s += " COMPLETED";
            }
            else{
                s += " ERROR";
            }
            return s;
        }
        AddOrder *AddOrder::clone() const {
            AddOrder *b = new AddOrder(customerId);
            if(getStatus() == ActionStatus::COMPLETED) { //Update the status in case add order was completed
                b->complete();
            }
            return b;
        }



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        AddCustomer::AddCustomer(string othercustomerName, string othercustomerType, int otherdistance, int othermaxOrders):
        BaseAction(), customerName(othercustomerName),customerType((othercustomerType == "soldier")?CustomerType::Soldier:CustomerType::Civilian),distance(otherdistance), maxOrders(othermaxOrders)
        {
        }
        string revSetCustomerType(CustomerType othercustomerType) {  //Reverse from enum to string
            if(othercustomerType == CustomerType::Soldier) {
                return "soldier" ;
            }
            else {
                return "civilian";
            }
            
        }
        void AddCustomer::act(WareHouse &wareHouse) {
            Customer *c;
            if(customerType == CustomerType::Soldier)
            {
                c = new SoldierCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
            }
            else {
                c = new CivilianCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
            }
            wareHouse.addCustomer(c);
            complete();
            wareHouse.addAction(this->clone());
        }
        AddCustomer *AddCustomer::clone() const {
            string t = revSetCustomerType(customerType);
            AddCustomer *b = new AddCustomer(customerName,t,distance,maxOrders);
            if(getStatus() == ActionStatus::COMPLETED) { //Update the status in case add customer was completed
                b->complete();
            }
            return b;
        }
        string AddCustomer::toString() const {
            string s = "customer "+ customerName+ " "+revSetCustomerType(customerType)+" "+ std::to_string(distance) + " "+ std::to_string(maxOrders);
            if(getStatus() == ActionStatus::COMPLETED) {
                s += " COMPLETED";
            }
            else {
                s += " ERROR";
            }
            return s;

        }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        PrintOrderStatus::PrintOrderStatus(int otherid):
        BaseAction(), orderId(otherid){
        }
        void PrintOrderStatus::act(WareHouse &wareHouse) {
            Order &o = wareHouse.getOrder(orderId);
            string s;
            if(o.getDisance() == -1) {  //If dummy order
                error("Error: Order doesn't exists");
                s = getErrorMsg();
                delete &o; //Delete dummy customer
            }
            else {  //If order is real
                s = o.toString();
                complete();
            }
            std::cout << s << std::endl;
            
            wareHouse.addAction(this->clone());
        }
        PrintOrderStatus *PrintOrderStatus::clone() const {
            PrintOrderStatus *b = new PrintOrderStatus(orderId);
            if(getStatus() == ActionStatus::COMPLETED) { //Update the status in case print order status was completed
                b->complete();
            }
            return b;
        }
        string PrintOrderStatus::toString() const {
            string s = "orderStatus " + std::to_string(orderId);
            if(getStatus() == ActionStatus::COMPLETED) {
                s += " COMPLETED";
            }
            else {
                s += " ERROR";
            }
            return s;
        }




//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        PrintCustomerStatus::PrintCustomerStatus(int othercustomerId):
        BaseAction(), customerId(othercustomerId)
        {  
        }
        void PrintCustomerStatus::act(WareHouse &wareHouse) {
            string s;
            Customer &customer = wareHouse.getCustomer(customerId);
            if(customer.getCustomerDistance() == -1) {  //If dummy customer
                error("Error: Customer doesn't exists");
                s = getErrorMsg();
                delete &customer; //Delete dummy customer
            }
            else { //If real customer
                s = "CustomerID: "+ std::to_string(customerId)+ "\n";
                for(int order: customer.getOrdersIds()){
                    Order o = wareHouse.getOrder(order);
                    s += "OrderID: " + std::to_string(o.getId()) + "\n";
                    s += "OrderStatus: " + o.getStringStatus()+"\n";
                    if(o.getDisance() == -1) {
                        delete &o;  //No longer needed
                    }
                    
                }
                s += "numOrdersLeft: "+ std::to_string(customer.getNumOrdersLeft());
                complete();
            }
            wareHouse.addAction(this->clone());
            std::cout << s << std::endl;
        }
        PrintCustomerStatus *PrintCustomerStatus::clone() const{
            PrintCustomerStatus *b = new PrintCustomerStatus(customerId);
            if(getStatus() == ActionStatus::COMPLETED) { //Update the status in case print customer status was completed
                b->complete();
            }
            return b;
        }
        string PrintCustomerStatus::toString() const{
            string s = "customerStatus "+ std::to_string(customerId);
            if(getStatus() == ActionStatus::COMPLETED) {
                s += " COMPLETED";
            }
            else {
                s += " ERROR";
            }
            return s;
        }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        PrintVolunteerStatus::PrintVolunteerStatus(int id):
        BaseAction(), volunteerId(id)
        {
        }
        void PrintVolunteerStatus::act(WareHouse &wareHouse) {
            string s;
            Volunteer &volunteer = wareHouse.getVolunteer(volunteerId); //If dummy volunteer
            if(volunteer.getId() == -1) {
                error("Error: Volunteer doesn't exists");
                s = getErrorMsg();
                delete &volunteer; //Delete dummy volunteer
            }
            else { //If real volunteer
                s = volunteer.toString();
                complete();
            }
            std::cout << s << std::endl;
            wareHouse.addAction(this->clone());
        }
        PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
            PrintVolunteerStatus *b = new PrintVolunteerStatus(volunteerId);
            if(getStatus() == ActionStatus::COMPLETED) //Update the status in case print volunteer status was completed
            {
                b->complete();
            }
            return b;
        }
        string PrintVolunteerStatus::toString() const{
            string s = "volunteerStatus "+ std::to_string(volunteerId);
            if(getStatus() == ActionStatus::COMPLETED)
            {
                s += " COMPLETED";
            }
            else{
                s += " ERROR";
            }
            return s;
        }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        PrintActionsLog::PrintActionsLog():
        BaseAction()
        {
        }
        void PrintActionsLog::act(WareHouse &wareHouse)
        {
            for(BaseAction *a : wareHouse.getActions()) {
                std::cout << a->toString() << std::endl;
            }
            complete();
            wareHouse.addAction(this->clone());
        }
        PrintActionsLog *PrintActionsLog::clone() const {
            PrintActionsLog *b = new PrintActionsLog();
            if(getStatus() == ActionStatus::COMPLETED) { //Update the status in case print actions log was completed
                b->complete();
            }
            return b;
        }
        string PrintActionsLog::toString() const{
            string s = "log COMPLETED";
            return s;
        }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Close::Close():
    BaseAction()
    {
    }
    void Close::act(WareHouse &wareHouse){
        string s;
        for(Order *o : wareHouse.getCompletedOrders()){
            s = "OrderID: " + std::to_string(o->getId()) + ", CustomerID: " + std::to_string(o->getCustomerId()) + ", Status:"+ o->getStringStatus();
            std::cout << s <<std::endl;
        }
        for(Order *o : wareHouse.getInProcessOrders()){
            s = "OrderID: " + std::to_string(o->getId()) + ", CustomerID: " + std::to_string(o->getCustomerId()) + ", Status:"+ o->getStringStatus();
            std::cout << s <<std::endl;
        }
        for(Order *o : wareHouse.getPendingOrders()){
            s = "OrderID: " + std::to_string(o->getId()) + ", CustomerID: " + std::to_string(o->getCustomerId()) + ", Status:"+ o->getStringStatus();
            std::cout << s <<std::endl;
        }
        wareHouse.close();
        complete();
    }
    Close *Close::clone() const{
        Close *b = new Close();
        if(getStatus() == ActionStatus::COMPLETED) {
            b->complete();
        }
        return b;
    }
    string Close::toString() const{
        return "close COMPLETED";
    }
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    BackupWareHouse::BackupWareHouse():
    BaseAction()
    {
    }
    void BackupWareHouse::act(WareHouse &wareHouse){
        if(backup == nullptr) {  //If there was no backup then make one useing copy constructor
            backup = new WareHouse(wareHouse);
        }
        else {
            *backup = wareHouse; //Else use move assignment operator
        }
        complete();
        wareHouse.addAction(this->clone());
    }
    BackupWareHouse *BackupWareHouse::clone() const {
        BackupWareHouse *b = new BackupWareHouse();
        if(getStatus() == ActionStatus::COMPLETED) {
            b->complete();
        }
        return b;
    }
    string BackupWareHouse::toString() const {
        string s = "backup COMPLETED";
        return s;
    }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    RestoreWareHouse::RestoreWareHouse():
    BaseAction()
    {
    }
    void RestoreWareHouse::act(WareHouse &wareHouse){
        if (backup != nullptr) { //If back up was performed
            wareHouse = *backup;
            complete();
        }
        else {
            error("Error: No backup available");
            std::cout << getErrorMsg() << std::endl;
        }
        wareHouse.addAction(this->clone());
        
    }
    RestoreWareHouse *RestoreWareHouse::clone() const{
        RestoreWareHouse *b = new RestoreWareHouse();
        if(getStatus() == ActionStatus::COMPLETED) { //Update the status in case restore was completed
            b->complete();
        }
        return b;
    }
    string RestoreWareHouse::toString() const {
        string s = "restore";
        if(getStatus() == ActionStatus::COMPLETED) {
            s += " COMPLETED";
        }
        else {
            s += " ERROR";
        }
        return s;
    }