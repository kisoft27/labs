#include <iostream>
#include <list>
#include <cstdlib>

using namespace std;

int gen_rand_int(int a, int b){
    return rand() % (b-a) + a;
}

list<int> gen_list(int size) {
	list<int> il;
	for (int i=0;i<size;i++) {
		il.push_back(gen_rand_int(1,20));
	}
	return il;
}

template <typename T> void print_list(T list) {
	cout << "Print list >>" << endl;
	if (list.empty()) {
		cout << "Empty" <<endl;
		return;
	}	
	typename T::iterator it;
	for (it = list.begin(); it!=list.end();it++) {
		cout << *it << " ";
	}
	cout << endl;
}

/*
Печатаем список с помощью двух итераторов с головы и с хвоста
Контролируем сколько напечатано проверкой переменной ls, в которую
занесли длину списка и
уменьшаем на 1 при печати каждого элемента
(в задании подсказка про рекурсию -- понятия не имею куда ее тут вставлять
и без нее отлично, а с ней только сложней будет)
*/
template <typename T> void print_list_ht(T list) {
	cout << "Print list from head and tail>>" << endl;
	if (list.empty()) {
		cout << "Empty" <<endl;
		return;
	}	
	int ls = list.size();
	typename T::iterator it = list.begin();
	typename T::reverse_iterator rit = list.rbegin();
	for ( ; ;it++,rit++) {
		if ( ls == 0 ) break;
		cout << *it << " ";
		ls--;
		if ( ls == 0 ) break;
		cout << *rit << " ";
		ls--;
	}
	cout << endl;
}

int main() {
	int test_sizes[] = {0, 1, 2, 3, 4, 5, 7, 14};
	list<int> list_int;
	
	for (int i=0; i<8;i++) {
		list_int = gen_list(test_sizes[i]);
		print_list(list_int);
		print_list_ht(list_int);
	}
}