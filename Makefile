NAME := portofoliu
FILE := ${NAME}.tex

default: build

build:
	pdflatex --shell-escape ${FILE}

open:
	evince ${NAME}.pdf&

clean:
	rm ${NAME}.aux
	rm ${NAME}.log
