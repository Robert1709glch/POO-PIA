
#include <engine/utils.h>
#include <engine/variables.h>
#include <Windows.h>
#include "Class.h"


//:::: CALLBACKS  Y FUNCIONES :::://
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void collidedObject_callback(string nameCollidedObject);

void joystick_callback(int jid, int event);
void processInput(GLFWwindow *window);
void rotateCamInput(double xoff, double yoff);
void actionKeys(GLFWwindow *window);

void setMultipleLight(Shader *shader, vector<glm::vec3> pointLightPositions);
void setSimpleLight(Shader *shader);
void drawModels(Shader *shader, glm::mat4 view, glm::mat4 projection);
void loadEnviroment(Terrain *terrain, SkyBox *sky, glm::mat4 view, glm::mat4 projection);
void initScene(Shader ourShader);
void collisions();

//=======AGUA ANIMATION=========//
void loadAnim(glm::mat4 view, glm::mat4 projection);

//JOYSTICK CALLBACK
void joystick_callback(int jid, int event)
{

    if (event == GLFW_CONNECTED)
    {
        isJoyStick = true;
        cout << "Se ha conectado un control" << endl;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        // The joystick was disconnected
        isJoyStick = false;
        cout << "Se ha desconectado el control" << endl;
    }
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    GLFWgamepadstate state;
    // The joystick was connected
    if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
    {
        isJoyStick = true;
        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
        {

            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] == -1)
            {
                camera.ProcessKeyboard(FORWARD, deltaTime);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] == 1)
            {
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            }

            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] == 1)
            {
                xJoy += 50.5;
                rotateCamInput(xJoy, yJoy);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] == -1)
            {
                xJoy -= 50.5;
                rotateCamInput(xJoy, yJoy);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] == 1)
            {
                yJoy += 50.5;
                rotateCamInput(xJoy, yJoy);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] == -1)
            {
                yJoy -= 50.5;
                rotateCamInput(xJoy, yJoy);
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_X] == 1) {
                //Instruction to pick an object(keys)
                isPick = true;
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_START] == 1) {
                MessageBox(NULL, L"Menu pausa\nPulse enter o un boton para continuar.", L"PAUSE", MB_ICONHAND || MB_ICONHAND);
            }
        }
    }
    else
    {
        //MEN??S
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        {
            int menu = (int)typemenu;
            if (menu < 4)
                menu += 1;
            else
                menu = 0;
            typemenu = (TypeActionKeyBoard)menu;
            system("cls");
            if (typemenu == GAME)
                cout << "\nMenu GAME seleccionado..." << endl;
            else if (typemenu == OBJECTS)
                cout << "\nMenu OBJECTS seleccionado..." << endl;
            else if (typemenu == COLLISION)
                cout << "\nMenu COLLISION seleccionado..." << endl;
            else if (typemenu == LIGHTING)
                cout << "\nMenu LIGHTING seleccionado..." << endl;
         
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            skyPos += camera.Front * glm::vec3(2);
            camera.ProcessKeyboard(FORWARD, deltaTime);
            colDetection = 25.0;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.notFrontMove = false;
            camera.ProcessKeyboard(BACKWARD, deltaTime);
            skyPos -= camera.Front * glm::vec3(2);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            skyPos -= camera.Right * glm::vec3(2);
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            skyPos -= camera.Right * glm::vec3(2);
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        /////////////////////////////////////
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {//PAUSE
            //MessageBox(NULL, L"Menu pausa\nPulse enter o un boton para continuar.", L"PAUSE", MB_ICONHAND || MB_ICONHAND);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {//AGARRAR OBJETOS
            isPick = true;
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {//ACTIVAR MOVIMIENTO DEL ENEMIGO
            if (isMoving == true) {
                isMoving = false;
                MessageBox(NULL, L"Enemigo sin mover", L"STOP", MB_ICONHAND | MB_ICONHAND);
            }
            else if (isMoving == false) {
                isMoving = true;
                MessageBox(NULL, L"Enemigo en movimiento", L"MOVE", MB_ICONHAND | MB_ICONHAND);
            }
        }
        /////////////////////////////////////

        isJoyStick = false;
        actionKeys(window);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}
void actionKeys(GLFWwindow *window)
{   
    if (typemenu == OBJECTS || typemenu == COLLISION)
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if (movement < 0.2)
                movement += 0.01;
            else
                movement = 0.0;
            system("cls");
            cout << "\nEl factor de movimiento es: " << movement << endl;
        }
    }

    if (typemenu == GAME)
    {
       //ACCIONES DE USTEDES
    }
    else if (typemenu == OBJECTS)
    {
        //::::::::::::::MOVER OBJETOS:::::::::::::://

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            if (indexObject < models.size()-1)
            {
                indexObject += 1;
            }
            else
            {
                indexObject = 0;
            }

            system("cls");
            cout << "\nModelo seleccionado: " << models[indexObject].name << endl;
        }

        //TRANSLACI??N DEL MODELO X
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.x -= movement;
            models[indexObject].setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.x += movement;
            models[indexObject].setPosition(posModel);
        }
        //TRANSLACI??N DEL MODELO Y
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.y -= movement;
            models[indexObject].setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.y += movement;
            models[indexObject].setPosition(posModel);
        }
        //TRANSLACI??N DEL MODELO Z
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.z -= movement;
            models[indexObject].setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.z += movement;
            models[indexObject].setPosition(posModel);
        }
        //ROTACI??N DEL MODELO X
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.x -= 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.x += 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        //ROTACI??N DEL MODELO Y
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.y -= 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.y += 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        //ROTACI??N DEL MODELO Z
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.z -= 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.z += 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        //IMPRIMIR EN CONSOLA
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            cout << "\nfloat initScale = 0.2f;";
            for (int i = 0; i < models.size(); i++)
            {
                glm::vec3 mPos = models[i].getPosition();
                glm::vec3 mRot = models[i].getAngles();
                float tx = mPos.x;
                float ty = mPos.y;
                float tz = mPos.z;

                float rx = mRot.x;
                float ry = mRot.y;
                float rz = mRot.z;
                cout << "\nmodels.push_back(Model(\"" << models[i].name << "\",\"" << models[i].path << "\", glm::vec3(" << tx << ", " << ty << "," << tz << "), glm::vec3(" << rx << ", " << ry << "," << rz << "), 0.0f, initScale));" << endl;
            }
        }
    }
    else if (typemenu == LIGHTING)
    {
        //::::::::::::::MOVER LUCES:::::::::::::://
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            const char *lightTypes[] = {"FLAT", "ALL", "DIRECTIONAL", "SPOT", "POINT"};
            int lt = (int)lightType;
            lt += 1;
            if (lt > 4)
                lt = 0;
            lightType = (LightType)lt;
            system("cls");
            cout << "\nEl tipo de luz seleccionada es : " << lightTypes[lt] << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            if (indexLight < pointLightPositions.size() - 1)
                indexLight += 1;
            else
                indexLight = 0;

            system("cls");
            cout << "\nLuz seleccionada: " << lightcubes.at(indexLight).first << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            renderLightingCubes = !renderLightingCubes;
            system("cls");
            if (renderLightingCubes)
                cout << "\nVisibilidad de light cubes activada..." << endl;
            else
                cout << "\nVisibilidad de light cubes desactivada..." << endl;
        }

        //TRANSLACI??N X
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].x -= 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].x += 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        //TRANSLACI??N Y
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].y -= 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].y += 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        //TRANSLACI??N Z
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].z -= 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].z += 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        //IMPRIMIR EN CONSOLA
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            for (int i = 0; i < pointLightPositions.size(); i++)
            {
                glm::vec3 mPos = pointLightPositions[i];
                float tx = mPos.x;
                float ty = mPos.y;
                float tz = mPos.z;
                cout << "\nglm::vec3(" << tx << ", " << ty << "," << tz << ");" << endl;
            }
        }
    }
    else if (typemenu == COLLISION)
    {
        int size = isCollBoxModel ? models.size() : collboxes.size();
        glm::vec3 transform(0);
        //::::::::::::::MOVER COLLISION BOX:::::::::::::://
        //SWITCH ENTRE COLLBOXES Y MODELS
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            isCollBoxModel = !isCollBoxModel;
            system("cls");
            if (isCollBoxModel)
            {
                indexCollBox = 0;
                transformObject = MODEL;
                cout << "\nSe ha seleccionado el movimiento de collbox de objetos..." << endl;
            }
            else
            {
                indexCollBox = 0;
                transformObject = COLLBOX;
                cout << "\nSe ha seleccionado el movimiento de collbox individuales..." << endl;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            renderCollBox = !renderCollBox;
            system("cls");
            if (renderCollBox)
                cout << "\nVisibilidad de collbox activada..." << endl;
            else
                cout << "\nVisibilidad de collbox desactivada..." << endl;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {      
            if (indexCollBox < size)
            {
                string name = isCollBoxModel ? models[indexCollBox].name : collboxes.at(indexCollBox).first;
                cb = isCollBoxModel ? &models[indexCollBox].collbox : &collboxes.at(indexCollBox).second;
                system("cls");
                cout << "\nCollision Box seleccionado: " << name << endl;
                indexCollBox += 1;
            }
            else
                indexCollBox = 0;
        }
        //ESCALA DEL COLLBOX X
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.x = (scaleCollBox.x < 0.5) ? 0.5 : scaleCollBox.x - movement;
            cb->setScale(scaleCollBox);
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.x += movement;
            cb->setScale(scaleCollBox);
        }
        //ESCALA DEL COLLBOX Y
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.y = (scaleCollBox.y < 0.5) ? 0.5 : scaleCollBox.y - movement;
            cb->setScale(scaleCollBox);
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.y += movement;
            cb->setScale(scaleCollBox);
        }
        //ESCALA DEL COLLBOX Z
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.z = (scaleCollBox.z < 0.5) ? 0.5 : scaleCollBox.z - movement;
            cb->setScale(scaleCollBox);
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.z += movement;
            cb->setScale(scaleCollBox);
        }
        //POSICI??N DEL COLLBOX X
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.x -= movement;
            cb->setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.x += movement;
            cb->setPosition(posModel);
        }
        //POSICI??N DEL COLLBOX Y
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.y -= movement;
            cb->setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.y += movement;
            cb->setPosition(posModel);
        }
        //POSICI??N DEL COLLBOX Z
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.z -= movement;
            cb->setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.z += movement;
            cb->setPosition(posModel);
        }
        cb->defineCollisionBox();
        //IMPRIMIR EN CONSOLA
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            cout << "\nCollisionBox collbox;" << endl;
           
            if (transformObject == COLLBOX)
            {
                int i = 0;
                for (pair<int, pair<string, CollisionBox>> colls : collboxes)
                {
                    string name = colls.second.first;
                    glm::vec3 mPos = colls.second.second.getPosition();
                    glm::vec3 mScale = colls.second.second.getScale();
                    cout << "\ncollbox = CollisionBox(glm::vec3(" << mPos.x << "," << mPos.y << "," << mPos.z << "), glm::vec3(" << mScale.x << "," << mScale.y << "," << mScale.z << "), glm::vec4(0.41f, 0.2f, 0.737f, 0.06f));"
                        << "\ncollboxes.insert(pair<int, pair<string, CollisionBox>>(" << i << ", pair<string, CollisionBox>(\"" << name << "\",collbox)));" << endl;
                    i++;
                }
            }
            else if (transformObject == MODEL)
            {
                for (int i=0; i<models.size(); i++)
                {
                    string name = models[i].name;
                    glm::vec3 mPos = models[i].collbox.getPosition();
                    glm::vec3 mScale = models[i].collbox.getScale();
                    cout << "\nmodels[" << i << "].collbox.setPosition(glm::vec3(" << mPos.x << "," << mPos.y << "," << mPos.z << "));"
                        << "\nmodels[" << i << "].collbox.setScale(glm::vec3(" << mScale.x << "," << mScale.y << "," << mScale.z << "));"
                        << "\nmodels[" << i << "].collbox.defineCollisionBox();" << endl;

                   
                }              
            }
            
           
        }
    }
}



