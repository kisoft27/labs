//#include <iterator>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept> 
class FIterator;

class Factorial {
public:
	friend FIterator; // ��������� friend ���� ������������ ��������� ������� get
	
	Factorial(void); 
	FIterator begin(); 
	FIterator end();
private:
	long get(int n); // ����� ����������� ��������, ������ �� ��������
	const static int size = 10;
};
 

// �������� ���������� ��� ������ ���������
/* 
	� ������ ��������� ���������� ����������� ������ �� 10!
	������ ����������� ����������.
	� ��������� _���_ �������� �� ������������ ��������
	int current
	������� �������� ����� ��������� �������� ��������� �� �������
	���������� ����������.
	� �������� ������ ��������� ��� ����� ������

������ ��������� ���� ���
http://www.cyberforum.ru/cpp-beginners/thread359114.html
	
*/
//template<class T>
class FIterator
{
public:
   typedef long value_type;
   typedef long* pointer;
   typedef long& reference;
   typedef std::forward_iterator_tag iterator_category;
   typedef ptrdiff_t difference_type;
   
   FIterator(int curr):current(curr)
   {
   }
   FIterator& operator =(const FIterator& other)
   {
       if (this != &other)
       {
           current = other.current;
       }
       return *this;
   }
   FIterator& operator ++()
   {
       ++current;
       return *this;
   }
   FIterator& operator --()
   {
       --current;
       return *this;
   }
   FIterator& operator +(int i)
   {
       current += i;
       return *this;
   }
   FIterator& operator -(int i)
   {
       current -= i;
       return *this;
   }
//   FIterator operator ++(int i)
//   {
//       FIterator tmp(current);
//       ++current;
//       return tmp;
//   }
   long operator *()
   {
	  Factorial f;
	  return f.get(current);
   }
//   int* operator ->()
//   {
//      return current;
//   }
   bool operator ==(const FIterator& other)
   {
      return current == other.current;
   }
   bool operator !=(const FIterator& other)
   {
      return !(*this == other);
   }
private:
   int current;
};

/*
���������� ������ ���������
��� ������� �� ��������
*/
Factorial::Factorial() {}; 
FIterator Factorial::begin() { return FIterator(1); }
FIterator Factorial::end()   { return FIterator(size+1); }
long Factorial::get(int n) {
//	std::cout<<"get="<<n<<"\n";
		if ( n > 10 || n < 1)
			throw std::runtime_error("Factorial too big!");;
		if ( n == 1 ) 
			return 1;
		return n*get(n-1);
	};

 
int main()
{
	std::cout << "OK\n";
   Factorial f;
   for (FIterator iter = f.begin(); iter != f.end(); ++iter)
   {
      std::cout << *iter << std::endl;
   }
   
  std::vector<long> myvector (7);

  std::copy ( f.begin(), f.begin()+7, myvector.begin() );

  std::cout << "myvector contains:";
  for (int i=0; i<6; ++i)
    std::cout << ' ' << myvector[i];

  std::cout << '\n';

// ��� ����� ����������   
  //FIterator ii = f.end();
  //std::cout << *ii << '\n';
}