#include <better_c_std/camera/camera.h>
#include <better_c_std/prettify/debug.h>
#include <math.h>
#include <float.h>

BcstdCamera BcstdCamera_create() {
    return (BcstdCamera) {
        .x = BcstdIntgrExpValue_from_exponent(BcstdCamera_DEFAULT_EXPONENT),
        .y = BcstdIntgrExpValue_from_exponent(BcstdCamera_DEFAULT_EXPONENT),
        .zoom = BcstdIntgrExpValue_from_exponent(BcstdCamera_DEFAULT_EXPONENT),

        .last_drag_time = {}, // can be uninit
        .drag_sensitivity = { 1, 1 },
        .drag_vel_multiplier = { 1, 1 },
        .drag_cumulative_vel = { 0, 0 },

        .zoom_sensitivity = 1.0,

        .time_getter = {
            .fn = BcstdTime_now_unix,
            // arg can be uninit
        },
    };
}

// Getters
// -- Returns BcstdVec3 { x, y, zoom };
BcstdVec3 BcstdCamera_get_pos(const BcstdCamera* self) {
    BcstdTime now = BcstdCamera_get_time(self);

    return (BcstdVec3) {
        .x = BcstdIntgrExpValue_get_pos(self->x, now),
        .y = BcstdIntgrExpValue_get_pos(self->y, now),
        .z = BcstdIntgrExpValue_get_pos(self->zoom, now),
    };
}
BcstdVec3 BcstdCamera_get_vel(const BcstdCamera* self) {
    BcstdTime now = BcstdCamera_get_time(self);

    return (BcstdVec3) {
        .x = BcstdIntgrExpValue_get_vel(self->x, now),
        .y = BcstdIntgrExpValue_get_vel(self->y, now),
        .z = BcstdIntgrExpValue_get_vel(self->zoom, now),
    };
}

BcstdTime BcstdCamera_get_time(const BcstdCamera* self) {
    return self->time_getter.fn(self->time_getter.arg);
}

long double BcstdCamera_get_x(const BcstdCamera* self) {
    return BcstdIntgrExpValue_get_pos(self->x, BcstdCamera_get_time(self));
}
long double BcstdCamera_get_y(const BcstdCamera* self) {
    return BcstdIntgrExpValue_get_pos(self->y, BcstdCamera_get_time(self));
}
long double BcstdCamera_get_zoom(const BcstdCamera* self) {
    return BcstdIntgrExpValue_get_pos(self->zoom, BcstdCamera_get_time(self));
}
long double BcstdCamera_get_unit_size(const BcstdCamera* self) {
    return powl(2.0, BcstdCamera_get_zoom(self)); // more zoom = closer
}

long double BcstdCamera_get_vel_x(const BcstdCamera* self) {
    return BcstdIntgrExpValue_get_vel(self->x, BcstdCamera_get_time(self));
}
long double BcstdCamera_get_vel_y(const BcstdCamera* self) {
    return BcstdIntgrExpValue_get_vel(self->y, BcstdCamera_get_time(self));
}
long double BcstdCamera_get_vel_zoom(const BcstdCamera* self) {
    return BcstdIntgrExpValue_get_vel(self->zoom, BcstdCamera_get_time(self));
}

// Setters/Modifiers
static void update_origin_x(BcstdCamera* self, BcstdTime now) {
    self->x = BcstdIntgrExpValue_update(self->x, now);
}
static void update_origin_y(BcstdCamera* self, BcstdTime now) {
    self->y = BcstdIntgrExpValue_update(self->y, now);
}
static void update_origin_zoom(BcstdCamera* self, BcstdTime now) {
    self->zoom = BcstdIntgrExpValue_update(self->zoom, now);
}


void BcstdCamera_set_pos(BcstdCamera* self, BcstdVec2 pos) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_x(self, now);
    update_origin_y(self, now);
    self->x.x0 = pos.x;
    self->y.x0 = pos.y;

}
void BcstdCamera_set_vel(BcstdCamera* self, BcstdVec2 vel) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_x(self, now);
    update_origin_y(self, now);
    self->x.v0 = vel.x;
    self->y.v0 = vel.y;
}

void BcstdCamera_set_x(BcstdCamera* self, long double x) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_x(self, now);
    self->x.x0 = x;
}
void BcstdCamera_set_y(BcstdCamera* self, long double y) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_y(self, now);
    self->y.x0 = y;
}
void BcstdCamera_set_zoom(BcstdCamera* self, long double zoom) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_zoom(self, now);
    self->zoom.x0 = zoom;
}
void BcstdCamera_set_zoom_by_unit_size(BcstdCamera* self, long double unit_size) {
    BcstdCamera_set_zoom(self, logl(unit_size)); // more size = more zoom
}

