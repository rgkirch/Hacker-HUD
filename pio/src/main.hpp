#pragma once

#include <vector>
#include <functional>

using std::string;
using std::vector;
using std::function;

int getWhichFrame(int numFrames, long timePerFrame, long currentTime) {
    int period = numFrames * timePerFrame;
    if (currentTime < 0) currentTime += period * (currentTime / period + 1);
    int currentFrame = (currentTime % (numFrames * timePerFrame)) / timePerFrame;
    return currentFrame;
}