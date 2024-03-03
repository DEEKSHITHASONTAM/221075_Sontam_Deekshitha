#include <iostream>
#include <vector>
#include <ctime>
#include <bsoncxx/builder/stream/document.hpp>
#include <optional>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

class Car;
class Customer;

class User {
    protected:
        std::string name;
        int ID;
        std::string password;

    public:
        User(std::string name, int ID,std::string password)  : name(name), ID(ID), password(password) {}

        virtual void rentCar(const Car& car) =0;
        virtual void returnCar(Car& car) =0;
        virtual void showInfo() const =0;

        virtual int getID() const = 0; 
};

class Car{
    private:
        int ID;
        std::string model;
        std::string condition;
        std::string otherDetails;
        bool rented;
        time_t dueDate;
    public:
        Car(int ID, std::string model, std::string condition, std::string otherDetails) :ID(ID), model(model), condition(condition), otherDetails(otherDetails) {}

        void rentCar(User& user) ;
        void returnCar(User& user);

//getter methods
        std::string getModel() const {
            return model;
        }

        std::string getCondition() const {
            return condition;
        }

        std::string getOtherDetails() const {
            return otherDetails;
        }

        time_t getDueDate() const{
            std::cout << "Due Date: " << dueDate << std::endl;
            return dueDate;
        }

        void showInfo() const {
            std::cout << "ID: "<< ID << "Model: " << model << ", Condition: " << condition << ", Other Details: " << otherDetails << std::endl;
        }

        bool isRented() const {
            return rented;
        }
 // lets write update functions
        void setID(const int newID) {
             ID = newID;
        }

        void setModel(const std::string& newModel) {
             model = newModel;
        }

        void setCondition(const std::string& newCondition) {
             condition = newCondition;
        }

        void setOtherDetails(const std::string& newOtherDetails) {
             otherDetails = newOtherDetails;
        }
        int getID() const ;
};

void Car::returnCar(User& user){
   if(rented==true){
    rented = false;
    user.returnCar(*this);
   }else{
    std::cout << "Sorry,this car has already been returned or you didn't rent this car." << std::endl;
   }
}

void Car::rentCar(User& user) {
    rented = true;
    dueDate =time(0) + 5*86400;
    user.rentCar(*this);
    std::cout<<"This car has been rented succesfully , ENJOY THE RIDE!!!"<<std::endl;
}

int Car::getID() const {
    return ID;
}

class Customer : public User{
    private:
        int rentedCars;
        int fineDue;
        int customerRecord;
        std::vector<int> rentedCarIDs;
    public:
        Customer(std::string name, int ID, std::string password, int rentedCars = 0, int fineDue = 0, int customerRecord =50)
            :User(name, ID, password), rentedCars(rentedCars), fineDue(fineDue), customerRecord(customerRecord) {}

        void rentCar(const Car& car) override;
        void returnCar(Car& car) override;
        void showInfo() const override;

        void clearDue();
        int getID() const override;
//getter methods 
        std::string getName() const {
            return name;
        }

        std::string getPassword() const {
            return password;
        }

        int getFineDue() const {
            return fineDue;
        }

        int getCustomerRecord() const {
            std::cout << "Customer Record: " << customerRecord << std::endl;
            return customerRecord;
        }

        void getRentedCarsID() const {
            for (const auto& rentedCarID : rentedCarIDs) {
                std::cout << rentedCarID << " ";
            }
        }
        int getRentedCars() const {
            std::cout << "Rented Cars: " << rentedCars << std::endl;
            return rentedCars;
        }
//setter methods 
        void setID(const int newID) {
             ID = newID;
        }

        void setName(const std::string newName) {
             name = newName;
        }

        void setPassword(const std::string newPassword) {
             password = newPassword;
        }

        void setRentedCars(const int newRentedCars) {
             rentedCars = newRentedCars;
        }

        void setFineDue(const int newFineDue) {
             fineDue = newFineDue;
        }

        void setCustomerRecord(const int newCustomerRecord) {
             customerRecord = newCustomerRecord;
        }

        const std::vector<int>& getRentedCarIDs() const {
            return rentedCarIDs;
        }   
};

int Customer::getID() const {
    return ID;
}

