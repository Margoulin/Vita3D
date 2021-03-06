PHONY := all package clean
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

CC := arm-vita-eabi-gcc
CXX := arm-vita-eabi-g++
STRIP := arm-vita-eabi-strip

PROJECT_TITLE := Vita3DSample
PROJECT_TITLEID := MARG00001

PROJECT := Vita3DSample
CXXFLAGS += -std=c++11 -Isrc/include

LIBS := -lVita3d -lSceCtrl_stub -lSceGxm_stub -lSceDisplay_stub -lpng\
	-lSceCommonDialog_stub -lz -lm -lc

SRC_C :=$(call rwildcard, src/, *.c)
SRC_CPP :=$(call rwildcard, src/, *.cpp)

OBJ_DIRS := $(addprefix out/, $(dir $(SRC_C:src/%.c=%.o))) $(addprefix out/, $(dir $(SRC_CPP:src/%.cpp=%.o)))
OBJS := $(addprefix out/, $(SRC_C:src/%.c=%.o)) $(addprefix out/, $(SRC_CPP:src/%.cpp=%.o))

all: package

package: $(PROJECT).vpk

$(PROJECT).vpk: eboot.bin param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin \
		--add sce_sys/icon0.png=sce_sys/icon0.png \
		--add sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		--add sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
		--add Resources/Master.PNG=Resources/Master.PNG \
		--add Resources/Sherlock.bo=Resources/Sherlock.bo \
		--add Resources/arm_dif.png=Resources/arm_dif.png \
		--add Resources/arm_showroom_spec.png=Resources/arm_showroom_spec.png \
		--add Resources/glass_dif.png=Resources/glass_dif.png \
		--add Resources/body_dif.png=Resources/body_dif.png \
		--add Resources/body_showroom_spec.png=Resources/body_showroom_spec.png \
		--add Resources/hand_dif.png=Resources/hand_dif.png \
		--add Resources/hand_showroom_spec.png=Resources/hand_showroom_spec.png \
		--add Resources/helmet_diff.png=Resources/helmet_diff.png \
		--add Resources/helmet_showroom_spec.png=Resources/helmet_showroom_spec.png \
		--add Resources/leg_dif.png=Resources/leg_dif.png \
		--add Resources/leg_showroom_spec.png=Resources/leg_showroom_spec.png \
		--add Resources/Uchiyamada.PNG=Resources/Uchiyamada.PNG \
		--add Resources/Nanosuit.bo=Resources/Nanosuit.bo \
		--add Resources/Cube.bo=Resources/Cube.bo \
	$(PROJECT).vpk

eboot.bin: $(PROJECT).velf
	vita-make-fself $(PROJECT).velf eboot.bin

param.sfo:
	vita-mksfoex -s TITLE_ID="$(PROJECT_TITLEID)" "$(PROJECT_TITLE)" param.sfo

$(PROJECT).velf: $(PROJECT).elf
	$(STRIP) -g $<
	vita-elf-create $< $@

$(PROJECT).elf: $(OBJS)
	$(CXX)  -Wl,-q -o $@ $^ $(LIBS)

$(OBJ_DIRS):
	mkdir -p $@

out/%.o : src/%.cpp | $(OBJ_DIRS)
	arm-vita-eabi-g++ $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(PROJECT).velf $(PROJECT).elf $(PROJECT).vpk param.sfo eboot.bin $(OBJS)
	rm -rr $(abspath $(OBJ_DIRS))

re: clean all