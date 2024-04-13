EMCC = em++
EXPORTED_FUNCTIONS = ["_malloc", "_free", "_quickShift", "_felzenszwalb", "_slic", "_compressionPalette", "_compressionPredictif", "_compressionRegion", "_displayEdges"]
EMXXFLAGS = -O3 -std=c++20 --no-entry -s WASM=1 -s TOTAL_MEMORY=512MB -s EXPORTED_FUNCTIONS='$(EXPORTED_FUNCTIONS)'

all:
	$(EMCC) $(EMXXFLAGS) wasm/*.cpp -o module.js