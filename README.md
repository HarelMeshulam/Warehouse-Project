Food Warehouse Management System
Description
This project is an assignment aimed at designing an object-oriented system using C++. The objective is to gain implementation experience while utilizing classes, standard data structures, and unique C++ properties such as the "Rule of 5." The system simulates a food warehouse management system where volunteers handle orders made by customers.

Getting Started
To run the program, follow these steps:

Compile the program using the provided makefile: make.
Execute the compiled program with the path to the configuration file as the first command-line argument: ./bin/warehouse config_file_path.
Program Flow
Upon execution, the program initializes the warehouse based on the provided configuration file and starts the simulation. The simulation allows users to interact with the system by executing various actions. The warehouse management policy includes placing orders, creating customers, checking order status, and performing simulation steps.

Classes
The program consists of several classes:

Warehouse: Manages volunteers, customers, and orders. It maintains lists of pending, in-process, and completed orders.
Customer: Represents different types of customers, such as soldier or civilian.
Volunteer: An abstract class representing different types of volunteers (collector or driver). Volunteers have specific roles and methods for handling orders.
Order: Describes an order with a unique ID, status, and associated customer and volunteers.
BaseAction: An abstract class for different action types. It enables logging of actions and contains methods for performing and representing actions.
Actions
Actions are core functionalities of the program, allowing users to interact with the system. Some key actions include:

SimulateStep: Advances the simulation by one unit of time.
AddOrder: Creates a new order and places it in the warehouse.
AddCustomer: Creates a new customer and stores them in the warehouse.
PrintOrderStatus: Prints information about a specific order.
PrintCustomerStatus: Prints information about a particular customer.
PrintVolunteerStatus: Prints information about a specific volunteer.
PrintActionsLog: Prints all actions performed by the user.
Close: Prints all orders with their status and closes the warehouse.
BackupWarehouse: Saves the current warehouse state for restoration.
RestoreWarehouse: Restores the previous warehouse state from backup.
