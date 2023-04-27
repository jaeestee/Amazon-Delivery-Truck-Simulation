//Izaac Facundo, Steven Oh
//imf339, jo25672

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <vector>
using std::vector;
#include <cmath>
using std::sqrt;
using std::abs;
using std::pow;


class Address {
private:
  double x, y; //x and y are the coordinates to the address
  bool primedelivery;
public:
  Address() {};

  Address(double x, double y) : x(x), y(y) {};

  Address(double x, double y, bool prime) : x(x), y(y), primedelivery(prime) {};

  //This method calculates the distance using a simple distance formula
  double distance(Address other) {
    return sqrt(pow((x - other.x),2) + pow((y - other.y),2));
  };

  //This method returns whether the list contains an address
  bool contains_value(Address newadd) {
    return x == newadd.x && y == newadd.y;
  };

  //This method returns the x-value of an address
  double value_x() {
    return x;
  };

  //This method returns the y-value of an address
  double value_y() {
    return y;
  };
};

class AddressList {
protected: //This is protected so that it can be used in the Route class
  vector<Address> list, temporarylist;
public:
  AddressList() {}

  //This method adds new addresses into the list
  void add_address(Address newadd) {
    if (list.size() == 0) //if there is nothing in the list, simply adds the new address
      list.push_back(newadd);
    else {
      for (int i = 0; i < list.size(); i++) { //for loop to go through the list and check for duplicates
        if (list.at(i).contains_value(newadd)) //if the list already contains the address, it doesn't add it
          break;
        else if (i+1 == list.size()) //else if it is at the last iteration of the for-loop, add the new address
          list.push_back(newadd);
      }
    }
  };

  //This method adds a whole new list of addresses into the list
  void add_address_list(vector<Address> newlist) {
    list = newlist;
  };

  //This method calculates the length of the entire route
  double length() {
    double total_length = 0;

    for (int k = 0; k < list.size() - 1; k++) {
      total_length += list.at(k).distance(list.at(k+1));
    }
    return total_length;
  };

  //This method returns the address that is closest to the given address
  Address index_closest_to(Address check) {
    double shortest_distance = 9999999; //hardcoded value chosen to indicate errors in method
    Address closest_add; //this is initializing the closest address

    for (int i = 0; i < list.size(); i++) { //this loop is used to iterate through the list to find the closest address
      if (check.value_x() == list.at(i).value_x() && check.value_y() == list.at(i).value_y()) //if the address is itself, doesn't make it the closest address
        continue;
      else if (list.at(i).value_x() == 0 && list.at(i).value_y() == 0) //if the address is the depot, doesn't make it the closest address
        continue;
      else if (check.distance(list.at(i)) < shortest_distance) { //if the distance is shorter than the previous shortest distance, replaces the closest address and the distance
        closest_add = list.at(i);
        shortest_distance = check.distance(list.at(i));
      }
    }
    remove(closest_add); //it needs to remove the closest address so that it doesn't spit out an address already given previously
    return closest_add;
  };

  //This method removes any address that is unwanted, even its duplicates
  void remove(Address add) {
    temporarylist.clear(); //in case the templist is full, clears it
    for (int i = 0; i < list.size(); i++) { //this loop creates a new temporary list and returns the new list without the unwanted addresses
      if (add.value_x() == list.at(i).value_x() && add.value_y() == list.at(i).value_y())
        continue;
      else
        temporarylist.push_back(list.at(i));
    }
    list.clear(); //clears the list so that we can push the new values in templist into the list
    list = temporarylist;
  };

  //This method prints every address in the list in (x,y) format
  void print() {
    for (int i = 0; i < list.size(); i++) {
      cout << " (" << list.at(i).value_x() << "," << list.at(i).value_y() << ")";
    }
    cout << endl;
  };
};

class Route : public AddressList {
private:
  vector<Address> templist;
public:
  //This method adds the depots (0,0) to the beginning and ends of the list
  void add_depot() {
    templist = list; //temporary list created to store the original list values
    list.clear(); //clears the list to push new values
    list.push_back(Address(0,0)); //add depot to the first element

    for (int i = 0; i < templist.size(); i++) { //this loop adds the remaining elements back into list
      list.push_back(templist.at(i));
    }
    list.push_back(Address(0,0)); //adds the depot at the end of the list
  };

  //This methodcreates a greedy route with points closest to each other
  void greedy_route() {
    Route object;
    object.add_address_list(list);
    templist.clear(); //templist needs to be clear in order to push values into it
    templist.push_back(Address(0,0)); //the first value needs to be the depot to find the closest point to it

    for (int i = 0; i < list.size(); i++) {
      templist.push_back(object.index_closest_to(templist.at(i))); //this will continue to find the next closest point to the point that it just stored previously
    }
    templist.push_back(Address(0,0)); //adds the depot at the end
    list = templist; //replaces the addresses in the list to the new list of addresses
  };

