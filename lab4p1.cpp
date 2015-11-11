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

class PhoneBook {
	public:
	PhoneBook() {
		_phonebook_iter = _phonebook_cont.end();
	}
	~PhoneBook() {}
	
	phonebook_cont_iter begin() {
		return _phonebook_cont.begin();
	}
	void push_back(PhoneBookRec pbr) {
		_phonebook_cont.push_back(pbr);
	}
	void push_back(string _name, string _phone) {
		_phonebook_cont.push_back(PhoneBookRec(_name,_phone));
	}
	template < typename InsertIterator >
	void modifyRecord(InsertIterator pCurrentRecord, PhoneBookRec newRecord) {
		*pCurrentRecord = newRecord;
	}
	void printAll() {
	cout << "printAll OK\n";
		for(phonebook_cont_iter it=_phonebook_cont.begin();it!=_phonebook_cont.end(); it++) {
			cout << " Name: "<<(*it).name <<endl;
			cout << "Prone: "<<(*it).phone <<endl << endl;
		}
	}
	phonebook_cont _phonebook_cont;
	phonebook_cont_iter _phonebook_iter;
};

int main() {
	cout << "OK\n";
	PhoneBook pb;
	pb.push_back("Linus T","98347602389");
	pb.push_back("Larry W","56876096765");
	pb.push_back("Peggy L","37658745563");
	phonebook_cont_iter iter = pb.begin();
	pb.modifyRecord(iter,PhoneBookRec("Elvis P","98347602389"));
	pb.modifyRecord(back_inserter(pb._phonebook_cont),PhoneBookRec("Gerry L L","98347602389"));
	pb.printAll();
}