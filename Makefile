# @file make build automation tool to generate a target
# @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz

export LD_LIBRARY_PATH=/usr/local/share/Qt-5.5.1/5.5/gcc_64/lib:$LD_LIBRARY_PATH

ifeq ($(wildcard /usr/local/share/Qt-5.5.1/5.5/gcc_64/bin/qmake),)
    QMAKE=qmake
else
	QMAKE=/usr/local/share/Qt-5.5.1/5.5/gcc_64/bin/qmake
endif

all: blockeditor

blockeditor:
	mkdir src/tmpMakeDir
	@cd src/tmpMakeDir && $(QMAKE) .. -o Makefile && make
	mv src/tmpMakeDir/ICP_proj ./
	mv ICP_proj blockeditor
	rm -rf src/tmpMakeDir

run:
	./blockeditor

doxygen:
	@cd ./doc && doxygen

pack:
	zip xkelem01_xlakat01-AA-BB src/* doc/* examples/* README.txt Makefile

clean:
	rm -rf src/tmpMakeDir/* blockeditor xkelem01_xlakat01-AA-BB.zip doc/doc
