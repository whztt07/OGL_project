#include "ogldev_app.h"
#include "ogldev_util.h"

#ifndef WIN32
Markup sMarkup = { (char*)"Arial", 64, 1, 0, 0.0, 0.0,
{ .1,1.0,1.0,.5 },{ 1,1,1,0 },
0,{ 1,0,0,1 }, 0,{ 1,0,0,1 },
0,{ 0,0,0,1 }, 0,{ 0,0,0,1 } };
#endif

OgldevApp::OgldevApp()
#ifndef WIN32
	: m_fontRenderer(sMarkup)
#endif
{
	m_frameCount = 0;
	m_frameTime = 0;
	m_fps = 0;

	m_frameTime = m_startTime = GetCurrentTimeMillis();
}

void OgldevApp::CalcFPS()
{
	m_frameCount++;

	long long time = GetCurrentTimeMillis();

	if (time - m_frameTime >= 1000) {
		m_frameTime = time;
		m_fps = m_frameCount;
		m_frameCount = 0;
	}
}

void OgldevApp::RenderFPS()
{
	char text[32];
	ZERO_MEM(text);
	SNPRINTF(text, sizeof(text), "FPS: %d", m_fps);

#ifndef WIN32
	m_fontRenderer.RenderText(10, 10, text);
#endif
}

float OgldevApp::GetRunningTime()
{
	float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;
	return RunningTime;
}