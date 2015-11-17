#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <string>

using namespace std;

int gen_rand_int(int a, int b){
    return rand() % (b-a) + a;
}

const char *shape_name[] = {"triangle","square","rectangle", "pentagon"};

typedef struct {
    int x,y;
} Point;

typedef struct {
    int vertex_num;
    std::vector <Point> vertexes;
	// добавим в структура пару своих параметров
	std::string name; // это для красоты и чтоб видней было при печати
	int sort; // это для сортировки и чтоб различать квадрат и прямоугольник
	/* если добавлять параметры нельзя то придется писать функцию которая будет отличать квадрат 
		или прямоугольник, а это глупо если есть возможность сразу задать
	*/
} Shape;

// Генерируем точку случайным образом
Point genPoint(){
    Point p;
    p.x = gen_rand_int(-10,11);
    p.y = gen_rand_int(-10,11);
    return p;
}

// Генерация фигуры
Shape genShape(){
    Shape sh;
	int type = gen_rand_int(0,4);
	sh.name = shape_name[type];
	sh.sort = type;
	switch ( type ) {
		case 0: {
			sh.vertex_num = 3;
			vector<Point> vp(sh.vertex_num);
			std::generate(vp.begin(), vp.end(), genPoint);
			sh.vertexes = vp;
			break;
			}
		case 1: {
			sh.vertex_num = 4;
			vector<Point> vp;
// чтоб получить квадрат генерим одну точку и длину стороны
// потом создаем три другие точки
			Point p = genPoint();
			int len = gen_rand_int(1,11);
			vp.push_back(p);
			p.x += len;
			vp.push_back(p);
			p.y += len;
			vp.push_back(p);
			p.x -= len;
			vp.push_back(p);
			sh.vertexes = vp;
			break;
			}
		case 2: {
			sh.vertex_num = 4;
			vector<Point> vp;
			Point p = genPoint();
			int len1 = gen_rand_int(1,11);
			int len2 = gen_rand_int(1,11);
// почти как в квадрате только две длины 
// нужно бы еще проверить что длины различаются, а то получим квадрат
			vp.push_back(p);
			p.x += len1;
			vp.push_back(p);
			p.y += len2;
			vp.push_back(p);
			p.x -= len1;
			vp.push_back(p);
			sh.vertexes = vp;
			break;
			}
		case 3: {
			sh.vertex_num = 5;
			vector<Point> vp(sh.vertex_num);
			std::generate(vp.begin(), vp.end(), genPoint);
			sh.vertexes = vp;
			break;
			}
	}
    return sh;
}

// 
vector<Shape> generateShapes(int n) {
    vector<Shape> vs(n);
    std::generate(vs.begin(), vs.end(), genShape);

    return vs;
}

//
void printPoint(Point p) {
	cout << "(" << p.x << "," << p.y << ") ";
}

// печать фигуры
void printShape(Shape sh) {
	cout << "Shape: " << sh.name << " = ";
	std::for_each(sh.vertexes.begin(), sh.vertexes.end(), printPoint);
	cout << endl;
}

// функция для подсчета числа вершин фигур через accumulate
int VSum (int x, Shape y) {return x+y.vertex_num;}

int main() {
	// 1. генерим фигуры
    vector<Shape> vs = generateShapes(10);
	//printShape(vs[0]);
	cout << "1. Generate" << endl;
	for_each(vs.begin(), vs.end(), printShape);
    cout << "\n";
	
	// 2. считаем количество вершин
    int ret = std::accumulate(vs.begin(), vs.end(), 0, VSum);
    cout << "2. Vertex count: " << ret << "\n\n";
	
	// 3. подсчеты
	// Компилируется только с ключом c++11 из за лямбда выражения 
	// можно вместо него использовать функцию
	// но с лямбда сразу видно код
	cout << "3. Count triangles etc" << endl;
	int num_tri = std::count_if(vs.begin(), vs.end(), [](Shape sh) {return sh.vertex_num == 3;});
	cout << "Triangles count: " << num_tri << endl;
	int num_squ = std::count_if(vs.begin(), vs.end(), [](Shape sh) {return sh.sort == 1;});
	cout << "Square count: " << num_squ << endl;
	int num_rec = std::count_if(vs.begin(), vs.end(), [](Shape sh) {return sh.sort == 2;});
	cout << "Rectangles count: " << num_rec << endl;
    cout << "\n";
	
	// 4. удаляем пятиугольники
	cout << "4. Delete pentagons" << endl;
	vector<Shape>::iterator it = std::remove_if(vs.begin(), vs.end(),  [](Shape sh) {return sh.vertex_num == 5;}); 
	vs.erase(it, vs.end());
	for_each(vs.begin(), vs.end(), printShape);
	cout << endl;
	
	// 5. вектор из точек из разных фигур
	cout << "5. Create vector of points" << endl;
	std::vector<Point> vp;
	std::transform (vs.begin(), vs.end(), std::back_inserter(vp), 
		[](Shape sh) { gen_rand_int(0,4);
			return sh.vertexes[gen_rand_int(0,sh.vertex_num)];});
	cout << "Points: "; 
	for_each(vp.begin(), vp.end(), printPoint);
	cout << endl << endl;
	
	// 6. сортировка
	cout << "6. Sort shapes" << endl;
	std::sort(vs.begin(), vs.end(), [](Shape a, Shape b) {return a.sort < b.sort;});
	for_each(vs.begin(), vs.end(), printShape);
	cout << endl;
}
