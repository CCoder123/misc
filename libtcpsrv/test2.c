#include <stdio.h>
#include "libtcpsrv.h"

typedef struct {
  int fd;
} slot_t;

char *addr(struct sockaddr_in6 *sa) {
  char buf[50];
  int rc = inet_ntop(AF_INET6, &sa->sin6_addr, buf, sizeof(buf));
  return rc ? buf : "?";
}

void greet(void *_slot, int fd, struct sockaddr_in6 *sa, void *data, int *flags) {
  char buf[] = "welcome (" __FILE__ ")\n";
  slot_t *slot = (slot_t*)_slot;
  fprintf(stderr,"accepting fd %d in main thread from %s\n", fd, addr(sa));
  write(fd, buf, sizeof(buf));
  *flags |= TCPSRV_DO_CLOSE;
}

tcpsrv_init_t parms = {
  .verbose=1,
  .nthread=2,
  .maxfd = 13,
  .timeout = 10,
  .port = 1099,
  .sz = sizeof(slot_t),
  .on_accept = greet, 
};

int main() {
  int rc=-1;
  void *t;

  t=tcpsrv_init(&parms); if (!t) goto done;
  tcpsrv_run(t);
  tcpsrv_fini(t);

 done:
  return rc;
}
