#include <kcheck.h>
#include <logger.h>
#include <helper.h>

void kcheck(int condition, char *label)
{
  MARK_PARAM_USED(label);

  if (condition) {
    klog_status_ok(label);
  } else {
    klog_status_fail(label);
  }
}
