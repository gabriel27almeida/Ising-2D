CXXFLAGS := $(shell root-config --cflags) #executa quando compilar
ROOTLIB := $(shell root-config --libs) #variáveis de ambiente

EIGEN_INC := $(shell pkg-config --cflags eigen3) -Wimplicit-int-float-conversion
#EIGEN_INC := -I /Users/gabrielalmeida/Desktop/FC/eigen-3.4.0

SRC_CPP := $(wildcard src/*.cpp)
SRC_C := $(wildcard src/*.C)
OBJS := $(patsubst src/%.cpp, bin/%.o, $(SRC_CPP))
OBJS += $(patsubst src/%.C, bin/%.o, $(SRC_C))

.PRECIOUS: $(OBJS)

VPATH = main:src #permite fazer %.C em vez de main/%.C ou src/%.C

bin/%.o : %.C
	g++ $(CXXFLAGS) -c -o $@ $^ -I src $(EIGEN_INC)

bin/%.o : %.cpp
	g++ $(CXXFLAGS) -c -o $@ $^ -I src $(EIGEN_INC)

bin/%.exe : bin/%.o $(OBJS) #cria o .o a partir do .C
	g++ $(CXXFLAGS) -o $@ $^ -I src $(EIGEN_INC) $(ROOTLIB)

clean:
	rm -f bin/*.exe bin/*.o lib/*