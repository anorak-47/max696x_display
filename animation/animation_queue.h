#ifndef ANIMATION_QUEUE_H_
#define ANIMATION_QUEUE_H_

#include <inttypes.h>
#include <stdbool.h>

#define ANIMATION_PAYLOAD_SIZE 24
#define ANIMATION_QUEUE_SIZE 8 // muss 2^n betragen (8, 16, 32, 64 ...)

struct _animation_queue_data_t
{
	uint8_t animation;
	uint8_t speed;
	uint8_t direction;
	uint8_t duration;
	uint8_t color;
	uint8_t font;
	uint8_t data_length;
	uint8_t data[ANIMATION_PAYLOAD_SIZE];
};

typedef struct _animation_queue_data_t animation_queue_data_t;

bool animation_enqueue_data(animation_queue_data_t const *data);
bool animation_get_queue_head(animation_queue_data_t **head);
bool animation_get_queue_next(animation_queue_data_t **data);
bool animation_get_queue_tail(animation_queue_data_t **tail);
bool animation_dequeue_head(animation_queue_data_t **head);

#endif /* QUEUE_H_ */
