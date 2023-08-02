NAME           := savethem

#------------------------------------------------

BUILD          := build
SOURCE         := source/

#------------------------------------------------

SOURCES        := $(shell find $(SOURCE) -maxdepth 2 -name '*.c')
OBJECTS        := $(addprefix $(BUILD)/,$(SOURCES:%.c=%.o))

#-------------------------------------------------

CFLAGS         += -Wall -g -fPIC 
LDFLAGS        := -lm -lraylib

#-------------------------------------------------

OUTPUT_BOLD    := `tput bold`
OUTPUT_GREEN   := `tput setaf 2`
OUTPUT_BLUE    := `tput setaf 6`
OUTPUT_NORMAL  := `tput sgr0`

#-------------------------------------------------

# Emscripten, web
ifeq ($(PLATFORM),WEB)
CFLAGS 		   += -DPLATFORM_WEB -s USE_GLFW=3 -sALLOW_MEMORY_GROWTH --preload-file assets
NAME 		   := $(NAME).js
CC 			   := emcc
endif

# Linux, 32-bit
ifeq ($(PLATFORM),LINUX32)
CFLAGS         += -m32
endif

# Windows, 32-bit
ifeq ($(PLATFORM),WINDOWS32)
CC 			   := i686-w64-mingw32-gcc
endif

# Windows, 64-bit
ifeq ($(PLATFORM),WINDOWS64)
CC 			   := x86_64-w64-mingw32-gcc
endif

#-------------------------------------------------
all: $(NAME)
#-------------------------------------------------

#-------------------------------------------------
$(NAME): $(OBJECTS)
#-------------------------------------------------
	@echo Linking...
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LDFLAGS)
	@echo $(OUTPUT_BOLD)$(OUTPUT_GREEN)Build done.$(OUTPUT_NORMAL)

#-------------------------------------------------
$(BUILD)/%.o: %.c
#-------------------------------------------------
	@echo $(OUTPUT_BOLD)- $(subst $(SOURCE)/,,$<)$(OUTPUT_NORMAL)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(dir $<) -c $< -o $@

#-------------------------------------------------
clean:
#-------------------------------------------------
	@echo $(OUTPUT_BLUE)$(OUTPUT_BOLD)Cleaning...$(OUTPUT_NORMAL)
	@rm -rf $(BUILD) $(NAME)
