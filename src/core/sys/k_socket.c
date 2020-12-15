#include "logging.h"
#include "syscall.h"
#include <errno.h>
#include <net/socket.h>
#include <proc/descriptor.h>

int k_socket(int domain, int type, int protocol)
{
  if (domain != AF_INET) {
    return -EAFNOSUPPORT;
  }

  if (type != SOCK_DGRAM) {
    return -ESOCKTNOSUPPORT;
  }

  if (!is_protocol_supported(type, protocol)) {
    return -EPROTONOSUPPORT;
  }

  int sd = create_socket_descriptor(domain, type, protocol);

  if (sd == -1) {
    CORE_SYS_DEBUG("%s", "too many files open");
    return -ENFILE;
  }

  CORE_SYS_DEBUG("open sd=%d", sd);
  return sd;
}
