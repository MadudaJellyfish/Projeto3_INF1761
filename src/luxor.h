#pragma once
#include <memory>
#include "node.h"
#include "mesh.h"
#include "luxor_engine.h"
#include "transform.h"


class Luxor;
using LuxorPtr = std::shared_ptr<Luxor>;

#ifndef LUXOR_H
#define LUXOR_H

class Luxor
{
	NodePtr m_node;
	NodePtr m_light_node;
	LuxorEnginePtr m_engine;
protected:
	Luxor();
public:
	virtual ~Luxor();
	static LuxorPtr Make();
	NodePtr GetNode();
	NodePtr GetLightNode();
	LuxorEnginePtr GetEngine();
};
#endif