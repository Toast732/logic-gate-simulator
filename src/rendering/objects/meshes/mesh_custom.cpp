#include "mesh_custom.hpp"

static Vertices MeshGetVertices(Mesh* mesh) {
	Vertices vertices;

	for (int vertex_index = 0; vertex_index < mesh->vertexCount * 3; vertex_index += 3) {
		vertices.push_back({
			mesh->vertices[vertex_index],
			mesh->vertices[vertex_index+1],
			mesh->vertices[vertex_index+2]
		});
	}

	return vertices;
}

/* 
* returns the vertices of the outline of the polygon
* 
* NOTE: more thought will need to be put in, only handles polygons facing in the X or Y axis, cannot handle any Z axis.
*/ 
static Vertices getOutlineVertices(Vertices* vertices) {

	// The number of vertices
	int vertex_count = vertices->size();

	/*
	* Validation
	*/

	// Print a warning if theres not at least 3 vertices.
	if (vertex_count < 3) {
		TraceLog(LOG_WARNING, std::format("Only {} Vertices were provided! Was expecting at least 3.", vertex_count).c_str());
	}

	// create the list.
	Vertices outline_vertices;

	/*
	* 
	* 1. Get the bounds of the polygon (min and max x and y)
	* 2. Add any vertex that is on any of the bounds to the outline vertices list
	* 3. Return the list
	* 
	*/

    // The minimum bounds
	Vector2 min_bounds = { FLT_MAX, FLT_MAX };

	// The maximum bounds
	Vector2 max_bounds = { -FLT_MAX, -FLT_MAX };

	/*
	* 1. Get the bounds of the polygon (min and max x and y)
	*/

	// go through all vertices
	for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++) {
		// get the vertex
		Vector3 vertex = vertices->at(vertex_index);

		// set the min bounds
		min_bounds.x = std::min(min_bounds.x, vertex.x); // x
		min_bounds.y = std::min(min_bounds.y, vertex.y); // y

		// set the max bounds
		max_bounds.x = std::max(max_bounds.x, vertex.x); // x
		max_bounds.y = std::max(max_bounds.y, vertex.y); // y
	}

	/*
	* 2. Add any vertex that is on any of the bounds to the outline vertices list
	*/

	// go through all vertices
	for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++) {
		// get the vertex
		Vector3 vertex = vertices->at(vertex_index);

		// if the vertex is on any of the bounds
		if (
			vertex.x == min_bounds.x || // if on min x bounds
			vertex.x == max_bounds.x || // if on max x bounds
			vertex.y == min_bounds.y || // if on min y bounds
			vertex.y == max_bounds.y // if on max y bounds
		) {
			// add the vertex to the outline vertices list
			outline_vertices.push_back(vertex);
		}
	}

	// Print a warning if the compiled list doesn't have at least 3 vertices.
	if (outline_vertices.size() < 3) {
		TraceLog(LOG_WARNING, std::format("Only {} Outline Vertices found! Expected at least 3!", outline_vertices.size()).c_str());
	}

	/*
	* 3. Return the list
	*/
	return outline_vertices;
}

