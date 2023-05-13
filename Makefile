CFLAGS = -Wall -g -Werror -Wno-error=unused-variable -I ./src/include

SRC_DIR := ./src
BUILD_DIR := ./build

all: $(BUILD_DIR) client

client: $(BUILD_DIR)/client.o  	\
$(BUILD_DIR)/input_parser.o 	\
$(BUILD_DIR)/server_com.o 		\
$(BUILD_DIR)/helpers.o 			\
$(BUILD_DIR)/requests.o			\
$(BUILD_DIR)/buffer.o
	$(CXX) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/input_parser.o: $(SRC_DIR)/input_parser.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/server_com.o: $(SRC_DIR)/server_com.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/helpers.o: $(SRC_DIR)/helpers.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/requests.o: $(SRC_DIR)/requests.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/buffer.o: $(SRC_DIR)/buffer.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	rm -rf client $(BUILD_DIR)/*o