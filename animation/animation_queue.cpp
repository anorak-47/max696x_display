#include "animation_queue.h"
#include "../device_configuration.h"
#include <string.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_ANIMATION_QUEUE_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

#define QUEUE_MASK (ANIMATION_QUEUE_SIZE - 1) // Klammern auf keinen Fall vergessen

static struct _queue_t
{
    animation_queue_data_t data[ANIMATION_QUEUE_SIZE];
    uint8_t read;  // zeigt auf das Feld mit dem ältesten Inhalt
    uint8_t write; // zeigt immer auf leeres Feld
    uint8_t pos;
} animation_queue = {{}, 0, 0};

bool animation_enqueue_data(animation_queue_data_t const *data)
{
    uint8_t next = ((animation_queue.write + 1) & QUEUE_MASK);
    if (animation_queue.read == next)
        return false;

    LS_("ani_enqueue_data");

    memcpy(&animation_queue.data[animation_queue.write], data, sizeof(animation_queue_data_t));

    // queue.data[queue.write & QUEUE_MASK] = byte; // absolut Sicher

    animation_queue.write = next;
    return true;
}

bool animation_get_queue_head(animation_queue_data_t **head)
{
    if (animation_queue.read == animation_queue.write)
    {
        *head = 0;
        return false;
    }
    LS_("ani_get_queue_head");
    *head = &(animation_queue.data[animation_queue.read]);
    animation_queue.pos = animation_queue.read;
    return true;
}

bool animation_get_queue_next(animation_queue_data_t **data)
{
    uint8_t next = ((animation_queue.pos + 1) & QUEUE_MASK);

    if (next == animation_queue.write)
    {
        *data = 0;
        return false;
    }

    *data = &(animation_queue.data[next]);
    animation_queue.pos = next;
    return true;
}

bool animation_get_queue_tail(animation_queue_data_t **tail)
{
    if (animation_queue.read == animation_queue.write)
    {
        *tail = 0;
        return false;
    }

    *tail = &(animation_queue.data[((animation_queue.write - 1) & QUEUE_MASK)]);
    return true;
}

bool animation_dequeue_head(animation_queue_data_t **head)
{
    if (animation_queue.read == animation_queue.write)
    {
        *head = 0;
        return false;
    }

    if (head != 0)
        *head = &(animation_queue.data[animation_queue.read]);
    animation_queue.read = (animation_queue.read + 1) & QUEUE_MASK;
    return true;
}
