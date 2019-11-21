#ifndef KCHECK_H_XRYFQPCA
#define KCHECK_H_XRYFQPCA

#include <config.h>

#ifdef DEBUG

/**
 * Check if the condition is true and print if check passed on LOG_INFO and if
 * failed on LOG_ERROR. Use for sanity checks and such.
 *
 * @param int       the expression to check
 * @param char *    the label to describe the check
 */
void kcheck(int condition, char *label);

#else
#define kcheck(...)
#endif

#endif /* end of include guard: KCHECK_H_XRYFQPCA */
