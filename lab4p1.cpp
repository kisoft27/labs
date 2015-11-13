#include <iostream>
#include <list>
#include <iterator>

using namespace std;

struct PhoneBookRec {
	PhoneBookRec(string _name, string _phone) {
		name = _name;
		phone = _phone;
	}
	~PhoneBookRec() {}
    string name;
    string phone;
};

typedef list<PhoneBookRec> phonebook_cont;
typedef list<PhoneBookRec>::iterator phonebook_cont_iter;


/*
  ���������� ������ 
  ����� ������ ������ � ������
  ��� ������� � ������������� �������� � ��������� ����� 
*/
class PhoneBook {
	public:
	PhoneBook() {
		toStart();
	}
	~PhoneBook() {
		_phonebook_cont.clear();
	}
	// 	������ ��� ��������� ��������� -- 
	//	�������������� ���� �� �� ��������� ������� ��������� ���������� _phonebook_cont
	phonebook_cont_iter begin() {
		return _phonebook_cont.begin();
	}
	phonebook_cont_iter end() {
		return _phonebook_cont.end();
	}
	phonebook_cont_iter current() {
		return _phonebook_iter;
	}
	// ���� ������� ��� ������� � ����� ����������
	void push_back(PhoneBookRec pbr) {
		_phonebook_cont.push_back(pbr);
		toStart();
	}
	void push_back(string _name, string _phone) {
		_phonebook_cont.push_back(PhoneBookRec(_name,_phone));
		toStart();
	}
	// ������� �����/����� ��������� ������ (���������)
	phonebook_cont_iter insert(phonebook_cont_iter iter, PhoneBookRec pbr) {
		_phonebook_iter = _phonebook_cont.insert(iter, pbr);
		return _phonebook_iter;
	}
	phonebook_cont_iter insertAfter(phonebook_cont_iter iter, PhoneBookRec pbr) {
		if ( iter != _phonebook_cont.end() ) 
			iter++;
		return insert(iter, pbr);
	}
	//������� ��� ������ ��� ������� � ����� (����� back_inserter)
	//  �� �������
	template < typename InsertIterator >
	void modifyRecord(InsertIterator pCurrentRecord, PhoneBookRec newRecord) {
		*pCurrentRecord = newRecord;
		toStart();
	}
	//����������� ��������� � ������ 
	phonebook_cont_iter toStart() {
		if (_phonebook_cont.empty()) {
			_phonebook_iter = _phonebook_cont.end();
			//cout << "iterator on end" << endl;
		} else {
			_phonebook_iter = _phonebook_cont.begin();
			//cout << "iterator on start" << endl;
		}
		return _phonebook_iter;
	}
	// ����� ��������� �� ���� ������� ������/����� � �� n �������
	phonebook_cont_iter moveNext() {
		if ( _phonebook_iter != _phonebook_cont.end() ) 
			_phonebook_iter++;
		return _phonebook_iter;
	}
	phonebook_cont_iter movePrev() {
		if ( _phonebook_iter != _phonebook_cont.begin() ) 
			_phonebook_iter--;
		return _phonebook_iter;
	}
	phonebook_cont_iter moveNext(int n) {
		for (int i=0;i<n;i++) 
			moveNext();
		return _phonebook_iter;
	}
	phonebook_cont_iter movePrev(int n) {
		for (int i=0;i<n;i++) 
			movePrev();
		return _phonebook_iter;
	}
	//������ ������� ������
	void printRec() {
		printRec(_phonebook_iter);
	}
	// ������ ��������� ������
	void printRec(phonebook_cont_iter it) {
			if (it == _phonebook_cont.end() ) {
				cout << "End of PhoneBook" << endl;
				return;
			}
			cout << " Name: "<<(*it).name <<endl;
			cout << "Prone: "<<(*it).phone <<endl << endl;
			if (it == _phonebook_cont.begin() ) {
				cout << ">>>>>>>>First Record" << endl;
				return;
			}
	}
	// ������ ���� ���.�����
	void printAll() {
		phonebook_cont_iter it=_phonebook_cont.begin();
		for(;it!=_phonebook_cont.end(); it++) {
			cout << " Name: "<<(*it).name <<endl;
			cout << "Prone: "<<(*it).phone <<endl << endl;
		}
	}
	// �� �������� ����� �� �������
	//private:
	phonebook_cont _phonebook_cont; // ��������� ���������� ������
	phonebook_cont_iter _phonebook_iter; // �������� ����� ����������
};


/*

  ����� ��� ���������� ���.������
  � �������� Model-View-Controller ����� ������������� ������� View-Controller 
  (����� PhoneBook ��� ������)

*/
class PhoneBookControl {
	PhoneBook *pb;
	public:
	PhoneBookControl(PhoneBook* _pb) {
		pb = _pb;
	}
	~PhoneBookControl() {}

	//���� ��� ������� � �������������
	void run() {
		char command = ' ';
		while (command != 'q') {
			printCurrentState();
			printCommands();
			cin>>command ;
			switch (command) {
				case '>':pb->moveNext(); break;
				case '<':pb->movePrev(); break;
				case 's' : pb->toStart(); break;
				case 'n' : pb->moveNext(3); break;
				case 'p' : pb->movePrev(3); break;
				case 'r' : pb->printAll(); break;
				case 'i' : pb->insert(pb->current(),getNameAndPhone()); break;
				case 'o' : pb->insertAfter(pb->current(),getNameAndPhone()); break;
				case 'a' : 
					pb->modifyRecord(back_inserter(pb->_phonebook_cont),getNameAndPhone());
					break;
				case 'm' : 
					pb->modifyRecord(pb->current(),getNameAndPhone());
					pb->printAll(); 
					break;
				default:;
			}
		}
	}
	void printCurrentState() {
		cout << "=====================" << endl;
		cout << "Current Record:" << endl;
		pb->printRec();
	}
	void printCommands() {
		cout << endl;
		cout << "---------------------" << endl;
		cout << "Available commands:" << endl;
		cout << " q - quit " << endl;
		cout << " < > - move to prev/next record " << endl;
		cout << " p n - move to prev/next 3 records " << endl;
		cout << " s - jump to start of phonebook " << endl;
		cout << " a - add a record to phonebook " << endl;
		cout << " i o - insert a record before/after current " << endl;
		cout << " m - modify current record to phonebook " << endl;
		cout << " r - print all phonebook " << endl;
		
	}
	// �������� �� ������������ ��� � �������
	// ������� ��������� ��� ���, ������ ���� 
	//����� ������ ��� � ����� ������� ��� ��������
	PhoneBookRec getNameAndPhone() {
		string _name, _phone;
		cout << "\n\nEnter name:";
		cin >> _name;
		cout << "\n\nEnter phone number:";
		cin >> _phone;
		return PhoneBookRec(_name,_phone);
	}
};

int main() {
	cout << "OK\n";
	PhoneBook pb;
	// ��� ������ �����-�� ������ � ���.�����
	pb.push_back("Linus T","98347602389");
	pb.push_back("Larry W","56876096765");
	pb.push_back("Peggy L","37658745563");
	phonebook_cont_iter iter = pb.begin();
	pb.modifyRecord(iter,PhoneBookRec("Elvis P","98347602389"));
	pb.modifyRecord(back_inserter(pb._phonebook_cont),PhoneBookRec("Gerry L L","98347602389"));
	pb.printAll();
	
	PhoneBookControl pbc(&pb);
	pbc.run();
}