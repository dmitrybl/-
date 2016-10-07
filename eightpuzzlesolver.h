/*  Автор: Беляев Дмитрий
 *  Задача: Восьминашки
 *  Программа отыскивает решение головоломки восьминашки(3x3).
 *  Вычисляется количество ходов, необходимых для того чтобы собрать пазл,
 *  а также последовательность ходов(последовательность движений пустой клетки)
*/

#include <vector>
#include <queue>
#include <set>
#include <string>
#include <utility>

class CVertex { //  Класс вершина
    private:
        const int N=3;  //  Размер игрового поля
        std::vector<int> state; //  Текущее состояние игрового поля
        std::vector<CVertex*> children; //  Потомки вершины
        int ev; //  Эвристическое расстояние
        int w;  //  Расстояние до вершины от корня дерева
        char direction; //  Символ, означающий направление движения, такое, что при движении пустой клетки,
        //мы получили данное состояние

    public:
        CVertex* parent; // Родитель вершины
        CVertex( std::vector<int> arr, int weight, char dir );
        void CalculateEvristic();   //  Вычисление эвристики(сумма манхетеновских расстояний)
        const std::vector<int>& GetState() const;   //  состояние поля
        int GetState( int index ) const;    //  Получить цифру, находящуюся в клетке с номером index
        void AddChild( std::vector<int> arr, char direction );  //  Добавить дочернюю вершину
        int GetChildrenCount() const;   //  Число потомков вершины
        int GetDirection();     //  Получить direction
        CVertex* GetChild( int index );  // Ребёнок с индексом index
        int GetPosition( int index ) const; //  Позиция цифры index в игровом поле
        int GetEv() const;  //  Эвристическое расстояние
        int GetW() const;   //  Расстояние до вершины от корня дерева
};

struct CCompare {   //  Компаратор для очереди с приоритетом
    bool operator()( CVertex* v1, CVertex* v2 );
};

class CEightPuzzleSolver {  //  Класс, реализующий алгоритм поиска решения

    private:
        const int N=3;
        std::vector<int> startState;    //  Начальное состояние игрового поля
        std::priority_queue<CVertex*, std::deque<CVertex*>, CCompare> pQ;   //  Очередь с приоритетом
        std::set<std::vector<int>> s;   //  Множество уже просмотренных вершин
        CVertex* root;  //  Корень дерева

    public:
        CEightPuzzleSolver(const std::vector<int>& state);
        ~CEightPuzzleSolver();
         bool CanSolve();   //  Функция, проверяющая, можно ли собрать восьминашки
         std::pair<int, std::string> Solve();   //  Функция, осуществляющая поиск решения

};

