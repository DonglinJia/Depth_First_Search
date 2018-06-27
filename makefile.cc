#include <iostream>
#include "makefile.h"

using namespace std;

// Makefile constructor: using MIL to initialize to avoid assignment twice
// set the number of targets to be 0, global time to be 0, using ostream out to
// replace cout; initializing the array of pointer to targets
Makefile::Makefile(ostream& out) :
num_of_tar{0},global_time{0},out{out}, targets{new Target*[maxTargets]} {}


// Makefile destructor: delete all the allocate memory for targets
// then delete the whole pointer array 
Makefile::~Makefile() {
    for (int i = 0; i < num_of_tar; i++) {
        delete targets[i];
    }
    delete[] targets;
}

// find_target(const string& name): consume a const reference of string and 
// check whether its corresponding target is exist in the array 
Target* Makefile::find_target(const string &name) {
    for (int i = 0; i < num_of_tar; i++) {
        if (targets[i]->name == name) {
            return targets[i];
        }
    }
    return nullptr;
}

// addDepend(const string& t, const string& d): consume two const reference of strings
// check them whether they have corresponding targets, make one if don't. Transfer them 
// into two target and add the second target as a dependence to the first one
void Makefile::addDepend(const string& t, const string &d) {
    Target* tar = find_target(t);
    Target* dep = find_target(d);
    
    // check whether they are already paired
    if (tar&&tar->in_list(d, tar->depends, tar->depindex)) return;
    else if (tar && dep) {
            if (!tar->in_list(d, tar->depends, tar->depindex)) {
                tar->addDepend(dep);
            }
    }  
    // in case there are more than 20 targets
    else if (num_of_tar >= maxTargets) {
        cout << "Max targets exceeded" <<endl;
    } else {
        if (num_of_tar >= 19 && !tar && !dep) {
            cout << "Max targets exceeded" <<endl;
        }
        else if (!tar && !dep && num_of_tar <= 18) {
            Target* tar1 = new Target(t, cout);
            targets[num_of_tar] = tar1;
            num_of_tar++;
            Target* tar2 = new Target(d, cout);
            targets[num_of_tar] = tar2;
            num_of_tar++;
            tar1->addDepend(tar2);
            tar1->leaf = false;
        } else if (tar) {
            Target* dep1 = new Target(d, cout);
            targets[num_of_tar] = dep1;
            tar->addDepend(dep1);
            tar->leaf = false;
            num_of_tar++;
        } else if (dep) {
            Target* tar1 = new Target(t, cout);
            targets[num_of_tar] = tar1;
            tar1->addDepend(dep);
            dep->leaf = true;
            num_of_tar++;
        }
   }
}

// changeTarget(const string& t) update the global time and be wrapper function 
// to call update in the target.cc
// effect: produce output
void Makefile::changeTarget(const string &t) {
    Target* tar = find_target(t);
    global_time++;
    tar->update(global_time);
}

// rebuild(const string& t) update the file by converting string t to target t
// building every child whose parents has been updated
void Makefile::rebuild(const string& t) {
    Target* tar = find_target(t);
    tar->build();
}

