/**
 * Helpers for manipulating strings.
 */
#define MAXARGS 10

/* Split the string 'string' into an array argtab of strings, 
   using the space delimiter
   @param string the string to split
   @param argtab the array of splits. 
   Must be initialized before call to a size big enough to get the result of splitting.
   Last element of argtab must be NULL, to indicate the end of split
   @return the number of elements in argtab.
*/
int str_split(char * string, char * argtab[]);


/* 
   Check if string 'str' starts with 'prefix'
   @param prefix the prefix string
   @param str the string to check against prefix
   @return 1 if string str starts with string prefix, 0 if not, -1 if strings
   are null.
*/
int startswith(char *prefix, char*str);

/* 
   Gives modified string containing only lowercase letters
   @param str the string
   @return modified string
*/
char * str_to_lower(char*str);

/* 
   String compare that ignores letter case
   @param str_a the first string
   @param str_b the second string
   @return 0 if equal, different of 0 if different
*/
int caseless_strcmp(char * str_a, char * str_b);
