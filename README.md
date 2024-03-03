# AllUPRODUCTS

This project is a simple car rental system named ALLUPRODUCTS that allows customers to rent cars, employees to manage cars, and a manager to oversee the entire system. The system uses object-oriented programming principles and stores data in a MongoDB database.

## Getting Started

### Prerequisites

To run the code, you need to have the following tools installed:

* A C++ compiler (such as g++)
* CMake (minimum version 3.10)
* MongoDB c++ Drivers (minimum version 4.0)
  After installing all this tools we may proceed

### Building the code

 Clone the repository:

   ```bash
   git clone https://github.com/your-username/car-rental-system.git
   ```
   ### compilation

After cloning the above repository  use the following command to compile the code . 
   ```bash 
   g++ alluproducts.cpp -o alluproducts $(pkg-config --cflags --libs libmongocxx) -std=c++11
```
###Usage 
Make sure to  go inside the file . If you have not done this you can use the following code
```bash
     ./alluproducts
```
### Functionality
  The system provides the following functionality:
  Initially the user is supposed to select his role . it has three roles
   * Customer
   * Employee
   * Manager
   Now if the user is either Customer or employee .He has options either for the following options
   * Renting a car: A customer can rent a car by selecting a car from the database and providing their ID and password for verification.
   
   * Returning a car: A customer can return a car by providing the ID of the car to be returned.
     
   * Showing profile: A customer or employee can view their profile by providing their ID.
    
   * Rented cars: A customer or employee can view the list of cars rented by them.
     
   * Record: A cusotmer or employee can view his record value by choosing the following option.
   If the  User is a manager . He has the following options 
   * UPDATE:Only Manager has access to update the details of car/customer/employee.
     
   * DELETE:Only Manager has access to delete the details of car/customer/employee.
     
   * ADD: Only Manager has access to add the details of car/customer/employee.
     
   *SEARCH: Only Manager has access to search the details of car/customer/employee.
   
   But the best part of the system is manager cannot still see the password or make changes to password to customer/employee.


## Acknowledgments
The project uses the following libraries
* The C++ Standard Library, which provides a wide range of libraries and algorithms for working with C++ code.
  
* The MongoDB C++ driver, which provides a convenient and easy-to-use interface for working with MongoDB databases in C++.
  
* The BSON C++ library, which provides a C++ library for working with BSON documents.


