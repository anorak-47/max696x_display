
#include "animation.h"
#include "../commands.h"
#include "../device_configuration.h"
#include "../global.h"
#include "../timer.h"
#include "animation_utils.h"
#include "box.h"
#include "scroll_text.h"
#include "show_text.h"
#include "sweep.h"
#include "typematrix.h"
#include <limits.h>
#include <string.h>

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_ANIMATION_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "../debug.h"

static uint8_t current_animation = MATRIX_FIRST_ANIMATION;
static int8_t info_text_timer = -1;

void cb_animate(void *);

void set_animation_show_text()
{
    LS_("show_text");
    animation.animationStart = 0;
    animation.animationStop = 0;
    animation.animationLoop = &show_text;
}

void set_animation_scroll_text()
{
    LS_("scroll_text");
    animation.animationStart = &start_scroll_text;
    animation.animationStop = &stop_scroll_text;
    animation.animationLoop = &show_scroll_text;
}

void set_animation_sweep()
{
    LS_("sweep");
    animation.animationStart = &start_sweep;
    animation.animationStop = &stop_sweep;
    animation.animationLoop = &show_sweep;
}

void set_animation_box()
{
    LS_("box");
    animation.animationStart = &start_box;
    animation.animationStop = &stop_box;
    animation.animationLoop = &show_box;
}

void set_animation_typematrix()
{
    LS_("typematrix");
    animation.animationStart = &start_typematrix;
    animation.animationStop = &stop_typematrix;
    animation.animationLoop = &show_typematrix;
}

bool set_animation(uint8_t animation_number)
{
    bool found = true;
    switch (animation_number | MATRIX_CMD_ANIMATION)
    {
    case MATRIX_CMD_ANIMATE_SWEEP:
        set_animation_sweep();
        break;
    case MATRIX_CMD_ANIMATE_BOX:
        set_animation_box();
        break;
    case MATRIX_CMD_ANIMATE_SHOW_TEXT:
        set_animation_show_text();
        break;
    case MATRIX_CMD_ANIMATE_SCROLL_TEXT:
        set_animation_scroll_text();
        break;
    case MATRIX_CMD_ANIMATE_TYPEMATRIX:
        set_animation_typematrix();
        break;
    default:
        found = false;
        break;
    }

    if (found)
        current_animation = animation_number | MATRIX_CMD_ANIMATION;

    return found;
}

void animation_next()
{
    current_animation++;
    if (current_animation > MATRIX_LAST_ANIMATION)
        current_animation = MATRIX_FIRST_ANIMATION;

    LV_("animation_next: %u", current_animation);

    if (!animation_is_running())
        return;

    stop_animation();

    set_animation(current_animation);
    start_animation();
}

void animation_previous()
{
    if (current_animation == MATRIX_FIRST_ANIMATION)
        current_animation = MATRIX_LAST_ANIMATION;
    else
        current_animation--;

    LV_("animation_previous: %u", current_animation);

    if (!animation_is_running())
        return;

    stop_animation();

    set_animation(current_animation);
    start_animation();
}

void animation_increase_speed(void)
{
    uint16_t speed = animation.delay_in_ms;

    if (speed > MATRIX_ANIMATION_MIN_SPEED)
        speed -= MATRIX_ANIMATION_CHANGE_SPEED;
    else
        speed = MATRIX_ANIMATION_MIN_SPEED;

    animation_set_speed(speed);
}

void animation_decrease_speed(void)
{
    animation_set_speed(animation.delay_in_ms + MATRIX_ANIMATION_CHANGE_SPEED);
}

void animation_set_speed(uint16_t speed)
{
    animation.delay_in_ms = speed;

    if (speed < MATRIX_ANIMATION_MIN_SPEED)
        animation.delay_in_ms = MATRIX_ANIMATION_MIN_SPEED;

    global.state.animation_speed = animation.delay_in_ms / 10;

    if (animation_is_running())
    {
        LV_("animation_set_speed %u", speed);
        delTimer(animation.loop_timer);
        animation.loop_timer = addLoopTimer(animation.delay_in_ms, cb_animate, 0);
    }
}

void animation_set_typematrix_key(uint8_t row_number, matrix_row_t row)
{
    if (current_animation != MATRIX_CMD_ANIMATE_TYPEMATRIX)
        return;

    LS_("set_typematrix_key");

    update_typematrix(row_number, row);
}

void animation_set_lock_state(uint8_t locks)
{
    LS_("set_lock_state");
}

void cb_info_text(void *)
{
	LS_("cb_info_text");

	delTimer(info_text_timer);
	info_text_timer = -1;

	if (!animation_is_running())
		clean_matrix_display();

	continue_animation();
}

void animation_show_info_text(animation_queue_data_t const *data)
{
    LV_("show_info_text [%s]", (char const *)(data->data));

    pause_animation();

    delTimer(info_text_timer);
    info_text_timer = addLoopTimer(5000, cb_info_text, 0);

    show_text(data);
}

