#include "Beeper.h"
#include "constants.h"

Beeper::Beeper() :
  numBeeps_{0}
{
  // ------ Load sound ------
  const bool loadedSuccessfully = sampleData_.loadFromFile(constants::SOUND_FILE);
  if (loadedSuccessfully == false) {
    throw std::runtime_error("Could not load sound.");
  }
  sample_.setBuffer(sampleData_);

  // ----------------------------
}

void Beeper::beep(const float ratio) {
  if (numBeeps_ < constants::MAX_BEEPS_PER_FRAME) {
    const float pitch = ratio * constants::PITCH_MULTIPLIER + 0.1;

    sample_.setPitch(pitch);
    sample_.play();
    numBeeps_++;
  }
}

