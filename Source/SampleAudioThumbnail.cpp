#include "SampleAudioThumbnail.h"

using namespace samplify;

SampleAudioThumbnail::SampleAudioThumbnail(int sourceSamplesPerThumbnailSample,
	AudioFormatManager& formatManagerToUse,
	AudioThumbnailCache& cacheToUse) :
	AudioThumbnail(sourceSamplesPerThumbnailSample, formatManagerToUse, cacheToUse)
{

}

SampleAudioThumbnail::~SampleAudioThumbnail()
{
}

void SampleAudioThumbnail::drawChannel(Graphics & g, const Rectangle<int>& area, double startTimeSeconds, double endTimeSeconds, int channelNum, float verticalZoomFactor)
{
	float peak = getApproximatePeak();

	int widthSegment = area.getWidth() / waveformLines;
	float segmentTimeLength = (endTimeSeconds - startTimeSeconds) / waveformLines;

	for (int i = 0; i < waveformLines; i++)
	{
		float max, min;
		getApproximateMinMax(startTimeSeconds + (segmentTimeLength*i), startTimeSeconds + (segmentTimeLength*i + 1), channelNum, min, max);
		//min /= peak; //normalize
		//max /= peak; //normalize
		float posX = area.getPosition().x + (widthSegment*i);
		float posY = ((area.getPosition().y) + (area.getHeight() / 2)) - (max * (area.getHeight() / 2));
		float sizeX = widthSegment * (fillSize / (fillSize + spacesSize));
		float sizeY = ((area.getHeight() / 2)*(min*-1)) + (max * (area.getHeight() / 2));
		Rectangle<float> rectangleArea(posX, posY, sizeX, sizeY);
		//Rectangle<float> r(Point<float>(0,0), Point<float>(0,0))
		g.fillRoundedRectangle(rectangleArea, 2.0f);
	}
}

void SampleAudioThumbnail::drawChannels(Graphics & g, const Rectangle<int>& area, double startTimeSeconds, double endTimeSeconds, float verticalZoomFactor)
{
	float heightChunk = area.getHeight() / getNumChannels();
	for (int i = 0; i < getNumChannels(); i++)
	{
		Rectangle<int> channelArea(0, i*heightChunk, area.getWidth(), heightChunk);
		drawChannel(g, channelArea, startTimeSeconds, endTimeSeconds, i, verticalZoomFactor);
	}
}