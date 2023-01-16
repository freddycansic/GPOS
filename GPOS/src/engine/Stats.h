#include <cstddef>

struct Stats
{
	static size_t drawCallsPerFrame;
	static size_t numVerticesPerFrame;

	static void resetPerFrameData();
};