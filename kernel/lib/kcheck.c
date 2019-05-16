#include <kcheck.h>
#include <logger.h>
#include <helper.h>

void kcheck(int condition, char *label)
{
  MARK_PARAM_USED(label);

  if (condition) {
    klog(LOG_INFO, "passed check: %s\n", label);
  } else {
    klog(LOG_ERROR, "failed check: %s\n", label);
  }
}
