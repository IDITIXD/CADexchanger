//_____________задача решена при центре X=0; Y=0__________________
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace::std;
#include <omp.h>
// Базовый класс для 3D-кривых
class Curve3D {
public:
    virtual ~Curve3D() {}

    // Для рассчёта 3D-точки при параметре t
    virtual double* calculatePoint(double t) = 0;

    // Для рассчёта первой производной (3D-вектора) при параметре t
    virtual double* calculateDerivative(double t) = 0;

    // Для вывода основной информации
    virtual void printInfo() const = 0;
};

// Класс для круга
class Circle : public Curve3D {
private:
    double radius;

public:
    Circle(double r) : radius(r) {
        if (radius <= 0) {
            throw invalid_argument("Радиус должен быть положительным.");
        }
    }
    // Для рассчёта 3D-точки при параметре t
    double* calculatePoint(double t) override {
        double* point = new double[3];
        point[0] = radius * cos(t);// X
        point[1] = radius * sin(t);// Y
        point[2] = 0; // Z-координата всегда равна 0 для круга
        return point;
    }
    // Для рассчёта первой производной (3D-вектора) при параметре t
    double* calculateDerivative(double t) override {
        double* derivative = new double[3];
        derivative[0] = -radius * sin(t);
        derivative[1] = radius * cos(t);
        derivative[2] = 0;
        return derivative;
    }
    // Для вывода информации о круге
    void printInfo() const override {
        cout << "Тип кривой: Круг" << endl;
        cout << "Радиус: " << radius << endl;
    }
    double getRadius() const {
        return radius;
    }
};

// Класс для эллипса
class Ellipse : public Curve3D {
private:
    double radiusX;
    double radiusY;

public:
    Ellipse(double rx, double ry) : radiusX(rx), radiusY(ry) {
        if (radiusX <= 0 || radiusY <= 0) {
            throw invalid_argument("Радиус должн быть положительным.");
        }
    }
    // Для рассчёта 3D-точки при параметре t
    double* calculatePoint(double t) override {
        double* point = new double[3];
        point[0] = radiusX * cos(t);
        point[1] = radiusY * sin(t);
        point[2] = 0; // Z-координата всегда равна 0 для эллипса
        return point;
    }
    // Для рассчёта первой производной (3D-вектора) при параметре t
    double* calculateDerivative(double t) override {
        double* derivative = new double[3];
        derivative[0] = -radiusX * sin(t);
        derivative[1] = radiusY * cos(t);
        derivative[2] = 0; // Z-координата всегда равна 0 для эллипса
        return derivative;
    }
    // Для вывода информации об эллипсе
    void printInfo() const override {
        cout << "Тип кривой: Эллипс" << endl;
        cout << "Радиус X: " << radiusX << endl;
        cout << "Радиус Y: " << radiusY << endl;
    }
};

// Класс для спирали
class Helix : public Curve3D {
private:
    double radius;
    double step;

public:
    Helix(double r, double s) : radius(r), step(s) {
        if (radius <= 0 || step <= 0) {
            throw invalid_argument("Радиус и шаг должны быть положительными.");
        }
    }
    // Для рассчёта 3D-точки при параметре t
    double* calculatePoint(double t) override {
        double* point = new double[3];
        point[0] = radius * cos(t);
        point[1] = radius * sin(t);
        point[2] = step * t / (2 * M_PI);
        return point;
    }
    // Для рассчёта первой производной (3D-вектора) при параметре t
    double* calculateDerivative(double t) override {
        double* derivative = new double[3];
        derivative[0] = -radius * sin(t);
        derivative[1] = radius * cos(t);
        derivative[2] = step / (2 * M_PI);
        return derivative;
    }
    // Для вывода информации о спирали
    void printInfo() const override {
        cout << "Тип кривой: Спираль" << endl;
        cout << "Радиус: " << radius << endl;
        cout << "Шаг: " << step << endl;
    }
};

int main() {
    // для Рус.языка
    setlocale(LC_ALL, "Russian");
    // Инициализация случайного числа для генерации случайных параметров
    srand(static_cast<unsigned>(time(nullptr)));

    // Создание контейнера для объектов Curve3D
    vector<Curve3D*> curves;

    // Заполнение контейнера случайными кривыми
    for (int i = 0; i < 5; ++i) {
        double randomRadius = (rand() % 10) + 1; // Случайный радиус от 1 до 10
        double randomRadius2 = (rand() % 10) + 1; // Случайный радиус от 1 до 10
        double randomStep = (rand() % 5) + 1;    // Случайный шаг от 1 до 5
        int curveType = rand() % 3;              // Случайный выбор типа кривой

        Curve3D* curve = nullptr;
        if (curveType == 0) {
            curve = new Circle(randomRadius);
        }
        else if (curveType == 1) {
            curve = new Ellipse(randomRadius, randomRadius2);
        }
        else {
            curve = new Helix(randomRadius, randomStep);
        }

        curves.push_back(curve);
    }
    // Печать координат точек и производных при t=PI/4
    int N = 1;
    double t = M_PI / 4;
    for (const auto& curve : curves) {
        double* point = curve->calculatePoint(t);
        double* derivative = curve->calculateDerivative(t);
        cout << N <<".";
        curve->printInfo();
        cout << "Точка при t=PI/4: (" << point[0] << ", " << point[1] << ", " << point[2] << ")" << endl;
        cout << "Производная при t=PI/4: (" << derivative[0] << ", " << derivative[1] << ", " << derivative[2] << ")" << endl;
        N++;
        delete[] point;
        delete[] derivative;
    }

    // Создание второго контейнера для кругов
    vector<Circle*> circleContainer;

    // Заполнение второго контейнера кругами из первого контейнера (с разделением через указатели)
    for (auto curve : curves) {
        if (dynamic_cast<Circle*>(curve) != nullptr) 
        {
            circleContainer.push_back(static_cast<Circle*>(curve));
        }
    }
    N = 0;
    // Вывод нового контейнера
    for (const auto& curve : circleContainer) {
        N++;
        if(N==1)cout << "\nКРУГИ:\n";
        cout << N << ".";
        curve->printInfo();
    }


    // Сортировка второго контейнера по радиусам в порядке возрастания
   sort(circleContainer.begin(), circleContainer.end(), [](const Circle* a, const Circle* b) {
        return a->getRadius() < b->getRadius();
        });
   
   N = 0;
   // Вывод отсортированного контейнера
   for (const auto& curve : circleContainer) {
       N++;
       if (N == 1)cout << "\nКРУГИ ПОСЛЕ СОРТИРОВКИ:\n";
       cout << N << ".";
       curve->printInfo();
   }

    //// Вычисление общей суммы радиусов всех кругов во втором контейнер
    //double totalRadiusSum = 0;
    //for (const auto& circle : circleContainer) {
    //    totalRadiusSum += circle->getRadius();
    //}

    // Параллельные вычисления с использованием OpenMP
    double totalRadiusSum = 0.0;  
#pragma omp parallel for reduction(+:totalRadiusSum)
    for (size_t i = 0; i < circleContainer.size(); ++i) {
        totalRadiusSum += circleContainer[i]->getRadius();
    }

    cout << "\nОбщая сумма радиусов кругов во втором контейнере: " << totalRadiusSum << endl;
     // Освобождение памяти
    for (auto curve : circleContainer) {
        delete curve;
    }
    // Освобождение памяти
    for (auto curve : curves) {
        delete curve;
    }
   
    return 0;
}