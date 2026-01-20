#include <stdio.h>
#include <math.h>
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

typedef struct point_3D
{
    double x_cord;
    double y_cord;
    double z_cord;
} Point;

enum sphere_consts
{
    VALUES_CNT = 4,
    X_IND = 0,
    Y_IND = 1,
    Z_IND = 2,
    RADIUS_IND = 3,
};

enum input_consts
{
    MAX_INPUT_SPHERES_CNT = 100,
};

enum status_codes
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 1,
};

double calc_dist(Point *fst_point, Point *sec_point);
bool is_common_point(double sphere_1[], double sphere_2[]);
bool parse_input(
    int *input_spheres_cnt, 
    double spheres[MAX_INPUT_SPHERES_CNT][VALUES_CNT]);
void print_result(int input_spheres_cnt,
    double spheres[MAX_INPUT_SPHERES_CNT][VALUES_CNT]);
bool init_3_d_point(Point *point,
    double x_cord, double y_cord, double z_cord);
double calc_cord_diff(double fst_cord, double sec_cord);

int 
main(void)
{
    double spheres[MAX_INPUT_SPHERES_CNT][VALUES_CNT] = {0};

    int input_spheres_cnt = 0;

    if (!parse_input(&input_spheres_cnt, spheres))
    {
        return EXIT_ERROR_CODE;
    }

    print_result(input_spheres_cnt, spheres);

    return EXIT_SUCCESS_CODE;
}

void
print_result(int input_spheres_cnt,
    double spheres[MAX_INPUT_SPHERES_CNT][VALUES_CNT])
{
    bool was_found = false;

    for (int sphere_fst = 0; sphere_fst < input_spheres_cnt; ++sphere_fst)
    {
        for (int sphere_sec = sphere_fst + 1; 
            sphere_sec < input_spheres_cnt; ++sphere_sec)
        {
            if (is_common_point(spheres[sphere_fst], spheres[sphere_sec]))
            {
                printf("YES\n");
                was_found = true;
                break;
            }
        }

        if (was_found)
        {
            break;
        }
    }

    if (!was_found)
    { 
        printf("NO\n");
    }
}

bool
parse_input(
    int *input_spheres_cnt, 
    double spheres[MAX_INPUT_SPHERES_CNT][VALUES_CNT])
{
    soft_assert(input_spheres_cnt == NULL,
        "A parse_input func. received a NULL pointer\n", false);

    int sphere_ind = 0;

    if (scanf("%d", input_spheres_cnt) != 1)
    {
        fprintf(stderr, "Error: Invalid input num\n");
        return false;
    }

    for (; sphere_ind < *input_spheres_cnt; ++sphere_ind)
    {
        if (scanf("%lf %lf %lf %lf", 
        &spheres[sphere_ind][X_IND],
        &spheres[sphere_ind][Y_IND],
        &spheres[sphere_ind][Z_IND],
        &spheres[sphere_ind][RADIUS_IND]) != VALUES_CNT)
        {
            fprintf(stderr, "Error: Invalid input num\n");
            return false;
        }
    }

    return true;
}

double
calc_dist(Point *fst_point, Point *sec_point)
{
    soft_assert(fst_point == NULL || sec_point == NULL,
        "A NULL pointer was received\n", NAN);

    double diff_x = calc_cord_diff(
        sec_point->x_cord, fst_point->x_cord);

    double diff_y = calc_cord_diff(
        sec_point->y_cord, fst_point->y_cord);

    double diff_z = calc_cord_diff(
        sec_point->z_cord, fst_point->z_cord);

    return sqrt(diff_x + diff_y + diff_z);
}

double
calc_cord_diff(double fst_cord, double sec_cord)
{
    return pow(fst_cord - sec_cord, 2); 
}

bool
is_common_point(double sphere_1[], double sphere_2[])
{
    const double LOW_VALUE = 1.0e-10;

    Point fst_point;
    init_3_d_point(&fst_point,
        sphere_1[X_IND], sphere_1[Y_IND], sphere_1[Z_IND]);

    Point sec_point;
    init_3_d_point(&sec_point,
        sphere_2[X_IND], sphere_2[Y_IND], sphere_2[Z_IND]);
    

    double dist_between_centrs = calc_dist( 
        &fst_point, &sec_point);

    return dist_between_centrs - (sphere_1[RADIUS_IND] + sphere_2[RADIUS_IND]) <= LOW_VALUE;
}

bool
init_3_d_point(Point *point,
    double x_cord, double y_cord, double z_cord)
{
    soft_assert(point == NULL,
        "A NULL pointer receoved\n", false);

    point->x_cord = x_cord;
    point->y_cord = y_cord;
    point->z_cord = z_cord;

    return true;
}
