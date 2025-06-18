#include <cassert>
#include <stdint.h>

static inline unsigned Fixed_To_Cardinal(unsigned base, unsigned fixed)
{
    uint32_t result = (uint32_t)base * (uint32_t)fixed;
    result += 0x80u;
    if (result & 0xFF000000u)
        return 0xFFFFu;
    return result >> 8;
}

static inline unsigned Cardinal_To_Fixed(unsigned base, unsigned cardinal)
{
    if (base == 0)
        return 0xFFFFu;
    return ((cardinal << 8) / base);
}

int main() {
    assert(Cardinal_To_Fixed(0, 10) == 0xFFFFu);
    assert(Cardinal_To_Fixed(10, 5) == 128u);
    assert(Fixed_To_Cardinal(1000, 128) == 500u);
    assert(Fixed_To_Cardinal(0xFFFFu, 0xFFFFu) == 0xFFFFu);
    return 0;
}
