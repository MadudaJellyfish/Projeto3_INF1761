#pragma once
#include <memory>
#include <string>

#include "node.h"
#include "engine.h"
#include "transform.h"
#include "animation.h"

class LuxorEngine;
using LuxorEnginePtr = std::shared_ptr<LuxorEngine>;

#ifndef LUXOR_ENGINE_H
#define LUXOR_ENGINE_H

class LuxorEngine : public Engine
{
	bool m_reverse;
	float m_head_angle;
	std::string m_status;
	AnimationPtr m_curr_anim, m_stand_down_anim, m_jump_forward_anim;
	TransformPtr m_trf_all, m_trf_base, m_trf_haste1, m_trf_haste2, m_trf_haste3, m_trf_cupula, m_trf_lampada;

	public:
	static LuxorEnginePtr Make(TransformPtr trf_all,
		TransformPtr trf_base, TransformPtr trf_haste1,
		TransformPtr trf_haste2, TransformPtr trf_haste3,
		TransformPtr trf_cupula, TransformPtr trf_lampada
	);

	LuxorEngine(TransformPtr trf_all,
		TransformPtr trf_base, TransformPtr trf_haste1,
		TransformPtr trf_haste2, TransformPtr trf_haste3,
		TransformPtr trf_cupula, TransformPtr trf_lampada
	);

	virtual ~LuxorEngine();
	bool StandUp();
	bool StandDown();
	bool JumpForward();
	bool JumpBackward();
	void TurnHead(float angle);
	virtual void Update(float dt);

private:
	void CreateStandDownAnimation();
	void CreateJumpForwardAnimation();
};

#endif