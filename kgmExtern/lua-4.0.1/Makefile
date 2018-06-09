# makefile for Lua hierarchy
# see INSTALL for installation instructions
# see file "config" for customization instructions

LUA= .

include $(LUA)/config

# primary targets (only "all" and "clean" are useful after distribution)
all clean co klean:
	cd include; $(MAKE) $@
	cd src; $(MAKE) $@
	cd src/lib; $(MAKE) $@

# (end of Makefile)
