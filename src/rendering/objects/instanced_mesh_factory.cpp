#include "instanced_mesh_factory.hpp"

#ifndef INSTANCED_MESH_FACTORY_CPP
#define INSTANCED_MESH_FACTORY_CPP

typedef std::shared_ptr<Material> SMaterial;
typedef ResourceManager<InstancedMesh> MeshInstances;
typedef ResourceManager<Renderer> RendererObject;

inline static void CreateInstancedMesh(std::string name, Mesh mesh, Color colour) {
	// create the material
	SMaterial material = std::make_shared<Material>(LoadMaterialDefault());

	// set the colour
	material->maps->color = colour;

	// create the instanced mesh
	InstancedMesh instanced_mesh = InstancedMesh(mesh, material);

	// register it
	MeshInstances::Register(name, std::make_shared<InstancedMesh>(*&instanced_mesh));

	// add it to the renderer's list
	RendererObject::Get("renderer")->object_renderer.addInstancedMesh(MeshInstances::Get(name));
}

#endif