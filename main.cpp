#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <cmath>
#include <math.h>
#include <algorithm>

using namespace std;

enum { NONE, ALPHA, INT, FLOAT };

/**

  - операции + - * / ^
  - специальные операции ( и )
  - операции функции sin(, cos( и т.д.
    при построении ОПЗ они действуют как "("
    а при вычислении выражения как унарная операция
    можно считать что "(" это тоже унарная операция, которая ничего не делает
                                       просто берет предыдущий результат

  - операция Var - типа переменная (пока вычисляется с помощью функции getVar
    и можно получить только e и pi). Задумано для обработки нескольких выражений типа
    x=1; b=2; f=2*x^2+3*x+b
    значения x b заталкиваются в некую таблицу переменных и потом используются при расчете f

  - операция Number - число, константа

*/

struct Operation {
    string name;
    //string shortname;
    int priority; // приоритет операции
    int col;      // номер позиции в исх строке (для сообще об ошибке)
    int op_count; // количество операндов у данной операции (не используется пока)
    string varname = ""; // имя переменной для Var
    double result = 0;   // число для Number (не используется)
    Operation(string _name, int _priority, int _col, int _op_count) :
        name(_name), priority(_priority), col(_col), op_count(_op_count) {};
    Operation(string _name, int _priority, int _col) :
        name(_name), priority(_priority), col(_col), op_count(0) {};
    void print() {
        cout << "Oper: " << name << " " << varname <<  "\n   col: " << col << "  priority: " << priority;
        cout << "\n";
    }

};

/**
    Список операций
    фактически просто таблица, один раз набивается данными
    (нужно было бы сделать ее статической наверное и возможно не list а map)
    - используется при парсинке
*/
class Operations {
public:
    bool isOperation(string str) {
        if ( lop.empty())
            return false;
        for (list<Operation>::iterator it = lop.begin(); it!=lop.end(); it++ )
            if ( str == (*it).name ) {
                return true;
            }
        return false;
    }
    int getPriotity(string str) {
        if ( lop.empty() )
            return 9999;
        for (list<Operation>::iterator it = lop.begin(); it!=lop.end(); it++ )
            if ( str == (*it).name ) {
                return (*it).priority;
            }
        return 9999;
    }

    list<Operation> lop;
};
Operations op;

/**
    Класс для хранения операций и операндов


*/
class CalcNode {
public:
    CalcNode() {
    };
    void setOperation(Operation op) {
        oper.name = op.name;
        oper.col = op.col;
        oper.priority = op.priority;
    }
    void setOperation(string opname, int col) {
        oper.name = opname;
        oper.col = col;
        oper.priority = op.getPriotity(opname);
    }
    inline void setVarName(string varname) { oper.varname = varname; }
    inline string getVarName() { return oper.varname; }
    inline Operation getOperation() { return oper; }
    inline string getName() { return oper.name; }
    inline int getPriority() { return oper.priority; }
    void setResult(double d) {
        result = d;
        calculated = true;
    }
    void setResult(string r) {
        //result = stod(r);
        // не работает в GNU c++
        std::stringstream ss;
        ss << r;
        ss >> result;

        if (ss.fail()) { /*!!! error */ }
        calculated = true;
    }
    double getResult() { return result; }
    string getStrResult() {
        if (!calculated)
            return "NaN";
        std::ostringstream sstream;
        sstream << result;
        return sstream.str();
    }
    inline bool getCalculated() { return calculated;  }
    void print() {
        cout << "CalcNode: result=" << getStrResult() <<"\n";
        oper.print();
    }
private:
    Operation oper = Operation("NOP",0,0,0);
    double result;
    bool calculated = false;

};

