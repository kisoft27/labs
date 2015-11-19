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

typedef struct {
    int x,y;
} Point;

// Генерируем точку случайным образом
Point genPoint(){
    Point p;
    p.x = gen_rand_int(-10,11);
    p.y = gen_rand_int(-10,11);
    return p;
}

class Shape {
public:
	Point center;
	Shape(){}
	bool IsMoreLeft(Shape sh) {
		return center.x < sh.center.x;
		//return true;
	}
	bool IsUpper(Shape sh) {
		return center.y > sh.center.y;
		//return true;
	}
	virtual void Draw() {}
};

class Circle : public Shape {
	void Draw() {
		cout << "Circle : (" << center.x << "," << center.y << ")\n";
	}
};

class Triangle : public Shape {
	void Draw() {
		cout << "Triangle : (" << center.x << "," << center.y << ")\n";
	}
};

class Square : public Shape {
	void Draw() {
		cout << "Square : (" << center.x << "," << center.y << ")\n";
	}
};

Shape* genShapeP() {
	Shape *shp;
	shp = new Shape;
	shp->center = genPoint();
	return shp;
}

Shape* genShape2P() {
	Shape *shp;
	int type = gen_rand_int(0,3);
	switch (type) {
		case 0:
			shp = new Circle;
			break;
		case 1:
			shp = new Triangle;
			break;
		case 2:
			shp = new Square;
			break;
	}
	//shp = new Shape;
	shp->center = genPoint();
	return shp;
}


int main() {
	//Shape* shp1,*shp2;
	//shp1 = new Shape;
	//shp2 = new Shape;
	//bool b = shp1->IsMoreLeft(*shp2);
	vector<Shape*> vshp(10);
	std::generate(vshp.begin(), vshp.end(), genShape2P);
	std::for_each(vshp.begin(), vshp.end(), [](Shape *shp){shp->Draw();});
	
	cout << "\n";
	cout << "Left to Right\n";
	std::sort(vshp.begin(), vshp.end(), [](Shape *shpa, Shape *shpb) {return shpa->IsMoreLeft(*shpb);});
	std::for_each(vshp.begin(), vshp.end(), [](Shape *shp){shp->Draw();});
	
	cout << "\n";
	cout << "Right to Left\n";
	std::reverse(vshp.begin(), vshp.end());
	//std::sort(vshp.begin(), vshp.end(), [](Shape *shpa, Shape *shpb) {return !shpa->IsMoreLeft(*shpb);});
	std::for_each(vshp.begin(), vshp.end(), [](Shape *shp){shp->Draw();});

	cout << "\n";
	cout << "Upper to Lower\n";
	std::sort(vshp.begin(), vshp.end(), [](Shape *shpa, Shape *shpb) {return shpa->IsUpper(*shpb);});
	std::for_each(vshp.begin(), vshp.end(), [](Shape *shp){shp->Draw();});

	cout << "\n";
	cout << "Lower to Upper\n";
	std::reverse(vshp.begin(), vshp.end());
	std::for_each(vshp.begin(), vshp.end(), [](Shape *shp){shp->Draw();});
	
	cout << "OK\n";
}