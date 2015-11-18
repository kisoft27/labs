#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int gen_rand_int(int a, int b){
    return rand() % (b-a) + a;
}
int gen_rand_int500(){
    return (rand() % 1001) - 500;
}

// ������� ��������� � �������� ������ ��� � ����������� std::generator �� ���������
template <int a, int b>
int  tgen_rand_int(){
    return rand() % (b-a) + a;
}

/*
1. ������������ ����� � ������������������.
2. ����������� ����� � ������������������.
3. ������� ����� � ������������������.
4. ���������� ������������� �����.
5. ���������� ������������� �����.
6. ����� �������� ��������� ������������������.
7. ����� ������ ��������� ������������������.
8. ��������� �� ������ � ��������� �������� ������������������.
*/
struct SuperFUNctor
{
    // ��� ����� ������ ������� ��� ������������� ���� ��� � ��� �������� ���������� ������� � ����������
    SuperFUNctor(int _first) 
		: first(_first), max(_first), min(_first), aver(0), count(0), 
		  count_positive(0), count_negative(0), sum_odd(0), sum_even(0)
		{}

    // for_each calls operator() for each container element
    void operator() (const int &x)
    {
		count++;
		if ( x > max )
			max = x;
		if ( x < min )
			min = x;
			
        if ( x > 0 )
			count_positive++;
        if ( x < 0 )
			count_negative++;
        if ( x % 2 == 0 )
			sum_even += x;
		else 
			sum_odd += x;
		aver = (sum_even + sum_odd) / count;
		// �� ������ ���� ����� �������� ������ � ���������� 
		// � � ����� � ��� ����� ��������� ������� � ���������� 
		// ����������: ��� ��������� ����������, �� � �� ���� ��� �� ������� 
		first_last = first == x;
		
    }

    int first, max, min, aver, count, count_positive, count_negative, sum_odd, sum_even;
	bool first_last;
};

int main() {

	int n = 23;
	std::vector<int> numbers(n);
	//std::generate(numbers.begin(), numbers.end(), gen_rand_int500);
	std::generate(numbers.begin(), numbers.end(), tgen_rand_int<-5,5>);

	// Invoke for_each, and capture the result
	SuperFUNctor func = std::for_each(numbers.begin(), numbers.end(), SuperFUNctor(*(numbers.begin())));
	std::cout << endl;
	std::cout << "Max: " << func.max << "\n";
	std::cout << "Min: " << func.min << "\n";
	std::cout << "Average: " << func.aver << "\n";
	std::cout << "Positive count: " << func.count_positive << "\n";
	std::cout << "Negative count: " << func.count_negative << "\n";
	std::cout << "Sum odd: " << func.sum_odd << "\n";
	std::cout << "Sum even: " << func.sum_even << "\n";
	std::cout << "First==Last: " << func.first_last << "\n";

}