static int getNextOutlineVertex(Vertices* vertices, Vector3 centre_vertex, Vector3* previous_vertex) {
	// stores the largest angle
	float largest_angle = 0;

	Vector3 next_vertex = Vector3Zero();

	int next_vertex_index = -1;

	// identify the previous vertex
	for (int vertex_index = 0; vertex_index < vertices->size(); vertex_index++) {

		Vector3 vertex = vertices->at(vertex_index);

		if (!Vector3Equals(vertex, *previous_vertex)){
			continue;
		}

		TraceLog(LOG_INFO, std::format("Previous Vertex: {}", vertex_index).c_str());
	}

	// go through all vertices
	for (int vertex_index = 0; vertex_index < vertices->size(); vertex_index++) {
		Vector3 vertex = vertices->at(vertex_index);

		// skip if this vertex is the same as the previous
		if (Vector3Equals(vertex, *previous_vertex)) {
			continue;
		}

		Vector3 relative_vertex = Vector3Subtract(vertex, centre_vertex);
		Vector3 relative_previous_vertex = Vector3Subtract(*previous_vertex, centre_vertex);

		//float angle = Vector3MagnitudeAngleBetween(centre_vertex, vertex);

		//float angle = atan2f(centre_vertex.x - vertex.x, centre_vertex.y - vertex.y);
		//float angle = wrap(atan2f(vertex.x - centre_vertex.x, vertex.y - centre_vertex.y), PI * -0.5, PI * 0.5);

		//float angle = abs(atan2f(vertex.x - centre_vertex.x, vertex.y - centre_vertex.y));

		float angle = abs(atan2f(relative_vertex.x - relative_previous_vertex.x, relative_vertex.y - relative_previous_vertex.y));

		if (angle > largest_angle) {
			largest_angle = angle;
			next_vertex = vertex;
			next_vertex_index = vertex_index;

			// Print that a new best angle has been found, print it's vertex index
			TraceLog(LOG_INFO, ("New Best Angle: " + std::to_string(largest_angle) + " Vertex Index: " + std::to_string(next_vertex_index) + "\n\tX: " + std::to_string(vertices->at(next_vertex_index).x) + "\n\tY: " + std::to_string(vertices->at(next_vertex_index).y) + "\n\tZ: " + std::to_string(vertices->at(next_vertex_index).z)).c_str());
		}
	}

	//TraceLog(LOG_INFO, ("Largest Angle: " + std::to_string(largest_angle) + " Next Vertex Index: " + std::to_string(next_vertex_index)).c_str());
	
	// print the centre
	TraceLog(LOG_INFO, ("Centre Vertex:\n\tX: " + std::to_string(centre_vertex.x) + "\n\tY: " + std::to_string(centre_vertex.y) + "\n\tZ: " + std::to_string(centre_vertex.z)).c_str());
	int x = 0;

	return next_vertex_index;
}

Points PointsFromVertices(Vertices* vertices) {
	Vector3 angles = { 0, 0, 0 };

	// Add up the angles of the vertices
	for (int vertex_index = 1; vertex_index < vertices->size(); vertex_index++) {
		Vector3 vertex = vertices->at(vertex_index);
		Vector3 previous_vertex = vertices->at(vertex_index - 1);

		angles = Vector3Add(angles, Vector3CrossProduct(vertex, previous_vertex));
	}

	int x = 0;

	return {};
}

// Turns vertices into a polygon
Polygon PolygonFromVertices(Vertices* vertices) {

	/*
	* 
	* First, identify the overall shape/outline of the polygon
	* 
	*/

	/*
	* Identify the centre of the polygon, by averaging all the vertices 	
	*/

	// create a vector3 to store the centre of the polygon
	Vector3 centre = { 0, 0, 0 };

	int vertex_count = vertices->size();

	for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++) {
		// get the vertex from vertices
		Vector3 vertex = vertices->at(vertex_index);

		// add the vertex to the centre
		centre = Vector3Add(centre, vertex);
	}

	// divide the centre by the number of vertices
	centre = Vector3Scale(centre, 1.0f / vertices->size());

	/*
	* Identify the first vertex of the polygon (the vertex furthest from the centre)
	*/

	// create a float to store the distance from the centre
	float furthest_distance = 0;

	// create a vector3 to store the first vertex
	Vector3 first_vertex = { 0, 0, 0 };

	for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++) {
		Vector3 vertex = vertices->at(vertex_index);

		// get the distance from the centre
		float distance = Vector3Distance(centre, vertex);

		// if the distance is greater than the furthest distance
		if (distance > furthest_distance) {
			// set the furthest distance
			furthest_distance = distance;

			// set the first vertex
			first_vertex = vertex;
		}
	}

	/*
	* Identify the edges of the polygon
	*/

	// contains the vertices of the outline of the polygon
	Vertices outline_vertices;

	outline_vertices.push_back(first_vertex);

	Vector3 next_vertex = { 0, 0 ,0 };

	int vertices_iterated_through = 0;

	// used to remove the vertices found in the outline, to avoid it constantly going back to previous vertices.
	Vertices outline_temp_vertices = *vertices;

	while (!Vector3Equals(first_vertex, next_vertex) && vertices_iterated_through < vertex_count) {
		// get the next vertex index
		int next_vertex_index = getNextOutlineVertex(&outline_temp_vertices, centre, &outline_vertices[outline_vertices.size() - 1]);
		// get the next vertex from the index.
		next_vertex = outline_temp_vertices[next_vertex_index];
		
		// add this vertex
		outline_vertices.push_back(next_vertex);

		// remove this vertex from outline_temp_vertices
		outline_temp_vertices.erase(outline_temp_vertices.begin() + next_vertex_index);

		vertices_iterated_through++;
	}

	if (vertices_iterated_through < vertex_count) {
		// print that we looped back around to the first vertex
		TraceLog(LOG_INFO, "Looped back around to the first vertex");
	}

	// Remove the duplicate first vertex
	outline_vertices.pop_back();

	// Add this as the outline polygon
	Polygon polygon = { PointsFromVertices(&outline_vertices) };

	return polygon;
}