void BcstdCamera_set_vel_x(BcstdCamera* self, long double vx) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_x(self, now);
    self->x.v0 = vx;
}
void BcstdCamera_set_vel_y(BcstdCamera* self, long double vy) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_y(self, now);
    self->y.v0 = vy;
}
void BcstdCamera_set_vel_zoom(BcstdCamera* self, long double vz) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_y(self, now);
    self->zoom.v0 = vz;
}

// Events

static void BcstdCamera_update_origin_timed(BcstdCamera* self, BcstdTime now) {
    update_origin_x(self, now);
    update_origin_y(self, now);
    update_origin_zoom(self, now);
}
void BcstdCamera_update_origin(BcstdCamera* self) {
    BcstdCamera_update_origin_timed(self, BcstdCamera_get_time(self));
}

#define CLAMP(a, min, max) (a) < (min) ? (min) : ((a) > (max) ? (max) : (a))
#define MAX_DRAG_TIME_SECS 0.25

void BcstdCamera_on_drag_start(BcstdCamera* self) {
    BcstdTime now = BcstdCamera_get_time(self);

    update_origin_x(self, now);
    update_origin_y(self, now);

    self->x.v0 = 0;
    self->y.v0 = 0;
    self->last_drag_time = now;
    self->drag_cumulative_vel = (BcstdVec2){ 0, 0 };
}

void BcstdCamera_on_drag_end(BcstdCamera* self) {
    BcstdTime now = BcstdCamera_get_time(self);
    BcstdTime elapsed = BcstdTime_diff(now, self->last_drag_time);
    long double elapsed_ld = BcstdTime_to_ldouble_secs_imprecise(elapsed);

    double drag_coef = CLAMP(MAX_DRAG_TIME_SECS - elapsed_ld, 0.0, MAX_DRAG_TIME_SECS) / MAX_DRAG_TIME_SECS;

    self->x.v0 = self->drag_cumulative_vel.x * self->drag_vel_multiplier.x * drag_coef;
    self->y.v0 = self->drag_cumulative_vel.y * self->drag_vel_multiplier.y * drag_coef;
    self->drag_cumulative_vel = (BcstdVec2){ 0, 0 };
    self->x.start_time = now;
    self->y.start_time = now;
}

void BcstdCamera_on_drag(BcstdCamera* self, BcstdVec2 diff_pos_units) {
    diff_pos_units.x *= self->drag_sensitivity.x;
    diff_pos_units.y *= self->drag_sensitivity.y;
    self->x.x0 += diff_pos_units.x;
    self->y.x0 += diff_pos_units.y;

    self->x.x0 = CLAMP(self->x.x0, -LDBL_MAX, LDBL_MAX);
    self->y.x0 = CLAMP(self->y.x0, -LDBL_MAX, LDBL_MAX);

    BcstdTime now = BcstdCamera_get_time(self);
    BcstdTime elapsed = BcstdTime_diff(now, self->last_drag_time);
    long double elapsed_ld = BcstdTime_to_ldouble_secs_imprecise(elapsed);

    if (elapsed_ld < 0.001) 
        elapsed_ld = 0.001;
    self->last_drag_time = now;

    BcstdVec2 this_drag_vel = { 
        .x = diff_pos_units.x / elapsed_ld, 
        .y = diff_pos_units.y / elapsed_ld,  
    };

    if (isnan(this_drag_vel.x)) this_drag_vel.x = 0.0;
    if (isinf(this_drag_vel.x))
        this_drag_vel.x = this_drag_vel.x < 0.0 ? -LDBL_MAX : LDBL_MAX;

    if (isnan(this_drag_vel.y)) this_drag_vel.y = 0.0;
    if (isinf(this_drag_vel.y))
        this_drag_vel.y = this_drag_vel.y < 0.0 ? -LDBL_MAX : LDBL_MAX;

    self->drag_cumulative_vel.x = self->drag_cumulative_vel.x * 0.2f + this_drag_vel.x * 0.8f;
    self->drag_cumulative_vel.y = self->drag_cumulative_vel.y * 0.2f + this_drag_vel.y * 0.8f;
}

void BcstdCamera_on_zoom(BcstdCamera* self, long double delta) {
    BcstdTime now = BcstdCamera_get_time(self);
    update_origin_zoom(self, now);
    self->zoom.v0 += delta * self->zoom_sensitivity;
}