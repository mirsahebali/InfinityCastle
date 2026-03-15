#ifndef UTILS_H
#define UTILS_H

#define VEC2(x_val, y_val) ((Vector2){.x = (float)(x_val), .y = (float)(y_val)})
#define VEC3(x_val, y_val, z_val) ((Vector3){.x = (float)(x_val), .y = (float)(y_val), .z = (float)(z_val)})
#define VEC4(x_val, y_val, z_val, w_val)                                                                               \
    ((Vector4){.x = (float)(x_val), .y = (float)(y_val), .z = (float)(z_val), .w = (float)(w_val)})

#define TO_RADIANS(deg) ((PI / 180) * deg)

#endif // !UTILS_H
