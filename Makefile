# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17 -march=native

# Source and base executable
TARGET_SRC = aosoa_measurement.cpp
EXEC_BASE = aosoa_measurement_test

# Vector lengths to test
VECTOR_LENGTHS = 2 4 8 16 32 64 128 256

# Array lengths for tests
LENGTHS = 1 10 100 1000 10000 100000 1000000 10000000 100000000

.PHONY: all clean $(addprefix test_, $(LENGTHS)) test_1K test_10K test_100K test_1M test_10M test_100M

all:
	@echo "Usage: make test_<N> (e.g., make test_1K, make test_1000000)"

# Compile a specific version of the program for each vector size V
$(EXEC_BASE)_V%: $(TARGET_SRC)
	@echo "Compiling for V=$* ..."
	$(CXX) $(CXXFLAGS) -DV=$* $(TARGET_SRC) -o $(EXEC_BASE)_V$*


define MAKE_TEST
test_$(1):
	@echo "============================================="
	@echo " Running AoSoA benchmark for N=$(1)"
	@echo "============================================="
	@echo "N, V, Time (ms)" > results_N$(1).csv
	@for VEC in $(VECTOR_LENGTHS); do \
		echo "→ Building for V=$$$$VEC ..."; \
		$(CXX) $(CXXFLAGS) -DV=$$$$VEC $(TARGET_SRC) -o $(EXEC_BASE)_V$$$$VEC; \
		echo "→ Running test with N=$(1), V=$$$$VEC"; \
		./$(EXEC_BASE)_V$$$$VEC $(1) >> results_N$(1).csv; \
		rm -f $(EXEC_BASE)_V$$$$VEC; \
	done
	@echo "Results saved to results_N$(1).csv"
endef

# Generate tests for all array lengths
$(foreach N,$(LENGTHS),$(eval $(call MAKE_TEST,$(N))))

test_1K: test_1000
test_10K: test_10000
test_100K: test_100000
test_1M: test_1000000
test_10M: test_10000000
test_100M: test_100000000

clean:
	rm -f $(EXEC_BASE)_V* results_N*.csv
