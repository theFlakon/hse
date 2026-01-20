#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define MAX_ROUTES 100

#define soft_assert(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
            fprintf(stderr, "Condition: %s\n", #condition); \
            fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
            return return_value; \
        } \
    } while(0)

enum status_codes
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 1,
};

typedef struct route_info
{
    double start_speed;
    double distance;
    double accel;
} route_info;

typedef struct dist_info 
{
    int routes_cnt;
    double distances_arr[MAX_ROUTES];
} dist_info;

double calc_curr_time(route_info *route);
bool parse_dist_input(dist_info *input_dist);
bool calc_total_time(
    double *total_time, dist_info *input_dist, route_info *route);
bool init_route(route_info *route);
bool init_dist_info(dist_info *input_dist);
bool are_doubles_equal(
    double fst_num, double sec_num, double epsilon);

int 
main(void)
{
    route_info route;

    init_route(&route);

    dist_info input_dist;

    init_dist_info(&input_dist);

    double total_time = 0.0;

    soft_assert(!parse_dist_input(&input_dist),
        "Ivalid input", EXIT_ERROR_CODE);

    soft_assert(!calc_total_time(&total_time, &input_dist, &route),
        "Invalid input", EXIT_ERROR_CODE);

    printf("%.4f\n", total_time);

    return EXIT_SUCCESS_CODE;
}


bool
calc_total_time(
    double *total_time, dist_info *input_dist, route_info *route)
{
    soft_assert(total_time == NULL,
        "total_time was received as a NULL pointer", false);

    soft_assert(input_dist == NULL,
        "input_dist was received as a NULL pointer", false);

    soft_assert(route == NULL,
        "route was received as a NULL pointer", false);

    double end_speed = 0.0;
    double curr_time = 0.0;

    for (int input_num_ind = 0; 
        input_num_ind < input_dist->routes_cnt; ++input_num_ind)
    {
        route->start_speed = end_speed;
        route->distance = input_dist->distances_arr[input_num_ind];

        soft_assert(scanf("%lf", &route->accel) != 1,
            "Invalid Input", false);

        curr_time = calc_curr_time(route);

        *total_time += curr_time;
        end_speed += route->accel * curr_time;

    }

    return true;
}

bool
parse_dist_input(dist_info *input_dist)
{
    soft_assert(input_dist == NULL,
        "A NULL pointer was received\n", false);

    soft_assert(scanf("%d", &input_dist->routes_cnt) != 1,
        "Invalid input", false);

    for (int idx = 0; idx < input_dist->routes_cnt; ++idx)
    {
        soft_assert(scanf("%lf", &input_dist->distances_arr[idx]) != 1,
            "Invalid input", false);
    }

    return true;
}

double
calc_curr_time(route_info *route)
{
    soft_assert(route == NULL, 
        "A NULL pointer was received\n", NAN);

    const double EPSILON = 1.0e-10;

    double a_val = 0.0;
    double b_val = 0.0;
    double c_val = 0.0;
    double discriminant = 0.0;

    double rs_time = 0.0;
    
    if (!are_doubles_equal(route->accel, 0.0, EPSILON))
    {
        a_val = route->accel / 2.0;
        b_val = route->start_speed;
        c_val = -route->distance;

        discriminant = (b_val * b_val) - (4.0 * a_val * c_val);
        
        discriminant = sqrt(discriminant);
        
        rs_time = (-b_val + discriminant) / (2.0 * a_val);
    }

    else
    {
        rs_time = route->distance / route->start_speed;
    }
    
    return rs_time;
}

bool
are_doubles_equal(double fst_num, double sec_num, double epsilon)
{
    return fabs(fst_num - sec_num) < epsilon;
}

bool
init_route(route_info *route)
{
    soft_assert(route == NULL,
        "route received as a NULL pointer", false);

    route->start_speed = 0.0;
    route->distance = 0.0;
    route->accel = 0.0;

    return true;
}

bool
init_dist_info(dist_info *input_dist)
{
    soft_assert(input_dist == NULL,
        "input_dist received as a NULL pointer", false);

    input_dist->routes_cnt = 0;

    for (int idx = 0; idx < MAX_ROUTES; ++idx)
        input_dist->distances_arr[idx] = 0.0;

    return true;
}
