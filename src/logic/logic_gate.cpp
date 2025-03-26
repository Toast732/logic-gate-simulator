#include "logic_gate.hpp"

typedef std::vector<LogicIO> Inputs;
typedef std::vector<LogicIO> Outputs;

#define MESH_GROUP_IDENTIFIER_BASE 0
#define MESH_GROUP_IDENTIFIER_BODY 1

inline Vector3 const base_scale{ 1.0f, 0.075f, 1.0f };
inline Vector3 const body_scale{ 0.95f, 0.5f, 0.95f };

/*template<typename T>
class LogicGate {
public:
    LogicGate(std::vector<T> inputs, std::vector<T> outputs) : inputs_(inputs), outputs_(outputs) {}

    virtual T evaluate() = 0;

protected:
    std::vector<T> inputs_;
    std::vector<T> outputs_;
};

class AndGate : public LogicGate<bool> {
public:
    AndGate(bool input1, bool input2) : LogicGate<bool>({ input1, input2 },
        { false }) {}

    bool evaluate() override {
        bool result = true;
        for (auto input : inputs_) {
            result &= input;
        }
        return result;
    }
};*/

inline void setup_logic_gate() {

    // create the logic base instanced mesh
    CreateInstancedMesh(
		"logic_base",
		GenMeshCube(base_scale.x, base_scale.y, base_scale.z),
		{ 200, 200, 200, 255 }
	);

    // logic body mesh
    Mesh logic_body_mesh = GenMeshCube(body_scale.x, body_scale.y, body_scale.z);

    // create the logic body on instanced mesh
    CreateInstancedMesh(
        "logic_body_on",
        logic_body_mesh,
        { 46, 109, 125, 200 }
    );

    // create the logic body off instanced mesh
    CreateInstancedMesh(
		"logic_body_off",
		logic_body_mesh,
		{ 125, 125, 125, 125 }
	);
}

// converts a boolean into a float, true is 1.0f, false is 0.0f.
inline float boolToFloat(bool value) {
    return value ? 1.0f : 0.0f;
}

// the percentage of the size of the object to use as a margin/spacing for the edges of the IO connections.
const float IO_MARGIN_PERCENT = 0.1f;



class LogicGate {
    public:
        LogicGate(Inputs inputs, Outputs outputs, Matrix matrix) {
            this->inputs = inputs;
            this->outputs = outputs;

            float length = float(std::max(inputs.size(), outputs.size()));

            // set the scale
            Vector3 scale = {
                length,
				1.0f,
                2.0f
			};

            //scale.x = 1.0f;
            //scale.z = 1.0f;

            // set the matrix scale
            /*matrix = MatrixMultiply(
                MatrixScale(scale.x, scale.y, scale.z),
                matrix
            );*/

            Matrix scale_matrix = MatrixScale(scale.x, scale.y, scale.z);

            // set position
            this->setPosition(std::make_shared<Matrix>(matrix));

            // set scale
            this->scale = scale;

            // create the mesh group
            this->mesh_group = MeshGroup(&this->matrix);

            this->mesh_group.addInstancedMesh(
                MESH_GROUP_IDENTIFIER_BASE,
                "logic_base",
                std::make_shared<Matrix>(scale_matrix)
            );

            // add the body
            this->mesh_group.addInstancedMesh(
                MESH_GROUP_IDENTIFIER_BODY,
                "logic_body_off",
                std::make_shared<Matrix>(MatrixMultiply(scale_matrix, MatrixTranslate(0.0f, 0.25f, 0.0f)))
            );

            // set the mesh group of the logic gates

            // set inputs
            for (int input_index = 0; input_index < this->inputs.size(); input_index++) {
				this->inputs[input_index].setMeshGroup(&this->mesh_group);
			}

            // set outputs
            for (int output_index = 0; output_index < this->outputs.size(); output_index++) {
                this->outputs[output_index].setMeshGroup(&this->mesh_group);
			}

            // set the input's positions and to render
            this->setLogicRendering(&this->inputs);

            // set the output's positions and to render
            this->setLogicRendering(&this->outputs, true);

            /* 
                get their local coordinates
            */
   //         float margin = scale.x * IO_MARGIN_PERCENT;

   //         //margin = 0.0f;

   //         float input_spacing = (scale.x-margin*2) / (std::max(1, int(this->inputs.size() - 1)));
   //         
   //         // set their positions
   //         for (int input_index = 0; input_index < this->inputs.size(); input_index++) {
   //             this->inputs[input_index].setPosition({
   //                 margin + (input_spacing) * input_index - scale.x/2 ,
			//		float(scale.y*0.5),
			//		scale.z/2
			//	});

   //             //std::cout << "Vector3.x: " << this->inputs[input_index].getPosition().x << "\n";

   //             this->inputs[input_index].createDraw();
   //             this->inputs[input_index].setRender();
			//}

			//// set the positions of the outputs

			///* 
   //             get their local coordinates
   //         */

   //         int outputs_size = this->outputs.size();
   //         float output_spacing = (scale.x - margin * 2) / (std::max(1, int(this->outputs.size() - 1)));
			//
			///*
   //             set their positions
   //         */

   //         // if theres more than one output, set them all to the same spacing.
   //         if (outputs_size > 1) {
   //             for (int output_index = 0; output_index < this->outputs.size(); output_index++) {
   //                 this->outputs[output_index].setPosition({
   //                     margin + (output_spacing)*output_index - scale.x / 2 ,
   //                     float(scale.y * 0.5),
   //                     float(scale.z * -0.5)
   //                     });

   //                 this->outputs[output_index].createDraw();
   //                 this->outputs[output_index].setRender();
   //             }
   //         } // otherwise, set it to be centred. Double check if theres even any outputs.
   //         if (outputs_size == 1) {

   //             // get the output IO
   //             LogicIO output = this->outputs[0];

   //             // set it's position
   //             output.setPosition({
   //                 float(scale.x * -0.5),
   //                 float(scale.y * 0.5),
   //                 float(scale.z * -0.5)
   //             });

   //             // create it's draw object.
   //             this->outputs[0].createDraw();

   //         }
        }

