dependency:
	cd build && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graphImage

prepare:
	rm -rf build
	mkdir build 

conan_d:
	rm -rf build
	mkdir build
	cd build && conan install .. -s build_type=Debug -s cimpiler.cppstd=17 --output-folder=. --build=missing

conan_r:
	rm -rf build
	mkdir build
	cd build && conan install .. -s build_type=Release -s cimpiler.cppstd=17 --output-folder=. --build=missing
