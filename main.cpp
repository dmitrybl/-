#include <fstream>
#include "eightpuzzlesolver.h"

int main() {
    std::vector<int> state;
    std::ifstream in( "puzzle.in" );
    for(int i=0; i < 9; i++) {
        int value=0;
        in >> value;
        state.push_back( value );
    }
    in.close();
    std::ofstream out( "puzzle.out" );
    CEightPuzzleSolver solver( state );
    std::pair<int, std::string> p=solver.Solve();
    if( p.first==-1 ) {
        out << p.first;
    }
    else {
        out << p.first << "\n" << p.second;
    }
    out.close();
    return 0;
}
