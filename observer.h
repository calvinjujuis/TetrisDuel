#ifndef observer_hpp
#define observer_hpp

#include <iostream>
#include <vector>
#include "grid.h"

class Observer {
  public:
    virtual ~Observer() = default;
    virtual void notify(Grid &theGrid) = 0;
    virtual void printGrid(Grid &theGrid) = 0;
    
    // special actions
    virtual void blind() = 0;
    virtual void unblind() = 0;
};

#endif /* observer_hpp */
