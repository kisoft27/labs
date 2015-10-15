#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

template <typename T> void print_vector(vector<T> & v) {
  cout << "Vector size: " << v.size() << endl;
  for (int i=0; i<v.size();i++) {
    cout << v[i] << endl;
  }
  cout <<endl;
}


// �������� �������� ������� �� ������ ��� �� num ��������
// ��� ������ �� ����� �� �������� ��������, ������� ������� 
// ��������� ��� ������ � �� �������� ���������� 1 ��� ����� �����
void print_str_num(vector<string> vs, int num) {
	int l = 0;
  for (int i=0; i<vs.size();i++) {
	if (vs[i].length() + l > num ) {
		cout << endl;
		l = 0;
	}
	cout << vs[i] << ' ';
	l += vs[i].length() +1; 
  }
}


string loadfile(char const *file) {
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( file , "rb" );
    if( !fp ) perror(file),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = (char*)calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
      fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    /* do your work here, buffer is a string contains the whole text */
    //size = (int *)lSize;
    fclose(fp);
	
	// ����������� ����� � ������ � ������ ��
	std::string s(buffer);
    return s;
}

/*
�������� ��������� ������ - ��������� ��� ����� �� ��������� ����� � ������� �� ��� ������ �����
��� ������ ����� ���� ���� �����
1 - <����-���-���������-����-����>
2 - <����-���-���������-����-����><����-����������>
3 - <����-����������>
*/
vector<string> obrabotka(string in) {
	
	// ������ ��� �������� �������� ���� (����-����) � ������ ����������
	string alf("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	string razd(".,!?:;");

	/*
	���� ����� ��� ������ ������������ �� ������� �� ������� ������� (�)
	�� �� �� ���������� ������ ��� ������ �� ��� �������, ������� �� �������� ��� �����-����� 
	� ����� ���������� ����� ������� ������������� � ����������
	for(std::string::iterator it = in.begin(); it != in.end(); ++it) {
		if ( *it == '\t' || *it == '\n' )
			*it = ' ';
	}
	*/
	vector <string> out;
	string temp("");
	bool probel = false; // 
	// ����������� ������ ��� ����� � ��������� ����� � ������ temp
	// ����� ����������� ����� ���������� ��� ����������� �� ��������� ���������� ����� � ������ out �
	// �������� ����� �����. 
	// ���� � ��� ����������� ������ (��� ������ �����������) �� ��� ����������, �� ���������� ��� ��� ������ ����
	// 1 ���� ���������� ���� ���������� �������� ��� � ����������� �����
	// 2 ���� ���������� �����-����� ������ ����� �����
	for(std::string::iterator it = in.begin(); it != in.end(); ++it) {
		//-------------------------------------------------
		if ( alf.find(*it) != std::string::npos ) {
			//����� �����
			if ( probel ) {
				//���� �� ����� ��� ������ �� ����� �������� ����� �����
				if ( temp.length() > 0 ) { // �� ������ ���� ���� ������� �� ������ 
									//����� ����� ���� ���� ���� ���������� ", " 
									//(������ ������� ����� ���� ����� ���� ����������, � ������ ������� ����� �����������)
					if ( temp.length() > 10 ) // ���� ����� ������� ��� ���� �������� �� ���
						temp = "Vau!!!";
					out.push_back(temp);
					temp = "";
				}
				temp += *it;
				probel = false;
			} else {
				temp += *it;
			}
		//-------------------------------------------------
		} else if ( razd.find(*it) != std::string::npos ) {
			//����� ���� ����������
			// ���� ���������� ���� -- ����� ����������� (�� ������� ����� ����� ���������� ������ ���� ������)
			if ( temp.length() > 10 ) // ���� ����� ������� ��� ���� �������� �� ���
				temp = "Vau!!!";
			// ������� � ����� ����, ����� ������� � ������ � ������ �����
			temp += *it;
			out.push_back(temp);
			temp = "";
			probel = false;
		//-------------------------------------------------
		} else {
			// ��� ������ ���� ������ ������� (� ���������� ����� ���� ����� ����)
			// �� �� ���������� � ������� ������������
			probel = true;
		}
		//-------------------------------------------------
	}
	if ( temp.length() > 0 ) {
		out.push_back(temp);
	}

	return out;
}

int main (void)
{
    char const * fn = "lab2.txt";
	string fcontent;
    fcontent = loadfile(fn);

	vector<string> vs;
	vs = obrabotka(fcontent);
	
	//print_vector(vs);
	print_str_num(vs,40);
    return 0;
}
