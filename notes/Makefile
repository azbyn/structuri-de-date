NAME := main
FILE := ${NAME}.tex

#CPP_SRC := src
#CPP_BUILD := cpp_latex
#CPP_FILES := \
#	$(wildcard ${CPP_SRC}/*.h) \
#	$(wildcard ${CPP_SRC}/*.cpp)
#
#LATEX_FILES := $(CPP_FILES:%=$(CPP_BUILD)/%.tex)

default: build

build:
	pdflatex --shell-escape ${FILE}

#mount:
#	mount -t tmpfs -o size=5m tmpfs ./cpp_latex

#build_cpp: $(LATEX_FILES)
#
#$(CPP_BUILD)/%.cpp.tex: %.cpp
#	@$(MKDIR_P) $(dir $@)
#	./cpp_to_latex $< > $@
#
#$(CPP_BUILD)/%.h.tex: %.h
#	@$(MKDIR_P) $(dir $@)
#	./cpp_to_latex $< > $@
#
clean:
	rm -f ${NAME}.aux
	rm -f ${NAME}.log
	rm -f ${NAME}.toc
	rm -f ${NAME}.out
#	rm -rf ${CPP_BUILD}

MKDIR_P ?= mkdir -p
