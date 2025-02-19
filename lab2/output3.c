#include <stdio.h>
#include    <stdlib.h>

/* Test program with multiple spaces    and tabs */
INT    main()   {
    INT    x    =    10;
    IF   (x    >   5)   {
        printf("Hello    World!\n");
    }
    
    WHILE   (x   >   0)    {
        x--;
    }
    
    #define MAX 100
    #ifdef DEBUG
    printf("Debug mode\n");
    #endif
    
    RETURN    0;
}