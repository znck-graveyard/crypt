include ../../Makefile.vars

unit-test: rijndael
	@echo "# Cleaning"; 
	@$(RM) -vr $(BUILDDIR) $(OUTPUT)
	@echo ""

rijndael: 
	@ make key_expansion
	@ make shift_row
	@ make mix_columns
	@ make sbox_inv

key_expansion: CFLAGS := -g -Wall -Ddebug=1 -Dunit_testing=1 -Dunit_test_rijndael_key_expansion=1
key_expansion: clean build
	@ echo "Test: rijndael::key_epansion:"
	 g++  $(CFLAGS) $(INC) -o .__tmp $(OBJECTS)
	@ ./test.sh

shift_row: CFLAGS = -g -Wall -Ddebug=1 -Dunit_testing=1 -Dunit_test_rijndael_shift_row=1
shift_row: clean build
	@ echo "Test: rijndael::shift_row:"
	g++  $(CFLAGS) $(INC) -o .__tmp $(OBJECTS)
	./.__tmp

mix_columns: CFLAGS = -g -Wall -Ddebug=1 -Dunit_testing=1 -Dunit_test_rijndael_mix_columns=1
mix_columns: clean build
	@ echo "Test: rijndael::mix_columns:"
	g++  $(CFLAGS) $(INC) -o .__tmp $(OBJECTS)
	./.__tmp

sbox_inv: CFLAGS = -g -Wall -Ddebug=1 -Dunit_testing=1 -Dunit_test_rijndael_sbox_inv=1
sbox_inv: clean build
	    @ echo "Test: rijndael::sbox_inv:"
		g++  $(CFLAGS) $(INC) -o .__tmp $(OBJECTS)
		./.__tmp
include ../../Makefile.in
