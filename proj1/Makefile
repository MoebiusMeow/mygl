# project 1

SHELL = /bin/bash

Output := proj1
LibDir := ./lib
XLibs := X11, Xxf86vm, Xrandr, Xi, Xcursor, Xinerama
Libs := GLEW, glfw3, GL, ${XLibs}, dl, pthread
IncludeDir := ../include;./include
ShaderDir := ./shaders
ExCommands := -g -Wall -std=gnu++11 -static
Libs := -l$(Libs:,= -l)
Cpps := $(wildcard *.cpp)
Objs := $(Cpps:.cpp=.o)
Deps := $(wildcard *.d)
Sdrs := $(wildcard $(ShaderDir)/*.sdr)
IncludeDir := $(subst ;, -I,$(IncludeDir))

ColorRed  = \033[01;33m
ColorBlue = \033[01;34m
ColorCyan = \033[01;36m
ColorNorm = \033[0m

all : 
	@echo $(IncludeDir)
	@rm -f Output
	@rm -f $(Objs)
	@make Output

ifneq ($(strip $(Deps)),0)
include $(Deps)
endif

Output : 
	@make $(ShaderDir)/shaderSrc.h
	@make $(Objs)
	@echo -e "$(ColorBlue) Linking $(notdir $<) $(ColorBlue)"
	@g++ -L$(LibDir) -o "$(Output)" $(Objs) $(Libs) -static-libgcc -static-libstdc++ -Wl,-Bstatic -Wl,--as-needed
	@echo -e "$(ColorBlue) Finished linking $(notdir $<) $(ColorNorm)"

$(ShaderDir)/shaderSrc.h : $(Sdrs) 
	@echo -e "$(ColorBlue) Packing up shaders $(ColorCyan)"
	@cp $(ShaderDir)/shaderSrc.h $(ShaderDir)/shaderSrc.h.tmp
	@$(ShaderDir)/pack.py $^
	@cp $(ShaderDir)/shaderSrc.h.tmp $(ShaderDir)/shaderSrc.h 
	@rm $(ShaderDir)/shaderSrc.h.tmp
	@echo -e "$(ColorBlue) Finished packing $(ColorNorm)"

$(Objs) : %.o : %.cpp
	@echo -e "$(ColorBlue) Building $(notdir $<) -> $(notdir $@) $(ColorNorm)"
	@g++ -I$(IncludeDir) -c -MMD -MP -MF"$(@:.o=.d)" -MT"$(@)" -o "$@" "$<" $(ExCommands)
	@echo -e "$(ColorBlue) Finished building $(notdir $<) -> $(notdir $@) $(ColorNorm)"

clean : 
	rm -f *.o
	rm ${Output}

.PHONY : all clean
