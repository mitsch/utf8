COMPILER=g++
EXEC_LINKER=g++
LIB_LINKER=ar
HEADER_PATH=src/

TEST_SOURCES=$(wildcard test/*.cpp)
TEST_OBJECTS=$(patsubst %.cpp,%.o,$(TEST_SOURCES))
TEST_EXECUTABLE=test/test

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(EXEC_LINKER) $(TEST_OBJECTS) -o $@ -lpthread -lgtest

.cpp.o:
	$(COMPILER) $< -o $@ -c -I $(HEADER_PATH) -std=c++0x

clean:
	rm $(TEST_EXECUTABLE) $(TEST_OBJECTS)