/**
*
* Функция парсинга входной строки

  @param str -
  @param s_start

  @return -

  На входе выражение типа 2*(3+4)/
  На выходе список узлов Number 2, Oper *, Oper ( и т.д.

  У нас могут быть функции, например sin(3.14)
  тогда операция будет называться sin(
  обязательно с открывающей скобкой!

*
*/
list<CalcNode> parse(string str, int s_start, int s_end = 0) {
    list<CalcNode> lcn;
    if ( s_end == 0 ) {
        s_end = str.length()-1;
    }
    if ( s_start > s_end )
        return lcn;

    int col = -1;
    string temp = ""; //здесь у нас будут символы из строки с которыми пока не опеределились
    int type = NONE; // а это тип того что может быть
                    // если в temp пусто то NONE
                    // если попадается цифра то будет INT (потом может добавиться "." и будет FLOAR
                    // если буква то переменная или функция
    for(int i=s_start; i<=s_end; i++) {

        if ( str[i] == ' ' ) {
            switch (type) {
                case NONE:
                    break;
                case ALPHA: {
                    //переменная
                    CalcNode cn;
                    cn.setOperation(Operation("Var",0,col));
                    cn.setVarName(temp);
                    lcn.push_back(cn);
                    temp = "";
                    type = NONE;
                    col = i+1;
                    break;
                }
                case INT:
                case FLOAT: {
                    // число
                    CalcNode cn;
                    cn.setOperation(Operation("Number",0,col));
                    cn.setResult(temp);
                    lcn.push_back(cn);
                    temp = "";
                    type = NONE;
                    col = i+1;
                    break;
                }
            }
            continue;

        }

        string t1 = temp + str[i];
        // вместе с последним символом это операция -- типа sin(
        if (op.isOperation(t1)) {
            CalcNode cn;
            cn.setOperation(t1,i);
            lcn.push_back(cn);
            temp = "";
            type = NONE;
            col = i+1;
            continue;
        }
        t1 = str[i];
        // это операция из одного символа - типа + или (
        if (op.isOperation(t1)) {
            switch (type) {
                case NONE:
                    break;
                case ALPHA: {
                    //переменная
                    CalcNode cn;
                    cn.setOperation(Operation("Var",0,col));
                    cn.setVarName(temp);
                    lcn.push_back(cn);
                    break;
                }
                case INT:
                case FLOAT: {
                    // число
                    CalcNode cn;
                    cn.setOperation(Operation("Number",0,col));
                    cn.setResult(temp);
                    lcn.push_back(cn);
                    break;
                }
                temp = "";
                type = NONE;
            }
            CalcNode cn;
            cn.setOperation(t1,i);
            lcn.push_back(cn);
            temp = "";
            type = NONE;
            col = i+1;
            continue;
        }

        // это цифра
        if (isdigit(str[i])) {
            switch (type) {
            case NONE:
            case INT:
                type = INT;
                temp += str[i];
                break;
            case FLOAT:
                temp += str[i];
                break;
            case ALPHA:
                // x1
                temp += str[i];
            }
            continue;
        }

        // это точка
        if ( str[i] == '.' ) {
            if (type==INT) {
                temp += str[i];
            } else {
                //!!!ошибка
// TODO (kis#1#):
            }
            continue;
        }

        // это буква
        if (isalpha(str[i])) {
            if (type==NONE ||type==ALPHA) {
                temp += str[i];
                type = ALPHA;
            }else {
                //!!!ошибка
            }
            continue;
        }

        temp += str[i];
        //
 //       if (temp is Operation) {
            //temp это операция из списка
 //           continue;
 //       }
    }
    return lcn;
}

/**
    Список узлов в обратную польскую запись (ОПЗ)
    Reverse polish notation (RPN)

    Список узлов после парсинга строки преобразуем в список
    обратной польской записи по алгоритму (типа) Э. Дейкстра
*/

list<CalcNode> list2RPN(list<CalcNode> lcn) {
    list<CalcNode> rpn_list; // здесь на выходу будет ОПЗ
    list<CalcNode> t_list; // стек (Техас)
    for( list<CalcNode>::iterator it = lcn.begin(); it!=lcn.end(); it++ ) {
        //cout << (*it).getName();
        CalcNode& r = *it;
        string opname = r.getName();
        // если переменная или констанка (число) то сразу в Калифорнию
        if ( opname=="Var" || opname=="Number" ) {
            rpn_list.push_back(r);
            continue;
        }
        // дальше только операции
        // если стек (Техас) пустой то любую операцию отправляем в стек
        if (t_list.empty()) {
            t_list.push_front(r);
            continue;
        }
        // операцию "(" и все функции (они заканчиваются на открывающую скобку!)
        // в стек
        if (*opname.rbegin() == '(' ) {
            t_list.push_front(r);
            continue;
        }
        // операция ")"
        // всё из стека до открывающей скобки (или функции!) включительно
        // отправляем в Калифорнию, саму ")" игнорируем
        if ( r.getName() == ")" ) {
            //bool b = true;
            while ( true ) {
                if (t_list.empty()) {
                    //!!!ошибка
                }
                string s = t_list.front().getName();
                rpn_list.push_back(t_list.front());
                t_list.pop_front();
                if (*s.rbegin() == '(' ) {
                    break;
                }
            }
            continue;
        }
        // Если приоритет операции >= чем у верхней в стеке
        // то текущую отравляем в стек
        if ( r.getPriority() > t_list.front().getPriority() ) {
            t_list.push_front(r);
            continue;
        } else {
            // иначе
            // приоритет текущей меньше чем у верхней в стеке
            // вынимаем из стека все операции у которых приоритет больше
            // и отправляем в калифорнию
            // потом текущую в стек
////            while (!t_list.empty()) {
////                if ( r.getPriority() < t_list.front().getPriority() ) {
////                    rpn_list.push_back(t_list.front());
////                    t_list.pop_front();
////                    continue;
////                }
////                break;
////            }
////            t_list.push_front(r);
////            ----
            rpn_list.push_back(t_list.front());
            t_list.pop_front();
            t_list.push_front(r);

        }
    } //for

    // Дошли до конца списка - все из стека отправляем в Калифорнию
    while (!t_list.empty()) {
        rpn_list.push_back(t_list.front());
        t_list.pop_front();
    }

    return rpn_list;
}