void Customer::rentCar(const Car& car)  {
    if(car.isRented()){
        if (rentedCars <3 && customerRecord >= 30){
            rentedCars++;
            rentedCarIDs.push_back(car.getID());
            std::cout << "Car rented successfully,Enjoy the RIDE!!!" << rentedCars << std::endl;
        }
        else{
            std::cout << "Sorry ,You cannot rent more then 5 cars or insufficient customerRecord. " << std::endl;
        }
    }else{
        std::cout << "This car has already been rented by other customers." << std::endl;
    }
    
}
void Customer::showInfo() const {
    std::cout << "Customer Name: " << name << ", ID: " << ID << ", Customer Record: " << customerRecord << std::endl;
    for (const auto& rentedCarID : rentedCarIDs) {
        std::cout << rentedCarID << " ";
    }
    std::cout << std::endl;
}
void Customer::returnCar(Car& car) {
    rentedCars--;
    if (time(0) > car.getDueDate()) {
        fineDue += (time(0) - car.getDueDate()) / (24 * 60 * 60) * 10;  // Fine of 10 rupees per day
    }

    // Remove the returned car ID from the vector
    auto it = std::find(rentedCarIDs.begin(), rentedCarIDs.end(), car.getID());
    if (it != rentedCarIDs.end()) {
        rentedCarIDs.erase(it);
    }
}

void Customer::clearDue() {
    fineDue = 0;
}
//employee classes
class Employee : public User {
private:
    int rentedCars;
    int fineDue;
    int employeeRecord;
public:
    Employee(std::string name, int ID, std::string password,int rentedCars = 0, int fineDue = 0, int employeeRecord = 70)
        : User(name, ID, password),rentedCars(rentedCars), fineDue(fineDue), employeeRecord(employeeRecord) {}

    void rentCar(const Car& car) override;
    void returnCar(Car& car) override;
    void showInfo() const override;

    void clearDue();
    //getter functions for employee 
    int getID() const override;

    std::string getName() const {
        return name;
    }

    std::string getPassword() const {
        return password;
    }

    int getFineDue() const {
        return fineDue;
    }

    int getEmployeeRecord() const {
        std::cout << "Employee Record: " << employeeRecord << std::endl;
        return employeeRecord;
    }

    int getRentedCars() const {
        std::cout << "Rented Cars: " << rentedCars << std::endl;
        return rentedCars;
    }
//setter functions for employee
       
        void setName(const std::string newName) {
             name = newName;
        }
         void setID(const int newID) {
             ID = newID;
        }
        void setRentedCars(const int newRentedCars) {
             rentedCars = newRentedCars;
        }
        void setPassword(const std::string newPassword) {
             password = newPassword;
        }
        void setEmployeeRecord(const int newEmployeeRecord) {
             employeeRecord = newEmployeeRecord;
        }
        void setFineDue(const int newFineDue) {
             fineDue = newFineDue;
        }
};

void Employee::rentCar(const Car& car) {
    if(car.isRented()){
        if (rentedCars < 2 && employeeRecord >= 40) {
        rentedCars++;
        std::cout << "Car rented successfully with a 15% discount!" << std::endl;
    } else {
        std::cout << "Cannot rent more cars or insufficient employee record." << std::endl;
    }
    }else{
        std::cout << "sorry this car is not available." << std::endl;
    }
    
}
void Employee::returnCar(Car& car) {
    rentedCars--;
    if (time(0) > car.getDueDate()) {
        fineDue += (time(0) - car.getDueDate()) / (24 * 60 * 60) * 10;  //fine of 10 ruppes per day
    }
}

void Employee::showInfo() const {
    std::cout << "Employee Name: " << name << ", ID: " << ID << ", Employee Record: " << employeeRecord << std::endl;
}

void Employee::clearDue() {
    fineDue = 0;
}
int Employee::getID() const {
    return ID;
}
//manager class implementation
class Manager : public User {
public:
    Manager(std::string name, int ID, std::string password)
        : User(name, ID, password) {}

    void showInfo() const override;
    int getID() const override;

};

int Manager::getID() const {
    return ID;
}

void Manager::showInfo() const {
    std::cout << "Manager Name: " << name << ", ID: " << ID << std::endl;
}
//database class Manager
class Database {
private:
    mongocxx::instance instance;  
    mongocxx::client client;      
    mongocxx::database db;        

