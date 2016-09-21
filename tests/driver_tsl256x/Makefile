APPLICATION = driver_tsl256x
include ../Makefile.tests_common

FEATURES_REQUIRED = periph_i2c

USEMODULE += tsl256x
USEMODULE += xtimer

# set default device parameters in case they are undefined
TEST_TSL256X_I2C ?= I2C_0

# export parameters
CFLAGS += -DTEST_TSL256X_I2C=$(TEST_TSL256X_I2C)
CFLAGS += -DTSL256X_I2C_ADDRESS_FLOAT

include $(RIOTBASE)/Makefile.include
