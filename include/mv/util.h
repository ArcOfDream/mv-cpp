#include <math.h>

#define deg_to_rad(val) ((val) * M_PI / 180.0)
#define rad_to_deg(val) ((val) * 180.0 / M_PI)

#define DEFAULT_UPDATE(class_name) void class_name::_update(double dt) {\
    for (auto &child : children) {                                      \
        child->_update(dt);                                             \
    }                                                                   \
                                                                        \
    if (wren_constructed) wren_update(this, dt);                        \
    else update(dt);                                                    \
}
