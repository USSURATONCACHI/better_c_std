#ifndef BETTER_C_STD_CAMERA_CAMERA_H_
#define BETTER_C_STD_CAMERA_CAMERA_H_

#include <better_c_std/camera/intgr_exp_value.h>
#include <better_c_std/camera/vec23.h>


// I have written this camera class in my really old Rust project, then 
// dragged it across multiple projects in different languages.
//
// The whole point of this class/struct is that it does not need:
//      - any real-time updating,
//      - threads,
//      - async,
//      - global state.
// + and all calls are O(1).
//
// You only feed it changes as they happen, then just use getters at any point in time later.
// Values will be correctly calculated, even if you leave the struct in exact same state for several hours.
//
// It works really well for me. It uses exponentially decaying speeds.
// Not gonna bother explaining all the exponential and logarithmic math behind it, btw.

typedef struct {
    // All these values only update when you call setters or event functions (`MgCameraModel_on_` and `_update_anim`).
    // These are not real-time. For real-time values, use getter functions.
    BcstdIntgrExpValue x;
    BcstdIntgrExpValue y;
    BcstdIntgrExpValue zoom;       // base-2 logarithm of unit cell size

    BcstdTime last_drag_time;
    BcstdVec2 drag_sensitivity;    // Default = { 1, 1 }
    BcstdVec2 drag_vel_multiplier; // Default = { 1, 1 }
    BcstdVec2 drag_cumulative_vel;

    long double zoom_sensitivity;

    struct {
        BcstdTime (*fn)(void* arg);
        void* arg;
    } time_getter;
} BcstdCamera;

// How much velocity drops each second
#define BcstdCamera_DEFAULT_EXPONENT 0.001

BcstdCamera BcstdCamera_create();

// Getters
// -- Returns BcstdVec3 { x, y, zoom };
BcstdVec3 BcstdCamera_get_pos(const BcstdCamera* self);
BcstdVec3 BcstdCamera_get_vel(const BcstdCamera* self);

BcstdTime BcstdCamera_get_time(const BcstdCamera* self);

long double BcstdCamera_get_x(const BcstdCamera* self);
long double BcstdCamera_get_y(const BcstdCamera* self);
long double BcstdCamera_get_zoom(const BcstdCamera* self);
long double BcstdCamera_get_unit_size(const BcstdCamera* self);

long double BcstdCamera_get_vel_x(const BcstdCamera* self);
long double BcstdCamera_get_vel_y(const BcstdCamera* self);
long double BcstdCamera_get_vel_zoom(const BcstdCamera* self);

// Setters/Modifiers
void BcstdCamera_set_pos(BcstdCamera* self, BcstdVec2 pos);
void BcstdCamera_set_vel(BcstdCamera* self, BcstdVec2 vel);

void BcstdCamera_set_x(BcstdCamera* self, long double x);
void BcstdCamera_set_y(BcstdCamera* self, long double y);
void BcstdCamera_set_zoom(BcstdCamera* self, long double zoom);
void BcstdCamera_set_zoom_by_unit_size(BcstdCamera* self, long double unit_size);

void BcstdCamera_set_vel_x(BcstdCamera* self, long double vx);
void BcstdCamera_set_vel_y(BcstdCamera* self, long double vy);
void BcstdCamera_set_vel_zoom(BcstdCamera* self, long double vzoom);

// Events
void BcstdCamera_update_origin(BcstdCamera* self);
void BcstdCamera_on_drag_start(BcstdCamera* self);
void BcstdCamera_on_drag_end(BcstdCamera* self);
void BcstdCamera_on_drag(BcstdCamera* self, BcstdVec2 diff_pos_units);
void BcstdCamera_on_zoom(BcstdCamera* self, long double delta);


#endif // BETTER_C_STD_CAMERA_CAMERA_H_