  //This method goes through every possible combination to find the best route
  void optimized_route() {
    vector<Address> swaplist,constantlist;
    Route depotlist,testprinter;
    list.erase(list.begin()); // delete depots, assuming route has depots added!
    list.erase(list.end());

    // testprinter.add_address_list(list); // for testing
    // testprinter.print();
    // cout << endl;

    double shortest_path = 999999; // hardcoded value chosen to indicate errors in method
    constantlist = list;
    for (int j = 0; j < constantlist.size()-1; j++) { // loop changes starting address
      templist = constantlist;
      templist.erase(templist.begin() + j); // start with new item each iteration
      templist.insert(templist.begin(),constantlist.at(j));
      
      
      for (int i = 0; i < constantlist.size(); i++) { // loop reverses addresses to get every combo
	swaplist = templist;
	
	if (i == 0){
	  depotlist.add_address_list(templist); // check the order after start is changed and before reversal
	} else {
	  swaplist.erase(swaplist.begin() + i); // check the order after reversals
	  swaplist.insert(swaplist.begin() + i-1,templist.at(i));
	  depotlist.add_address_list(swaplist);
	}

	//depotlist.print(); //for testing
	//cout << endl;

	depotlist.add_depot(); // add depots for accurate length

	//cout << depotlist.length() << endl; // for testing
	if (depotlist.length() < shortest_path) {
	  shortest_path = depotlist.length();
	  list = swaplist; // change the core list if its the shortest path
	  // cout << "Changed!" << endl; // for testing
	}
      }
    }
    add_depot(); // add depot to route after optimal route has been found
    
  }



  //This method splits up the routes and prints them according to a given integer num
  void multiple_trucks(int num) {
    vector<Address> constantlist;
    int numPer; //this is number per truck or day

    if (list.size()/num == 0) //if the size of the list is too small for the given num, it will automatically set the numPer to 1
      numPer = 1;
    else //else it will split it evenly
      numPer = list.size()/num;

    remove(Address(0,0)); //removes the depots
    constantlist = list;

    for (int i = 1; i <= num; i++) { //this loop for each truck/day
      list = constantlist;
      templist.clear();

      for (int j = 0; j < numPer; j++) { //this loop creates a temporary list for each truck/day and splits it evenly
        if (list.size() != 0) { //if the list has elements left, puts it into the corresponding truck/day
          templist.push_back(list.at(0)); //adds the next value
          list.erase(list.begin()); //removes the value
          constantlist = list;
        }
      }

      list = templist;
      add_depot();
      cout << "Truck/Day " << i << ": ";
      print();
      cout << "has length " << length() << endl;
    }
  };
};

