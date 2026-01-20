#include <stdio.h>
#include <stdbool.h>

#define soft_assert(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
            fprintf(stderr, "Condition: %s\n", #condition); \
            fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
            return return_value; \
        } \
    } while(0)

struct triangle_coords
{
    double x1;
    double x2;
    double x3;
    double y1;
    double y2;
    double y3;
};

bool calc_intersection_x(struct triangle_coords *triangle,
    double *calc_intersection_x);
bool calc_intersection_y(struct triangle_coords *triangle,
    double *calc_intersection_y);
bool triangle_init(struct triangle_coords *triangle);

int
main(void)
{
    struct triangle_coords triangle;
    int scanf_rt_code = 0;

    soft_assert(triangle_init(&triangle) == false,
        "struct initialization was failed", 1);
    
    double intersection_x = 0.0;
    double intersection_y = 0.0;

    scanf_rt_code = scanf("%lf %lf %lf %lf %lf %lf", 
        &triangle.x1,
        &triangle.y1,
        &triangle.x2,
        &triangle.y2,
        &triangle.x3,
        &triangle.y3);

    soft_assert(scanf_rt_code != 6, 
        "Invalid input arguments", 1);


    soft_assert(calc_intersection_x(&triangle, &intersection_x) == false,
        "calculation failed", 1);

    soft_assert(calc_intersection_y(&triangle, &intersection_y) == false,
        "calculation failed", 1);

    printf("%.4lf %.4lf\n", intersection_x, intersection_y);
}

bool
calc_intersection_x(struct triangle_coords *triangle,
    double *intersection_x)
{
    soft_assert(triangle == NULL, "a NULL pointer was received", false);
    soft_assert(intersection_x == NULL, "a NULL pointer was received", false);

    *intersection_x = (triangle->x1 + triangle->x2 + triangle->x3) / 3.0;

    return true;
}

bool
calc_intersection_y(struct triangle_coords *triangle,
    double *intersection_y)
{
    soft_assert(triangle == NULL, "a NULL pointer was received", false);
    soft_assert(intersection_y == NULL, "a NULL pointer was received", false);

    *intersection_y = (triangle->y1 + triangle->y2 + triangle->y3) / 3.0;

    return true;
}

bool
triangle_init(struct triangle_coords *triangle)
{
    soft_assert(triangle == NULL, "a NULL pointer was received", false);

    triangle->x1 = 0.0;
    triangle->x2 = 0.0;
    triangle->x3 = 0.0;
    triangle->y1 = 0.0;
    triangle->y2 = 0.0;
    triangle->y3 = 0.0;
    
    return true;
}
