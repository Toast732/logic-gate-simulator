#include "mesh_data.hpp"

#ifndef MESH_DATA_CPP
#define MESH_DATA_CPP

typedef std::shared_ptr<Material> SMaterial;
typedef ResourceManager<Shader> Shaders;

inline int previous_alpha = 0;
inline int previous_instance_alpha = 0;

// lets you shift the mesh by the provided vector3.
//inline static void ShiftMeshPosWithVector3(Mesh* m, Vector3 v) {
//	for (int i = 3; i < m->vertexCount; i++) {
//		#define X 0
//		#define Y 1
//		//#define Z 2 (if not x or y, then it's z, so we dont need to check for it.)
//		int position_index = i % 3; // 0 = x, 1 = y, 2 = z.
//		if (position_index == X) { // shift x position
//			m->vertices[i] += v.x;
//		}
//		else if (position_index == Y) { // shift y position
//			m->vertices[i] += v.y;
//		}
//		else { // shift z position
//			m->vertices[i] += v.z;
//		}
//
//		//TraceLog(LOG_INFO, ("Vertex " + std::to_string(i) + ": " + std::to_string(m->vertices[i])).c_str());
//	}
//}

class MeshData {
	public:
		Mesh mesh;
		//Vector3 position;
		//Vector3 rotation;
		Matrix* matrix;
		SMaterial material;
		//Color colour;
		MeshData() {
			this->mesh = GenMeshPoly(6, 1.0f);
			//this->position = Vector3{0, 0, 0};
			//this->rotation = Vector3{0, 0, 0};
			this->matrix = new Matrix (MatrixTranslate(0.0f, 0.0f, 0.0f));
			//this->colour = PINK;
			//this->material = Material();//std::make_shared<Material>(Material(LoadMaterialDefault()));
			//(*this->material.get()).maps->color = this->colour;
		}

		// create mesh data with a matrix.
		MeshData(Mesh mesh, Matrix* matrix, Color colour) {
			// set mesh
			this->mesh = mesh;

			// set matrix
			this->matrix = matrix;

			// set the colour
			//this->colour = colour;

			// load the default material as a smart pointer
			this->material = std::make_shared<Material>(Material(LoadMaterialDefault()));

			// set the material's colour to the colour desired.
			(*this->material.get()).maps->color = colour;
			this->updateShaderAlpha();
		}

		// create mesh data with a shared/smart matrix.
		MeshData(Mesh mesh, SMatrix matrix, Color colour) {
			// set mesh
			this->mesh = mesh;

			// set matrix, dereference the smart pointer.
			this->matrix = &*matrix;

			// set the colour
			//this->colour = colour;

			// load the default material as a smart pointer
			this->material = std::make_shared<Material>(Material(LoadMaterialDefault()));

			// set the material's colour to the colour desired.
			(*this->material.get()).maps->color = colour;
			this->updateShaderAlpha();
		}

		// create mesh data with a shared/smart matrix and a shared material.
		MeshData(Mesh mesh, SMatrix matrix, SMaterial material) {

			// set mesh
			this->mesh = mesh;

			// set matrix, dereference the smart pointer.
			this->matrix = &*matrix;

			// set the material
			this->material = material;

			this->material->shader = *Shaders::Get("primary").get();

			this->shader_alpha_loc = GetShaderLocation(this->material->shader, "alpha");

			this->updateShaderAlpha();
		}

		MeshData(Mesh mesh, Matrix *matrix, Material material) {
			this->mesh = mesh;
			/*this->position = position;
			this->rotation = rotation;*/
			this->matrix = matrix;
			this->material = std::make_shared<Material>(Material(material));
			//this->colour = PINK;
			this->updateShaderAlpha();
		}

		void draw() {
			updateShaderAlpha();
			DrawMesh(this->mesh, *this->material, *this->matrix);
		}

		// you should call whenever you update the material alpha.
		void updateShaderAlpha() {
			//printf(("\033[91mAlpha: " + std::to_string(this->material->maps->color.a) + "\033[0m\n").c_str());

			//printf(("\033[92mAlpha Set: " + std::to_string(alpha) + "\033[0m\n").c_str());

			int alpha = this->material->maps->color.a;

			if (alpha != previous_alpha) {
				previous_alpha = alpha;
				float converted_alpha = alpha / 255.0f;



				SetShaderValue(this->material->shader, this->shader_alpha_loc, &converted_alpha, SHADER_UNIFORM_FLOAT);
			}
		}
	private:
		int shader_alpha_loc;
};

