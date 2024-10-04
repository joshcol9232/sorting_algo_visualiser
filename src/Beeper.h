#pragma once

#include <SFML/Audio.hpp>

class Beeper {
 public:
  Beeper();
  void beep(const float ratio);
  void resetCounter() { numBeeps_ = 0; }
 private:
  size_t numBeeps_;
  sf::Sound sample_;
  sf::SoundBuffer sampleData_;
};

