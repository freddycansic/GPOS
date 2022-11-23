#include "Stats.h"

unsigned int Stats::drawCallsPerFrame = 0;

void Stats::resetPerFrameData()
{
	drawCallsPerFrame = 0;
}