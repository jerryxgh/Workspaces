/* @(#)main.c
 */

#include <stdio.h>
typedef struct P {
    double x;
    double y;
    double z;
} Point;

void printPoint(Point p);
double shortest(int loc, Point points[], int len);
double distance(Point a, Point b);
void qsort(double number[], int left, int right);
void swap(double number[ ], int i, int j);

int main(int argc, char *argv[]) {
    int num, K;
    Point points[5];
    double everyShortest[5];
    scanf("%d", &num);
    scanf("%d", &K);
    for (int i = 0; i < num; ++i) {
        scanf("%lf", &(points[i].x));
        scanf("%lf", &(points[i].y));
        scanf("%lf", &(points[i].z));
    }

    for (int i = 0; i < num; ++i) {
        everyShortest[i] = shortest(i, points, num);
    }

    qsort(everyShortest, 0, num - 1);
    
    printf("%.6lf\n", everyShortest[num - 1 - (K - 2)]);
    return 0;
}

void printPoint(Point p) {
    printf ("x: %lf  y: %lf z:%lf\n", p.x, p.y, p.z);
}

double shortest(int loc, Point points[], int len) {
    double result = 2;
    for (int i = 0; i < len; ++i) {
        if (i == loc)
            continue;
        
        double d = distance(points[loc], points[i]);
        if (d < result)
            result = d;
    }
    return result;
}

double distance(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}


void qsort(double number[],int left,int right) {
    int last=left,i;
    /*递归出口*/
    if( left >= right )
        return;
    for(i=left+1;i<=right;i++ )
        if( number[ i ]<number[ left ] )
            swap( number,i,++last );
    swap(number, left,last );
    qsort( number,left,last-1 );
    qsort( number,last+1,right );
}

void swap(double number[ ],int i, int j ) {
    double temp=number[ i ];
    number[ i ]=number[ j ];
    number[ j ]=temp;
}
