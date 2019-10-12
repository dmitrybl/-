#include <iostream>
#include "eightpuzzlesolver.h"

int main() {
    std::vector<int> state;
    for(int i=0; i < 9; i++) {
        int value=0;
        std::cin >> value;
        state.push_back( value );
    }
    
    CEightPuzzleSolver solver( state );
    std::pair<int, std::string> p=solver.Solve();
    if( p.first==-1 ) {
        std::cout << p.first;
    }
    else {
        std::cout << p.first << "\n" << p.second;
    }
    
    return 0;
}
