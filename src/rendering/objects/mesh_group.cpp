#include "mesh_group.hpp"

#ifndef MESH_GROUP_CPP
#define MESH_GROUP_CPP

// this is honestly a pretty bad system for checking NULL, especially for integers, but too bad. I dont know enough to make it better :)
#define INT_NULL -1
#define STR_NULL ""

class InstancedMeshStateData {
	public:
		InstancedMeshStateData() {}

		// create it with a string and a matrix.
		InstancedMeshStateData(std::string instanced_mesh_name, SMatrix local_matrix, SMatrix* parent_matrix) {

			// set the local matrix.
			this->local_matrix = local_matrix;

			// set the parent matrix.
			this->parent_matrix = parent_matrix;

			// create the mesh instance
			this->create(instanced_mesh_name);
		}

		// removes the mesh
		void remove() {

			// check if the name is not STR_NULL
			if (this->name == STR_NULL) {
				TraceLog(LOG_WARNING, "Attempted to remove a mesh with a NULL name.");
				return;
			}

			// check if the instance_id is not INT_NULL
			if (this->instance_id == INT_NULL) {
				TraceLog(LOG_WARNING, "Attempted to remove a mesh with a NULL instance_id.");
				return;
			}

			// remove the mesh's instance
			MeshInstances::Get(this->name)->removeInstance(this->instance_id);

			// set instance_id to NULL.
			this->instance_id = INT_NULL;

			// set name to NULL.
			this->name = STR_NULL;
		}

		// replace the mesh with another one
		void replace(std::string instanced_mesh_name) {

			// remove the curent instance
			this->remove();

			// create the new instance
			this->create(instanced_mesh_name);
		}

		// "tick" the position
		void tickPos() {
			// check if it's rendering.
			if (!this->rendering()) {
				// return, as we wont have any mesh to update the position of.
				return;
			}

			// update the mesh's position
			MeshInstances::Get(this->name)->moveInstancePos(this->instance_id, this->getGlobalPosition());
		}

		// update the local matrix
		void setPos(SMatrix matrix) {

			// set the local matrix
			this->local_matrix = matrix;

			// tick the position
			this->tickPos();
		}

		SMatrix getGlobalPosition() {
			return std::make_shared<Matrix>(MatrixMultiply(**&this->local_matrix, **this->parent_matrix));
		}

	private:

		/*
			Variables
		*/

		// local matrix of the instanced mesh.
		SMatrix local_matrix;

		// parent matrix of the instanced mesh. Stored as a pointer so we dont have to constantly re-sync.
		SMatrix* parent_matrix;

		// id of the instanced mesh.
		int instance_id = INT_NULL;
		// name of the instanced mesh.
		std::string name = STR_NULL;

		/*
			Functions
		*/

		// creating the mesh instance
		void create(std::string name) {

			// check if the name is STR_NULL
			if (name == STR_NULL) {
				TraceLog(LOG_WARNING, "Attempted to create a mesh with a NULL name.");
				return;
			}

			// ensure theres not already an existing instance
			if (this->instance_id != INT_NULL) {
				TraceLog(LOG_WARNING, "Attempted to create the new mesh %s, but theres already a mesh created.");
				return;
			}

			// set the name
			this->name = name;

			// get the global matrix of the instance
			SMatrix matrix = this->getGlobalPosition();

			// create the mesh instance
			this->instance_id = MeshInstances::Get(this->name)->addInstance(matrix);
		}

		// if it's rendering or not, returns true if it is.
		bool rendering() {
			return this->instance_id != INT_NULL && this->name != STR_NULL;
		}
};

class MeshGroup {
	public:

		MeshGroup() {}
		// constructor
		MeshGroup(SMatrix* matrix) {
			// set the global position
			this->matrix = matrix;
		}

		// add a new instanced mesh to this group.
		void addInstancedMesh(int identifier, std::string instanced_mesh_name, SMatrix local_matrix) {

			// check if the identifier already exists
			if (this->instanced_meshes.find(identifier) != this->instanced_meshes.end()) {
				TraceLog(LOG_WARNING, "Attempted to add an already existing instanced mesh.");
				return;
			}

			// create the new instanced mesh
			InstancedMeshStateData instanced_mesh = InstancedMeshStateData(instanced_mesh_name, local_matrix, this->matrix);

			// add it to the list
			this->instanced_meshes[identifier] = instanced_mesh; 
		}

		void removeInstancedMesh(int identifier) {
			// check if the identifier exists
			if (this->instanced_meshes.find(identifier) == this->instanced_meshes.end()) {
				TraceLog(LOG_WARNING, "Attempted to remove a non-existant instanced mesh.");
				return;
			}

			// remove the instanced mesh
			this->instanced_meshes[identifier].remove();

			// remove it from the list
			this->instanced_meshes.erase(identifier);
		}

		void replaceInstancedMesh(int identifier, std::string instanced_mesh_name) {
			// check if the identifier exists
			if (this->instanced_meshes.find(identifier) == this->instanced_meshes.end()) {
				TraceLog(LOG_WARNING, "Attempted to replace a non-existant instanced mesh.");
				return;
			}

			// replace the instanced mesh
			this->instanced_meshes[identifier].replace(instanced_mesh_name);
		}

		// "ticks" the global position, as drawing instanced meshes uses arrays and we turn from vector to array, we wont be able to make them pointers
		// so we have to update them manually.
		void updateGlobalPosition() {
			for (auto & [_, instance_mesh_state_data] : instanced_meshes) {
				// tick the position of it
				instance_mesh_state_data.tickPos();
			}
		}

		int getInstancedMeshCount() {
			return this->instanced_meshes.size();
		}

		SMatrix getGlobalInstancePosition(int identifier) {
			return this->instanced_meshes[identifier].getGlobalPosition();
		}

	private:

		/*
			Variables
		*/

		// matrix of the group.
		SMatrix* matrix;

		// instanced meshes of the group.
		std::map<int, InstancedMeshStateData> instanced_meshes;
};

#endif