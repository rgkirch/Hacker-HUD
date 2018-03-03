#pragma once

#include <functional>
#include <vector>

using namespace std;

string whatToDisplay(long time) {
  long frameTime = time % 10;
  if (frameTime < 2) {
    return "hello";
  } else if (frameTime < 4) {
    return "world";
  } else {
    return NTP.getTimeStr(time).c_str();
  }
}

int getWhichFrame(int numFrames, long timePerFrame, long currentTime) {
  int period = numFrames * timePerFrame;
  if (currentTime < 0)
    currentTime += period * (currentTime / period + 1);
  int currentFrame = (currentTime % (numFrames * timePerFrame)) / timePerFrame;
  return currentFrame;
}