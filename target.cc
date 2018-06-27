#include <iostream>
#include "target.h"

using namespace std;

// Target constructor: initialize the name of the target, set its leaf to be true
// 			allocate memory for its dependency, set the ostream out to 
//			replace cout, set its number of depends to be 0 
Target::Target(const string& name, ostream& out):
        name{name}, local_times{0} ,leaf{true},
        depindex{0}, depends{new Target*[maxDepends]}, out{out} {}

// ~Target(): Target destructor, help makefile.cc to delete all the allocated memory
// (delete the dependency array)
Target::~Target() {
    delete[] depends;
}



void Target::build() {
    // base case
    if (leaf) return;
    // initialize the temp time to be the local time
    int temp = local_times;
    for (int i = 0; i < depindex; i++) {
        // recrusive all build (its children)
        depends[i]->build();
        // if the temp is not the maximum then change it to a bigger one
        if (temp < depends[i]->lastUpdated()) {
            temp = depends[i]->local_times;
        }
    }
    // print out the message and set the temp to be the greatest	
    if (temp > local_times) {
        out << "Building " << name << endl;
        local_times = temp;
    }
}

// return true if this target is a leaf
bool Target::isLeaf() const {
    return leaf;
}

// in_list: a helper function to help search target in certain array of pointer to targets
bool Target:: in_list(const string& name, Target* array[], int num) {
    for (int i = 0; i < num; i++) {
        if (name == array[i]->name) return true;
    }
    return false;
}

// update(int time); print out the message how many times does its has been updated
// set the local time of the target to be the global one
void Target::update(int time) {
    if (leaf) {
        local_times = time;
        out << name << " updated at time "<< time << endl;
    } else {
        out << "Cannot update non-leaf object" << endl;
    }
    
}

// addDepend(Target* d) consume a pointer to target d, and add it to current target's
// dependency list
void Target::addDepend(Target *d) {
    // if it is already in the list, then just return
    if (in_list(d->name, this->depends, this->depindex)) return;
    // check whether it exceeded the maximum number of dependencies
    else if (depindex == maxDepends) {
        cout << "Max dependencies exceeded" <<endl;
    } else if (!in_list(d->name, depends, depindex)) {
        depends[depindex] = d;
        // increment the number of dependencies
        depindex++;
    }
}

// return the target's local time
int Target::lastUpdated() const {
    return local_times;
}

