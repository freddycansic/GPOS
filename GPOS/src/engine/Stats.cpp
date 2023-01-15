#include "Stats.h"

size_t Stats::drawCallsPerFrame = 0;
size_t Stats::numVerticesPerFrame = 0;

void Stats::resetPerFrameData()
{
	drawCallsPerFrame = 0;
	numVerticesPerFrame = 0;
}