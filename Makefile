CXX = g++


.PHONY: all wrapper assignment01 clean


all: wrapper assignment01


wrapper:
	$(MAKE) -C common_wrapper


assignment01:
	$(MAKE) -C assignment_01


clean:
	$(MAKE) -C common_wrapper clean
	$(MAKE) -C assignment_01 clean