inline int next_instance_id = 0;

// used to share but still modify the id.
class InstanceID {
	public:
		int instance_id;
		int id = -1;
		InstanceID() {
			this->instance_id = next_instance_id;
			next_instance_id++;
		}
		InstanceID(int id) {
			if (this->instance_id != -1) {
				this->instance_id = next_instance_id;
				next_instance_id++;
			}
			this->id = id;
		}

		~InstanceID() {}

		void modifyID(int shift) {
			this->id += shift;
		}
};

typedef std::shared_ptr<InstanceID> SInstanceID;

class InstancedMesh {
	private:
		int next_instance_id = 0;
		std::vector<InstanceID> instance_ids;
		int shader_alpha_loc;
	public:
		int instance_count = 0;
		Mesh mesh;
		std::vector<Matrix> matrices;
		SMaterial material;
		InstancedMesh() {}
		InstancedMesh(Mesh mesh, SMaterial material) {
			this->mesh = mesh;
			this->material = material;

			this->material->shader = *Shaders::Get("primary_instanced").get();

			this->shader_alpha_loc = GetShaderLocation(this->material->shader, "alpha");
		}

		int addInstance(SMatrix matrix) {

			InstanceID instance_id = InstanceID(this->instance_ids.size());

			this->instance_count++;

			this->instance_ids.push_back(instance_id);

			this->matrices.push_back(*matrix);

			return this->instance_ids[this->instance_ids.size()-1].instance_id;
		}

		void moveInstancePos(int instance_id, SMatrix matrix) {
			int id = -1;

			for (int instance_index = 0; instance_index < this->instance_ids.size(); instance_index++) {
				if (this->instance_ids[instance_index].instance_id == instance_id) {
					id = this->instance_ids[instance_index].id;
					break;
				}
			}

			if (id == -1) {
				TraceLog(LOG_WARNING, ("[moveInstance] Failed to find matching id for instance id : " + std::to_string(instance_id)).c_str());
				return;
			}

			this->matrices[id] = *matrix;
		}

		void removeInstance(int instance_id) {

			int id = -1;

			for (int instance_index = 0; instance_index < this->instance_ids.size(); instance_index++) {
				if (this->instance_ids[instance_index].instance_id == instance_id) {
					id = this->instance_ids[instance_index].id;
					break;
				}
			}

			if (id == -1) {
				TraceLog(LOG_WARNING, ("[removeInstance] Failed to find matching id for instance id : " + std::to_string(instance_id)).c_str());
				return;
			}

			int instance_size = this->instance_ids.size();

			// check if this id is higher than what is possible.
			if (id > instance_size - 1) {
				TraceLog(LOG_ERROR, ("Invalid instance id given: " + std::to_string(id) + " Maximum: " + std::to_string(instance_size - 1) + "\n").c_str());
				return;
			}
			// remove the matrix with this id
			this->matrices.erase(this->matrices.begin() + id);

			// shift the instance ids above this one down by 1.
			for (int instance_index = id; instance_index < instance_size; instance_index++) {
				this->instance_ids[instance_index].id -= 1;
			}

			this->instance_count--;


			// remove the instance id with this id
			this->instance_ids.erase(this->instance_ids.begin() + id);
		}

		void draw() {
			// check if theres anything to draw
			if (this->instance_count > 0) {
				// update the shader's alpha
				this->updateShaderAlpha();

				// draw the meshes.
				DrawMeshInstanced(this->mesh, *this->material, &this->matrices[0], this->instance_count);
			}
		}

		// should be called right before the instance object is drawn.
		void updateShaderAlpha() {

			// save the alpha as a variable so we dont have to constantly index it.
			int alpha = this->material->maps->color.a;

			// if the instance alpha has changed at all
			if (alpha != previous_instance_alpha) {

				// set the previous instance alpha to the new alpha
				previous_instance_alpha = alpha;

				// convert from 0-255 to 0-1
				float converted_alpha = alpha / 255.0f;

				// set the shader alpha.
				SetShaderValue(this->material->shader, this->shader_alpha_loc, &converted_alpha, SHADER_UNIFORM_FLOAT);
			}
		}
};

#endif