        void updateRenderPositions() {
			this->mesh_group.updateGlobalPosition();
		}

        LogicGate copy() {
            return *this;
		}

        void setInputValue(std::string logic_name, float value) {

            //std::cout << "Setting input " << logic_name << " to " << value << "\n";
            
            // search inputs
            LogicIO* input = getInputByName(logic_name);
            // I need to figure out how to check if we failed to find it, if we get invalid logic name, this will error.
            (*input).setValue(value);
        }

        float getInputValue(std::string logic_name) {
            // search inputs
            LogicIO* input = getInputByName(logic_name);
            // I need to figure out how to check if we failed to find it, if we get invalid logic name, this will error.
            return (*input).getValue();
        }

        LogicIO* getInput(std::string logic_name) {
            // search inputs
            LogicIO* input = getInputByName(logic_name);
            // I need to figure out how to check if we failed to find it, if we get invalid logic name, this will error.
            return input;
        }

        LogicIO* getOutput(std::string logic_name) {
            // search outputs
            LogicIO* output = getOutputByName(logic_name);
            // I need to figure out how to check if we failed to find it, if we get invalid logic name, this will error.
            return output;
        }

        void setPosition(SMatrix matrix) {
            this->matrix = matrix;
            this->updateRenderPositions();

            // update logic io positions
            //this->updateLogicIOPos(&this->inputs, position);
            //this->updateLogicIOPos(&this->outputs, position);
        }

        // overload when not sent as a pointer.
        /*void setPosition(SMatrix matrix) {
            this->setPosition(matrix);
        }*/

        SMatrix getPosition() {
            return this->matrix;
        }

        virtual void evaluate() {};
        //virtual void updateRenderPositions() {};
    private:

        LogicIO* getInputByName(std::string logic_name) {
            // search inputs
            for (int i = 0; i < inputs.size(); i++) {
                if (inputs[i].getName() == logic_name) {
                    return &inputs[i];
                }
            }
        }
        LogicIO* getOutputByName(std::string logic_name) {
            // search outputs
            for (int i = 0; i < outputs.size(); i++) {
                if (outputs[i].getName() == logic_name) {
                    return &outputs[i];
                }
            }
        }
        // sets a logic group as rendering, automatically setting positions, creating draw objects and then setting them to render.
        void setLogicRendering(std::vector<LogicIO> *logic_group, bool is_output = false) {
            int group_size = (*logic_group).size();

            //std::cout << "logic group: " << logic_group << "\n";

            // check if group is empty, if so, return.
            if (group_size == 0) {
                // return early, as the group is empty.
                return;
            }

            // set the z multiplier, 0.5 if it's an input, -0.5 if it's an output.
            float z_multiplier = is_output ? -0.5f : 0.5f;

            // check if the group only contains one, as we can skip some of the calculations if so.
            if (group_size == 1) {
                // get the IO
                LogicIO l_io = (*logic_group)[0];

                // set it's position
                /*l_io.setOffset({
                    this->position->x,
                    float(this->scale.y * 0.5) + this->position->y + 0.05f,
                    float(this->scale.z * z_multiplier) + this->position->z
                });*/
                l_io.setOffset({
                    0.0f,
                    float(this->scale.y * 0.5) + 0.05f,
                    float(this->scale.z * z_multiplier)
                });

                // create it's draw object.
                l_io.createDraw();

                // set it rendering
                l_io.setRender();

                (*logic_group)[0] = l_io;

                return /*logic_group*/;
            }

            // we've got more than 1 IO to draw, so give them all the same spacing.

            // get the margin so we dont draw too close to the edges.
            float margin = this->scale.x * IO_MARGIN_PERCENT;

            // get the spacing.
            float io_spacing = (this->scale.x - margin * 2) / (group_size - 1);

            /*
                set their positions
            */

            // iterate through all of the IO.
            for (int group_index = 0; group_index < group_size; group_index++) {
                // get the IO
                LogicIO l_io = (*logic_group)[group_index];

                // set it's position
                //l_io.setOffset({
                //    float(margin + io_spacing*group_index - this->scale.x * 0.5 ) + this->position->x, // x
                //    float(this->scale.y * 0.5) + this->position->y + 0.05f, // y
                //    float(this->scale.z * z_multiplier) + this->position->z // z
                //});

                l_io.setOffset({
                    float(margin + io_spacing * group_index - this->scale.x * 0.5), // x
                    float(this->scale.y * 0.5) + 0.05f, // y
                    float(this->scale.z * z_multiplier) // z
                });

                l_io.createDraw();
                l_io.setRender();

                (*logic_group)[group_index] = l_io;
            }
        }

