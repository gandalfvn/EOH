/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2012 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "SpaceInvaders.hpp"

#include "../RomUtils.hpp"

ActionVect SpaceInvadersSettings::actions;

SpaceInvadersSettings::SpaceInvadersSettings() {
    reset();
}


/* create a new instance of the rom */
RomSettings* SpaceInvadersSettings::clone() const { 
    
    RomSettings* rval = new SpaceInvadersSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void SpaceInvadersSettings::step(const System& system) {

    // update the reward
    int score = getDecimalScore(0xE8, 0xE6, &system);
    m_reward = score - m_score;
    m_score = score;

    int my_lives = readRam(&system, 0xC9);


    // update terminal status
    int reset_val = readRam(&system, 0xAA);
    reset_val = (reset_val - (reset_val & 15)) >> 4;
    if (reset_val == 8) {
        // game is not reset yet; not end of game
        m_terminal = false; 
    } else {
        // If bit 0x80 is on, then game is over 
        int some_byte = readRam(&system, 0x98); 
        int lives = readRam(&system, 0xC9);
        m_terminal = (some_byte & 0x80) || lives == 0;
    }
    //    if (my_lives != 3)
    //      m_terminal = true;
}


/* is end of game */
bool SpaceInvadersSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t SpaceInvadersSettings::getReward() const { 

    return m_reward; 
}


/* is an action legal */
bool SpaceInvadersSettings::isLegal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_LEFT:
        case PLAYER_A_RIGHT:
        case PLAYER_A_FIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_RIGHTFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void SpaceInvadersSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}

        
/* saves the state of the rom settings */
void SpaceInvadersSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void SpaceInvadersSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
}

ActionVect& SpaceInvadersSettings::getAvailableActions() {
  if (actions.empty()) {
    // incomplete! for testing purposes
    actions.push_back(PLAYER_A_NOOP);
    actions.push_back(PLAYER_A_FIRE);
    actions.push_back(PLAYER_A_LEFT);
    actions.push_back(PLAYER_A_RIGHT);
  }

  return actions;
}