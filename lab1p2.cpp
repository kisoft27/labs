#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

char* loadfile(char const *);


int main (void)
{
    char* fcontent;
    char const * fn = "lab1p2.cpp";
    fcontent = loadfile(fn);

//    cout << fcontent << endl;
    long fl = strlen(fcontent);
    vector<char> v(fcontent,fcontent+fl);
    v.push_back('\n');
    for(int i=0;i<v.size();i++)
        cout << v[i];
	cout <<endl<<v[20]<<endl;
    return 0;
}

char* loadfile(char const *file) {
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( file , "rb" );
//    fp = fopen ( file , "r" ); -- так делать не нужно!! 
    if( !fp ) perror(file),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = (char*)calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
		//fputs("entire read fails",stderr);
      fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    /* do your work here, buffer is a string contains the whole text */
    //size = (int *)lSize;
    fclose(fp);
    return buffer;
}
