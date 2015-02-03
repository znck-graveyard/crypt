include ../../Makefile.vars

CFLAGS += -Ddebug=1 -Dunit_testing=1 -Dunit_test_gmix_col=1

unit-test: clean build rindael
	@echo "# Cleaning"; 
	@$(RM) -r $(BUILDDIR) $(OUTPUT)
	@echo ""

rindael: rindael_key_expansion

rindael_key_expansion:
	@ echo "Test: rijndael::key_epansion:"
	@ g++  $(CFLAGS) $(INC) -o .__tmp $(OBJECTS)
	@ ./.__tmp

include ../../Makefile.in