void rotateCamInput(double xoff, double yoff)
{
    if (firstMouse)
    {
        lastX = xJoy;
        lastY = yJoy;
        firstMouse = false;
    }

    float xoffset = (xJoy - lastX) * 0.2;
    float yoffset = (lastY - yJoy) * 0.2; // reversed since y-coordinates go from bottom to top

    lastX = xJoy;
    lastY = yJoy;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (!isJoyStick)
    {

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    auto pos = camera.Position;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
       //ACCI??N
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


void collidedObject_callback(string nameCollidedObject)
{
    ENEMIGO1 enemigo1;
    ENEMIGO2 enemigo2;
    //SI OCURRE UNA COLISI??N HACEMOS ALGO
    if (nameCollidedObject == "key" && isPick == true) {
        n_key = true;
        glm::vec3 keyPosition = models[0].getPosition();
        keyPosition.x = 0;
        keyPosition.y = 0;
        keyPosition.z = 0;
        models[0].setPosition(keyPosition);
    }
    if (n_key == true && isPick == true) {
        MessageBox(NULL, L"1/3", L"LLAVE", MB_ICONHAND || MB_ICONHAND);
        n_key = false;
        isPick = false;
        if (isDoorOpen == false)
            isDoorOpen = true;
        keys += 1;
    }
    if (nameCollidedObject == "key2" && isPick == true) {
        n_key2 = true;
        glm::vec3 keyPosition2 = models[1].getPosition();
        keyPosition2.x = 0;
        keyPosition2.y = 0;
        keyPosition2.z = 0;
        models[1].setPosition(keyPosition2);
    }
    if (n_key2 == true && isPick == true) {
        MessageBox(NULL, L"2/3", L"LLAVE", MB_ICONHAND || MB_ICONHAND);
        n_key2 = false;
        isPick = false;
        if (isDoorOpen == false)
            isDoorOpen = true;
        keys += 1;
    }
    if (nameCollidedObject == "key3" && isPick == true) {
        n_key3 = true;
        glm::vec3 keyPosition3 = models[2].getPosition();
        keyPosition3.x = 0;
        keyPosition3.y = 0;
        keyPosition3.z = 0;
        models[2].setPosition(keyPosition3);
    }    
    if (n_key3 == true && isPick == true) {
        MessageBox(NULL, L"3/3\nESCAPA!", L"LLAVE", MB_ICONHAND || MB_ICONHAND);
        n_key3 = false;
        isPick = false;
        if(isDoorOpen == false)
            isDoorOpen = true;
        keys += 1;
    }
    if (nameCollidedObject == "puerta") {
        n_exit = true;
        if (n_exit == true && keys == 3) {
            MessageBox(NULL, L"3 llaves", L"GANASTE", MB_ICONHAND || MB_ICONHAND);
            n_exit = false;
            isDoorOpen = false;
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo") {
        enemigo1.ataque(timer);
        if (enemigo1.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo2") {
        /*if (timer != 0) {
            if (timer < 0) {
                timer = timer + 1;
                Sleep(7);
            }
            else {
                timer = timer - 1;
                Sleep(7);
            }
        }*/
        enemigo1.ataque(timer);
        if (enemigo1.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo3") {
        enemigo2.ataque(timer);
        /*if (hola.timer2 != 0) {
            if (hola.timer2 < 0) {
                hola.timer2 = hola.timer2 + 1;
                Sleep(7);
            }
            else {
                hola.timer2 = hola.timer2 - 1;
                Sleep(7);
            }
        }*/
        if (enemigo2.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo4") {
        /*if (hola.timer2 != 0) {
            if (hola.timer2 < 0) {
                hola.timer2 = hola.timer2 + 1;
                Sleep(7);
            }
            else {
                timer = timer - 1;
                Sleep(7);
            }
        }*/
        enemigo1.ataque(timer);
        if (enemigo1.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo5") {
        /*if (timer != 0) {
            if (timer < 0) {
                timer = timer + 1;
                Sleep(7);
            }
            else {
               timer = timer - 1;
                Sleep(7);
            }
        }*/
        enemigo2.ataque(timer);
        if (enemigo2.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo6") {
        /*if (timer != 0) {
            if (timer < 0) {
                timer = timer + 1;
                Sleep(7);
            }
            else {
                timer = timer - 1;
                Sleep(7);
            }
        }*/
        enemigo1.ataque(timer);
        if (enemigo1.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo7") {
        /*if (timer != 0) {
            if (timer < 0) {
                timer = timer + 1;
                Sleep(7);
            }
            else {
                timer = timer - 1;
                Sleep(7);
            }
        }*/
        enemigo2.ataque(timer);
        if (enemigo2.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    if (nameCollidedObject == "enemigo8") {
        /*if (timer != 0) {
            if (timer < 0) {
                timer = timer + 1;
                Sleep(7);
            }
            else {
                timer = timer - 1;
                Sleep(7);
            }
        }*/
        enemigo1.ataque(timer);
        if (enemigo1.timeOut == true) {
            MessageBox(NULL, L"Moriste", L"FIN DEL JUEGO", MB_ICONHAND || MB_ICONHAND);
            glfwTerminate();
            exit(0);
        }
    }
    /*if (nameCollidedObject == "techo") {
        MessageBox(NULL, L"techo jsjs", L"GANASTE", MB_ICONHAND || MB_ICONHAND);
    }*/
}