#ifndef CUSTOM_ASSERT_H
#define CUSTOM_ASSERT_H

//#include "FreeRTOS.h"
#ifdef DEBUG
//#include "printf.h"
#endif

// Define your custom assert function if DEBUG is defined, otherwise define an empty macro
#ifdef DEBUG
#define CUSTOM_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            while (1) { \
                /* Infinite loop */ \
            } \
        } \
    } while (0)
#else
#define CUSTOM_ASSERT(expr) ((void)0)  // Define an empty macro if DEBUG is not defined
#endif

//taskDISABLE_INTERRUPTS();

#endif /* CUSTOM_ASSERT_H */
