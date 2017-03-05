TARGET_LIB = libvita3d.a

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

INCLUDES   = include


SRC_C :=$(call rwildcard, src/, *.c)
SRC_CPP :=$(call rwildcard, src/, *.cpp)
SRC_GXP :=$(call rwildcard, shaders/, *.gxp)

OBJ_DIRS := $(addprefix out/, $(dir $(SRC_C:src/%.c=%.o))) $(addprefix out/, $(dir $(SRC_CPP:src/%.cpp=%.o))) $(addprefix out/, $(dir $(SRC_GXP:shaders/%.gxp=%.o)))	
OBJS := $(addprefix out/, $(SRC_C:src/%.c=%.o)) $(addprefix out/, $(SRC_CPP:src/%.cpp=%.o)) $(addprefix out/, $(SRC_GXP:shaders/%.gxp=%.o))
	
PREFIX  ?= ${VITASDK}/arm-vita-eabi
CC      = arm-vita-eabi-gcc
CXX		= arm-vita-eabi-g++
AR      = arm-vita-eabi-ar
STRIP := arm-vita-eabi-strip
CFLAGS  = -Wl,-q -Wall -O3 -I$(INCLUDES) -I$(VITASDK)/arm-vita-eabi/include/freetype2 -ffat-lto-objects -flto
CXXFLAGS = -Wl,-q -Wall -std=c++11 -I$(INCLUDES) -I$(VITASDK)/arm-vita-eabi/include/freetype2 -ffat-lto-objects -flto
ASFLAGS = $(CXXFLAGS)

all: $(TARGET_LIB)

debug: CFLAGS += -DDEBUG_BUILD
debug: all

$(TARGET_LIB): $(OBJS)
	$(AR) -rc $@ $^	

$(OBJ_DIRS):
	mkdir -p $@

out/%.o : shaders/%.gxp | $(OBJ_DIRS)
	arm-vita-eabi-ld -r -b binary -o $@ $^

out/%.o : src/%.cpp | $(OBJ_DIRS)
	arm-vita-eabi-g++ $(CXXFLAGS) -c -o $@ $<	

clean:
	rm -rf $(TARGET_LIB) $(OBJS)

install: $(TARGET_LIB)
	@mkdir -p $(DESTDIR)$(PREFIX)/lib/
	cp $(TARGET_LIB) $(DESTDIR)$(PREFIX)/lib/
	@mkdir -p $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/include/Vita3DMath
	cp include/Vita3D.hpp $(DESTDIR)$(PREFIX)/include/
	cp include/Vita3DMath/Vita3DMath.hpp $(DESTDIR)$(PREFIX)/include/Vita3DMath/
	cp include/Vita3DMath/Math.hpp $(DESTDIR)$(PREFIX)/include/Vita3DMath/
	cp include/Vita3DMath/Vector.hpp $(DESTDIR)$(PREFIX)/include/Vita3DMath/
	cp include/Vita3DMath/Transform.hpp $(DESTDIR)$(PREFIX)/include/Vita3DMath/
	cp include/Vita3DMath/Matrix.hpp $(DESTDIR)$(PREFIX)/include/Vita3DMath/
	cp include/Vita3DMath/Quaternion.hpp $(DESTDIR)$(PREFIX)/include/Vita3DMath/

re: clean all

rei: re install