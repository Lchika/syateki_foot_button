set(srcs 
    "util/font/comic24.c"
    "util/font/def_small.c"
    "util/font/default_tiny_9pt.c"
    "util/font/DefaultFont.c"
    "util/font/DejaVuSans18.c"
    "util/font/DejaVuSans24.c"
    "util/font/minya24.c"
    "util/font/SmallFont.c"
    "util/font/tooney32.c"
    "util/font/Ubuntu16.c"
    "util/AXP192.c"
    "util/button.c"
    "util/i2smic.c"
    "util/MPU6886.c"
    "util/spi_master_lobo.c"
    "util/tft.c"
    "util/tftspi.c"
    "util/wire.c"
    "m5stickc.c")

set(include_dirs "include" "util/include")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "${include_dirs}")