    // Helper functions
    bsoncxx::document::value createCarDocument(const Car& car) const;
    bsoncxx::document::value createCustomerDocument(const Customer& customer) const;
    bsoncxx::document::value createEmployeeDocument(const Employee& employee) const;
public:
    Database(const std::string& connectionString);

    void showAllCars() const;
    void addCar(const Car &car);
    void updateCar(const Car &car);
    void deleteCar(const Car &car);
    bool searchCar(const Car &car);
    
    void showAllEmployees() const;
    void addEmployee(const Employee &employee);
    void updateEmployee(const Employee &employee);
    void deleteEmployee(const Employee &employee);
    bool searchEmployee(const Employee &employee);
    bool verifyEmployee(int employeeId, const std::string &password) const;

    void showAllCustomers() const;
    void addCustomer(const Customer &customer);
    void updateCustomer(const Customer &customer);
    void deleteCustomer(const Customer &customer);
    bool searchCustomer(const Customer &customer);
    bool verifyCustomer(int customerId, const std::string &password) const;
   
    bool verifyManager(int managerId, const std::string &password) const;

    // std::vector<Customer> getAllCustomers();

    void clearCollection(const std::string& collectionName);
};

Database::Database(const std::string &connectionString)
    : client(mongocxx::uri(connectionString)), db(client["carRentalDB"])
{
    // Initializing mongodb
    if (!client)
    {
        std::cerr << "Error: Failed to connect to the database." << std::endl;
    }
    else
    {
        std::cout << "Connected to the database successfully!" << std::endl;
    }
}

//functions to convert  car objects into mongodb document format

bsoncxx::document::value Database::createCarDocument(const Car &car) const
{
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;
    using bsoncxx::builder::stream::open_document;

    return document{}
           << "model" << car.getModel()
           << "id" << car.getID()
           << "condition" << car.getCondition()
           << "otherDetails" << car.getOtherDetails()
           << "rented" << car.isRented()
           << "dueDate" << bsoncxx::types::b_date{std::chrono::system_clock::from_time_t(car.getDueDate())}
           << bsoncxx::builder::stream::finalize;
}

// Helper function to convert Customer object to MongoDB document
bsoncxx::document::value Database::createCustomerDocument(const Customer &customer) const
{
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;
    using bsoncxx::builder::stream::open_document;

    return document{}
           << "name" << customer.getName()
           << "id" << customer.getID()
           << "password" << customer.getPassword()
           << "rentedCars" << customer.getRentedCars()
           << "fineDue" << customer.getFineDue()
           << "customerRecord" << customer.getCustomerRecord()
           << bsoncxx::builder::stream::finalize;
}

// Helper function to convert Employee object to MongoDB document
bsoncxx::document::value Database::createEmployeeDocument(const Employee &employee) const
{
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;
    using bsoncxx::builder::stream::open_document;

    return document{}
           << "name" << employee.getName()
           << "id" << employee.getID()
           << "password" << employee.getPassword()
           << "rentedCars" << employee.getRentedCars()
           << "fineDue" << employee.getFineDue()
           << "employeeRecord" << employee.getEmployeeRecord()
           << bsoncxx::builder::stream::finalize;
}

//add fucntions to the database
void Database::addCar(const Car &car)
{
    auto collection = db["cars"];
    auto document = createCarDocument(car);
    collection.insert_one(document.view());
}

void Database::addCustomer(const Customer &customer)
{
    auto collection = db["customers"];
    auto document = createCustomerDocument(customer);
    collection.insert_one(document.view());
}

void Database::addEmployee(const Employee &employee)
{
    auto collection = db["employees"];
    auto document = createEmployeeDocument(employee);
    collection.insert_one(document.view());
}

//update functions for the database
void Database::updateCar(const Car &car)
{
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;
    auto update = createCarDocument(car);
    collection.update_one(filter.view(), update.view());
}

void Database::updateCustomer(const Customer &customer)
{
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << customer.getID() << bsoncxx::builder::stream::finalize;
    auto update = createCustomerDocument(customer);
    collection.update_one(filter.view(), update.view());
}

void Database::updateEmployee(const Employee &employee)
{
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;
    auto update = createEmployeeDocument(employee);
    collection.update_one(filter.view(), update.view());
}
// functions to delete object from the database

void Database::deleteCar(const Car &car)
{
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;
    collection.delete_one(filter.view());
}

