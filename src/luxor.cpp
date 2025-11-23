#include "luxor.h"
#include "material.h"
#include "texture.h"

Luxor::Luxor()
{
	AppearancePtr red = Material::Make(1.0f, 0.5f, 0.5f);
	AppearancePtr white = Material::Make(1.0f, 1.0f, 1.0f);
	

	MeshPtr base_a = Mesh::Make("luxor/meshes/base_a.msh");
	MeshPtr base_b = Mesh::Make("luxor/meshes/base_b.msh");
	MeshPtr haste1 = Mesh::Make("luxor/meshes/haste1.msh");
	MeshPtr haste2 = Mesh::Make("luxor/meshes/haste2.msh");
	MeshPtr haste3 = Mesh::Make("luxor/meshes/haste3.msh");
	MeshPtr cupula = Mesh::Make("luxor/meshes/cupula.msh");
	MeshPtr lampada = Mesh::Make("luxor/meshes/lampada.msh");

	TransformPtr trf_all = Transform::Make();
	TransformPtr trf_base = Transform::Make();
	TransformPtr trf_haste1 = Transform::Make();
	TransformPtr trf_haste2 = Transform::Make();
	TransformPtr trf_haste3 = Transform::Make();
	TransformPtr trf_cupula = Transform::Make();
	TransformPtr trf_lampada = Transform::Make();

	trf_haste1->Translate(0.0f, 4.0f, 0.0f);
	trf_haste2->Translate(0.0f, 17.15f, 0.0f);
	trf_haste3->Translate(0.0f, 16.78f, 0.0f);
	trf_cupula->Translate(0.0f, 18.12f, 0.0f);
	trf_lampada->Translate(0.0f, 8.4f, 9.0f);

	m_light_node = Node::Make(trf_lampada, { white }, { lampada }, {});
	m_node = Node::Make(trf_all, { red }, {
			Node::Make(trf_base, { base_a, base_b }, {
			Node::Make(trf_haste1, { haste1 }, { m_light_node })
		})
	});