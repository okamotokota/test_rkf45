#include <stdio.h>
#include <math.h>
#include <math.h>

#include <Eigen/Core>

using namespace Eigen;

// 状態変数の型の定義（次元に合わせる）
#define state_type Vector2d

// 運動方程式の定義
state_type f(double t, state_type y)
{
    state_type dy;
    dy(0) = y(1);
    dy(1) = sin(t);
    return dy;
}

state_type rkf45(double *t, state_type y, double h, double *h_next, double tolerance)
{
    state_type k1, k2, k3, k4, k5, k6;
    state_type y1, y2;
    double R, delta;

    k1 = h * f(*t, y);
    k2 = h * f(*t + h / 4, y + k1 / 4);
    k3 = h * f(*t + 3 * h / 8, y + 3 * k1 / 32 + 9 * k2 / 32);
    k4 = h * f(*t + 12 * h / 13, y + 1932 * k1 / 2197 - 7200 * k2 / 2197 + 7296 * k3 / 2197);
    k5 = h * f(*t + h, y + 439 * k1 / 216 - 8 * k2 + 3680 * k3 / 513 - 845 * k4 / 4104);
    k6 = h * f(*t + h / 2, y - 8 * k1 / 27 + 2 * k2 - 3544 * k3 / 2565 + 1859 * k4 / 4104 - 11 * k5 / 40);

    y1 = 25 * k1 / 216 + 1408 * k3 / 2565 + 2197 * k4 / 4104 - k5 / 5;
    y2 = 16 * k1 / 135 + 6656 * k3 / 12825 + 28561 * k4 / 56430 - 9 * k5 / 50 + 2 * k6 / 55;

    state_type diff_y = y1 - y2;
    R = diff_y.norm() / h;
    delta = 0.84 * pow(tolerance / R, 0.25);

    if (delta <= 0.1)
    {
        *h_next = 0.1 * h;
    }
    else if (delta >= 4)
    {
        *h_next = 4 * h;
    }
    else
    {
        *h_next = delta * h;
    }

    if (R <= tolerance)
    {
        *t = *t + h;
        y = y + y1;
    }
    return y;
}

int main()
{
    double t = 0.0, h = 1, h_next;
    state_type y;
    y(0) = -1;
    y(1) = 0;
    double tolerance = 1e-6;

    // 書き込みファイルを開く
    FILE *fp;
    fp = fopen("output.csv", "w");

    while (t < 15)
    {
        printf("t: %f, y: %f\n", t, y(1));
        fprintf(fp, "%f,%f\n", t, y(1));
        y = rkf45(&t, y, h, &h_next, tolerance);
        h = h_next;
    }

    fclose(fp);

    return 0;
}