void Database::deleteCustomer(const Customer &customer)
{
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << customer.getID() << bsoncxx::builder::stream::finalize;
    collection.delete_one(filter.view());
}
void Database::deleteEmployee(const Employee &employee)
{
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;
    collection.delete_one(filter.view());
}

// Search functions
bool Database::searchCar(const Car &car)
{
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result)
    {
        bsoncxx::document::view document = result->view();
        return true;
    }
    else
    {
        std::cout << "couldn't find this car in the database" << std::endl;
        return false;
    }
}

bool Database::searchCustomer(const Customer &customer)
{
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "ID" << customer.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result)
    {
        bsoncxx::document::view document = result->view();
        return true;
    }
    else
    {
        std::cout << "Couldn't find the Customer in the database " << std::endl;
        return false;
    }
}

bool Database::searchEmployee(const Employee &employee)
{
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result)
    {
        bsoncxx::document::view document = result->view();
        return true;
    }
    else
    {
        std::cout << "Couldn't find the employee in the database " << std::endl;
        return false;
    }
}

// Show functions to  display data from the database
void Database::showAllCars() const
{
    auto collection = db["cars"];
    auto cursor = collection.find({});
    for (auto &&doc : cursor)
    {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

// Show all customers in the database
void Database::showAllCustomers() const
{
    auto collection = db["customers"];
    auto cursor = collection.find({});
    for (auto &&doc : cursor)
    {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

// Show all employees in the database
void Database::showAllEmployees() const
{
    auto collection = db["employees"];
    auto cursor = collection.find({});
    for (auto &&doc : cursor)
    {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}
//verify functions to  verify the id and password of the User.
bool Database::verifyCustomer(int customerId, const std::string &password) const
{
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << customerId << "password" << password << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    return result ? true : false;
}

bool Database::verifyEmployee(int employeeId, const std::string &password) const
{
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employeeId << "password" << password << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    return result ? true : false;
}

bool Database::verifyManager(int managerId, const std::string &password) const
{
    const int knownManagerId = 1;
    const std::string knownManagerPassword = "manager123";

    return (managerId == knownManagerId) && (password == knownManagerPassword);
}

int main() {
    

 std::string atlasConnectionString = "mongodb+srv://deekshitha:deekshi@cluster0.nc8vgqk.mongodb.net/<dbname>?retryWrites=true&w=majority";
 Database database(atlasConnectionString);

Car car1(1, "Tesla", "bad", "Red");
Car car2(2, "Honda", "Excellent", "Blue");
Car car3(3, "Toyota", "Average", "Silver");
Car car4(4, "BMW", "Fair", "Black");
Car car5(5, "Mercedes", "Excellent", "White");

Customer customer1("Jane Smith", 101, "pass123A", 1, 12, 25);
Customer customer2("Jim Brown", 102, "pass789B", 2, 10, 35);
Customer customer3("Olivia Johnson", 103, "pass101C", 0, 15, 85);
Customer customer4("Daniel Lee", 104, "pass202D", 3, 8, 50);
Customer customer5("James White", 105, "pass303E", 4, 5, 70);

Employee employee1("Alice Johnson", 201, "empPassF", 2, 20, 75);
Employee employee2("Bob Williams", 202, "empPass123G", 1, 18, 82);
Employee employee3("Charlie Davis", 203, "empPass456H", 3, 22, 92);
Employee employee4("Eva Lee", 204, "passIabc", 5, 19, 35);
Employee employee5("David Smith", 205, "empPass789J", 0, 0, 65);

    int newID;
    
    std::vector<Customer> customers;
    
    int customerId;
    std::string customerPassword;

    int updateCustomerID;
    Customer customerToUpdate(" ",0," ",0,0,0);

    std::string newName,newPassword;
    int newRentedCars, newFineDue, newCustomerRecord;


    std::vector<Employee> employees;

    std::vector<Car> cars;

    int carID;

    std::string carModel;
    int updateCarID;

    std::string newModel, newCondition, newOtherDetails;
    Car carToUpdate(0, " "," "," ");

    int employeeId;
    std::string employeePassword;

    int updateEmployeeID;
    Employee employeeToUpdate(" ",0," ",0,0,0);

    
    int newEmployeeRecord;

    cars.push_back(car1);
    cars.push_back(car2);
    cars.push_back(car3);
    cars.push_back(car4);
    cars.push_back(car5);

    customers.push_back(customer1);
    customers.push_back(customer2);
    customers.push_back(customer3);
    customers.push_back(customer4);
    customers.push_back(customer5);

   

    employees.push_back(employee1);
    employees.push_back(employee2);
    employees.push_back(employee3);
    employees.push_back(employee4);
    employees.push_back(employee5);

    
    int choice;
    do {
        
        std::cout << "\n**WELCOME  TO ALLU-PRODUCTS*\n";
        std::cout<<"--------------------------------------------"<<std::endl;
        std::cout<<"-----------Choose who  you are-------- "<<std::endl;
        std::cout << "1. Customer\n";
        std::cout << "2. Employee\n";
        std::cout << "3. Manager\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            // Verification
            std::cout << "Enter Customer ID: ";
            std::cin >> customerId;
            std::cout << "Enter Customer Password: ";
            std::cin >> customerPassword;


            if (database.verifyCustomer(customerId, customerPassword))
            {
                std::cout << "Customer Login Successful!\n";
                int customerChoice;
                std::cout << "\n******** ALLU-PRODUCT Menu for costumers**\n";
                std::cout << "1. Show profile\n";
                std::cout << "2. Rent Car\n";
                std::cout << "3. Return Car\n";
                std::cout << "4. Rented Cars\n";
                std::cout << "5. Customer Record\n";
                std::cout << "0. Exit\n";
                std::cout << "Enter your choice: ";
                std::cin >> customerChoice;

                switch (customerChoice)
                {
                case 1:
                    for (const auto &customer : customers)
                    {
                        if (customer.getID() == customerId)
                        {
                            customer.showInfo();
                            break; // Assuming customer IDs are unique, exit loop once found
                        }
                    }
                    break;

                case 2:
                    database.showAllCars();

                    std::cout << " please select your car id to rent: ";
                    std::cin >> carID;
                    for (auto &car : cars)
                    {
                        if (car.getID() == carID)
                        {
                            for (auto &customer : customers)
                            {
                                if (customer.getID() == customerId)
                                    car.rentCar(customer);
                                std::cout << "Success, Car rented successfully.";
                                break; // Assuming customer IDs are unique, exit loop once found
                            }
                        }
                    }
                    break;

                case 3:
                    std::cout << "please type your car id that you wanted to return . ";
                    std::cin >> carID;
                    for (auto &car : cars)
                    {
                        if (car.getID() == carID)
                        {
                            for (auto &customer : customers)
                            {
                                if (customer.getID() == customerId)
                                {
                                    car.returnCar(customer);
                                }

                                break; 
                            }
                        }
                    }
            
                    break;

                case 4:
                    for (const auto &customer : customers)
                    {
                        if (customer.getID() == customerId)
                        {
                            customer.getRentedCars();
                            break; 
                        }
                    }
                    break;

                case 5:
                    for (const auto &customer : customers)
                    {
                        if (customer.getID() == customerId)
                        {
                            customer.getCustomerRecord();
                            break; 
                        }
                    }
                    break;

                case 0:
                    // Exit the program
                    break;

                default:
                    std::cout << "Invalid choice. Please enter a valid option.\n";
                    break;
                }
            }
            else
            {
                std::cout << "Incorrect Credentials.Login failed!!!\n";
            }
            break;

        case 2:

            // Verify Employee ID and Password
            std::cout << "Enter Employee ID: ";
            std::cin >> employeeId;
            std::cout << "Enter Employee Password: ";
            std::cin >> employeePassword;

            // Employee verification logic
            if (database.verifyEmployee(employeeId, employeePassword))
            {
                std::cout << "------- You are logged in succesfully!-------\n";
                int employeeChoice;
                std::cout << "\n******** AllU-PRODUCT Menu for  Employees *****\n";
                std::cout << "1. Show profile\n";
                std::cout << "2. Rent Car\n";
                std::cout << "3. Return Car\n";
                std::cout << "4. Rented Cars\n";
                std::cout << "5. Employee Record\n";
                std::cout << "0. Exit\n";
                std::cout << "Enter your choice: ";
                std::cin >> employeeChoice;

                switch (employeeChoice)
                {
                case 1:
                    for (const auto &employee : employees)
                    {
                        if (employee.getID() == employeeId)
                        {
                            employee.showInfo();
                            break;
                        }
                    }
                    break;

                case 2:
                    database.showAllCars();

                    std::cout << " please, select your car id that you wanted to rent: ";
                    std::cin >> carID;
                    for (auto &car : cars)
                    {
                        if (car.getID() == carID)
                        {
                            for (auto &employee : employees)
                            {
                                if (employee.getID() == employeeId)
                                    car.rentCar(employee);
                                std::cout << "car rented successfully, Happy riding!!!";
                                break; 
                            }
                        }
                    }
                    break;

                case 3:
                    std::cout << "select your car id that you wanted to return: ";
                    std::cin >> carID;
                    for (auto &car : cars)
                    {
                        if (car.getID() == carID)
                        {
                            for (auto &employee : employees)
                            {
                                if (employee.getID() == employeeId)
                                {
                                    car.returnCar(employee);
                                }

                                break; 
                            }
                        }
                    }
                    break;

                case 4:
                    for (const auto &employee : employees)
                    {
                        if (employee.getID() == employeeId)
                        {
                            employee.getRentedCars();
                            break;
                        }
                    }
                    break;

                case 5:
                    for (const auto &employee : employees)
                    {
                        if (employee.getID() == employeeId)
                        {
                            employee.getEmployeeRecord();
                            break;
                        }
                    }
                    break;

                case 0:
                    // Exit the program
                    break;

                default:
                    std::cout << "Invalid choice. Please enter a valid option.\n";
                    break;
                }
            }
            else
            {
                std::cout << "Invalid Credentials.Login failed!!!\n";
            }

            break;

        case 3:
            int managerId;
            std::string managerPassword;

            // Verification 
            std::cout << "Enter Manager ID: ";
            std::cin >> managerId;
            std::cout << "Enter Manager Password: ";
            std::cin >> managerPassword;

            if (database.verifyManager(managerId, managerPassword))
            {
                std::cout << "Manager Login Successful!\n";

                // Manager operations
                int managerChoice;
                std::cout << "\n******** ALLU-PRODUCTS menu for Manager*\n";
                std::cout << "1. show all CARS\n";
                std::cout << "2. search/update/add/delete operations for CARS\n";

                std::cout << "3. show all CUSTOMERS\n";
                std::cout << "4. search/update/add/delete operations for CUSTOMERS\n";

                std::cout << "5. show all EMPLOYEES\n";
                std::cout << "6. search/update/add/delete operations for EMPLOYEES\n";

                std::cout << "0. Exit\n";
                std::cout << "Enter your choice: ";
                std::cin >> managerChoice;

                switch (managerChoice)
                {
                case 1:
                    database.showAllCars();
                    break;
                case 2:
                    int carOperation;
                    std::cout << "\n******** ALLU-PRODUCTS manager operations**\n";
                    std::cout << "1. update car\n";
                    std::cout << "2. add a car\n";
                    std::cout << "3. delete a car\n";
                    std::cout << "0. Exit\n";
                    std::cout << "Enter your choice: ";
                    std::cin >> carOperation;

                    switch (carOperation)
                    {
                    case 1:

                        std::cout << "Enter the ID of the car to update: ";
                        std::cin >> updateCarID;

                        carToUpdate.setID(updateCarID);

                        if (database.searchCar(carToUpdate))
                        {
                            // Car found, proceed with update

                            std::cout << "Enter the new model: ";
                            std::cin >> newModel;

                            std::cout << "Enter the new condition: ";
                            std::cin >> newCondition;

                            std::cout << "Enter the new other details: ";
                            std::cin >> newOtherDetails;

                            carToUpdate.setModel(newModel);
                            carToUpdate.setCondition(newCondition);
                            carToUpdate.setOtherDetails(newOtherDetails);

                            database.updateCar(carToUpdate);
                            std::cout << "Car details updated successfully!\n";
                        }
                        else
                        {
                            std::cout << "Car not found. Update failed.\n";
                        }
                        break;

                    case 2:
                    {
                        
                        std::cout << "Enter the model of the new car: ";
                        std::cin >> newModel;

                        std::cout << "Enter the condition of the new car: ";
                        std::cin >> newCondition;

                        std::cout << "Enter the otherDetails of the new car: ";
                        std::cin >> newOtherDetails;

                        std::cout << "Please create a new ID for the car: ";
                        std::cin >> newID;

                        carToUpdate.setID(newID);
                        carToUpdate.setModel(newModel);
                        carToUpdate.setCondition(newCondition);
                        carToUpdate.setOtherDetails(newOtherDetails);

                        database.addCar(carToUpdate);
                        std::cout << "------New car was added to the database!-------\n";
                    }
                    break;

                    case 3:
                    {
                        std::cout << "Enter the ID of the car to delete: ";
                        std::cin >> updateCarID;

                        for (const auto &car : cars)
                        {
                            if (car.getID() == updateCarID)
                            {
                                //  newPassword=customer.getPassword();
                                database.deleteCar(car);
                                std::cout << "Car deleted from the database!\n";
                            }
                        }
                    }
                    break;

                    case 0:
                        // Exit the program
                        break;

                    default:
                        std::cout << "Invalid choice. Please enter a valid option.\n";
                        break;
                    }
                    break;

                case 3:
                    database.showAllCustomers();
                    break;

                case 4:
                    int customerOperation;
                    std::cout << "\n******** ALLU-PRODUCTS manager  operations**\n";
                    std::cout << "1. update customer\n";
                    std::cout << "2. add a customer\n";
                    std::cout << "3. delete a customer\n";
                    std::cout << "0. Exit\n";
                    std::cout << "Enter your choice: ";
                    std::cin >> customerOperation;

                    switch (customerOperation)
                    {
                    case 1:

                        std::cout << "Enter the ID of the customer to update: ";
                        std::cin >> updateCustomerID;

                        customerToUpdate.setID(updateCustomerID);

                        if (database.searchCustomer(customerToUpdate))
                        {
                            // Customer found, proceed with update
                            std::string newName, newPassword;
                            int newRentedCars, newCustomerFine, updateCustomerRecord;

                            std::cout << "Enter the new name: ";
                            std::cin >> newName;
                            for (const auto &customer : customers)
                            {
                                if (customer.getID() == updateCustomerID)
                                {
                                    newPassword = customer.getPassword();
                                }
                            }
                            std::cout << "Enter the new number of rented cars: ";
                            std::cin >> newRentedCars;
                            std::cout << "Enter the new value of fine_due: ";
                            std::cin >> newFineDue;
                            std::cout << "Enter the new value of customer record value: ";
                            std::cin >> updateCustomerRecord;

                            customerToUpdate.setName(newName);
                            customerToUpdate.setPassword(newPassword);
                            customerToUpdate.setRentedCars(newRentedCars);
                            customerToUpdate.setFineDue(newFineDue);
                            customerToUpdate.setCustomerRecord(newCustomerRecord);

                            database.updateCustomer(customerToUpdate);
                            std::cout << "Customer details updated successfully!\n";
                        }
                        else
                        {
                            std::cout << "Sorry this Customer does not exist.please enter a valid id of the customer.\n";
                        }
                        break;

                    case 2:
                    {

                        std::cout << "Enter the name of the new customer: ";
                        std::cin.ignore(); // Ignore the newline character left in the buffer
                        std::getline(std::cin, newName);  

                        std::cout << "create a  password for the new customer: ";
                        std::cin >> newPassword;

                        std::cout << "Enter the number of rented cars for the new customer: ";
                        std::cin >> newRentedCars;

                        std::cout << "Enter the fine due for the new customer: ";
                        std::cin >> newFineDue;

                        std::cout << "Enter the customer record for the new customer: ";
                        std::cin >> newCustomerRecord;

                        std::cout << " create a id for the  new customer: ";
                        std::cin >> newID;

                        customerToUpdate.setID(newID);
                        customerToUpdate.setName(newName);
                        customerToUpdate.setPassword(newPassword);
                        customerToUpdate.setRentedCars(newRentedCars);
                        customerToUpdate.setFineDue(newFineDue);
                        customerToUpdate.setCustomerRecord(newCustomerRecord);

                        database.addCustomer(customerToUpdate);
                        std::cout << "-------New customer added to the database!-------\n";
                    }
                    break;

                    case 3:
                    {
                        std::cout << "Enter the ID of the customer that you wanted to  delete: ";
                        std::cin >> updateCustomerID;

                        for (const auto &customer : customers)
                        {
                            if (customer.getID() == updateCustomerID)
                            {
                                //  newPassword=customer.getPassword();
                                database.deleteCustomer(customer);
                                std::cout << "------Customer deleted from the database!--------\n";
                            }
                        }
                    }
                    break;

                    case 0:
                        // Exit the program
                        break;

                    default:
                        std::cout << "Invalid choice. Please enter a valid option.\n";
                        break;
                    }
                    break;
                case 5:
                    database.showAllEmployees();
                    break;           
                case 6:
                    int employeeOperation;
                    std::cout << "\n*ALLU-PRODUCTS menu for manager operations*\n";
                    std::cout << "1. update employee\n";
                    std::cout << "2. add a employee\n";
                    std::cout << "3. delete a employee\n";
                    std::cout << "0. Exit\n";
                    std::cout << "Enter your choice: ";
                    std::cin >> employeeOperation;

                    switch (employeeOperation)
                    {
                    case 1:

                        std::cout << "Enter the ID of the employee to update: ";
                        std::cin >> updateEmployeeID;

                        employeeToUpdate.setID(updateEmployeeID);

                        if (database.searchEmployee(employeeToUpdate))
                        {

                            std::cout << " Please Enter the new name: ";
                            std::cin >> newName;
                            for (const auto &employee : employees)
                            {
                                if (employeeToUpdate.getID() == updateEmployeeID)
                                {
                                    newPassword = employeeToUpdate.getPassword();
                                }
                            }
                            std::cout << "Enter the new number of rented cars: ";
                            std::cin >> newRentedCars;

                            std::cout << "Enter the new fine due: ";
                            std::cin >> newFineDue;

                            std::cout << "Enter the new employee record: ";
                            std::cin >> newEmployeeRecord;

                            employeeToUpdate.setName(newName);
                            employeeToUpdate.setPassword(newPassword);
                            employeeToUpdate.setRentedCars(newRentedCars);
                            employeeToUpdate.setFineDue(newFineDue);
                            employeeToUpdate.setEmployeeRecord(newEmployeeRecord);

                            database.updateEmployee(employeeToUpdate);
                            std::cout << "Employee details updated successfully!\n";
                        }
                        else
                        {
                            std::cout << "This Employee does not exist.Type the correct id of the employee.\n";
                        }

                        break;

                    case 2:
                    {

                        std::cout << "Enter the name of the new employee: ";
                        std::cin.ignore(); // Ignore the newline character left in the buffer
                        std::getline(std::cin, newName);

                        std::cout << "Enter the password of the new employee: ";
                        std::cin >> newPassword;

                        std::cout << "Enter the number of rented cars for the new employee: ";
                        std::cin >> newRentedCars;

                        std::cout << "Enter the fine due for the new employee: ";
                        std::cin >> newFineDue;

                        std::cout << "Enter the employee record for the new employee: ";
                        std::cin >> newEmployeeRecord;

                        std::cout << "Create the new id for the  employee:";
                        std::cin >> newID;


                        employeeToUpdate.setName(newName);
                        employeeToUpdate.setID(newID);
                        employeeToUpdate.setPassword(newPassword);
                        employeeToUpdate.setRentedCars(newRentedCars);
                        employeeToUpdate.setFineDue(newFineDue);
                        employeeToUpdate.setEmployeeRecord(newEmployeeRecord);

                        database.addEmployee(employeeToUpdate);
                        std::cout << "-------New employee added to the database!--------\n";
                    }
                    break;

                    case 3:
                    {
                        std::cout << "Enter the ID of the employee to delete: ";
                        std::cin >> updateEmployeeID;

                        for (const auto &employee : employees)
                        {
                            if (employee.getID() == updateEmployeeID)
                            {
                                //  newPassword=customer.getPassword();
                                database.deleteEmployee(employee);
                                std::cout << "Employee deleted from the database!\n";
                            }
                        }
                    }
                    break;

                    case 0:
                        break;

                    default:
                        std::cout << "Invalid choice,please select a correct choice.\n";
                        break;
                    }

                    break;

                case 0:
                    // get back to main menu
                    
                    break;

                default:
                    std::cout << "invalid choice, please enter a valid choice.\n";
                    break;
                }
            }
            else
            {
                std::cout << "Invalid Manager ID or Password. Login failed.\n";
            }
        }

    } while (choice != 0);

    return 0;
}