void animation_toggle(void)
{
    LS_("animation_toggle");

    if (animation_is_running())
    {
        stop_animation();
    }
    else
    {
        set_animation(current_animation);
        start_animation();
    }
}

bool animation_is_running()
{
    return (animation.is_running);
}

void pause_animation()
{
    if (!animation_is_running())
        return;

    if (!animation.is_paused)
    {
    	LS_("pause_animation");
        animation.is_paused = true;
        global.state.animation_state = 2;
        pauseTimer(animation.loop_timer);
        pauseTimer(animation.duration_timer);
    }
}

void continue_animation()
{
    if (!animation_is_running())
        return;

    if (animation.is_paused)
    {
        LS_("continue_animation");
    	animation.is_paused = false;
        global.state.animation_state = 1;
        continueTimer(animation.loop_timer);
        continueTimer(animation.duration_timer);
    }
}

void stop_animation()
{
    LS_("stop_animation");

    delTimer(animation.loop_timer);
    animation.loop_timer = -1;

    delTimer(animation.duration_timer);
    animation.duration_timer = -1;

    if (animation.animationStop)
        animation.animationStop(&animation.data);

    animation.is_running = false;
    global.state.animation_state = 0;
}

void cb_animate(void *)
{
    if (animation.animationLoop == 0)
        return;

    animation.animationLoop(&animation.data);
}

void cb_animate_stop(void *)
{
    stop_animation();
}

void start_animation()
{
    LV_("start_animation %u", animation.data.animation);

	delTimer(info_text_timer);
	info_text_timer = -1;

    animation.is_running = true;
    animation.is_paused = false;
    global.state.animation = animation.data.animation;
    global.state.animation_state = 1;

    if (animation.animationStart)
        animation.animationStart(&animation.data);

    if (animation.duration_in_s == 0 && animation.animationLoop)
    {
        animation.animationLoop(&animation.data);
        animation.is_running = false;
        animation.is_paused = false;
        global.state.animation_state = 0;
    }
    else if (animation.delay_in_ms && animation.data.animation)
    {
        LS_("start timers");
        animation.loop_timer = addLoopTimer(animation.delay_in_ms, cb_animate, 0);

        if (animation.duration_in_s < UCHAR_MAX)
            animation.duration_timer = addLongTimer(animation.duration_in_s, 0, cb_animate_stop, 0);
    }
    else
    {
        LS_("FAILED");
        animation.is_running = false;
        animation.is_paused = false;
        global.state.animation_state = 0;
    }
}

uint16_t get_delay_in_ms(animation_queue_data_t *data)
{
    uint16_t delay_in_ms = data->speed;
    delay_in_ms *= 10;

    if (animation.data.animation == MATRIX_CMD_ANIMATE_TYPEMATRIX)
        delay_in_ms = 1000 * data->speed;

    return delay_in_ms;
}

uint8_t get_duration_in_s(animation_queue_data_t *data)
{
    return data->duration;
}

bool handle_animation(animation_queue_data_t *head)
{
    if (animation_is_running())
    {
        return false;
    }

    memcpy(&animation.data, head, sizeof(animation_queue_data_t));

    LS_("handle_animation");
    LV_("ani:%u 0x%X", (animation.data.animation & ~0x80), animation.data.animation);
    LV_("dir:%u", animation.data.direction);
    LV_("dur:%u", animation.data.duration);
    LV_("spe:%u", animation.data.speed);
    LV_("col:%u", animation.data.color);
    LV_("fnt:%u", animation.data.font);
    LV_("len:%u", animation.data.data_length);
    LV_("str:%s", (char *)animation.data.data);

    if (animation.data.speed == 0)
    {
        animation.data.speed = global.state.animation_speed;
    }

    global.state.animation_speed = animation.data.speed;

    animation.delay_in_ms = get_delay_in_ms(head);
    animation.duration_in_s = get_duration_in_s(head);

    LV_("dly:%u", animation.delay_in_ms);

    if (set_animation(animation.data.animation))
    {
        start_animation();
    }

    return true;
}

void initAnimation()
{
    memset((void *)&animation, 0, sizeof(animation_interface));

    current_animation = MATRIX_FIRST_ANIMATION;
    global.state.animation = MATRIX_FIRST_ANIMATION;

    animation.data.animation = MATRIX_FIRST_ANIMATION;
    animation.data.direction = MATRIX_ANIMATION_DIRECTION_LEFT;
    animation.data.duration = MATRIX_ANIMATION_RUN_FOREVER;
    animation.data.speed = 3;
    animation.data.color = 3;
    animation.data.font = 0;

    global.state.animation_speed = animation.data.speed;

    animation.delay_in_ms = animation.data.speed;
    animation.delay_in_ms *= 10;
    animation.duration_in_s = animation.data.duration;

    animation.duration_timer = -1;
    animation.loop_timer = -1;
}

void animationMainLoop()
{
    static animation_queue_data_t *head;

    if (animation_get_queue_head(&head))
    {
        if (handle_animation(head))
        {
            animation_dequeue_head(&head);
        }
    }
}
