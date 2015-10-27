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

/** QueueElement structure */
struct QueueElement {
    string name;
    ElementPriority priority;
};

bool compare_priority (const QueueElement& first, const QueueElement& second){
  return ( first.priority < second.priority );
}

class QueueWithPriority {
private:
    list<QueueElement> qwp_list;
    void addElement( const QueueElement & el );
public:
    QueueWithPriority() ;
    ~QueueWithPriority() ;
    void PutElementToQueue( const QueueElement & element );
    void PutElementToQueue( string name, ElementPriority priority );
    QueueElement GetElementFromQueue() ;
    void Accelerate();
    void print();
} ;

int main() {
    QueueElement qe;
    qe.name = "zzz";
    qe.priority = LOW;

    QueueWithPriority qwp;
    qwp.PutElementToQueue(qe);

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
void QueueWithPriority::addElement( const QueueElement & el ) {
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
void QueueWithPriority::PutElementToQueue( const QueueElement & element ) {
    addElement(element);
}
void QueueWithPriority::PutElementToQueue( string name, ElementPriority priority ) {
    QueueElement qel;
    qel.name =  name;
    qel.priority =  priority;
    addElement(qel);
}
void QueueWithPriority::print() {
    cout << "--- Print Queue ---" << endl;
    if ( qwp_list.empty() ) {
        cout << "Queue is empty" << endl;
    } else {
        cout << "Queue size: " << qwp_list.size() << endl;
    }
    list<QueueElement>::reverse_iterator it;
    for (it=qwp_list.rbegin(); it!=qwp_list.rend(); ++it) {
        cout << "QueueElement: " << (*it).name << " - " << (*it).priority << endl;
    }
}

void QueueWithPriority::Accelerate(){
    if ( qwp_list.empty() )
        return;
    list<QueueElement>::iterator it;
    bool isLOW = false;
    for (it=qwp_list.begin();it!=qwp_list.end();++it) {
        if( (*it).priority > LOW ) {
            break;
        }
        (*it).priority = HIGH;
        cout << "Acc " << (*it).name <<endl;
        isLOW = true;
    }
    if ( isLOW ) {
        qwp_list.sort(compare_priority);
    }
}

QueueElement QueueWithPriority::GetElementFromQueue(){
    if ( qwp_list.empty() )
        throw std::runtime_error("QueueIsEmptyExeption");
//      throw "QueueIsEmptyExeption" ;
    QueueElement qe;
    list<QueueElement>::iterator it;
    qe = *qwp_list.end();
    qwp_list.pop_back();
    return qe;
}
