#pragma once

#include <raylib-cpp.hpp>
#include <rlgl.h>
#include <string>
#include <vector>
#include "rect.cpp"
#include "../meshes/mesh_custom.hpp"
//#include "../../renderer.cpp"

#define TEXT3D_MODE_RANDOM_COLOUR 0

class Text3D {
	private:
		std::string text = "";
		Font font = GetFontDefault();
		int font_size = 10;
		Color colour = RAYWHITE;
		std::map<int, float> modes;
		std::vector<MeshData> meshes;
		SMatrix matrix;
	public:
		RenderObject render_object;
		std::vector<Rect> rects;
		std::vector<Vector3> vertices;
		Text3D();
		Text3D(std::string text, Font font, int font_size, Color colour, Matrix matrix);
		void setMode(int mode, float value);
		void draw();
		void setText(std::string text);
		void setFont(Font font);
		void setFontSize(int font_size);
		void setColour(Color color);
		std::string getText();
		Font getFont();
		int getFontSize();
		Color getColour();
};