/**
    Функция для подстановки переменных
    (временно, нужно придумать что то получше)
*/
double getVar(string var) {
    if ( var == "e" )
        return 2.7182818284590452354 ;
    if ( var == "pi" )
        return 3.14159265358979323846;
}


/**
    Вычисление выражения по обратной польсокй записи
*/
void Calc(list<CalcNode> rpn) {
    list<CalcNode> calcstack;
    for(list<CalcNode>::iterator it=rpn.begin(); it != rpn.end(); it++) {
        CalcNode& r = *it;
        string opname = r.getName();
        if ( opname=="Number" ) {
            calcstack.push_front(r);
            continue;
        }
        if ( opname=="Var" ) {
            if (!r.getCalculated()) {
                r.setResult(getVar(r.getVarName()));
            }
            calcstack.push_front(r);
            continue;
        }
        if ( opname == "(" ) {
        }
        if ( opname == "+" || opname == "*" || opname == "-" || opname == "/"
            || opname == "^") {
                cout << "\nOper " ;
            double op1 = calcstack.front().getResult();
            calcstack.pop_front();
            double op2 = calcstack.front().getResult();
            cout << op1 << opname << op2 << "=";
            if ( opname == "+" ) {
                op1 += calcstack.front().getResult();
            } else
            if ( opname == "-" ) {
                op1 = calcstack.front().getResult() - op1;
            } else
            if ( opname == "*" ) {
                op1 *= calcstack.front().getResult();
            } else
            if ( opname == "/" ) {
                op1 = calcstack.front().getResult() / op1;
            } else
            if ( opname == "^" ) {
                op1 = pow(calcstack.front().getResult(), op1);
            }

            calcstack.pop_front();
            r.setResult(op1);
            calcstack.push_front(r);
            cout << op1;
        }
//        if ( opname == "*" ) {
//            double op1 = calcstack.front().getResult();
//            calcstack.pop_front();
//            op1 *= calcstack.front().getResult();
//            calcstack.pop_front();
//            r.setResult(op1);
//            calcstack.push_front(r);
//        }
        if ( *opname.rbegin() == '(' ) {
                cout << "\n" << opname << calcstack.front().getResult() << ")";
            double op1;
            if ( opname == "sqrt(" ) {
               op1 = sqrt(calcstack.front().getResult());
            }
            if ( opname == "sin(" ) {
               op1 = sin(calcstack.front().getResult());
            }
            if ( opname == "cos(" ) {
               op1 = cos(calcstack.front().getResult());
            }
            calcstack.pop_front();
            r.setResult(op1);
            calcstack.push_front(r);
            cout << " = " << op1;
        }

    }
    double res = calcstack.front().getResult();
    cout << "\n\nResult = " << res;
}

int main(int argc, char* argv[])
{
    cout << "Hello world!" << endl;
    string cmdl = "";
    for(int i = 1; i < argc; i++)
        cmdl += argv[i];
    cmdl += ' ';
      //cout << "argv[" << i << "] = " << argv[i] << endl;

    op.lop.push_back(Operation("NOP",0,0,0));
    op.lop.push_back(Operation("+", 20,0,2));
    op.lop.push_back(Operation("-", 20,0,2));
    op.lop.push_back(Operation("*", 30,0,2));
    op.lop.push_back(Operation("/", 30,0,2));
    op.lop.push_back(Operation("^", 40,0,2));
    op.lop.push_back(Operation("(", 10,0,0));
    op.lop.push_back(Operation(")", 10,0,0));
    op.lop.push_back(Operation("sin(",10,0,1));
    op.lop.push_back(Operation("cos(",10,0,1));
    op.lop.push_back(Operation("sqrt(",10,0,1));

    list<CalcNode> lcn;
    //lcn = parse("(1+2)*e+sin(3.14) ",0);
    //lcn = parse("(1+2)*3 ",0);
    lcn = parse(cmdl,0);
    std::for_each(lcn.begin(),lcn.end(),[](CalcNode cn){cn.print();cout << "\n";});
cout << "================================\n\n";
    list<CalcNode> rpn;
    rpn = list2RPN(lcn);
    std::for_each(rpn.begin(),rpn.end(),[](CalcNode cn){cn.print();cout << "\n";});
    Calc(rpn);
    return 0;
}
