#include <ctype.h>

/*----------------------------------------------------------------------
 * Convert an ASCII string into an integer.
 * Arguments:
 *      str     String of ASCII digits, possibly preceded by white
 *              space. For bases greater than 10, either lower- or
 *              upper-case digits may be used.
 *
 * Results:
 *      The return value is the integer equivalent of string. If there
 *      are no decimal digits in string, then 0 is returned.
 *
 * Side effects:
 *      None.
 *--------------------------------------------------------------------*/
int
atoi(char *str)
{
    int result = 0;
    unsigned int digit;
    int sign;

    // Skip any leading blanks.
    while (isspace(*str)) {
        str++;
    }

    // Check for a sign.
    if (*str == '-') {
        sign = 1;
        str++;
    } else {
        sign = 0;
        if (*str == '+') {
            str++;
        }
    }

    for ( ; ; str++) {
        digit = *str - '0';
        if (digit > 9) {
            break;
        }
        result = (10 * result) + digit;
    }

    if (sign) {
        return -result;
    }
    return result;
}

long
atol(char *str)
{
    long result = 0;
    unsigned int digit;
    int sign; // bool

    // Skip any leading blanks.
    while (isspace(*str)) {
        str++;
    }

    // Check for a sign.
    if (*str == '-') {
        sign = 1;
        str++;
    } else {
        sign = 0;
        if (*str == '+') {
            str++;
        }
    }

    for ( ; ; str++) {
        digit = *str - '0';
        if (digit > 9) {
            break;
        }
        result = (10 * result) + digit;
    }

    if (sign) {
        return -result;
    }
    return result;
}
