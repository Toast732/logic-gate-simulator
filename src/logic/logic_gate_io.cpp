#include "string"
#include "vector3.hpp"
#include "..\rendering\objects\volumes\rect.cpp"
#include "..\resource_manager\resource_manager.cpp"
#include "..\rendering\renderer.cpp"
#include "raylib-cpp.hpp"
#include "memory"
#include "..\rendering\objects\mesh_data.cpp"
#include "..\rendering\objects\instanced_mesh_factory.cpp"
#include "..\rendering\objects\mesh_group.cpp"

#ifndef LOGIC_GATE_IO_CPP
#define LOGIC_GATE_IO_CPP

#define IO_TYPE_INPUT 0
#define IO_TYPE_OUTPUT 1

typedef std::shared_ptr<Material> SMaterial;
typedef ResourceManager<Material> Materials;
typedef ResourceManager<Renderer> RendererObject;
typedef ResourceManager<InstancedMesh> MeshInstances;
typedef std::shared_ptr<InstancedMesh> SInstancedMesh;
typedef std::shared_ptr<InstanceID> SInstanceID;

const float IO_RADIUS = 0.125;

inline void setup_logic_gate_io() {

	// create the logic io on mesh
	CreateInstancedMesh(
		"logic_io_on",
		GenMeshSphere(IO_RADIUS, 10, 10),
		{ 25, 255, 25, 255 }
	);

	// create the logic io off mesh
	CreateInstancedMesh(
		"logic_io_off",
		GenMeshSphere(IO_RADIUS, 10, 10),
		{ 255, 25, 25, 255 }
	);
}

class LogicIO {
	public:
		// constructor
		LogicIO(std::string name, int io_type, MeshGroup* mesh_group) {
			this->name = name;
			this->io_type = io_type;
			this->mesh_group = mesh_group;
		}
		
		// getting it's name
		std::string getName() {
			return this->name;
		}

		// setting it's value
		void setValue(float value) {
			this->value = value;
			// update the material
			this->updateMaterial();
		}

		// getting it's value
		float getValue() {
			return this->value;
		}

		// create a copy of it
		LogicIO getCopy() {
			return *this;
		}

		// set it's offset
		void setOffset(Vector3 offset) {
			this->offset = offset;
		}

		// set it's position
		void setPosition(Vector3 position) {
			this->position = position;

			if (this->rendering) {
				if (this->previous_value == 1.0f) {
					//MeshInstances::Get("logic_io_on")->moveInstancePos(this->instance_id, std::make_shared<Matrix>(MatrixFromVector3(Vector3Add(this->position, this->offset))));
				}
				else {
					//MeshInstances::Get("logic_io_off")->moveInstancePos(this->instance_id, std::make_shared<Matrix>(MatrixFromVector3(Vector3Add(this->position, this->offset))));
				}
			}
		}

		// get it's position
		SMatrix getPosition() {
			//return Vector3Add(this->position, this->offset);
			return this->mesh_group->getGlobalInstancePosition(this->instance_id);
		}

		// Create draw object
		void createDraw() {

			// set this io's ID
			this->instance_id = this->mesh_group->getInstancedMeshCount() + 2;

			this->mesh_group->addInstancedMesh(
				this->instance_id,
				"logic_io_off",
				std::make_shared<Matrix>(MatrixFromVector3(this->offset))
			);

			//this->instance_id = MeshInstances::Get("logic_io_off")->addInstance(std::make_shared<Matrix>(MatrixFromVector3(Vector3Add(this->position, this->offset))));

			this->rendering = true;

			//InstancedMesh *mesh = MeshInstances::Get("logic_io_off").get();
			// create the material
			/*this->material = std::make_shared<Material>(LoadMaterialDefault());

			//new (&this->material) SMaterial(Materials::Get("debug_logic"));

			// set the colour
			this->material->maps->color = { 200, 25, 25, 255 };

			// create the logic render object
			this->rect = Rect(
				MatrixTranslate(this->position.x, this->position.y, this->position.z),
				IO_RADIUS,
				IO_RADIUS,
				IO_RADIUS,
				this->material
			);*/
		}

		// Set the object to draw.
		void setRender() {
			// set that we're rendering.
			this->rendering = true;

			// add the render object to be rendered.
			//RendererObject::Get("renderer")->object_renderer.addObject(this->rect.render_object);

			// update the material.
			this->updateMaterial();
		}

	private:
		// private variables

		//int instance_id; // the instance id of the logic io

		MeshGroup* mesh_group; // the mesh group of the logic io
		int instance_id; // the mesh group instance id of the logic io

		std::string name; // the name of the logic io
		float value = 0; // the value of the logic io
		Vector3 position = { 0.0f, 0.0f, 0.0f }; // the position of the logic io
		Vector3 offset = { 0.0f, 0.0f, 0.0f }; // the offset of the logic io
		Rect rect; // it's rect render object
		SMaterial material; // it's render material
		bool rendering = false; // if it's rendering or not.
		float previous_value = 0.0f;
		int io_type;

		// update the material.
		void updateMaterial() {
			// only update if it's rendering (as the material won't have been created otherwise.
			if (!this->rendering) {
				return;
			}

			float current_value = this->getValue();

			// if the value has not changed
			if (this->previous_value == current_value) {
				return;
			}

			// set previous value to the current one.
			this->previous_value = current_value;

			// check if it's on or off.
			if (current_value == 1.0f) {
				// turn on.
				this->mesh_group->replaceInstancedMesh(
					this->instance_id,
					"logic_io_on"
				);

				// green if on

				//printf(("ID Before Removal (off): " + std::to_string(this->instance_id->id) + "\n").c_str());

				// remove the current off instance.
				//MeshInstances::Get("logic_io_off")->removeInstance(this->instance_id);

				//delete this->instance_id;
				//printf(("ID After Removal: " + std::to_string(this->instance_id->id) + "\n").c_str());



				// create an on instance.
				//this->instance_id = MeshInstances::Get("logic_io_on")->addInstance(std::make_shared<Matrix>(MatrixFromVector3(Vector3Add(this->position, this->offset))));

				//printf(("New ID: " + std::to_string(this->instance_id->id) + "\n").c_str());

				//printf("whar2");

				//this->material->maps->color = { 25, 255, 25, 255 };
			}
			else {

				// turn off.
				this->mesh_group->replaceInstancedMesh(
					this->instance_id,
					"logic_io_off"
				);

				// red if off
				//printf(("ID Before Removal (on): " + std::to_string(this->instance_id->id) + "\n").c_str());

				// remove the current on instance.
				//MeshInstances::Get("logic_io_on")->removeInstance(this->instance_id);

				// create an on instance.
				//this->instance_id = MeshInstances::Get("logic_io_off")->addInstance(std::make_shared<Matrix>(MatrixFromVector3(Vector3Add(this->position, this->offset))));

				//this->material->maps->color = { 255, 25, 25, 255 };
			}
		}
};

#endif