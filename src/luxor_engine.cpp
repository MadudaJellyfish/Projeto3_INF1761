#include "luxor_engine.h"
#include "linear_interpolator.h"
#include "cubic_interpolator.h"

LuxorEngine::LuxorEngine(
	TransformPtr trf_all,
	TransformPtr trf_base,
	TransformPtr trf_haste1,
	TransformPtr trf_haste2,
	TransformPtr trf_haste3,
	TransformPtr trf_cupula,
	TransformPtr trf_lampada
)
	: m_reverse(false),
	m_head_angle(0.0f),
	m_status("up"),
	m_curr_anim(),
	m_trf_all(trf_all),
	m_trf_base(trf_base),
	m_trf_haste1(trf_haste1),
	m_trf_haste2(trf_haste2),
	m_trf_haste3(trf_haste3),
	m_trf_cupula(trf_cupula),
	m_trf_lampada(trf_lampada)
{
	CreateStandDownAnimation(); // private method
	CreateJumpForwardAnimation(); // private method
}

LuxorEngine::~LuxorEngine()
{
}

LuxorEnginePtr LuxorEngine::Make(
	TransformPtr trf_all,
	TransformPtr trf_base,
	TransformPtr trf_haste1,
	TransformPtr trf_haste2,
	TransformPtr trf_haste3,
	TransformPtr trf_cupula,
	TransformPtr trf_lampada
)
{
	return LuxorEnginePtr(new LuxorEngine(
		trf_all,
		trf_base,
		trf_haste1,
		trf_haste2,
		trf_haste3,
		trf_cupula,
		trf_lampada
	));
}

void LuxorEngine::CreateStandDownAnimation()
{
	MovementPtr move = Movement::Make(0.5f);
	move->AddRotation(m_trf_haste1,
		LinearInterpolator::Make(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(-30.0f, 0.0f, 0.0f)));
	move->AddRotation(m_trf_haste2,
		LinearInterpolator::Make(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(12.0f, 0.0f, 0.0f)));
	move->AddRotation(m_trf_haste3,
		LinearInterpolator::Make(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(-12.0f, 0.0f, 0.0f)));
	move->AddRotation(m_trf_cupula,
		LinearInterpolator::Make(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(30.0f, 0.0f, 0.0f)));
	m_stand_down_anim = Animation::Make({ move });
}

bool LuxorEngine::StandUp()
{
	if (m_curr_anim || m_status != "down")
		return false;
	m_curr_anim = m_stand_down_anim;
	m_reverse = true;
	m_status = "up"; // next status
	return true;
}
bool LuxorEngine::StandDown()
{
	if (m_curr_anim || m_status != "up")
		return false;
	m_curr_anim = m_stand_down_anim;
	m_reverse = false;
	m_status = "down"; // next status
	return true;
}

void LuxorEngine::CreateJumpForwardAnimation()
{
	// first move: jump up
	//os movimentos ainda não estão perfeitos
	MovementPtr move1 = Movement::Make(0.5f);
	move1->AddTranslation(m_trf_all,
		CubicInterpolator::Make(
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 100.0f)));
	move1->AddRotation(m_trf_base,
		LinearInterpolator::Make(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(-30.0f, 0.0f, 0.0f)));
	MovementPtr move2 = Movement::Make(0.5f);
	move2->AddRotation(m_trf_haste1,
		LinearInterpolator::Make(glm::vec3(-40.0f, 0.0f, 0.0f),
			glm::vec3(10.0f, 0.0f, 0.0f)));
	MovementPtr move3 = Movement::Make(0.5f);
	move3->AddRotation(m_trf_haste2,
		LinearInterpolator::Make(glm::vec3(-40.0f, 0.0f, 0.0f),
			glm::vec3(10.0f, 0.0f, 0.0f)));
	MovementPtr move4 = Movement::Make(0.5f);
	move4->AddRotation(m_trf_haste3,
		LinearInterpolator::Make(glm::vec3(-40.0f, 0.0f, 0.0f),
			glm::vec3(10.0f, 0.0f, 0.0f)));
	m_jump_forward_anim = Animation::Make({ move1, move2, move3, move4 });
}
bool LuxorEngine::JumpForward()
{
	if (m_curr_anim || m_status != "down")
		return false;
	m_curr_anim = m_jump_forward_anim;
	m_reverse = false;
	m_status = "down"; // next status
	return true;
}
bool LuxorEngine::JumpBackward()
{
	if (m_curr_anim || m_status != "down")
		return false;
	m_curr_anim = m_jump_forward_anim;
	m_reverse = true;
	m_status = "down"; // next status
	return true;
}

void LuxorEngine::TurnHead(float angle)
{
	m_trf_cupula->Rotate(angle, 0.0f, 1.0f, 0.0f);
	m_head_angle += angle;
}

void LuxorEngine::Update(float dt)
{
	if (m_curr_anim) {
		if (m_curr_anim->Advance(dt, m_reverse)) {
			m_curr_anim = AnimationPtr(); // finished
		}
	}
}