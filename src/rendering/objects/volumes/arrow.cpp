#include "arrow.hpp"

#ifndef ARROW_CPP
#define ARROW_CPP

class Arrow {
	private:
		Matrix line_matrix;
		Matrix head_matrix;
		float step = 0.0f;
		float length = 0.0f;
	public:
		MeshData line_mesh;
		MeshData head_mesh;
		std::vector<MeshData> meshes;
		RenderObject render_object;
		Color colour;

		Arrow(
			Vector3 position,
			Vector3 rotation,
			float length,
			float width,
			Color colour)
		{

			float arrow_head_length = width * 3;
			length -= arrow_head_length;

			this->length = length;

			this->updatePosition(position, rotation);

			//std::cout << "(Arrow) Created arrow with width: " << width << " and length: " << length << "\n";
			new (&this->line_mesh) MeshData(
				GenMeshCylinder(width, length, 32),
				&this->line_matrix,
				colour
			);

			this->meshes.push_back(this->line_mesh);

			Vector3 arrow_head_pos = position;
			/*arrow_head_pos = Vector3RotateByAxisAngle(arrow_head_pos, {1.0f, 0.0f, 0.0f}, rotation.x);
			arrow_head_pos = Vector3RotateByAxisAngle(arrow_head_pos, { 0.0f, 1.0f, 0.0f }, rotation.y);
			arrow_head_pos = Vector3RotateByAxisAngle(arrow_head_pos, { 0.0f, 0.0f, 1.0f }, rotation.z);*/

			arrow_head_pos = Vector3Add(arrow_head_pos, Vector3RotateByAxisAngle({0.0f, 0.0f, length}, {1.0f, 0.0f, 0.0f}, rotation.x));
			arrow_head_pos = Vector3Add(arrow_head_pos, Vector3RotateByAxisAngle({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, rotation.y));
			arrow_head_pos = Vector3Add(arrow_head_pos, Vector3RotateByAxisAngle({ 0.0f, length, 0.0f }, { 0.0f, 0.0f, 1.0f }, rotation.z));
			//arrow_head_pos.x += length;

			

			new (&this->head_mesh) MeshData(
				GenMeshCone(width*2, width*3, 32),
				///*MatrixAdd(MatrixTranslate(position.x, position.y, position.z), */MatrixMultiply(MatrixRotateXYZ(rotation), MatrixTranslate(0.0f, 0.0f, 0.0f))/*)*/,
				&this->head_matrix,
				colour
			);
			this->meshes.push_back(this->head_mesh);

			new (&this->render_object) RenderObject(this->meshes);
		}
		void updatePosition(Vector3 position, Vector3 rotation) {

			// create rotation matrix.
			Matrix rotation_matrix = MatrixRotateXYZ(rotation);

			/* 
				set where the line is to be drawn
			*/

			// Translate to position, multiply to the rotation to make the line point in the correct direction.
			this->line_matrix = MatrixMultiply(rotation_matrix, MatrixTranslate(position.x, position.y, position.z));

			/*
				set where the arrow head is to be drawn
			*/

			// get the offset for the arrow head, used to move the arrow head where it should be down the line
			Matrix offset = MatrixMultiply(MatrixTranslate(0.0f, length, 0.0f), rotation_matrix);
			// add the arrow head's offset to the arrow position, And multiply to the rotation matrix to make the arrow head point in the correct direction.
			this->head_matrix = MatrixMultiply(rotation_matrix, MatrixTranslate(position.x + offset.m12, position.y + offset.m13, position.z + offset.m14));

			//Matrix r_matrix = MatrixMultiplyRotation(MatrixTranslate(length, 0.0f, length), MatrixRotateXYZ(rotation));
			//Matrix p_matrix = MatrixTranslate(position.x, position.y, position.z);
			//Matrix p_matrix = MatrixTranslate(position.x * this->step, position.y * this->step, position.z * this->step);
			//Matrix p_matrix = MatrixTranslate(0.0f, 0.0f, 0.0f);

			//Vector3 p_vector = {position.x * this-> step, position.y * this->step, position.z * this->step};
			//Vector3 p_vector = { position.x, position.y, position.z };

			//this->step = std::min(double(1.0f), this->step + 1.001);

			//Vector3 head_vector = RotateAtDistance(p_vector, { length, length, length }, rotation);

			//p_vector = Vector3Add(p_vector, RotateAtDistance({0.0f, 0.0f, 0.0f}, {length / 2, length / 2, length / 2}, {rotation.x * this->step, rotation.y * this->step, -rotation.z * this->step}));

			//Vector3 r_vector = Vector3Add(p_vector, head_vector);

			//this->head_matrix = MatrixTranslate(head_vector.x, head_vector.y, head_vector.z);

			//this->head_matrix = MatrixMultiply(MatrixTranslate(p_vector.x, p_vector.y, p_vector.z), MatrixRotateXYZ(rotation));

			// Gets where the head is (on the opposite side)
			//this->head_matrix = MatrixMultiply(MatrixTranslate(position.x, position.y, position.z), MatrixRotateXYZ({ rotation.x * this->step, rotation.y * this->step, -rotation.z * this->step }));

			// gets how much to subtract from where the head is
			//this->head_matrix = MatrixMultiply(MatrixTranslate(0.0f, length / 2, 0.0f), MatrixRotateXYZ({ rotation.x * this->step, rotation.y * this->step, rotation.z * this->step }));

			//this->head_matrix = MatrixTranslate(0.0f, 0.0f, 0.0f);

			//r_matrix = MatrixAddPosition(r_matrix, p_matrix);

			//r_matrix.m14 -= this->length * sin(rotation.z);

			//std::cout << "p_matrix.m14:" << p_matrix.m14 << "\n";

			//this->head_matrix = head_;
		}
		void setRender() {

		}
};
#endif