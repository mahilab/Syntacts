#include "Sequencer.hpp"

Sequencer::Sequencer(Gui* gui)
    : Widget(gui)
{
    m_sequence.frameMin = 0;
    m_sequence.frameMax = 100;
    m_sequence.items.push_back(CueSequence::Item{0, 0, 20, false});
}

void Sequencer::render()
{
    ImSequencer::Sequencer(&m_sequence, &m_currentFrame, &m_expanded, &m_selectedEntry, &m_firstFrame,
                           ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_CHANGE_FRAME);
}