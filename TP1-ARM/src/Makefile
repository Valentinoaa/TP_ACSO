sim: shell.c sim.c
	gcc -g -O0 $^ -o $@

.PHONY: clean
clean:
	-@if exist *.o del /Q *.o
	-@if exist *~ del /Q *~
	-@if exist sim del /Q sim
