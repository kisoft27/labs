#include <list>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;

typedef enum
{
    LOW,
    NORMAL,
    HIGH } ElementPriority;

/** QueueElement
Можно структуру сделать с конструктором -- будет удобней инициализировать элементы
и можно определить оператор << для вывода
*/
struct QueueElement {
    string name;
    ElementPriority priority;
};

// Функция для сравнения QueueElement по приоритету
// (для стандартной сортировки
bool compare_priority (const QueueElement& first, const QueueElement& second){
  return ( first.priority < second.priority );
}

/*
На мой взгляд в очереди эффективней использовать list чем deque
Хотя вроде бы deque оптимизирован для добавления-удаления элементов в начало и конец
но все же это больше массив (сплошной) и вставки в list должны быть эффективней 
(и в середину в том числе).
Сортировка типа пузырька будет на list должна быть тоже эффективней (а какая сортировка в std?) 
за счет эффективности вставки элементов.
А сортировки, которые используют прямой доступ куда нибудь в середину очереди могут быть 
эффективней в deque за счет возможно прямого обращения к любому элементу.

И еще одна мысль по реализации, которая была у меня с самого начала была...
Сделать вместо одного списка - три (для каждого приоритета) т.е. типа
list<QueueElement> qwp_list[3]
Тогда мы разом избавляемся вообще от всех сортировок -- при вставке тупо вставляем 
в хвост соответствующей очереди qwp_list[priority]
когда нужно выбрать первый - ищем первую непустую очередь и берем из головы
И при акселерации просто переставляем все элементы из очереди LOW в очередь HIGH
Просто и эффективно!

*/

class QueueWithPriority {
private:
    list<QueueElement> qwp_list; // доступ к очереди ограничим
public:
    QueueWithPriority() ;
    ~QueueWithPriority() ;
    void PutElementToQueue( const QueueElement & el );
    void PutElementToQueue( string name, ElementPriority priority );
    QueueElement GetElementFromQueue() ;
    void Accelerate();
    void print();
	//Нужно бы еще метод типа: bool empty() {return qwp_list.empty();}
} ;

int main() {
    QueueElement qe;
    qe.name = "zzz";
    qe.priority = LOW;

	// Вставим один элемент в очередь
    QueueWithPriority qwp;
    qwp.PutElementToQueue(qe);

	// вынимаем два элемента и получим эксепшн
    try {
        qe = qwp.GetElementFromQueue();
        qe = qwp.GetElementFromQueue();
    }  catch (std::runtime_error &e)  {
        cerr << "Nothing to do" << endl;
    }

    qwp.PutElementToQueue(qe);
    qwp.PutElementToQueue("01_low ", LOW);
    cout << "---Print---" << endl;
    qwp.print();
    qwp.PutElementToQueue("02_norm", NORMAL);
    qwp.PutElementToQueue("03_low ", LOW);
    qwp.PutElementToQueue("031_low ", LOW);
    qwp.PutElementToQueue("032_low ", LOW);
    qwp.PutElementToQueue("033_low ", LOW);
    qwp.PutElementToQueue("05_high", HIGH);
    qwp.PutElementToQueue("06_norm", NORMAL);
    qwp.PutElementToQueue("07_high", HIGH);
    qwp.PutElementToQueue("08_norm", NORMAL);

    cout << "---Print---" << endl;
    qwp.print();
    cout << "Accelerate" << endl;
    qwp.Accelerate();
    cout << "---Print---" << endl;
    qwp.print();

    cout << "OK" << endl;
    return 0;
}

QueueWithPriority::QueueWithPriority(){} ;
QueueWithPriority::~QueueWithPriority(){
    qwp_list.clear();
} ;
/*
 ВАЖНО!!! Наша очередь будет задом наперед!!!
 т.е. добавляем элементы к голове, а достаем из хвоста
 для списка это все равно, зато при поиске места для вставки элемента
 так оказалось удобней потому что функция insert вставляет элемент _до_ указанного.
 А если искать место для вставки с конца то когда нашли куда вставить нужно итератор 
 сдвинуть назад.
 
 Алгоритм:
 -- Если список пустой или в наш элемент с приоритетом LOW то без раздумий добавляем в голову
 -- если первый элемент в очереди с приоритетом >= добавляемому -  добавляем в голову
 -- в цикле ищем элемент с приоритетом >= добавляемому, если нашли делаем insert и return
 -- если прошли весь цикл до конца то значит в очереди нет элементов с приоритетом бОльшим чем
    у вставляемого -- добавляем в хвост списка(т.е. начало очереди)
*/
void QueueWithPriority::PutElementToQueue( const QueueElement & el ) {
    //cout << endl << "addElement " << el.name << " " << el.priority << endl;
    if ( qwp_list.empty() || el.priority==LOW ) {
        qwp_list.push_front(el);
        return;
    }
    list<QueueElement>::iterator it;
    it=qwp_list.begin();
    if( (*it).priority >= el.priority ) {
        qwp_list.push_front(el);
        return;
    }
    it++;
    for (; it!=qwp_list.end(); it++) {
        //cout << "QueueElement: " << (*it).name << " - " << (*it).priority << endl;
        if ( (*it).priority >= el.priority ) {
            //cout << "insert" << endl;
            qwp_list.insert(it,el);
            return;
        }
    }
    qwp_list.push_back(el);
}
void QueueWithPriority::PutElementToQueue( string name, ElementPriority priority ) {
    QueueElement qel;
    qel.name =  name;
    qel.priority =  priority;
	PutElementToQueue(qel);
}
void QueueWithPriority::print() {
    cout << "--- Print Queue ---" << endl;
    if ( qwp_list.empty() ) {
        cout << "Queue is empty" << endl;
    } else {
        cout << "Queue size: " << qwp_list.size() << endl;
    }
	// обратный итератор потому что наша очередь наоборот
    list<QueueElement>::reverse_iterator it;
    for (it=qwp_list.rbegin(); it!=qwp_list.rend(); ++it) {
        cout << "QueueElement: " << (*it).name << " - " << (*it).priority << endl;
    }
}

void QueueWithPriority::Accelerate(){
    if ( qwp_list.empty() )
        return;
    list<QueueElement>::iterator it;
    bool isLOW = false; // флаг для определения если у нас элементы с проритетом LOW
    for (it=qwp_list.begin();it!=qwp_list.end();++it) {
        if( (*it).priority > LOW ) {
            break;
        }
        (*it).priority = HIGH;
        cout << "Acc " << (*it).name <<endl;
        isLOW = true;
    }
	// если нет элементов с приоритетом LOW то не зачем и сортировать
    if ( isLOW ) {
        qwp_list.sort(compare_priority);
		/* Здесь стандартная функция сортировки
		  но можно было бы сделать эффективней.
		  Поскольку у нас уже сортированный список, то можно найти 
		  отделить кусок с приоритетом LOW, найти место где начинается приоритет HIGH
		  L-L-L-L-N-N-N-N-H-H-H-H
		         ^       ^
		  И переставить (ну конечно сменить приоритет LOW -> HIGH)
		  N-N-N-N-L-L-L-L-H-H-H-H
		  т.е. будет миним операций -- один проход по списку и отрезание-вставка это фактически 
		  нужно только переписать связи у пары узлов списка
		*/
    }
}

QueueElement QueueWithPriority::GetElementFromQueue(){
    if ( qwp_list.empty() )
        throw std::runtime_error("QueueIsEmptyExeption");
//  Если список пустой то выбросим прерывание 
    QueueElement qe;
    list<QueueElement>::iterator it;
    qe = *qwp_list.end();
    qwp_list.pop_back();
    return qe;
}
