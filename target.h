#ifndef _TARGET_H_
#define _TARGET_H_
#include <iostream>
#include <string>
#include <sstream>

const int maxDepends = 10;

using namespace std;

struct Target {  // You may add fields and methods as you see fit
                      // Do *not* change the provided methods.
  int local_times;
  bool leaf;
  string name;
  int depindex;
  ostream& out;
  Target **depends;
  ~Target();
  bool in_list(const string& name, Target* array[], int num);
  Target(const std::string &name, std::ostream &out = std::cout);
  bool isLeaf() const;
  void addDepend(Target *d);
  void update(int time);
  int lastUpdated() const;
  void build();
};

#endif
