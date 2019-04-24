rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
TARGET := pcsx-redux

CXXFLAGS := -std=c++2a
CPPFLAGS += -Isrc
CPPFLAGS += -Ithird_party
CPPFLAGS += -O0
CPPFLAGS += -g

LDFLAGS += -lstdc++fs
LDFLAGS += -g

LD := $(CXX)

SRC_CC := $(call rwildcard,src/,*.cc)
SRC_CPP := $(wildcard third_party/imgui/*.cpp)
OBJECTS := $(patsubst %.cc,%.o,$(SRC_CC))

all: dep $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cc
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

%.dep: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -M -MT $(addsuffix .o, $(basename $@)) -MF $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPS)

gitclean:
	git clean -f -d -x
	git submodule foreach --recursive git clean -f -d -x

DEPS := $(patsubst %.cc,%.dep,$(SRC_CC))

dep: $(DEPS)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), gitclean)
-include $(DEPS)
endif
endif
