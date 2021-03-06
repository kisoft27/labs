/**
    Formula Calculator
    v.0.3.1

    @author

    @todo
    1. ������ � ������� ������� ����� ������� � ������� �������� �����
        �� � ��� ������ ���� ������� � NodeCalc::col
    2. �������� ������ ����c����� (������� �� 0, tan(pi/2) � �.�.)
    3. ?
*/
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <cmath>
#include <math.h>
#include <algorithm>
//#include <exception>
#include <stdexcept>

#define DEBUGPRINT false

using namespace std;

enum { NONE, ALPHA, INT, FLOAT };

/**

  - �������� + - * / ^
  - ����������� �������� ( � )
  - �������� ������� sin(, cos( � �.�.
    ��� ���������� ��� ��� ��������� ��� "("
    � ��� ���������� ��������� ��� ������� ��������
    ����� ������� ��� "(" ��� ���� ������� ��������, ������� ������ �� ������
                                       ������ ����� ���������� ���������

  - �������� Var - ���� ���������� (���� ����������� � ������� ������� getVar
    � ����� �������� ������ e � pi). �������� ��� ��������� ���������� ��������� ����
    x=1; b=2; f=2*x^2+3*x+b
    �������� x b ������������� � ����� ������� ���������� � ����� ������������ ��� ������� f

  - �������� Number - �����, ���������

*/

struct Operation {
    string name;
    //string shortname;
    int priority; // ��������� ��������
    int col;      // ����� ������� � ��� ������ (��� ������ �� ������)
    int op_count; // ���������� ��������� � ������ �������� (�� ������������ ����)
    string varname = ""; // ��� ���������� ��� Var
    double result = 0;   // ����� ��� Number (�� ������������)
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
    ������ ��������
    ���������� ������ �������, ���� ��� ���������� ������� - ������������ ��� ��������
    (��������� �� � ����������� -- �� ���� ����� � c++ ��� �� �� ������� ����� ����� ������
     � ����� ����� ���� ������������ �� list � map)
*/
class Operations {
public:
    static bool isOperation(string str) {
        if ( lop.empty())
            return false;
        for (list<Operation>::iterator it = lop.begin(); it!=lop.end(); it++ )
            if ( str == (*it).name ) {
                return true;
            }
        return false;
    }
    static int getPriotity(string str) {
        if ( lop.empty() )
            return 9999;
        for (list<Operation>::iterator it = lop.begin(); it!=lop.end(); it++ )
            if ( str == (*it).name ) {
                return (*it).priority;
            }
        return 9999;
    }
    static void addOper(Operation op) {
        lop.push_back(op);
    }