        void updateLogicIOPos(std::vector<LogicIO>* logic_group, Vector3* position) {
            int group_size = (*logic_group).size();

            for (int group_index = 0; group_index < group_size; group_index++) {
                (*logic_group)[group_index].setPosition(*position);
            }
        }
    protected:
        Inputs inputs;
        Outputs outputs;
        Vector3 scale;
        MeshGroup mesh_group;
        SMatrix matrix = std::make_shared<Matrix>(MatrixTranslate(0, 0, 0));
};

typedef ResourceManager<Material> Materials;

class AndGate : public LogicGate {
    private:
        //Rect rect;
        //Rect base;
        //SMaterial base_material;
        //SMaterial material;
        int base_instance_id;
        int body_instance_id;
        bool previous_value = false;

        SMatrix base_matrix = std::make_shared<Matrix>(MatrixTranslate(0, 0, 0));
        SMatrix body_matrix = std::make_shared<Matrix>(MatrixTranslate(0, 0.25f, 0));
        bool instances_created = false;
    public:
        AndGate(Matrix matrix, Renderer* renderer) : LogicGate(
            {
                LogicIO("A", IO_TYPE_INPUT),
                LogicIO("B", IO_TYPE_INPUT)
            },
            {
				LogicIO("Output", IO_TYPE_OUTPUT)
			},
            matrix
        ) {

            // set the position
            //this->setPosition(matrix);

            this->updateRenderPositions();

            // get the position
            //Vector3* pos = this->getPosition();

            // add the base
   //         this->mesh_group.addInstancedMesh(
   //             MESH_GROUP_IDENTIFIER_BASE,
			//	"logic_base",
			//	this->base_matrix
			//);

   //         // add the body
   //         this->mesh_group.addInstan
   // 
   // 
   // 
   //         cedMesh(
   //             MESH_GROUP_IDENTIFIER_BODY,
   //             "logic_body_off",
   //             this->body_matrix
   //         );

            //this->base_instance_id = MeshInstances::Get("logic_base")->addInstance(this->base_matrix);

            //this->body_instance_id = MeshInstances::Get("logic_body_off")->addInstance(this->body_matrix);

            this->instances_created = true;
        }

        /*void updateRenderPositions() {
            this->mesh_group.updateGlobalPosition();

            ///*
            //    Update body position
            //*/
            //this->body_matrix->m12 = pos->x; // x position
            //this->body_matrix->m13 = pos->y + 0.25f; // y position
            //this->body_matrix->m14 = pos->z; // z position

            //if (this->instances_created) {
                /*MeshInstances::Get("logic_base")->moveInstancePos(this->base_instance_id, this->base_matrix);

                if (this->previous_value == true) {
                    MeshInstances::Get("logic_body_on")->moveInstancePos(this->body_instance_id, this->body_matrix);
                }
                else {
                    MeshInstances::Get("logic_body_off")->moveInstancePos(this->body_instance_id, this->body_matrix);
                }*/
            //}
        //}

        void evaluate() override {


            bool output0 = true;

            for (int input_index = 0; input_index < inputs.size(); input_index++) {
                if (inputs[input_index].getValue() != 1.0f) {
                    output0 = false;
                    break;
                }
            }
            //bool output0 = (inputs[0].getValue() + inputs[1].getValue()) == 2.0f;

            outputs[0].setValue(boolToFloat(output0));

            //std::cout << inputs[0].getValue() << " + " << inputs[1].getValue() << " = 2.0f : " << output0 << "\n";

            // skip if the value has not changed.
            if (output0 == this->previous_value) {
                return;
            }

            // set the previous value to the current value.
            this->previous_value = output0;

            ///if (output0) {
            //    // turning on
            //    this->mesh_group.replaceInstancedMesh(
            //        MESH_GROUP_IDENTIFIER_BODY,
            //        "logic_body_on"
            //    );
            //}
            //else {
            //    // turning off
            //    this->mesh_group.replaceInstancedMesh(
            //        MESH_GROUP_IDENTIFIER_BODY,
            //        "logic_body_off"
            //    );
            //}

            //this->material.maps->color = { 150, 150, 150, 100 };
        }
};

