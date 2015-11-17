#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <stdexcept>

using namespace std;

//https://rsdn.ru/forum/cpp/2933384.1

std::string loadfile(char const * file) 
{
	std::ifstream fp;
	fp.open(file, std::ios::in | std::ios::binary);
	if (!fp)
	{
		throw std::runtime_error("Could not open file");
	}
	std::ifstream::pos_type size = 0;
    if (fp.seekg(0, std::ios::end))
    {
       size = fp.tellg();
    }
    std::string buffer;
	fp.seekg(0, std::ios::beg);
	if (size)
    { 
	   buffer.resize(size);
       fp.read(&buffer[0], size);
    }
	return buffer;
}

/*
string loadfile2(char const *file) {
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( file , "rb" );
    if( !fp ) perror(file),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    // allocate memory for entire content 
    buffer = (char*)calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    // copy the file into the buffer 
    if( 1!=fread( buffer , lSize, 1 , fp) )
      fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    // do your work here, buffer is a string contains the whole text 
    //size = (int *)lSize;
    fclose(fp);

        std::string s(buffer);
    return s;
}
*/

// строка для теста - }repeat }repeat }repeat }repeat

// фунция из прочитанного файла делает вектор слов
vector<string> s2v(string in) {
    vector<string> vs;
    string temp("");
    string razd(" \t\r\n"); // наши разделители по заданию
    for(std::string::iterator it = in.begin(); it != in.end(); ++it) {
        if ( razd.find(*it) == std::string::npos ) {
            temp += *it;
        } else if ( temp.length() > 0 ) {
                vs.push_back(temp);
                temp = "";
        }
    }
    if ( temp.length() > 0 ) {
        vs.push_back(temp);
    }

    return vs;
}

int main(){
    string s = loadfile("lab6p1.cpp");
    vector<string> vs = s2v(s);

std::ostream_iterator<string> itOut(std::cout,"\n");
std::copy(vs.begin(), vs.end(), itOut);
//exit(0);
std::cout << "\n";
//сортируем чтоб все одинаковые слова оказались подряд
std::sort(vs.begin(), vs.end());
// выделяем повторы
std::vector<string>::iterator it = std::unique(vs.begin(), vs.end());
std::copy(vs.begin(), vs.end(), itOut);
std::cout << "\n";
// удаляем повторы
vs.erase(it, vs.end());
std::copy(vs.begin(), vs.end(), itOut);
std::cout << "\n";
}

