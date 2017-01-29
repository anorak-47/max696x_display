#include "command_queue.h"
#include "device_configuration.h"
#include <string.h>
#include <util/atomic.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_COMMAND_QUEUE_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

#define QUEUE_MASK (CMD_QUEUE_SIZE - 1) // Klammern auf keinen Fall vergessen

static struct _queue_t
{
    cmd_queue_data_t data[CMD_QUEUE_SIZE];
    uint8_t read;  // zeigt auf das Feld mit dem ältesten Inhalt
    uint8_t write; // zeigt immer auf leeres Feld
    uint8_t pos;
} command_queue = {{}, 0, 0};

bool cmd_enqueue_data(uint8_t cmd, uint8_t const *data, uint8_t data_length)
{
    LS_("cmd_enqueue_data");
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        uint8_t next = ((command_queue.write + 1) & QUEUE_MASK);
        if (command_queue.read == next)
            return false;

        command_queue.data[command_queue.write].cmd = cmd;
        // queue.data[queue.write & QUEUE_MASK] = byte; // absolut Sicher

        if (data != 0 && data_length > 0 && data_length <= CMD_PAYLOAD_SIZE)
        {
            memcpy(command_queue.data[command_queue.write].data, data, data_length);
            command_queue.data[command_queue.write].data_length = data_length;
        }
        else
        {
            command_queue.data[command_queue.write].data_length = 0;
        }

        command_queue.write = next;
    }
    return true;
}

bool cmd_get_queue_head(cmd_queue_data_t **head)
{
    LS_("cmd_get_queue_head");
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (command_queue.read == command_queue.write)
        {
            *head = 0;
            return false;
        }
        *head = &(command_queue.data[command_queue.read]);
        command_queue.pos = command_queue.read;
    }
    return true;
}

bool cmd_get_queue_next(cmd_queue_data_t **data)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        uint8_t next = ((command_queue.pos + 1) & QUEUE_MASK);

        if (next == command_queue.write)
        {
            *data = 0;
            return false;
        }

        *data = &(command_queue.data[next]);
        command_queue.pos = next;
    }
    return true;
}

bool cmd_get_queue_tail(cmd_queue_data_t **tail)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (command_queue.read == command_queue.write)
        {
            *tail = 0;
            return false;
        }

        *tail = &(command_queue.data[((command_queue.write - 1) & QUEUE_MASK)]);
    }
    return true;
}

bool cmd_dequeue_head(cmd_queue_data_t **head)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (command_queue.read == command_queue.write)
        {
            *head = 0;
            return false;
        }

        if (head != 0)
            *head = &(command_queue.data[command_queue.read]);
        command_queue.read = (command_queue.read + 1) & QUEUE_MASK;
    }
    return true;
}