    static list<Operation> lop ;
};
//Operations op;
list<Operation> Operations::lop {};

/**
    ����� ��� �������� �������� � ���������
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
        oper.priority = Operations::getPriotity(opname);
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
        // �� �������� � GNU c++
        std::stringstream ss;
        ss << r;
        ss >> result;

        if (ss.fail()) { /*!!! error */
            throw std::runtime_error("Error convert string to double: "+r);
        }
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
* ������� �������� ������� ������

  @param str -
  @param s_start

  @return - ������ CalcNode

  �� ����� ��������� ���� 2*(3+4)/
  �� ������ ������ ����� Number 2, Oper *, Oper ( � �.�.

  � ��� ����� ���� �������, �������� sin(3.14)
  ����� �������� ����� ���������� sin(
  ����������� � ����������� �������!

*
*/
list<CalcNode> _parse(string str, int s_start, int s_end = 0) {
    list<CalcNode> lcn;
    if ( s_end == 0 ) {
        s_end = str.length()-1;
    }
    if ( s_start > s_end )
        return lcn;

    int col = -1;
    string temp = ""; //����� � ��� ����� ������� �� ������ � �������� ���� �� �������������
    int type = NONE; // � ��� ��� ���� ��� ����� ����
                    // ���� � temp ����� �� NONE
                    // ���� ���������� ����� �� ����� INT (����� ����� ���������� "." � ����� FLOAR
                    // ���� ����� �� ���������� ��� �������
    for(int i=s_start; i<=s_end; i++) {

        if ( str[i] == ' ' || str[i] == ';' ) {
            switch (type) {
                case NONE:
                    break;
                case ALPHA: {
                    //����������
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
                    // �����
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
            if ( str[i] == ';' ) break;
            continue;

        }

        string t1 = temp + str[i];
        // ������ � ��������� �������� ��� �������� -- ���� sin(
        if (Operations::isOperation(t1)) {
            CalcNode cn;
            cn.setOperation(t1,i);
            lcn.push_back(cn);
            temp = "";
            type = NONE;
            col = i+1;
            continue;
        }
        t1 = str[i];
        // ��� �������� �� ������ ������� - ���� + ��� (
        if (Operations::isOperation(t1)) {
            switch (type) {
                case NONE:
                    break;
                case ALPHA: {
                    //����������
                    CalcNode cn;
                    cn.setOperation(Operation("Var",0,col));
                    cn.setVarName(temp);
                    lcn.push_back(cn);
                    break;
                }
                case INT:
                case FLOAT: {
                    // �����
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

        // ��� �����
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

        // ��� �����
        if ( str[i] == '.' ) {
            if (type==INT) {
                temp += str[i];
            } else {
                //!!!������
// TODO (kis#1#):
                throw std::runtime_error("Wrong number: "+temp+str[i]);
            }
            continue;
        }

        // ��� �����
        if (isalpha(str[i])) {
            if (type==NONE ||type==ALPHA) {
                temp += str[i];
                type = ALPHA;
            }else {
                //!!!������
                throw std::runtime_error("Wrong number: "+temp+str[i]);
            }
            continue;
        }

        temp += str[i];
        //
 //       if (temp is Operation) {
            //temp ��� �������� �� ������
 //           continue;
 //       }
    }
    return lcn;
}

/**
    ������ ����� � �������� �������� ������ (���)
    Reverse polish notation (RPN)

    ������ ����� ����� �������� ������ ����������� � ������
    �������� �������� ������ �� ��������� (����) �. ��������
*/

list<CalcNode> _list2RPN(list<CalcNode> lcn) {
    list<CalcNode> rpn_list; // ����� �� ������ ����� ���
    list<CalcNode> t_list; // ���� (�����)
    for( list<CalcNode>::iterator it = lcn.begin(); it!=lcn.end(); it++ ) {
        //cout << (*it).getName();
        CalcNode& r = *it;
        string opname = r.getName();
        // ���� ���������� ��� ��������� (�����) �� ����� � ����������
        if ( opname=="Var" || opname=="Number" ) {
            rpn_list.push_back(r);
            continue;
        }
        // ������ ������ ��������
        // ���� ���� (�����) ������ �� ����� �������� ���������� � ����
        if (t_list.empty()) {
            t_list.push_front(r);
            continue;
        }
        // �������� "(" � ��� ������� (��� ������������� �� ����������� ������!)
        // � ����
        if (*opname.rbegin() == '(' ) {
            t_list.push_front(r);
            continue;
        }
        // �������� ")"
        // �� �� ����� �� ����������� ������ (��� �������!) ������������
        // ���������� � ����������, ���� ")" ����������
        if ( r.getName() == ")" ) {
            //bool b = true;
            while ( true ) {
                if (t_list.empty()) {
                    //!!!������
                    throw std::runtime_error("Mismatched parenthesis? Can't find: (");
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
        // ���� ��������� �������� >= ��� � ������� � �����
        // �� ������� ��������� � ����
        if ( r.getPriority() > t_list.front().getPriority() ) {
            t_list.push_front(r);
            continue;
        } else {
            // �����
            // ��������� ������� ������ ��� � ������� � �����
            // �������� �� ����� ��� �������� � ������� ��������� ������
            // � ���������� � ����������
            // ����� ������� � ����
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

    // ����� �� ����� ������ - ��� �� ����� ���������� � ����������
    while (!t_list.empty()) {
        rpn_list.push_back(t_list.front());
        t_list.pop_front();
    }

    return rpn_list;
}


/**
    ��������� ���������� - ���� <���,��������>
    ����� ���������� ��������� ���� �� ����������� ���������
    (� ������, ������� ����� �� ����� �.�.
       x=1;f=sin(x)
    ������� � Variables ���������� x �� ��������� 1
    � � ��������� ������� ����� ��������� f=sin(1)
*/
map <string,double> Variables = {{ "e", 2.7182818284590452354 },
                                 { "pi", 3.14159265358979323846 },///map ���� ����������������
                                 { "Sister", 20. }};

/**
    ������� ��� ����������� ����������
*/
double getVar(string var) {
    map<string,double>::iterator it = Variables.find(var);
//    for (it=Variables.begin();it!=Variables.end();it++)
//        cout <<"\n"<<(*it).first << " : " << it->second;
//    cout << "\n";
//    it = Variables.find(var);
    if (it != Variables.end())
        return it->second;
/*    if ( var == "e" )
        return 2.7182818284590452354 ;
    if ( var == "pi" )
        return 3.14159265358979323846;
*/
    throw std::runtime_error("Can't find variable: "+var);
}


/**
    ���������� ��������� �� �������� �������� ������
*/
double _Calc(list<CalcNode> rpn) {
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
            if (DEBUGPRINT)
                cout << "\nOper " ;

            //� ����� ������ ���� ��� ��������
            if (calcstack.size()<2) {
                    throw std::runtime_error("No value in: "+opname);
            }
            double op1 = calcstack.front().getResult();
            calcstack.pop_front();
            double op2 = calcstack.front().getResult();
            if (DEBUGPRINT)
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
            if (DEBUGPRINT)
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
            if (DEBUGPRINT)
                cout << "\n" << opname << calcstack.front().getResult() << ")";
            double op1;
            //� ����� ������ ���� �� ������� ���� ���� ��������
            if (calcstack.empty()) {
                    throw std::runtime_error("No value in: "+opname);
            }
            if ( opname == "(" ) {
               op1 = calcstack.front().getResult();
            }
            if ( opname == "sqrt(" ) {
               op1 = sqrt(calcstack.front().getResult());
            }
            if ( opname == "sin(" ) {
               op1 = sin(calcstack.front().getResult());
            }
            if ( opname == "cos(" ) {
               op1 = cos(calcstack.front().getResult());
            }

            if ( opname == "asin(" ) {
               op1 = asin(calcstack.front().getResult());
            }
            if ( opname == "acos(" ) {
               op1 = acos(calcstack.front().getResult());
            }
            if ( opname == "tan(" ) {
               op1 = tan(calcstack.front().getResult());
            }
            if ( opname == "atan(" ) {
               op1 = atan(calcstack.front().getResult());
            }
            if ( opname == "exp(" ) {
               op1 = exp(calcstack.front().getResult());
            }
            if ( opname == "log(" ) {
               op1 = log(calcstack.front().getResult());
            }
            if ( opname == "log10(" ) {
               op1 = log10(calcstack.front().getResult());
            }
            if ( opname == "log2(" ) {
               op1 = log2(calcstack.front().getResult());
            }
            if ( opname == "ceil(" ) {
               op1 = ceil(calcstack.front().getResult());
            }
            if ( opname == "floor(" ) {
               op1 = floor(calcstack.front().getResult());
            }
            if ( opname == "trunc(" ) {
               op1 = trunc(calcstack.front().getResult());
            }
            if ( opname == "abs(" ) {
               op1 = abs(calcstack.front().getResult());
            }
//asin acos tan(tg?) atan exp log log10 log2 ceil floor trunc abs
//            if ( opname == "(" ) {
//               op1 = cos(calcstack.front().getResult());
//            }
            calcstack.pop_front();
            r.setResult(op1);
            calcstack.push_front(r);
            if (DEBUGPRINT)
                cout << " = " << op1;
        }

    }
    double res = calcstack.front().getResult();
    //cout << "\n\nResult = " << res;
    return res;
}

/**
    ����� ��� ����������� ����� �������
    - ������� ������
    - ������� � ���
    - ���������� ���������

    � �������� �������� ������ �������� ����� ����� =
    ��� ��� ����� ����� ������� �� ��� ����������, ������� ���������
    x=1+2...
    ���� = ��� �� ����������� ��� Noname

    ����� ���������� ��������� ��������� ��������� � map Variables � ����� ���� �����������
    ��� ����������� � ������ �������

    TODO !!! ����.
    ����� ���� ��� ���������� ������� ������ ������ �����, �� ����...

*/
class Formula {
public:
    Formula(string s, int s_start=0) :str_formula(s),srt_start(s_start) {};
    void setStrFormula(string s, int s_start=0) { }; //?
    void Calc() {
        parse();
        list2RPN();
        Result = _Calc(rpn);
        Variables.insert(make_pair(name,Result));
    };
    void printLCN() {
        std::for_each(lcn.begin(),lcn.end(),[](CalcNode cn){cn.print();cout << "\n";});
    }
    void printRPN() {
        std::for_each(rpn.begin(),rpn.end(),[](CalcNode cn){cn.print();cout << "\n";});
    }
    inline string getName() {return name;}
    inline double getResult() {return Result;}
    void printResult() {
        cout << name << " = " << Result << endl;
    }

private:
    void parse() {
        unsigned int n1 = str_formula.find(';',srt_start);
        unsigned int n2 = str_formula.find('=',srt_start);
        if (n2!=std::string::npos) {
            if (n2<n1) {
                // ������� ��� ���������� � ������ ������� ������ �����
                name = str_formula.substr(srt_start,n2-srt_start);
                name.erase(name.find_last_not_of(" ")+1);
                name.erase(0, name.find_first_not_of(" "));
////                cout << "\n" << "n2=" << n2 <<"\n" ;
////                cout << "srt_start: " << srt_start << "\n";
////                cout << str_formula <<"\n";
////                cout << "\n"<< name << "!\n";
                srt_start = n2+1;
            }
        }
        lcn = _parse(str_formula,srt_start);
        };
    void list2RPN() {
        rpn = _list2RPN(lcn);
    };
    list <CalcNode> lcn;
    list <CalcNode> rpn;
    string str_formula;
    int srt_start;
    string name = "Noname";
    double Result;
};

void printUsage() {
    cout << "Incredible Formula Calculator (ver.0.3)\n\n";
    cout << "Usage:\n";
    cout << "  fc <formula>[;<formula2...]\n\n";
    cout << "  where formula is [<name>=]1+2*4\n";
    cout << "  Availabale operations: +-*/^\n";
    cout << "  Availabale functions: sqrt sin cos asin acos tan(tg?) atan exp\n";
    cout << "                        log log10 log2 ceil floor trunc abs\n\n";
    cout << "Examples:\n";
    cout << "  1+2\n";
    cout << "  x=1+2;y=2-3\n";
    cout << "  x=1;f=sin(x)^2+cos(x)^2\n";

    exit(0);
}

string getCmdl(int argc, char* argv[]) {
    if (argc<2)
        printUsage();
    string cmdl = "";
    for(int i = 1; i < argc; i++)
        cmdl += argv[i];
      //cout << "argv[" << i << "] = " << argv[i] << endl;

    cmdl.erase(cmdl.find_last_not_of("; \n\r\t")+1);

    cmdl += ';';
    return cmdl;
}

int main(int argc, char* argv[])
{
    //cout << "Hello world!" << endl;

    string cmdl = getCmdl(argc, argv);
    cout << "Incredible Formula Calculator (ver.0.3)\n\n";
    cout << cmdl << endl;
    cout << "\nResult:\n";
    Operations::addOper(Operation("NOP",0,0,0));
    Operations::addOper(Operation("+", 20,0,2));
    Operations::addOper(Operation("-", 20,0,2));
    Operations::addOper(Operation("*", 30,0,2));
    Operations::addOper(Operation("/", 30,0,2));
    Operations::addOper(Operation("^", 40,0,2));
    Operations::addOper(Operation("(", 10,0,0));
    Operations::addOper(Operation(")", 10,0,0));
    Operations::addOper(Operation("sin(",10,0,1));
    Operations::addOper(Operation("cos(",10,0,1));
    Operations::addOper(Operation("sqrt(",10,0,1));
    Operations::addOper(Operation("tan(",10,0,1));

    Operations::addOper(Operation("asin(",10,0,1));
    Operations::addOper(Operation("acos(",10,0,1));
    Operations::addOper(Operation("atan(",10,0,1));
    Operations::addOper(Operation("exp(",10,0,1));
    Operations::addOper(Operation("log(",10,0,1));

    Operations::addOper(Operation("log10(",10,0,1));
    Operations::addOper(Operation("log2(",10,0,1));
    Operations::addOper(Operation("ceil(",10,0,1));
    Operations::addOper(Operation("floor(",10,0,1));
    Operations::addOper(Operation("trunc(",10,0,1));
//asin acos tan(tg?) atan exp log log10 log2 ceil floor trunc abs
// ��������� ������� ����� � ���� ������, ��� ��� �������� �
// � ������� _calc ��� ������� -- �� ����� ������, �� �� ������� ���� �� ���������
    Operations::addOper(Operation("abs(",10,0,1));

/**
    ��� ��������� ������� ���������� �� �������� � ����� ;
    ; -- ����������� ������ (���� �� ������ �����)
    ������ ������� ���������� � 0
    ����� �� ���� ; � �� ��������� ������� � ��� ����� �������
    ��� ������� �� �������� � ����������� ������ Formula
*/
    list<Formula> lf;
    int start_pos = 0;
    while (true) {
        unsigned int n = cmdl.find(';',start_pos);
        if (n==std::string::npos)
            break;
        Formula f(cmdl,start_pos);
        f.Calc();
        f.printResult();
        start_pos = n+1;

    }

    //Formula f(cmdl,0);
    //list<CalcNode> lcn;
    //lcn = parse("(1+2)*e+sin(3.14) ",0);
    //lcn = parse("(1+2)*3 ",0);
    //lcn = parse(cmdl,0);
    //f.parse();
    //f.printLCN();
    //cout << "================================\n\n";
    //list<CalcNode> rpn;
    //rpn = _list2RPN(lcn);
//    f.list2RPN();
//    f.printRPN();
    //f.Calc();
    return 0;
}
