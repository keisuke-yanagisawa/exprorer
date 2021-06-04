CXX=g++
CXXFLAGS= -O2
BOOST_INSTALL_PATH ?= /usr
OBABEL_INSTALL_PATH ?= /usr

ifeq ($(Debug),Y) 
CXXFLAGS = -g -O0 
else
CXXFLAGS = -O2 
endif

ifeq ($(Symbols),Y)
CXXFLAGS += -g
endif



BOOSTIP=-I$(BOOST_INSTALL_PATH)/include
BOOSTLP=-L$(BOOST_INSTALL_PATH)/lib64 -L$(BOOST_INSTALL_PATH)/lib
OPENBABELIP = -I$(OBABEL_INSTALL_PATH)/include/openbabel-2.0
OPENBABELLP = -L$(OBABEL_INSTALL_PATH)/lib

.SUFFIXES: .cc .o

_DECOMPOSE_OBJS = decompose_main.o UnionFindTree.o utils.o OBMolReader.o Converter.o MoleculeToFragments.o AtomType.o Atom.o Molecule.o Vector3d.o Fragment.o infile_reader.o log_writer_stream.o

ALL = objs decompose

DECOMPOSE_OBJS = $(patsubst %,objs/%,$(_DECOMPOSE_OBJS))

.PHONY: clean all

all: $(ALL)

objs:
	mkdir -p objs

decompose: $(DECOMPOSE_OBJS)
	$(CXX)  $(CXXFLAGS) -o $@ $^ $(OPENBABELLP) $(BOOSTLP) -lboost_regex -lboost_program_options -lopenbabel



objs/UnionFindTree.o: src/UnionFindTree.cc src/UnionFindTree.hpp
	$(CXX)  $(CXXFLAGS) $(BOOSTIP) -o $@ -c $<

objs/decompose_main.o: src/decompose_main.cc
	$(CXX)  $(CXXFLAGS) $(BOOSTIP) $(OPENBABELIP) -o $@ -c $< 

objs/paramAD_reader.o: src/paramAD_reader.cc
	$(CXX)  $(CXXFLAGS) $(BOOSTIP) -o $@ -c $<

objs/Vector3d.o: src/Vector3d.cc src/Vector3d.hpp src/common.hpp
	$(CXX)  $(CXXFLAGS) $(BOOSTIP) -o $@ -c $<

objs/AtomType.o: src/AtomType.cc src/AtomType.hpp src/common.hpp
	$(CXX)  $(CXXFLAGS) $(BOOSTIP) -o $@ -c $<

objs/%.o: src/%.cc src/%.hpp src/common.hpp 
	$(CXX)  $(CXXFLAGS) $(BOOSTIP) $(OPENBABELIP) -o $@ -c $<

objs/%.o: src/%.cc src/common.hpp 
	$(CXX)  $(CXXFLAGS) $(BOOSTIP) $(OPENBABELIP) -o $@ -c $<

clean:
	rm -rf $(ALL)
