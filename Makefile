NAME := portofoliu
FILE := ${NAME}.tex

default: build

build:
	pdflatex --shell-escape ${FILE}

clean:
	rm -f ${NAME}.aux
	rm -f ${NAME}.log
	rm -f ${NAME}.toc
	rm -f ${NAME}.out
