.PHONY: all compile clean

all: compile 
 
compile: 
	@cc test.c cml_AudioPlayer_Interfaces.c CML_AudioPlayer_types.c CML_errormsg.c -o play -lasound -lpthread


clean:
	@rm play


