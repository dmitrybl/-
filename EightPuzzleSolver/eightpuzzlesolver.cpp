#include "eightpuzzlesolver.h"
#include <iostream>
#include <fstream>

CVertex::CVertex( std::vector<int> arr, int weight, char dir ) {
    state=arr;
    CalculateEvristic();
    w=weight;
    direction=dir;
    parent=nullptr;
}

void CVertex::CalculateEvristic() {
    int r=0;
    for( int i=0; i<N * N; i++ ) {
        if( state[i]!=0 ) {
            r+=abs(( state[i] - 1 ) / N - (i) / N) + abs(( state[i]-1 ) % N - ( i ) % N);
        }
    }
    ev=r;
}

const std::vector<int>& CVertex::GetState() const {
    return state;
}

int CVertex::GetState( int index ) const {
    return state[index];
}

void CVertex::AddChild( std::vector<int> arr, char direction ) {
    CVertex* v = new CVertex( arr, w + 1, direction );
    v->parent = this;
    children.push_back( std::move(v) );
}

int CVertex::GetChildrenCount() const {
    return children.size();
}

int CVertex::GetDirection() {
    return direction;
}

CVertex*  CVertex::GetChild( int index ) {
    return children[index];
}

int CVertex::GetPosition( int index ) const {
    int pos=0;
    for(int i=0; i<N * N; i++) {
        if( state[i]==index ) {
            pos=i;
        }
    }
    return pos;
}

int CVertex::GetEv() const {
    return ev;
}
int CVertex::GetW() const {
    return w;
}

bool CCompare::operator()( CVertex* v1, CVertex* v2 ) {
    return (v1->GetEv() + v1->GetW() > v2->GetEv() + v2->GetW());
}

CEightPuzzleSolver::CEightPuzzleSolver(const std::vector<int>& state) {
    startState=state;
    root=new CVertex( startState, 0, '0' );
}

CEightPuzzleSolver::~CEightPuzzleSolver() {
    delete root;
}

bool CEightPuzzleSolver::CanSolve() {
    int inv = 0;
    //  В цикле считаем число инверсий. Проходим по строкам игрового поля и для каждой фишки с номером i
    //  подсчитываем количество фишек с номерами > i и позициями > index
    //  где index - позиция фишки с номером i
    for( int i=1; i < N * N; i++ ) {
        int position=root->GetPosition(i);
        position++;
        while( position <= N * N-1 ) {
            if( root->GetState( position ) < i && root->GetState( position ) != 0 ) { inv++; }
            position++;
        }
    }

    //  Если число инверсий нечетно, то восьминашки неразрешимы
    if( inv % 2==1 ) {
        return false;
    }
    return true;
}

std::pair<int, std::string> CEightPuzzleSolver::Solve() {
    if( !CanSolve() ) {
        std::pair<int, std::string> pair;
        pair.first=-1;
        pair.second="no solution";
        return pair;
    }
    CVertex* current;
    pQ.push( root );
    while( true ) {
        current=pQ.top();
        if( current->GetEv()==0 ) { break; }
        std::vector<char> directions; // Для извлечённой из очереди вершины подсчитываем направления, куда можно
        //   переместить пустую клетку
        int i=current->GetPosition( 0 );

        if( i==0 ) {
            directions.push_back( 'R' );
            directions.push_back( 'D' );
        }

        else if( i==N - 1 ) {
            directions.push_back( 'L' );
            directions.push_back( 'D' );
        }

        else if( i==N * N - 1 ) {
            directions.push_back( 'U' );
            directions.push_back( 'L' );
        }

        else if( i==N * N - N ) {
            directions.push_back( 'R' );
            directions.push_back( 'U' );
        }

        else if( i%N==0 ) {
            directions.push_back( 'U' );
            directions.push_back( 'R' );
            directions.push_back( 'D' );
        }
        else if( i%N==N-1 ) {
            directions.push_back( 'U' );
            directions.push_back( 'D' );
            directions.push_back( 'L' );
        }
        else if( i/N==0 ) {
            directions.push_back( 'D' );
            directions.push_back( 'L' );
            directions.push_back( 'R' );
        }
        else if( i/N==N-1 ) {
            directions.push_back( 'U' );
            directions.push_back( 'R' );
            directions.push_back( 'L' );
        }
        else {
            directions.push_back( 'U' );
            directions.push_back( 'D' );
            directions.push_back( 'L' );
            directions.push_back( 'R' );
        }

        auto arr=current->GetState();
        for( size_t i=0; i<directions.size(); i++ ) {
            auto arr2=arr;
            int j=current->GetPosition( 0 );
            switch( directions[i] ) {
                case 'R':
                    std::swap( arr2[j], arr2[j+1] );
                    break;
                case 'L':
                    std::swap( arr2[j], arr2[j-1] );
                    break;
                case 'U':
                    std::swap( arr2[j], arr2[j-N] );
                    break;
                case 'D':
                    std::swap( arr2[j], arr2[j+N] );
                    break;
            }

            //  Если поле с таким состоянием уже было рассмотрено, то не добавляем его в дерево
            if( s.find(arr2)==s.end() ) {
                current->AddChild( arr2, directions[i] );
            }

        }

        //  Перед тем, как удалять вершину из очереди с приоритетом, надо добавить её во множество уже просмотренных
        s.insert( pQ.top()->GetState() );
        pQ.pop();
        for( int i=0; i<current->GetChildrenCount(); i++ ) {
            pQ.push( current->GetChild(i) );
        }

    }

    std::string solution1;
    int count=0;
    // Восстанавливаем путь от конца к началу дерева
    while( current != root ) {
        solution1.push_back( current->GetDirection() );
        current=current->parent;
        count++;
    }
    std::string solution;
    for(int i=solution1.size()-1; i >= 0; i--) {
        solution.push_back(solution1[i]);
    }

    std::pair<int, std::string> pair;
    pair.first=count;
    pair.second=solution;
    return pair;
}