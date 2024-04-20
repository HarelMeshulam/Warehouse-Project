#include <string>
#include <vector>
using std::string;
using std::vector;
#include "../include/Order.h"

class Volunteer;

#define NO_VOLUNTEER -1

    Order::Order(int otherid, int othercustomerId, int otherdistance):
        id(otherid),customerId(othercustomerId),distance(otherdistance),status(OrderStatus::PENDING),collectorId(NO_VOLUNTEER),driverId(NO_VOLUNTEER)
        {
        }
        int Order::getId() const {
            return id;
        }
        int Order::getCustomerId() const {
            return customerId;
        }
        void Order::setStatus(OrderStatus otherstatus) {
            status = otherstatus;
        }
        void Order::setCollectorId(int othercollectorId) {
            collectorId = othercollectorId;
        }
        void Order::setDriverId(int otherdriverId) {
            driverId = otherdriverId;
        }
        int Order::getCollectorId() const {
            return collectorId;
        }
        int Order::getDriverId() const {
            return driverId;
        }
        int Order::getDisance() const {
            return distance;
        }
        OrderStatus Order::getStatus() const {
            return status;
        }
        const string Order::getStringStatus() const {
            if(status == OrderStatus::PENDING) {
                return "Pending";
            }
            else if(status == OrderStatus::COLLECTING) {
                return "Collecting";
            }
            else if(status == OrderStatus::DELIVERING) {
                return "Delivering";
            }
            return "Completed";
        }
        const string Order::toString() const{
            string output = "";
            output += "OrderID: " +std::to_string(getId())+"\n";
            output += "OrderStatus: " + getStringStatus()+"\n";
            output += "CustomerID: " + std::to_string(getCustomerId())+"\n";
            if(collectorId != -1) {
                output+="Collector: "+ std::to_string(getCollectorId())+"\n";
            }
            else {
                output+="Collector: None \n";
            }
            if(driverId!=-1) {
                output+="Driver: "+ std::to_string(getDriverId());
            }
            else {
                output+="Driver: None";
            }
            return output;
        }
        Order *Order::clone() const { //Deep copy
            Order *output = new Order(id,customerId,distance);
            output->setStatus(this->getStatus());
            output->setCollectorId(this->getCollectorId());
            output->setDriverId(this->getDriverId());
            return output;
        }