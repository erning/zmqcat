#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#define BUFSIZE 1024

static int b_send(void *socket, void *buffer, size_t size) {
    int rc;
    zmq_msg_t message;
    zmq_msg_init_size(&message, size);
    memcpy(zmq_msg_data(&message), buffer, size);
    rc = zmq_send(socket, &message, 0);
    zmq_msg_close(&message);
    return(rc);
}

int main(int argc, char **argv) {
    void *context = zmq_init(1);
    void *pub = zmq_socket(context, ZMQ_PUB);

    uint64_t hwm = 10240;
    zmq_setsockopt(pub, ZMQ_HWM, &hwm, sizeof(hwm));

    char *endpoint = argc > 1 ? argv[1] : "tcp://*:8964";
    zmq_bind(pub, endpoint);

    char buf[BUFSIZE];
    char *end = buf + BUFSIZE;

    char *p = buf;
    int ch;
    while (ch = fgetc(stdin)) {
        if (feof(stdin) && p > buf) {
            b_send(pub, buf, p - buf);
            break;
        }
        *p = ch;
        p++;
        if (ch == '\n' || p > end) {
            b_send(pub, buf, p - buf);
            p = buf;
        }
    }

    zmq_close(pub);
    zmq_term(context);
    return 0;
}

