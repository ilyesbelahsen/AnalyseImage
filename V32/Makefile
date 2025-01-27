CXX = g++
CXXFLAGS = -Wall -g $(shell pkg-config --cflags opencv4)
LDFLAGS = $(shell pkg-config --libs opencv4)

TARGET = main
SRCS = derivatives.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
