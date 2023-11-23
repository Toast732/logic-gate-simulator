#include "objects.hpp"

// define the next render object id
#ifndef OBJECTS_CPP
#define OBJECTS_CPP
inline int next_render_object_id = 0;

typedef std::shared_ptr<InstancedMesh> SInstancedMesh;

class RenderObject {
	private:
	public:
		std::vector<MeshData> meshes_data;
		int id;
		RenderObject() {}
		RenderObject(std::vector<MeshData> meshes_data) {
			// set the id
			this->id = next_render_object_id;

			// set the mesh data
			this->meshes_data = meshes_data;

			// increment the next_render_object_id
			next_render_object_id++;

			//std::cout << "(RenderObject) Created render object with id: " << this->id << "\n";
		}
		~RenderObject() {
			
		}
		void draw() {
			// do the draw thing (probably)
			//std::cout << this->id;
			for (int i = 0; i < meshes_data.size(); i++) {
				//MeshData mesh_data = meshes_data[i];

				//mesh_data.draw();
				meshes_data[i].draw();
				//std::cout << "Drew render object with id " << this->id << "\n";
			}
		}
};

class ObjectRenderer {
	private:
		// contains the ids of the render objects
		std::vector<int> render_object_ids;
		// contains the render objects, indexed by their id, non sequential
		std::map<int, RenderObject> render_objects;

		// contains the instanced meshes.
		std::vector<SInstancedMesh> instanced_meshes;
		
	public:
		int object_count = 0;
		int instanced_mesh_count = 0;
		ObjectRenderer() {}
		void addObject(RenderObject render_object) {
			render_object_ids.push_back(render_object.id);
			render_objects[render_object.id] = render_object;
			this->object_count += render_object.meshes_data.size();
		}
		void addInstancedMesh(SInstancedMesh instanced_mesh) {
			instanced_meshes.push_back(instanced_mesh);
		}
		void draw() {

			this->instanced_mesh_count = 0;

			// iterate through all of the mesh instances.
			for (int i = 0; i < instanced_meshes.size(); i++) {
				instanced_meshes[i]->draw();
				this->instanced_mesh_count += instanced_meshes[i]->instance_count;
			}

			// iterate through all of the render objects
			for (int i = 0; i < render_object_ids.size(); i++) {
				render_objects[render_object_ids[i]].draw();
				// get the id of the render object
				//int id = render_object_ids[i];
				// get the render object
				//RenderObject r_object = render_objects[id];
				// draw the object
				//r_object.draw();
			}
		}

		
};

#endif


// Contains the RenderObjects
//std::vector<RenderObject> render_objects;
//
//void drawObjects() {
//	// iterate through all of the render objects
//	for (int i = 0; i < render_objects.size(); i++) {
//		RenderObject r_object = render_objects[i];
//		// draw the object
//		r_object.draw();
//	}
//}