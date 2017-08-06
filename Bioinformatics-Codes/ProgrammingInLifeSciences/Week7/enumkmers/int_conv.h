/* converts a string to a number using strtol(). Hard exits the program if the
   string is no valid number. Assumes base 0, that is it should handle
   hexadecimal and octal values.
   Returns the converted Integer. */

#ifndef INT_CONV_H
#define INT_CONV_H
/* Returns an the integer parsed from the string <number> or exits */
int get_int(const char *const number);

/* Returns an the long integer parsed from the string <number> or exits */
long get_long(const char *const number);

/* Returns an the unsigned integer parsed from the string <number> or exits */
int get_uint(const char *const number);

/* Returns an the unsigned long integer parsed from the string <number> or exits
*/
long get_ulong(const char *const number);
#endif
