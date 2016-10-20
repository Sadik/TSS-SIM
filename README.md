Dependencies

sudo apt-get install libboost1.58-dev

Note:
only works when boost is compiled in thirdparty/boost
because CMakeLists.txt looks there for boost

	set(BOOST_INCLUDEDIR ./thirdparty/boost)
	set(BOOST_LIBRARYDIR ./thirdparty/boost/stage/lib)

TSS-SIM
   |
   --CMakeLists.txt
   --build/
   --src/
   --thirdparty/
       |
       --boost


to compile boost there, download boost version 1.62.0 to thirdparty, extract it and rename the new folder to "boost". Then just type

./bootstrap.sh

and then

./b2


