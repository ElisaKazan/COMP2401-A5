/*
 * Function: safe_string_input
 * Purpose: Performs standard string input fodder: read a string (without newline) from the console with a prompt.
 * out: The buffer to read the input into.
 * in: the prompt to use.
 */
void safe_string_input(char buf[MAX_STR], const char *prompt) {
    printf("%s? ", prompt);
    fflush(stdout);
    fgets(buf, MAX_STR, stdin);
    buf[strlen(buf)-1] = '\0';
}

/*
 * Function: safe_string_input
 * Purpose: Performs "safe" integer input. This entails using fgets to read into a buffer then sscanf to get the actual
 *          number (retrying if it is not a number). We also check (optionally) against two bounds, min and max.
 * out: A temporary buffer to store intermediate string input.
 * in: the prompt to use.
 * in: the minimum value the integer input should take on (-1 if no min)
 * in: the maximum value the integer input should take on (-1 if no max)
 * out: the integer pointer to store the validated integer in.
 */
void safe_integer_input(char buf[MAX_STR], const char *prompt, int min, int max, int *into) {
    if (min == -1) {
        min = INT_MIN;
    }
    if (max == -1) {
        max = INT_MAX;
    }

    int ret = 0;
    int input = 0;
    do {
        printf("%s? ", prompt);
        fflush(stdout);

        fgets(buf, MAX_STR, stdin);
        ret = sscanf(buf, "%d", &input);
    } while (input < min || input > max || ret != 1);

    *into = input;
}
