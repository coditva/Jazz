#include <kcheck.h>
#include <logger.h>

#ifdef DEBUG
void kcheck(int condition, char *label)
{
  if (condition) {
    klog_status_ok(label);
  } else {
    klog_status_fail(label);
  }
}
#endif