int main() {
  /*
  Route deliveries;
  deliveries.add_address( Address(0,5) );
  deliveries.add_address( Address(5,0) );
  deliveries.add_address( Address(5,5) );
  deliveries.add_depot();

  cout << "Travel in order: " << deliveries.length() << endl;
  deliveries.greedy_route();
  cout << "Square route: ";
  deliveries.print();
  cout << "\n has length " << deliveries.length() << endl;

  //optimized route test
  cout << "Optimized route: ";
  deliveries.optimized_route();
  deliveries.print();
  cout << "\n has length " << deliveries.length() << endl;
  //  deliveries.multiple_trucks(5);

  */
  // Experiment 1: Greedy vs. Optimized (3 sets of 7 addresses)
  Route greedy, optimized, display, empty;

  greedy.add_address(Address(1,1));
  greedy.add_address(Address(2,9));
  greedy.add_address(Address(-8,-7));
  greedy.add_address(Address(0,-9));  
  greedy.add_address(Address(10,-1));  
  greedy.add_address(Address(-4,7));
  greedy.add_address(Address(4,-4));
  display = greedy;
  optimized = greedy;
  optimized.add_depot();

  greedy.greedy_route();  
  optimized.optimized_route();

  cout << "Experiment 1: Greedy vs. Optimized" << endl;
  cout << "Address set 1: ";
  display.print();

  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();
  cout << endl;

  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(1,1));
  greedy.add_address(Address(2,3));
  greedy.add_address(Address(8,7));
  greedy.add_address(Address(0,9));  
  greedy.add_address(Address(10,1));  
  greedy.add_address(Address(4,7));
  greedy.add_address(Address(4,0));
  display = greedy;
  optimized = greedy;
  optimized.add_depot();

  greedy.greedy_route();  
  optimized.optimized_route();

 
  cout << "Address set 2: ";
  display.print();

  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();


  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(2,1));
  greedy.add_address(Address(-2,-3));
  greedy.add_address(Address(8,7));
  greedy.add_address(Address(0,-9));  
  greedy.add_address(Address(10,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,0));
  display = greedy;
  optimized = greedy;
  optimized.add_depot();

  greedy.greedy_route();  
  optimized.optimized_route();

 
  cout << endl << "Address set 3: ";
  display.print();

  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();


  // Experiment 2: Greedy + Optimized

  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(1,1));
  greedy.add_address(Address(2,9));
  greedy.add_address(Address(-8,-7));
  greedy.add_address(Address(0,-9));  
  greedy.add_address(Address(10,-1));  
  greedy.add_address(Address(-4,7));
  greedy.add_address(Address(4,-4));
  display = greedy;
  optimized = greedy;
  optimized.add_depot();

  greedy.greedy_route();  
  optimized.greedy_route();
  optimized.optimized_route();

  cout << endl << "Experiment 2: Greedy + Optimized" << endl;
  cout << "Address set 1: ";
  display.print();

  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();
  cout << endl;

  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(1,1));
  greedy.add_address(Address(2,3));
  greedy.add_address(Address(8,7));
  greedy.add_address(Address(0,9));  
  greedy.add_address(Address(10,1));  
  greedy.add_address(Address(4,7));
  greedy.add_address(Address(4,0));
  display = greedy;
  optimized = greedy;
  optimized.add_depot();

  greedy.greedy_route();  
  optimized.greedy_route();
  optimized.optimized_route();

 
  cout << "Address set 2: ";
  display.print();

  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();


  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(2,1));
  greedy.add_address(Address(-2,-3));
  greedy.add_address(Address(8,7));
  greedy.add_address(Address(0,-9));  
  greedy.add_address(Address(10,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,0));
  display = greedy;
  optimized = greedy;
  optimized.add_depot();

  greedy.greedy_route();  
  optimized.greedy_route();
  optimized.optimized_route();

 
  cout << endl << "Address set 3: ";
  display.print();

  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();

  // Experiment 3

  cout << endl << "Experiment 3: Greedy vs. Optimal, Variable Size Address List" << endl;
  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(5,1));
  greedy.add_address(Address(2,3));
  greedy.add_address(Address(8,5));
  greedy.add_address(Address(0,9));  
  greedy.add_address(Address(10,1));  
  greedy.add_address(Address(4,7));
  greedy.add_address(Address(4,6));
  greedy.add_address(Address(2,1));
  greedy.add_address(Address(-2,-3));
  greedy.add_address(Address(-8,7));
  greedy.add_address(Address(0,-9));  
  greedy.add_address(Address(0,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,0));
  greedy.add_address(Address(1,1));
  greedy.add_address(Address(2,-3));
  greedy.add_address(Address(8,-7));
  greedy.add_address(Address(0,9));  
  greedy.add_address(Address(-10,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,-5));
  display = greedy;
  optimized = greedy;
  optimized.add_depot();
  greedy.greedy_route();
  optimized.optimized_route();

  cout << "Long Address List(20): ";
  display.print();
  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();

  greedy = empty;
  optimized = empty;
  display = empty;


  greedy.add_address(Address(4,6));
  greedy.add_address(Address(2,1));
  greedy.add_address(Address(-2,-3));
  greedy.add_address(Address(-8,7));
  greedy.add_address(Address(0,-9));  
  greedy.add_address(Address(0,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,0));
  greedy.add_address(Address(1,1));
  greedy.add_address(Address(2,-3));
 
  display = greedy;
  optimized = greedy;
  optimized.add_depot();
  greedy.greedy_route();
  optimized.optimized_route();

  cout << endl << "Medium Address List(10): ";
  display.print();
  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();

  greedy = empty;
  optimized = empty;
  display = empty;


  greedy.add_address(Address(4,6));
  greedy.add_address(Address(-2,-3));
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(2,-3));
 
  display = greedy;
  optimized = greedy;
  optimized.add_depot();
  greedy.greedy_route();
  optimized.optimized_route();

  cout << endl << "Short Address List(4): ";
  display.print();
  cout << "Greedy Route length: " << greedy.length() << endl;
  cout << "Optimized Route length: " << optimized.length() << endl;
  cout << "Greedy Route: " << endl;
  greedy.print();
  
  cout << "Optimized Route: " << endl;
  optimized.print();

  // Experiment 4 

  cout << endl << "Experiment 4: Two Trucks" << endl;
  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(5,1));
  greedy.add_address(Address(2,3));
  greedy.add_address(Address(8,5));
  greedy.add_address(Address(0,9));  
  greedy.add_address(Address(-10,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,-5));
  display = greedy;
  greedy.greedy_route();
  

  cout << "Greedy Two Trucks Addresses: ";
  display.print();
  greedy.multiple_trucks(2);

  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(5,1));
  greedy.add_address(Address(2,3));
  greedy.add_address(Address(8,5));
  greedy.add_address(Address(0,9));  
  greedy.add_address(Address(-10,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,-5));
  display = greedy;
  optimized = greedy;
  optimized.optimized_route();
  

  cout << "Optimized Two Trucks Addresses: ";
  display.print();
  optimized.multiple_trucks(2);
  
  greedy = empty;
  optimized = empty;
  display = empty;

  greedy.add_address(Address(5,1));
  greedy.add_address(Address(2,3));
  greedy.add_address(Address(8,5));
  greedy.add_address(Address(0,9));  
  greedy.add_address(Address(-10,1));  
  greedy.add_address(Address(-4,-7));
  greedy.add_address(Address(4,-5));
  display = greedy;
  greedy.greedy_route();
  greedy.optimized_route();

  cout << "Greedy + Optimized Two Trucks Addresses: ";
  display.print();
  greedy.multiple_trucks(2);


  return 0;
}