Mesh GenMeshVertices(Vertices vertices) {

	// calculate vertex count
	int vertex_count = vertices.size();
	
	// create base mesh
	Mesh mesh = { 0 };

	// set triangle count
	mesh.triangleCount = ceil(vertex_count / 3.0f);

	// set vertexCount
	mesh.vertexCount = vertex_count;

	/*
		C style code
	*/

	// allocate for the vertices
	mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));

	// allocate for normals
	mesh.normals = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));

	// allocate for texcoords
	mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));

	/*
		C++ style code
	*/

	float max_x = -999999999.0f;
	float max_z = -999999999.0f;

	// find the maximum x and z value
	for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++) {

		// get the vertex from vertices
		Vector3* vertex = &vertices[vertex_index];
		
		// get highest x
		max_x = std::max(max_x, vertex->x);
		
		// get highest z
		max_z = std::max(max_z, vertex->z);
	}

	// set vertices, and compute normals
	for (int vertex_index = 0; vertex_index < vertex_count*3; vertex_index+=3) {

		/*
		* Get Indexes
		*/ 

		// x index
		int x_index = vertex_index;
		
		// y index
		int y_index = vertex_index + 1;

		// z index
		int z_index = vertex_index + 2;

		// get the vertex from vertices
		Vector3* vertex = &vertices[vertex_index/3];

		/*
		* Set Vertex
		*/

		// set x vertex
		mesh.vertices[x_index] = vertex->x;

		// set y vertex
		mesh.vertices[y_index] = vertex->y;

		// set z vertex
		mesh.vertices[z_index] = vertex->z;

		/*
		* Set Normal
		*/

		// set x normal
		mesh.normals[x_index] = 0;

		// set y normal
		mesh.normals[y_index] = 1;

		// set z normal
		mesh.normals[z_index] = 0;
	}

	//for (int tc_index = 0; tc_index < mesh.vertexCount * 2; tc_index += 2) {

	//	/*
	//	* Get Indexes
	//	*/

	//	// x index
	//	int x_index = tc_index;

	//	// y index
	//	int y_index = tc_index + 1;

	//	/*
	//	* Set Texture Coordinates
	//	*/

	//	// set x texcoord
	//	mesh.texcoords[x_index] = vertex->x / max_x;

	//	// set y texcoord
	//	mesh.texcoords[y_index] = vertex->z / max_z;
	//}

	// move mesh from RAM to VRAM
	UploadMesh(&mesh, false);

	Vertices mesh_vertices = MeshGetVertices(&mesh);

	//int x = 0;

	// return the mesh.
	return mesh;
}