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


// печатаем красивые строчки не больше чем по num символов
// наш вектор из строк не содержит пробелов, поэтому пробелы 
// вставляем при печати и не забываем прибавлять 1 для учета длины
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
	
	// преобразуем буфер в строку и вернем ее
	std::string s(buffer);
    return s;
}

/*
Основная обработка текста - разбиваем наш текст на отдельные слова и создаем из них вектор строк
эти строки могут быть трех видов
1 - <одна-или-несколько-букв-цифр>
2 - <одна-или-несколько-букв-цифр><знак-препинания>
3 - <знак-препинания>
*/
vector<string> obrabotka(string in) {
	
	// строки для проверки символов слов (букв-цифр) и знаков препинания
	string alf("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	string razd(".,!?:;");

	/*
	этот кусок для замены разделителей на пробелы по условию задания (а)
	но он не обязателен потому что дальше мы все символы, которые не попадают под буквы-цифры 
	и знаки препинания будем считать разделителями и игнорируем
	for(std::string::iterator it = in.begin(); it != in.end(); ++it) {
		if ( *it == '\t' || *it == '\n' )
			*it = ' ';
	}
	*/
	vector <string> out;
	string temp("");
	bool probel = false; // 
	// посимвольно читаем наш текст и добавляем слова в строку temp
	// когда встречаются знаки препинания или разделители то добавляем полученное слово в вектор out и
	// начинаем новое слово. 
	// Если у нас встречается пробел (или другое разделитель) мы его игнорируем, но запоминаем что был пробел чтоб
	// 1 если встретится знак препинания добавить его к предыдущему слову
	// 2 если встретится буква-цифра начать новое слово
	for(std::string::iterator it = in.begin(); it != in.end(); ++it) {
		//-------------------------------------------------
		if ( alf.find(*it) != std::string::npos ) {
			//Нашли букву
			if ( probel ) {
				//если до этого был пробел то Нужно начинать новое слово
				if ( temp.length() > 0 ) { // но только если наше текущее не пустое 
									//такое может быть если идет комбинация ", " 
									//(вместо запятой может быть любой знак препинания, а вместо пробела любой разделитель)
					if ( temp.length() > 10 ) // если слово длиннее чем надо заменяем на вау
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
			//Нашли знак препинания
			// Если встретился знак -- слово закончилось (по условию после знака препинания должен быть пробел)
			if ( temp.length() > 10 ) // если слово длиннее чем надо заменяем на вау
				temp = "Vau!!!";
			// добавим к слову знак, слово добавим в вектор и начнем новое
			temp += *it;
			out.push_back(temp);
			temp = "";
			probel = false;
		//-------------------------------------------------
		} else {
			// тут должны быть только пробелы (в реальности может быть много чего)
			// мы их игнорируем и считаем разделителем
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
