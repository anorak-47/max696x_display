#ifndef QUEUE_H_
#define QUEUE_H_

#include <inttypes.h>
#include <stdbool.h>

#define CMD_QUEUE_SIZE 4 // muss 2^n betragen (2, 4, 8, 16, 32, 64 ...)
#define CMD_PAYLOAD_SIZE 32

struct _cmd_queue_data_t
{
	uint8_t cmd;
    uint8_t data[CMD_PAYLOAD_SIZE];
    uint8_t data_length;
};

typedef struct _cmd_queue_data_t cmd_queue_data_t;

bool cmd_enqueue_data(uint8_t cmd, uint8_t const *data, uint8_t data_length);
bool cmd_get_queue_head(cmd_queue_data_t **head);
bool cmd_get_queue_next(cmd_queue_data_t **data);
bool cmd_get_queue_tail(cmd_queue_data_t **tail);
bool cmd_dequeue_head(cmd_queue_data_t **head);

#endif /* QUEUE_H_ */
