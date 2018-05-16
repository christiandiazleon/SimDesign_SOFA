#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    double x;
    char *s;

    s = " -2309.12E-15";
    x = atof(s); /* x = -2309.12E-15 */

    printf("x = %.4e\n", x);
}