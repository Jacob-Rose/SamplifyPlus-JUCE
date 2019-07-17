#include "SampleAudioThumbnail.h"
#include "SamplifyLookAndFeel.h"

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

	float widthSegment = ((float)area.getWidth()) / AUDIO_THUMBNAIL_LINE_COUNT;
	float segmentTimeLength = (endTimeSeconds - startTimeSeconds) / AUDIO_THUMBNAIL_LINE_COUNT;

	for (int i = 0; i < AUDIO_THUMBNAIL_LINE_COUNT; i++)
	{
		float max, min;
		getApproximateMinMax(startTimeSeconds + (segmentTimeLength*i), startTimeSeconds + (segmentTimeLength*i + 1), channelNum, min, max);
		//min /= peak; //normalize
		//max /= peak; //normalize
		float posX = area.getPosition().x + (widthSegment*i);
		float posY = ((area.getPosition().y) + (area.getHeight() / 2)) - (max * (area.getHeight() / 2));
		float sizeX = widthSegment * (AUDIO_THUMBNAIL_LINE_FILL_WIDTH / (AUDIO_THUMBNAIL_LINE_FILL_WIDTH + AUDIO_THUMBNAIL_LINE_GAP_WIDTH));
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