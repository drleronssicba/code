.PHONY: all
all:auto_ptr scoped_ptr shared_ptr
auto_ptr:auto_ptr.cpp
	g++ $^ -o $@

scoped_ptr:scoped_ptr.cpp
	g++ $^ -o $@

shared_ptr:shared_ptr.cpp
	g++ $^ -o $@
.PHONY:clean
clean:
	rm auto_ptr scoped_ptr shared_ptr