class NOTGate : public LogicGate {
private:
    int base_instance_id;
    int body_instance_id;
    bool previous_value = false;

    SMatrix base_matrix = std::make_shared<Matrix>(MatrixTranslate(0, 0, 0));
    SMatrix body_matrix = std::make_shared<Matrix>(MatrixTranslate(0, 0.25f, 0));
    bool instances_created = false;
public:
    NOTGate(Matrix matrix, Renderer* renderer) : LogicGate(
        {
            LogicIO("A", IO_TYPE_INPUT),
        },
        {
            LogicIO("Output", IO_TYPE_OUTPUT)
        },
        matrix
    ) {

        // set the position
        //this->setPosition(matrix);

        this->updateRenderPositions();

        // get the position
        //Vector3* pos = this->getPosition();

        // add the base
        //this->mesh_group.addInstancedMesh(
        //    MESH_GROUP_IDENTIFIER_BASE,
        //    "logic_base",
        //    this->base_matrix
        //);

        //// add the body
        //this->mesh_group.addInstancedMesh(
        //    MESH_GROUP_IDENTIFIER_BODY,
        //    "logic_body_off",
        //    this->body_matrix
        //);

        //this->base_instance_id = MeshInstances::Get("logic_base")->addInstance(this->base_matrix);

        //this->body_instance_id = MeshInstances::Get("logic_body_off")->addInstance(this->body_matrix);

        this->instances_created = true;
    }

    void updateRenderPositions() {
        this->mesh_group.updateGlobalPosition();
    }

    void evaluate() override {

        bool output0 = inputs[0].getValue() != 1.0f;

        outputs[0].setValue(boolToFloat(output0));

        if (output0 == this->previous_value) {
            return;
        }

        this->previous_value = output0;

        //if (output0) {
        //    // turning on
        //    this->mesh_group.replaceInstancedMesh(
        //        MESH_GROUP_IDENTIFIER_BODY,
        //        "logic_body_on"
        //    );
        //}
        //else {
        //    // turning off
        //    this->mesh_group.replaceInstancedMesh(
        //        MESH_GROUP_IDENTIFIER_BODY,
        //        "logic_body_off"
        //    );
        //}
    }
};

class ORGate : public LogicGate {
private:
    int base_instance_id;
    int body_instance_id;
    bool previous_value = false;

    SMatrix base_matrix = std::make_shared<Matrix>(MatrixTranslate(0, 0, 0));
    SMatrix body_matrix = std::make_shared<Matrix>(MatrixTranslate(0, 0.25f, 0));
    bool instances_created = false;
public:
    ORGate(Matrix matrix, Renderer* renderer) : LogicGate(
        {
            LogicIO("A", IO_TYPE_INPUT),
            LogicIO("B", IO_TYPE_INPUT)
        },
        {
            LogicIO("Output", IO_TYPE_OUTPUT)
        },
        matrix
    ) {

        // set the position
        //this->setPosition(matrix);

        this->updateRenderPositions();

        // add the base
        this->mesh_group.addInstancedMesh(
            MESH_GROUP_IDENTIFIER_BASE,
            "logic_base",
            this->base_matrix
        );

        // add the body
        this->mesh_group.addInstancedMesh(
            MESH_GROUP_IDENTIFIER_BODY,
            "logic_body_off",
            this->body_matrix
        );

        this->instances_created = true;
    }

    void updateRenderPositions() {
        this->mesh_group.updateGlobalPosition();
    }

    void evaluate() override {

        bool output0 = false;

        for (int input_index = 0; input_index < inputs.size(); input_index++) {
            if (inputs[input_index].getValue() == 1.0f) {
                output0 = true;
                break;
            }
        }

        outputs[0].setValue(boolToFloat(output0));

        if (output0 == this->previous_value) {
            return;
        }

        this->previous_value = output0;

        //if (output0) {
        //    // turning on
        //    this->mesh_group.replaceInstancedMesh(
        //        MESH_GROUP_IDENTIFIER_BODY,
        //        "logic_body_on"
        //    );
        //}
        //else {
        //    // turning off
        //    this->mesh_group.replaceInstancedMesh(
        //        MESH_GROUP_IDENTIFIER_BODY,
        //        "logic_body_off"
        //    );
        //}
    }
};
