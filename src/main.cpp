/*******************************************************************************************
*
*   raylib-cpp [core] example - Basic window (adapted for HTML5 platform)
*
*   This example is prepared to compile for PLATFORM_WEB, PLATFORM_DESKTOP and PLATFORM_RPI
*   As you will notice, code structure is slightly diferent to the other examples...
*   To compile it for PLATFORM_WEB just uncomment #define PLATFORM_WEB at beginning
*
*   This example has been created using raylib-cpp (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"
#include "iostream"
#include "rendering/renderer.cpp"
#include "rendering/objects/objects.cpp" 
#include "rendering/objects/volumes/arrow.cpp"
#include "rendering/objects/volumes/rect.cpp"
#include "resource_manager/resource_manager.cpp"
#include "memory"
#include "logic/logic_gate.cpp"
#include "thread"
#include "logic/logic_pipe.cpp"
#define RLIGHTS_IMPLEMENTATION
#include "../resources/shaders/rlights.h"

#define GLSL_VERSION 330

raylib::Camera3D camera;

int counter = 0;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 1920;
int screenHeight = 1080;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void);     // Update and Draw one frame

std::vector<AndGate> and_gates = std::vector<AndGate>();

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    /*
        Initialization
    */

    SetTraceLogLevel(LOG_DEBUG);

    Renderer temp_renderer(screenWidth, screenHeight, "Logic Gate Simulator");

    typedef ResourceManager<Renderer> RendererResource;
    typedef std::shared_ptr<Renderer> SRenderer;

    RendererResource::Register("renderer", std::make_shared<Renderer>(*&temp_renderer));

    SRenderer renderer = RendererResource::Get("renderer");

    raylib::Window window = (*renderer).getWindow();

    /*
        Create materials
    */

    typedef ResourceManager<Shader> Shaders;

    //Shader shader = LoadShader("E:/Documents/GitHub/Applications/C++/Logic Gate Simulator/resources/shaders/lighting_instancing.vs", "E:/Documents/GitHub/Applications/C++/Logic Gate Simulator/resources/shaders/lighting.fs");
    //Shader primary_shader = LoadShader("resources/shaders/lighting.vs", "resources/shaders/lighting.fs");
    /*Shader primary_shader = LoadShader("E:/Documents/GitHub/Applications/C++/Logic Gate Simulator/resources/shaders/lighting.glslv", "E:/Documents/GitHub/Applications/C++/Logic Gate Simulator/resources/shaders/lighting.glslf");
    Shader primary_instancing_shader = LoadShader("resources/shaders/lighting_instancing.glslv", "resources/shaders/lighting.glslf");*/
    Shader primary_shader = LoadShader("resources/shaders/lighting.glslv", "resources/shaders/lighting.glslf");
    Shader primary_instancing_shader = LoadShader("resources/shaders/lighting_instancing.glslv", "resources/shaders/lighting.glslf");

    // Get shader locations
    primary_instancing_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(primary_instancing_shader, "mvp");
    primary_instancing_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(primary_instancing_shader, "viewPos");
    primary_instancing_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(primary_instancing_shader, "instanceTransform");

    primary_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(primary_shader, "mvp");
    primary_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(primary_shader, "viewPos");
    primary_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(primary_shader, "instanceTransform");
    //primary_shader.locs[SHADER_LOC_COLOR_DIFFUSE] = GetShaderLocation(primary_shader, "vertexColour");

    // Set shader value: ambient light level
    //int ambientLoc = ;
    float v[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    SetShaderValue(primary_instancing_shader, GetShaderLocation(primary_instancing_shader, "ambient"), v, SHADER_UNIFORM_VEC4);
    SetShaderValue(primary_shader, GetShaderLocation(primary_shader, "ambient"), v, SHADER_UNIFORM_VEC4);

    // Create one light
    CreateLight(LIGHT_DIRECTIONAL, { 50.0f, 50.0f, 0.0f }, Vector3Zero(), WHITE, primary_instancing_shader);
    CreateLight(LIGHT_DIRECTIONAL, { 50.0f, 50.0f, 0.0f }, Vector3Zero(), WHITE, primary_shader);

    Shaders::Register("primary_instanced", std::make_shared<Shader>(primary_instancing_shader));
    Shaders::Register("primary", std::make_shared<Shader>(primary_shader));

    // Set shader value: ambient light level
    //int ambientLoc = GetShaderLocation(shader, "ambient");
    //SetShaderValue(shader, ambientLoc, { 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Create one light
    //CreateLight(LIGHT_DIRECTIONAL, (Vector3) { 50.0f, 50.0f, 0.0f }, Vector3Zero(), WHITE, shader);

    //ResourceManager<Material>;

    // create yellow debug material
    Material debug_yellow = LoadMaterialDefault();
    
    // set the material's colour to the colour desired.
    debug_yellow.maps->color = {200, 200, 25, 255};
    typedef ResourceManager<Material> Materials;
    Materials::Register("Debug Yellow", std::make_shared<Material>(debug_yellow));

    Material debug_logic = LoadMaterialDefault();

    debug_logic.maps->color = { 255, 25, 25, 255 };
    Materials::Register("debug_logic", std::make_shared<Material>(debug_logic));

    /*
        Other setup
    */
    setup_logic_gate_io();
    setup_logic_pipe();
    setup_logic_gate();

    //SetConfigFlags(FLAG_MSAA_4X_HINT);

    //ToggleFullscreen();

    DisableCursor();

    SetTargetFPS(144);   // Set our game to run at 144 frames-per-second
    //--------------------------------------------------------------------------------------

    //raylib::Vector3 origin(0.0f, 0.0f, .0f);
    //raylib::Camera3D camera(origin);
    /*camera = camera(0);
    camera.position = (raylib::Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (raylib::Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (raylib::Vector3){ 0.0f, 0.0f, 1.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;*/

    camera = { 0 };
    camera.position = { 0.0f, 5.0f, 5.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;

    int camera_mode = CAMERA_FREE;

    (*renderer).setCameraMode(camera_mode);

    /*Vector3 pos1 = {0.0f, 0.0f, 0.0f};
    Vector3 pos2 = { 10.0f, 10.0f, 10.0f };

    Arrow arrow = Arrow(pos1, pos1, 10.0f, 0.2f, RED);

    //RenderObject renderer_object1 = RenderObject(arrow.line_mesh);
    //RenderObject renderer_object2 = RenderObject();
    
    renderer.object_renderer.addObject(arrow.render_object);*/

    //renderer.object_renderer.addObject(renderer_object2);


    /*
        Initial Object Creation
    */
    float surface_size = 50.0f;
    float grid_spacing = 1.0f;
    float surface_size_h = surface_size / 2.0f;

    /*Matrix demo_matrix = MatrixTranslate(10.0f, 0.0f, 0.0f);

    std::cout << "Non Rotated Matrix:\n";

    MatrixPrint(demo_matrix);

    demo_matrix = MatrixMultiply(MatrixRotateXYZ({ 0.0f, 0.0f, PI * 0.5 }), demo_matrix);

    std::cout << "Rotated Matrix (z rotation = PI * 0.5):\n";

    MatrixPrint(demo_matrix);*/

    //std::vector<Matrix> result_matrix;

    //for (int i = 0; i < 10; i += 1) {

    //    //result_matrix.push_back();

    //    //Matrix* matrix_instance = new Matrix();
    //    Matrix* matrix_instance = new Matrix(MatrixTranslate(i, 0.0f, 0.0f));

    //    std::cout << "Matrix Memory Address: " << &matrix_instance << "\n";
    //}

    std::vector<Rect> cubes;

    std::vector<Matrix> matrices;

    

  //  for (float i = 0; i < PI * 2; i += 0.001f) {

  //      /*float x = 10.0f;
  //      float y = 0.0f;
  //      float z = 10.0f;

  //      Matrix rotation_matrix = MatrixRotateXYZ({ i, i, i });

  //      Matrix result_matrix = MatrixMultiplyRotation(MatrixTranslate(x, y, z), rotation_matrix);*/

  //      //Vector3 v = RotateAtDistance({ 0.0f, 0.0f, i }, 10.0f);

  //      Vector3 v = RotateAtDistance({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, i });

  //      Matrix result_matrix = MatrixTranslate(v.x, v.y, v.z);

  //      SMaterial debug_yellow_material = Materials::Get("Debug Yellow");

  //      // create a cube
  //      Rect cube = Rect(
  //          result_matrix,
		//	0.05f,
  //          0.05f,
  //          0.05f,
  //          Materials::Get("Debug Yellow")
		//);

  //      cubes.push_back(cube);

  //      renderer.object_renderer.addObject(cube.render_object);
  //  }

    /*Rect cube0 = cubes[0];
    Rect cube1 = cubes[1];

    // check if their render object matrix is the same, this shouldn't be the case
    if (cube0.render_object.meshes_data[0].matrix->m12 == cube1.render_object.meshes_data[0].matrix->m12) {
        std::cout << "\033[91mCube Mesh Matrices are the same! This should not be the case!\033[0m\n";

        // check if their volume matricies are different, if they are differnet, then this is not a co-incidence.
        if (MatrixFromSMatrix(cube0.matrix).m12 != MatrixFromSMatrix(cube1.matrix).m12) {
            std::cout << "\033[96mCube Matrices are different! So this is not a co-incedence!\033[0m\n";
        }
        else {
            std::cout << "\033[93mCube Matrices are the same! This is a co-incedence! Disregard the above message.\033[0m\n";
        }
    }*/

    bool normal_draw_and_gate = false;

    if (normal_draw_and_gate) {
        AndGate and_gate = AndGate(std::make_shared<Matrix>(MatrixFromVector3({ 0.0f, 0.0f, 0.0f })), &*renderer);
    }


    //and_gates = ;
    bool draw_lots_and_gate = true;

    

    auto setupAndGates = [](float surface_size_h, SRenderer renderer) {
        for (float x = -surface_size_h * 1.5; x < surface_size_h; x += 1.75f) {
            for (float z = -surface_size_h * 1.5; z < surface_size_h; z += 1.75f) {
                and_gates.push_back(AndGate(std::make_shared<Matrix>(MatrixFromVector3({ x, 0.0f, z })), &*renderer));
                //break;
            }
            //break;
        }
    };

    //std::thread andGateSetupThread(setupAndGates, surface_size_h, renderer);

    //for (float x = surface_size_h; x > -surface_size_h * 5; x -= 1.75f) {
    //    for (float z = surface_size_h; z > -surface_size_h * 5; z -= 1.75f) {
    //        and_gates.push_back(AndGate({ x, 0.0f, z }, &*renderer));
    //        //break;
    //    }
    //    //break;
    //}

    AndGate and_gate1 = AndGate(std::make_shared<Matrix>(MatrixFromVector3({ 1.0f, 0.0f, 7.0f })), &*renderer);
    AndGate and_gate2 = AndGate(std::make_shared<Matrix>(MatrixFromVector3({ -1.0f, 0.0f, 7.0f })), &*renderer);
    AndGate and_gate3 = AndGate(std::make_shared<Matrix>(MatrixFromVector3({ 0.0f, 0.0f, 3.0f })), &*renderer);

    LogicPipe pipe1 = LogicPipe(and_gate1.getOutput("Output"), and_gate3.getInput("B"));
    LogicPipe pipe2 = LogicPipe(and_gate2.getOutput("Output"), and_gate3.getInput("A"));

    //LogicPipe pipe3 = LogicPipe(and_gate2.getOutput("Output"), and_gate2.getInput("A"));

    //for (float x = -surface_size_h * 24.2982213; x < surface_size_h; x += 2.0f) {
    //    for (float z = -surface_size_h * 24.2982213; z < surface_size_h; z += 2.0f) {
    //        and_gates.push_back(AndGate({ x, 0.0f, z }, &*renderer));
    //        //break;
    //    }
    //    //break;
    //}

    //int counter = 0;

    /*for (int x = 0; x < 447; x++) {
        for (int z = 0; z < 446; z++) {
            counter++;
        }
    }*/

    /*for (float x = surface_size_h; x > -surface_size_h * 5; x -= 1.75f) {
        for (float z = surface_size_h; z > -surface_size_h * 5; z -= 1.75f) {
            counter++;
        }
    }

    TraceLog(LOG_WARNING, ("Counter: " + std::to_string(counter)).c_str());*/

    /*for (int x = 0; x < 1000; x++) {
        for (int z = 0; z < 200; z++) {
            and_gates.push_back(AndGate({ x*-1.75f + surface_size_h, 0.0f, z*-1.75f + surface_size_h }, &*renderer));
        }
    }*/

    /*and_gate.setInputValue("A", 1.0f);
    and_gate.setInputValue("B", 1.0f);

    and_gate.evaluate();*/

    Matrix rotation_matrix = MatrixRotateXYZ({ 1, 2, 3 });
    //std::cout << "Rotation Matrix:\n";
    //MatrixPrint(rotation_matrix);

    //std::cout << "Rotated Matrix: \n> m12: " << demo_matrix.m12 << "\n> m13: " << demo_matrix.m13 << "\n> m14: " << demo_matrix.m14 << "\n";

    // Draw axis lines
    float axis_line_thickness = 0.2f;

    // Draw x axis arrow
    Arrow x_axis_arrow = Arrow(
        {-surface_size_h, 0, surface_size_h },
        { 0.0f, 0.0f, PI * -0.5 },
        surface_size,
        axis_line_thickness,
        { 200, 25, 25, 200 }
    );

    /*Vector3 v = RotateAtDistance({0.0f, 0.0f, PI * -0.5}, surface_size);

    Matrix result_matrix = MatrixTranslate(v.x, v.y, v.z);

    Rect cube = Rect(
        result_matrix,
        0.5f,
        0.5f,
        0.5f,
        { 200, 200, 200, 255 }
    );

    renderer.object_renderer.addObject(cube.render_object);*/

    (*renderer).object_renderer.addObject(x_axis_arrow.render_object);

    // Draw y axis arrow
    Arrow y_axis_arrow = Arrow(
        { surface_size_h, surface_size, surface_size_h },
        { 0.0f, 0.0f, 0.0f},
        surface_size,
        axis_line_thickness,
        { 25, 200, 25, 200 }
    );

    (*renderer).object_renderer.addObject(y_axis_arrow.render_object);

    // Draw z axis arrow
    Arrow z_axis_arrow = Arrow(
		{ surface_size_h, 0, -surface_size_h },
		{ PI * 0.5, 0.0f, 0.0f },
		surface_size,
		axis_line_thickness,
		{ 25, 25, 200, 200 }
	);

    (*renderer).object_renderer.addObject(z_axis_arrow.render_object);

    int colour_index = 0;
    int colour_speed = 5;

    float i = 0.0f;
    float i_step = 0.01f;
    int cubes_per_frame = 5;

    // Main game loop

    int tick_counter = 0;

    while (!window.ShouldClose())    // Detect window close button or ESC key
    {

        if (IsKeyPressed(KEY_R)) {
            //(*renderer).reloadWindow();
        }

        if (IsKeyPressed(KEY_G)) {
            and_gate1.setInputValue("A", 1.0f - and_gate1.getInputValue("A"));
            //and_gate.setInputValue("A", 1.0f - and_gate.getInputValue("A"));
            /*for (int and_gate_index = 0; and_gate_index < and_gates.size(); and_gate_index++) {
                and_gates[and_gate_index].setInputValue("A", 1.0f - and_gates[and_gate_index].getInputValue("A"));
            }*/
        }

        if (IsKeyPressed(KEY_H)) {
            and_gate1.setInputValue("B", 1.0f - and_gate1.getInputValue("B"));
            //and_gate.setInputValue("B", 1.0f - and_gate.getInputValue("B"));
            /*for (int and_gate_index = 0; and_gate_index < and_gates.size(); and_gate_index++) {
                and_gates[and_gate_index].setInputValue("B", 1.0f - and_gates[and_gate_index].getInputValue("B"));
            }*/
        }

        if (IsKeyPressed(KEY_T)) {
            and_gate2.setInputValue("A", 1.0f - and_gate2.getInputValue("A"));
        }

        if (IsKeyPressed(KEY_Y)) {
            and_gate2.setInputValue("B", 1.0f - and_gate2.getInputValue("B"));
        }

        if (IsKeyDown(KEY_U)) {

            SMatrix and_gate3_pos = and_gate3.getPosition();

            and_gate3_pos->m13 += 0.01f;

            and_gate3.setPosition(and_gate3_pos);

            //Vector3* and_gate3_pos = and_gate3.getPosition();

            //and_gate3.setPosition({ and_gate3_pos->x, and_gate3_pos->y + 0.01f, and_gate3_pos->z });
        }

        if (IsKeyDown(KEY_J)) {
            SMatrix and_gate3_pos = and_gate3.getPosition();

            and_gate3_pos->m13 -= 0.01f;

            and_gate3.setPosition(and_gate3_pos);

            //and_gate3.setPosition({ and_gate3_pos->x, and_gate3_pos->y - 0.01f, and_gate3_pos->z });
        }

        if (IsKeyDown(KEY_I)) {
            SMatrix and_gate3_matrix = and_gate3.getPosition();

            and_gate3_matrix = std::make_shared<Matrix>(MatrixMultiply(MatrixScale(1.001f, 1.001f, 1.001f), *and_gate3_matrix));

            and_gate3.setPosition(and_gate3_matrix);
        }

        if (IsKeyDown(KEY_K)) {
            SMatrix and_gate3_matrix = and_gate3.getPosition();

            and_gate3_matrix = std::make_shared<Matrix>(MatrixMultiply(MatrixScale(0.999f, 0.999f, 0.999f), *and_gate3_matrix));

            and_gate3.setPosition(and_gate3_matrix);
        }

        pipe1.update();
        pipe2.update();
        //pipe3.update();

        and_gate1.evaluate();
        and_gate2.evaluate();
        and_gate3.evaluate();

        (*renderer).draw(&camera);

        x_axis_arrow.updatePosition({ -surface_size_h, 0, surface_size_h }, { 0.0f, 0.0f, PI * -0.5 });
        y_axis_arrow.updatePosition({ surface_size_h, 0, surface_size_h }, { 0.0f, 0.0f, 0.0f });
        z_axis_arrow.updatePosition({ surface_size_h, 0, -surface_size_h }, { PI * 0.5, 0.0f, 0.0f });

        if ((tick_counter) % 4 == 0) {
            /*for (int and_gate_index = and_gates.size() - 1; and_gate_index > -1; and_gate_index--) {
            
                and_gates[and_gate_index].evaluate();
            }*/
        }

        if(colour_index == 0){
            debug_yellow.maps->color.b = std::max(debug_yellow.maps->color.b - colour_speed, 0);
            debug_yellow.maps->color.r = std::min(debug_yellow.maps->color.r + colour_speed, 255);
            if(debug_yellow.maps->color.r == 255){
                colour_index = 1;
			}
		} else if (colour_index == 1) {
            debug_yellow.maps->color.r = std::max(debug_yellow.maps->color.r - colour_speed, 0);
			debug_yellow.maps->color.g = std::min(debug_yellow.maps->color.g + colour_speed, 255);
			if(debug_yellow.maps->color.g == 255){
                colour_index = 2;
            }
        }
        else if (colour_index == 2) {
            debug_yellow.maps->color.g = std::max(debug_yellow.maps->color.g - colour_speed, 0);
            debug_yellow.maps->color.b = std::min(debug_yellow.maps->color.b + colour_speed, 255);
            if (debug_yellow.maps->color.b == 255) {
                colour_index = 0;
			}
        }

        tick_counter++;

        /*if (i < PI * 2) {
            for (int cube_index = 0; cube_index < cubes_per_frame; cube_index++) {
                /*float x = 10.0f;
                float y = 0.0f;
                float z = 10.0f;

                Matrix rotation_matrix = MatrixRotateXYZ({ i, i, i });

                Matrix result_matrix = MatrixMultiplyRotation(MatrixTranslate(x, y, z), rotation_matrix);*/

                //Vector3 v = RotateAtDistance({ 0.0f, 0.0f, i }, 10.0f);

                /*Vector3 r = {0.0f, 0.0f, i};
                Vector3 cr = { 0.0f, 0.0f, i - PI / 4 };

                float size = 1.0f;

                Vector3 v = RotateAtDistance({ -size, -size, -size }, { size, size, size }, r);

                Matrix result_matrix = MatrixTranslate(v.x, v.y, v.z);

                result_matrix = MatrixMultiply(MatrixRotateXYZ(cr), result_matrix);

                SMaterial debug_yellow_material = Materials::Get("Debug Yellow");

                // create a cube
                Rect cube = Rect(
                    result_matrix,
                    0.05f,
                    0.05f,
                    0.05f,
                    Materials::Get("Debug Yellow")
                );

                cubes.push_back(cube);

                renderer.object_renderer.addObject(cube.render_object);

                i += i_step;
            }
        }*/

        
    }

    return 0;
}