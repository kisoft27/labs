#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

int gen_rand_int(int a, int b){
    return rand() % (b-a) + a;
}

typedef struct {
	int key1;
	int key2;
	std::string str;
} DataStruct;

std::string strArray[] = {
	"string 01","string 02","string 03",
	"string 04","string 05","string 06",
	"string 07","string 08","string 09",
	"string 10"
}; 


DataStruct createDataStruct() {// int _key1, int _key2, std::string _str) {
	DataStruct ds;
	ds.key1 = gen_rand_int(-5, 5);
	ds.key2 = gen_rand_int(-5, 5);
	int i = gen_rand_int(0, 10);
	ds.str = strArray[i];
	return ds;
}

DataStruct createDataStructX() {// int _key1, int _key2, std::string _str) {
	DataStruct ds;
	ds.key1 = 1;
	ds.key2 = 2;
	ds.str = "x";
	return ds;
}

void printDataStructVector(vector<DataStruct> dsv) {
	for(int i=0; i<dsv.size();i++) {
		cout << "DataStruct {" << endl;
		cout << "    key1 : " << dsv[i].key1 << endl;
		cout << "    key2 : " << dsv[i].key2 << endl;
		cout << "     str : " << dsv[i].str << endl;
		cout << "}" << "\n\n";
	}
}

bool dsc(DataStruct ds1, DataStruct ds2) {
	if ( ds1.key1!=ds2.key1 )
		return ds1.key1<ds2.key1;
	if ( ds1.key2 != ds2.key2 )
		return ds1.key2<ds2.key2;
	return ds1.str.size() < ds1.str.size();
}

int main() {
	vector<DataStruct> dsv;
	for(int i=0; i<7; i++) {
		dsv.push_back(createDataStruct());
	}
	printDataStructVector(dsv);
	cout << "-----------------------------------------------------\n\n";
	sort(dsv.begin(),dsv.end(),dsc);
	printDataStructVector(dsv);
	return 0;
}