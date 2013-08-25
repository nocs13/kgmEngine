include Makefile.mac

subdirs := $(wildcard kgm*/)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
objects := $(patsubst %.cpp,%.o,$(sources))

OUT_SO = libkgmEngine.so
OUT_A  = libkgmEngine.a

all: $(OUT_SO)
	make -C Test

$(objects) : %.o : %.cpp %.h
	$(CC) $(FLGS) $(DEFS) -DDEBUG -DTEST -c $< -o $@ $(DIRS)

$(OUT_A): $(objects)
	$(AR) -r -c -s $(OUT_A) $(objects)

$(OUT_SO): $(OUT_A)
	$(CC) -shared -o $(OUT_SO) $(OUT_A) $(FLGS) $(DEFS) $(DIRS) $(LIBS) -DDEBUG -DTEST

sdk: $(OUT_A)  $(OUT_SO)
	mkdir -p sdk
	cp $(OUT_A)  sdk/.
	cp $(OUT_SO) sdk/.

clean:
#	echo "val: $(os)" # use cmd line make os=<some_val> clean	
	rm -f $(objects)

distclean: clean
	rm -f $(OUT_SO) $(OUT_A)
