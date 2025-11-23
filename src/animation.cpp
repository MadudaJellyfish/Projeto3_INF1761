#include "animation.h"

AnimationPtr Animation::Make(std::initializer_list<MovementPtr> moves) {
	return AnimationPtr(new Animation(moves));
}
Animation::Animation(std::initializer_list<MovementPtr> moves)
	: m_curr(0), m_moves(moves)
{
}
bool Animation::Advance(float dt, bool reverse)
{
	int idx = reverse ? (m_moves.size() - 1) - m_curr : m_curr;
	if (m_moves[idx]->Advance(dt, reverse)) {
		if (++m_curr == m_moves.size()) {
			m_curr = 0; // it's done
			return true;
		}
	}
	return false; // it continues
}
