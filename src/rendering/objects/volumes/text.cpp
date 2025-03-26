#include "text.hpp"

Text3D::Text3D() {}

inline bool getPixelValue(std::vector<std::vector<bool>>* pixel_map, int x, int y) {
	if (x < 0 || x >= pixel_map->size()) {
		return false;
	}
	if (y < 0 || y >= pixel_map->at(x).size()) {
		return false;
	}
	return pixel_map->at(x).at(y);
}

// Text3D constructor
Text3D::Text3D(std::string text, Font font, int font_size, Color colour, Matrix matrix) {
	this->font = font;
	this->font_size = font_size;
	this->colour = colour;
	this->matrix = std::make_shared<Matrix>(matrix);

	this->setText(text);
}

void Text3D::setMode(int mode, float value) {
	this->modes[mode] = value;

	// re-generate text if random colour mode is changed.
	if (mode == TEXT3D_MODE_RANDOM_COLOUR) {
		this->setText(this->text);
	}
}

Color Text3D::getColour() {
	if (this->modes[TEXT3D_MODE_RANDOM_COLOUR] != 1.0f) {
		return this->colour;
	} 
	else {
		return Color{
			(unsigned char)GetRandomValue(0, 255),
			(unsigned char)GetRandomValue(0, 255),
			(unsigned char)GetRandomValue(0, 255),
			255
		};
	}
}

void Text3D::setText(std::string text) {
	this->text = text;

	this->rects = std::vector<Rect>();

	std::vector<std::vector<bool>> pixel_map;

	float pixel_size = 0.01f;

	Image text_image = ImageTextEx(font, text.c_str(), font_size, 1.0f, WHITE);

	// iterate through all pixels on the image
	for (int x = 0; x < text_image.width; x++) {
		std::vector<bool> row;

		for (int y = 0; y < text_image.height; y++) {
			Color pixel = GetImageColor(text_image, x, y);

			if (pixel.a == 255) {
				row.push_back(true);
			}
			else {
				row.push_back(false);
			}
		}

		pixel_map.push_back(row);
	}

	int sequence_number = 0;

	std::vector<Color> colour_sequence = {
		{255, 0, 0, 255},
		{0, 255, 0, 255},
		{0, 0, 255, 255},
		{255, 255, 0, 255},
		{0, 255, 255, 255},
		{255, 0, 255, 255},
		{255, 255, 255, 255},
		{0, 0, 0, 255},
		{100, 100, 100, 255},
		{150, 150, 100, 255},
		{150, 50, 50, 255},
		{50, 50, 150, 255}
	};

	// draw a cube at the position if the pixel is true
	for (int x = 0; x < pixel_map.size(); x++) {
		for (int y = 0; y < pixel_map[x].size(); y++) {
			if (pixel_map[x][y]) {

				/*this->rects.push_back(
					Rect(
						MatrixTranslate(
							x * pixel_size,
							y * -pixel_size + 3.0f,
							0.0f
						),
						pixel_size * 4,
						pixel_size,
						pixel_size,
						{255, 0, 0, 150}
					)
				);*/

				Color debug_colour = colour_sequence[sequence_number];

				// skip if the pixel to the left and right is also true
				if (getPixelValue(&pixel_map, x-1, y) && getPixelValue(&pixel_map, x+1, y)) {
					// check if the pixel above and below is also true
					if (y > 0 && y < pixel_map[x].size() - 1) {
						if (pixel_map[x][y - 1] && pixel_map[x][y + 1]) {
							// if it is, check if any of the 4 corners are all filled
							if (pixel_map[x - 1][y - 1] && pixel_map[x + 1][y - 1] && pixel_map[x - 1][y + 1] && pixel_map[x + 1][y + 1]) {
								continue;
								//debug_colour = BLUE;
							}
						}
						else {
							continue;
							//debug_colour = GREEN;
						}
					}
					else {
						continue;
						//debug_colour = YELLOW;
					}
				}
				else {
					//continue;
					//debug_colour = PURPLE;
					// check if the pixel above and below is also true
					if (y > 0 && y < pixel_map[x].size() - 1) {
						if (pixel_map[x][y - 1] && pixel_map[x][y + 1]) {
							continue;
							//debug_colour = PURPLE;
						}
					}
				}

				// skip if the pixel above and below is also true
				/*if (y > 0 && y < pixel_map[x].size() - 1) {
					if (pixel_map[x][y - 1] && pixel_map[x][y + 1]) {
						continue;
					}
				}*/

				this->rects.push_back(
					Rect(
						MatrixTranslate(
							x * pixel_size + matrix->m12,
							y * -pixel_size + matrix->m13,
							pixel_size * 2.0f + matrix->m14
						),
						pixel_size,
						pixel_size,
						pixel_size,
						debug_colour
						//this->getColour()
					)
				);

				// back
				this->rects.push_back(
					Rect(
						MatrixTranslate(
							x * pixel_size + matrix->m12,
							y * -pixel_size + matrix->m13,
							pixel_size * -2.0f + matrix->m14
						),
						pixel_size,
						pixel_size,
						pixel_size,
						debug_colour
						//this->getColour()
					)
				);

				this->vertices.push_back(
					{
						x * pixel_size,
						y * -pixel_size,
						0.0f
					}
				);

				sequence_number++;

				/*this->vertices.push_back(
					{
						x * pixel_size,
						y * -pixel_size,
						2.0f * pixel_size
					}
				);*/
			}
		}
	}

	//this->vertices = { {0.0f, 0.0f, 0.0f}, {10.0, 0.0f, 20.0f}, {20.0f, 0.0f, 0.0f} };

	Polygon polygon = PolygonFromVertices(&this->vertices);

	this->vertices = {
		this->vertices[0],
		this->vertices[1],
		this->vertices[5],
		this->vertices[4]
	};

	// test triangle
	this->meshes.push_back(MeshData(GenMeshVertices(this->vertices), this->matrix, RED));

	//this->meshes.push_back(MeshData(GenMeshVertices(this->vertices), this->matrix, this->colour));

	new (&this->render_object) RenderObject(this->meshes);
}

/*void Text3D::draw() {
	for (int i = 0; i < this->meshes.size(); i++) {
		DrawMesh(this->meshes[i], LoadMaterialDefault(), MatrixTranslate);
	}dd
}*/
