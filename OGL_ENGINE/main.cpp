
#include <GLFW/glfw3.h>
#include <engine/Billboard.h>
#include <engine/CollisionBox.h>
#include <engine/Objectives.h>
#include <engine/Particles.h>
#include <engine/Plane.h>
#include <engine/QuadTexture.h>
#include <engine/RigidModel.h>
#include <engine/Terrain.h>
#include <engine/functions.h>
#include <engine/shader_m.h>
#include <engine/skybox.h>
#include <engine/textrenderer.h>
#include <glad/glad.h>
#include <iostream>
#include <Windows.h>
#include "Class.h"
#include <engine/camera.h>
#include <ctime>
#include <time.h>

#define TIMER 100

int main()
{
    //:::: INICIALIZAMOS GLFW CON LA VERSIÓN 3.3 :::://
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //:::: CREAMOS LA VENTANA:::://
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SuperMarket Rescue", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //:::: OBTENEMOS INFORMACIÓN DE TODOS LOS EVENTOS DE LA VENTANA:::://
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetJoystickCallback(joystick_callback);

    //:::: DESHABILITAMOS EL CURSOR VISUALMENTE EN LA PANTALLA :::://
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //:::: INICIALIZAMOS GLAD CON LAS CARACTERÍSTICAS DE OPENGL ESCENCIALES :::://
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //INICIALIZAMOS LA ESCENA
    Shader ourShader("shaders/multiple_lighting.vs", "shaders/multiple_lighting.fs");
    Shader selectShader("shaders/selectedShader.vs", "shaders/lighting_maps.fs");
    initScene(ourShader);
    Terrain terrain("textures/heightmap.jpg", texturePaths);
    SkyBox sky(1.0f, "6");//===========CON ESTA LINEA SE CAMBIA LA CARPETA DEL SKYBOX================//
    cb = isCollBoxModel ? &models[indexCollBox].collbox : &collboxes.at(indexCollBox).second;

    //:::: RENDER:::://
    while (!glfwWindowShouldClose(window))
    {
        keyRot = 0;
        keyRot < 360 ? keyRot += 0.1f : keyRot = 0;
        //::::TIMING:::://Ayuda a crear animaciones fluidas
        float currentFrame = glfwGetTime();
        deltaTime = (currentFrame - lastFrame);
        lastFrame = currentFrame;
        respawnCount += 0.1;
        changeSky += 0.1;

        //::::ENTRADA CONTROL:::://
        processInput(window);
        //:::: LIMPIAMOS BUFFERS:::://
        glClearColor(0.933f, 0.811f, 0.647f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //:::: PASAMOS INFORMACIÓN AL SHADER:::://
        ourShader.use();

        //:::: DEFINICIÓN DE MATRICES::::// La multiplicaciónd e model*view*projection crea nuestro entorno 3D
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //:::: RENDER DE MODELOS:::://
        drawModels(&ourShader, view, projection);
        //:::: SKYBOX Y TERRENO:::://
        loadEnviroment(&terrain, &sky, view, projection);

        //:::: KEY ANIMATION :::://
        glm::vec3 keyPosition = models[0].getPosition();
        
        if (keyPosition.y <= 2.0f && !moveKey) {
            keyPosition.y += 0.10f * deltaTime;
            ke.setRotation(glm::vec3(Y), keyRot);
        }
        else {
            moveKey = true;
            if (keyPosition.y >= 1.7f) {
                keyPosition.y -= 0.07f * deltaTime;
                ke.setRotation(glm::vec3(Y), keyRot);
            }
            else {
                moveKey = false;
            }
        }models[0].setPosition(keyPosition);

        glm::vec3 keyPosition2 = models[1].getPosition();
        if (keyPosition2.y <= 1.5f && !moveKey2) {
            keyPosition2.y += 0.05f * deltaTime;
        }
        else {
            moveKey2 = true;
            if (keyPosition2.y >= 1.0f) {
                keyPosition2.y -= 0.05f * deltaTime;
            }
            else {
                moveKey2 = false;
            }
        }models[1].setPosition(keyPosition2);

        glm::vec3 keyPosition3 = models[2].getPosition();
        if (keyPosition3.y <= 5.0f && !moveKey3) {
            keyPosition3.y += 0.10f * deltaTime;
        }
        else {
            moveKey3 = true;
            if (keyPosition3.y >= 4.0f) {
                keyPosition3.y -= 0.15f * deltaTime;
            }
            else {
                moveKey3 = false;
            }
        }models[2].setPosition(keyPosition3);

        //:::: FISH ANIMATION :::://
        glm::vec3 fishPos = models[28].getPosition();
        if (fishPos.z <= 6.0f && moveFish == false) {
            fishPos.z += 0.5f * deltaTime;
        }
        else {
            moveFish = true;
            if (fishPos.z >= 2.0f) {
                fishPos.z -= 0.5f * deltaTime;
            }
            else {
                moveFish = false;
            }
        }//models[28].setPosition(fishPos);

        if (fishPos.y <= 0.2f && moveFish2 == false) {
            fishPos.y += 0.1f * deltaTime;
        }
        else {
            moveFish2 = true;
            if (fishPos.y >= -0.9f) {
                fishPos.y -= 0.5f * deltaTime;
            }
            else {
                moveFish2 = false;
            }
        }models[28].setPosition(fishPos);

        //:::: FLOWER ANIMATION :::://
        glm::vec3 flowerPos = models[11].getPosition();
        if (flowerPos.y <= 0.0f && !moveFlower) {
            flowerPos.y += 0.05f;
        }
        else {
            moveFlower = true;
            if (flowerPos.y >= 0.5f && moveFlower) {
                flowerPos.y -= 0.05f;
            }
            else {
                moveFlower = false;
            }
        }models[11].setPosition(flowerPos);

        //:::: DUCK ANIMATION :::://
        glm::vec3 duckPos = models[27].getPosition();
        if (duckPos.x <= -4.2f) {
            duckPos.x -= 0.6f * deltaTime;
        }
        else {
            moveDuck = true;
            if (duckPos.x >= -5.0f) {//-8.2
                duckPos.x += 0.6f * deltaTime;
            }
            else {
                moveDuck = false;
            }
        }models[27].setPosition(duckPos);

        /*HASTA LA PROXIMA*/
        //glm::vec3 posicion(enemy.getPosition());
        //if (posicion.x == camera.Position.x) {
        //    enemy.setPosition(glm::vec3(camera.Position.x + i, camera.Position.y, camera.Position.z - 2));
        //    i = i + 1 * deltaTime;
        //    //i = 0;
        //}
        //else {
        //    enemy.setPosition(glm::vec3(camera.Position.x - i, camera.Position.y, camera.Position.z - 2));
        //    i = i + i * deltaTime;
        //}

        /*if (i == 7) {
            enemy.setPosition(glm::vec3(camera.Position.x + i, camera.Position.y, camera.Position.z - 2));
            i = 0;
            
        }
        else {
            i = i + 1 * deltaTime;
        }*/
       /* glm::vec3 posicion(enemy.getPosition());
        if (camera.Position.x > posicion.x + 5) {
            enemy.setPosition(glm::vec3(camera.Position.x - i, camera.Position.y, camera.Position.z));
            i = i - 0.5 * deltaTime;
        }
        if (camera.Position.x == posicion.x+5) {
            enemy.setPosition(glm::vec3(camera.Position.x + i, camera.Position.y, camera.Position.z));
            i = 0;
        }
        
        if (camera.Position.z > posicion.z + 5) {
            enemy.setPosition(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z - i));
            i = i - 0.5 * deltaTime;
        }
        if (camera.Position.z == posicion.z +5) {
            enemy.setPosition(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z + i));
            i = 0;
        }*/
       

        //:::: BIRD ANIMATION :::://
        glm::vec3 birdPos = models[26].getPosition();
        glm::vec3 birdRot = models[26].getRotationAxis();
        float birdRotate = models[26].getRotation();

        //glm::mat4 birdRot = glm::mat4(models[26].getRotation());
        if (birdPos.z <= 4.5 && !moveBird) {
            birdPos.z -= 0.5 * deltaTime;
            birdPos.x += 0.5 * deltaTime;
            birdRotate += 90.0f;
            //birdRot.x += 90;
            //birdRot += 90.1;
            //birdRot = glm::rotate(birdRot, glm::radians(45.0f), glm::vec3(0.5f, -0.5f, 0.0f));
        }
        else {
            moveBird = true;
            if (birdPos.z >= 0.5 && moveBird) {
                //birdPos.r += 90;
                birdPos.z -= 0.5;
            }
            else {
                moveBird = false;
            }
        }models[26].setPosition(birdPos);
        models[26].setRotation(birdRot, birdRotate);
        //models[26].setRotationAxis(birdRot);
        //models[26].setRotation(glm::vec3(0, birdRot, 0), birdRot);
        //models[26].setRotationAxis(glm::vec3(birdRot, 0, 0));
        

        //==========TEXT==========//
        //Text->RenderText("Holiwas", -0.45f, 0.7f, 0.001f, glm::vec3(0.0, 0.0, 0.0));

        //:::: PUERTA :::://
        if (isDoorOpen == true) {
            CollisionBox collbox2;
            glm::vec4 colorCollbox2(0.41f, 0.2f, 0.737f, 0.06f);
            collbox2 = CollisionBox(glm::vec3(26.8704, 2.85975, 4.94002), glm::vec3(2.36, 3.46, 0.5), colorCollbox2);
            collboxes.insert(pair<int, pair<string, CollisionBox>>(13, pair<string, CollisionBox>("puerta", collbox2)));
            //isDoorOpen = false;
        }

        //:::: ROTACION :::://
       
       /* if (timer != 0) {
            if (timer < 0) {
                timer = timer + 1;
                Sleep(80);
            }
            else {
                timer = timer - 1;
                Sleep(80);
            }
        }
        if (timer == 0.0) {
            MessageBox(NULL, L"SI JALA", L"WOW", MB_ICONHAND | MB_ICONHAND);
        }*/


        ////=======PARTICLES=======//
        //rains = Particles("textures/particulas/rain.png");
        ////MOVEMENT VALUES//
        //m_rain.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
        //m_rain.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f,  1.0f };
        //m_rain.SizeVariation = 0.3f;
        //m_rain.SizeBegin = 0.5f;
        //m_rain.SizeEnd = 0.0f;
        //m_rain.LifeTime = 1.0f;
        //m_rain.Velocity = { 0.2f, 1.0f, 1.0f };
        //m_rain.VelocityVariation = { 0.2f, 1.0f, 0.5f };
        //m_rain.Position = { 0.0f, 0.0f, 0.0f };

        //=======WATER=======//
        if (animWaterY <= 4.0f && animWaterX <= 4.0f && !is_water_out) {
            animWaterX += 0.001f;
            animWaterY += 0.001f;
        }
        else {
            is_water_out = true;
            if (animWaterY > 0.0f && animWaterX > 0.0f && is_water_out) {
                animWaterX -= 0.001f;
                animWaterY -= 0.001f;
            }
            else {
                is_water_out = false;
            }
        }
        plane.draw(animWaterX, animWaterY, view, projection);
        //:::: COLISIONES :::://
        collisions();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //:::: LIBERACIÓN DE MEMORIA:::://   
    delete[] texturePaths;
    sky.Release();
    terrain.Release();
    for (int i = 0; i < lightcubes.size(); i++)
        lightcubes[i].second.Release();
    for (int i = 0; i < collboxes.size(); i++)
        collboxes[i].second.Release();
    for (int i = 0; i < models.size(); i++)
        models[i].Release();
    for (int i = 0; i < rigidModels.size(); i++)
    {
        physicsEnviroment.Unregister(rigidModels[i].getRigidbox());
    }
    physicsEnviroment.Unregister(&piso);
    physicsEnviroment.Unregister(&pared);

    //=======RAIN=======//
    rain.Release();
    //rains.Release();
    //srand(time(0));

    //======WATER PLANE=========//
    plane.Release();
    glfwTerminate();
    KillTimer(NULL, TIMER);

    return 0;
}

void initScene(Shader ourShader)
{
    //INITIALIZE SRAND VARIABLE
    srand(time(0));

    SetTimer(NULL, TIMER, 30, NULL);
    timer = WM_TIMER;

    //AGUA
    //:::: DEFINIMOS LAS TEXTURAS DE LA MULTITEXTURA DEL TERRENO :::://
    texturePaths = new const char *[4];
    texturePaths[0] = "textures/multitexture_colors.jpg";
    texturePaths[1] = "textures/terrain1.png";
    texturePaths[2] = "textures/terrain2.png";
    texturePaths[3] = "textures/terrain3.png";

    //:::: POSICIONES DE TODAS LAS LUCES :::://
    pointLightPositions.push_back(glm::vec3(2.3f, 5.2f, 2.0f));
    pointLightPositions.push_back(glm::vec3(2.3f, 10.3f, -4.0f));
    pointLightPositions.push_back(glm::vec3(1.0f, 9.3f, -7.0f));
    pointLightPositions.push_back(glm::vec3(0.0f, 10.0f, -3.0f));

    //:::: POSICIONES DE TODOS LOS OBJETOS CON FISICAS :::://
    physicsObjectsPositions.push_back(glm::vec3(0.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(2.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(1.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(-2.0, 10.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(-2.0, 10.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(15.0, 1.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(15.0, 1.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(25.0, 10.0, -2.0));

     
    //:::: ESTADO GLOBAL DE OPENGL :::://
    glEnable(GL_DEPTH_TEST);

    //Definimos los collbox de la camara
    camera.setCollBox();

    //:::: CARGAMOS LOS SHADERS :::://
    ourShader.use();
       

    //:::: INICIALIZAMOS NUESTROS MODELOS :::://    
    //models.push_back(Model("carrorojo", "models/CarroRojo.obj", glm::vec3(5.3, 0.5, -4.3), glm::vec3(0, 90, 0), 0.0f, initScale));
    //models.push_back(Model("carroazul", "models/CarroAzul.obj", glm::vec3(-9.6, 0.7, -2), glm::vec3(0, 0, 0), 0.0f, initScale));
    //models.push_back(Model("van", "models/Van.obj", glm::vec3(12, 0.8, -4.5), glm::vec3(0, 90, 0), 0.0f, initScale));
    
    models.push_back(Model("key", "models/key.obj", glm::vec3(36.5398, 1.35, -2.08), glm::vec3(0, 108, 0), 0.0f, initScale));   //0
    models.push_back(Model("key2", "models/key.obj", glm::vec3(18.7002, 0.24, -8.26009), glm::vec3(0, 0, 0), 0.0f, initScale)); //1
    models.push_back(Model("key3", "models/key.obj", glm::vec3(17.5702, 4.22, -1.73), glm::vec3(0, 0, 0), 0.0f, initScale));    //2
    //models.push_back(Model("key4", "models/key.obj", glm::vec3(0, 0.2, -1.66), glm::vec3(0, 0, 0), 0.0f, initScale));
    models.push_back(Model("bed", "models/bed.obj", glm::vec3(16.79, 3.01, -11.4702), glm::vec3(0, 0, 0), 0.0f, 1.5f));         //3
    models.push_back(Model("couch", "models/couch.obj", glm::vec3(36.47, 0.6, 1.26), glm::vec3(0, -180, 0), 0.0f, 1.5f));       //4
    models.push_back(Model("Bush", "models/Bush.obj", glm::vec3(-0.659848, -0.1, 12.3203), glm::vec3(0, 0, 0), 0.0f, 2.2f));    //5
    models.push_back(Model("Bush2", "models/Bush.obj", glm::vec3(rand() % 10 - 5, -0.1, rand() % 20 + 1), glm::vec3(0, 0, 0), 0.0f, 2.2f));//6
    models.push_back(Model("Bush3", "models/Bush.obj", glm::vec3(rand() % 10 - 5, -0.1, rand() % 20 - 5), glm::vec3(0, 0, 0), 0.0f, 2.2f));//7
    models.push_back(Model("Bush4", "models/Bush.obj", glm::vec3(rand() % 10 - 5, -0.1, rand() % 20 - 10), glm::vec3(0, 0, 0), 0.0f, 2.2f));//8
    models.push_back(Model("closet", "models/closet.obj", glm::vec3(16.4603, 3.29999, -7.75018), glm::vec3(0, 0, 0), 0.0f, 2.0f));//9
    models.push_back(Model("Flower", "models/Flower.obj", glm::vec3(32.0704, -0.0900092, 13.7401), glm::vec3(0, 0, 0), 0.0f, 1.0f));//10
    models.push_back(Model("Flower2", "models/Flower.obj", glm::vec3(29.4104, -0.139999, 12.1401), glm::vec3(0, 0, 0), 0.0f, 1.0f));//11
    models.push_back(Model("Grass", "models/Grass.obj", glm::vec3(9.97011, -0.1, 14.8301), glm::vec3(0, 0, 0), 0.0f, 1.0f));//12 con initScale se pone su tamaño original
    models.push_back(Model("Grass2", "models/Grass.obj", glm::vec3(rand() % 31 + 16, -0.1, rand() % 14 + 11), glm::vec3(0, 0, 0), 0.0f, 1.0f));//13
    models.push_back(Model("Grass3", "models/Grass.obj", glm::vec3(rand() % 25 + 15, -0.1, rand() % 14 + 11), glm::vec3(0, 0, 0), 0.0f, 1.0f));//14
    models.push_back(Model("Grass3", "models/Grass.obj", glm::vec3(rand() % 15 + 1, -0.1, rand() % 14 + 11), glm::vec3(0, 0, 0), 0.0f, 1.0f));//15
    models.push_back(Model("Rama", "models/rama.obj", glm::vec3(2.6, -0.1, -4.3), glm::vec3(0, 0, 0), 0.0f, 2));//16
    models.push_back(Model("stone", "models/stone.obj", glm::vec3(10.0001, -0.12, 8.35002), glm::vec3(0, 0, 0), 0.0f, initScale));//17
    models.push_back(Model("stone2", "models/stone.obj", glm::vec3(rand() % 10 + 5, -0.12, rand() % 8 - 10), glm::vec3(0, 0, 0), 0.0f, initScale));//18
    models.push_back(Model("stone3", "models/stone.obj", glm::vec3(rand() % 5 + 1, -0.12, rand() % 5 - 7), glm::vec3(0, 0, 0), 0.0f, initScale));//19
    models.push_back(Model("stone2", "models/stone.obj", glm::vec3(rand() % 10 + 1, -0.12, rand() % 9 - 5), glm::vec3(0, 0, 0), 0.0f, initScale));//20
    models.push_back(Model("stone3", "models/stone.obj", glm::vec3(rand() % 8 + 1, -0.12, rand() % 10 - 10), glm::vec3(0, 0, 0), 0.0f, initScale));//21
    models.push_back(Model("Stump", "models/Stump.obj", glm::vec3(3.25, -0.0299998, -4.03002), glm::vec3(0, 0, 0), 0.0f, 0.5));//22
    models.push_back(Model("tv", "models/tv.obj", glm::vec3(35.4, 0.71, -6.60005), glm::vec3(0, 180, 0), 0.0f, 2.0f));//23
    //models.push_back(Model("fire", "models/fire.obj", glm::vec3(-11.1, 0.5, 8.4), glm::vec3(0, 0, 0), 0.0f, 1));
    models.push_back(Model("lights", "models/lights.obj", glm::vec3(34.1304, 1.12, -3.21007), glm::vec3(0, 0, 0), 0.0f, 1.5f));//24
    //models.push_back(Model("bath", "models/bath.obj", glm::vec3(-8.4, 0.5, -5.5), glm::vec3(0, 0, 0), 0.0f, 1));
    models.push_back(Model("box", "models/box.obj", glm::vec3(35.2304, 0.93, -9.30019), glm::vec3(0, 0, 0), 0.0f, 1));//25
    models.push_back(Model("birds", "models/birds.obj", glm::vec3(5, 15, 5), glm::vec3(0, 0, 0), 0.0f, 2));//26
    //                                                            -5.10004, 15,-6.39007 -> animacion
    models.push_back(Model("duck", "models/duck.obj", glm::vec3(-11.0802, -0.6, 5.47003), glm::vec3(0, 0, 0), 0.0f, 1));//27
    //                                                          -11.0802, -0.58, 2.29   -> adelante
    //                                                          -14.2903, -0.58,1.52    -> diagonal
    models.push_back(Model("fish", "models/fish.obj", glm::vec3(-13, -1.0, 0), glm::vec3(0, 0, 0), 0.0f, 0.1));//28

    //pickModel.push_back(Model("key", "models/key.obj", glm::vec3(7.25, 0.2, -1.66), glm::vec3(0, 0, 0), 0.0f, 0.1));
    //=======CONSTRUCTION===========//
    //models.push_back(Model("house", "models/house.obj", glm::vec3(2.21997, 3.18, -3.57007), glm::vec3(0, -269, 0), 0.0f, 1));
    models.push_back(Model("house2", "models/house2.obj", glm::vec3(2.72, 0.0, -2), glm::vec3(0, 0, 0), 0.0f, 1));
   /*
                                                              NOTE
                                    We need the .blend with all textures in the same path
                                        of the models in the same file of the project.

                                    Then, we need export the .blend to .obj in models file
                                        and put the reference ONLY of the .obj in code.

                                                               NOTE2
                                    In the .mtl we need to put exactly the path of the texture.
   */
   
    //CREAMOS TODAS  LAS CAJAS DE COLISION INDIVIDUALES
    CollisionBox collbox;
    glm::vec4 colorCollbox(0.41f, 0.2f, 0.737f, 0.06f);
    collbox = CollisionBox(glm::vec3(36.8693, 3.09, -0.860156), glm::vec3(0.3, 4.75, 10.1101), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(0, pair<string, CollisionBox>("pared_atras", collbox)));
    collbox = CollisionBox(glm::vec3(14.96, 4.15, -3.32), glm::vec3(0.5, 8.13003, 17.7901), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(1, pair<string, CollisionBox>("pared_frente_izq", collbox)));
    collbox = CollisionBox(glm::vec3(25.0, 3.95, -13.27), glm::vec3(14.97, 8.11, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(2, pair<string, CollisionBox>("pared_frente_der", collbox)));
    collbox = CollisionBox(glm::vec3(19.36, 5.12002, 6.94001), glm::vec3(6.08002, 9.71013, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(3, pair<string, CollisionBox>("pared_frente_arriba", collbox)));
    collbox = CollisionBox(glm::vec3(28.5604, 10, -0.8501), glm::vec3(9.3101, 0.49, 5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(4, pair<string, CollisionBox>("techo", collbox)));
    collbox = CollisionBox(glm::vec3(18.7502, 0.899866, -4.66011), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(5, pair<string, CollisionBox>("enemigo", collbox)));
    collbox = CollisionBox(glm::vec3(29.6004, 1.46987, -7.62021), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(6, pair<string, CollisionBox>("enemigo2", collbox)));
    collbox = CollisionBox(glm::vec3(35.6599, 1.59987, -3.02013), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(7, pair<string, CollisionBox>("enemigo3", collbox)));
    collbox = CollisionBox(glm::vec3(34.6601, 1.55987, 1.29987), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(8, pair<string, CollisionBox>("enemigo4", collbox)));
    collbox = CollisionBox(glm::vec3(18.3802, 0.97987, -9.25025), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(9, pair<string, CollisionBox>("enemigo5", collbox)));
    collbox = CollisionBox(glm::vec3(22.1803, 1.27987, -2.48013), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(10, pair<string, CollisionBox>("enemigo6", collbox)));
    collbox = CollisionBox(glm::vec3(16.4001,5.02989,-2.5901), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(11, pair<string, CollisionBox>("enemigo7", collbox)));
    collbox = CollisionBox(glm::vec3(21.1603,5.32989,-8.4102), glm::vec3(0.49, 0.5, 0.5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(12, pair<string, CollisionBox>("enemigo8", collbox)));
    /*
    if (timer != 0) {
        if (timer < 0) {
            timer = timer + 1;
            Sleep(80);
        }
        else {
            timer = timer - 1;
            Sleep(80);
        }
    }
    if (timer == 0.0) {
        MessageBox(NULL, L"SI JALA", L"WOW", MB_ICONHAND | MB_ICONHAND);
    }
    */
    /*if (isDoorOpen == true) {
        collbox = CollisionBox(glm::vec3(26.8704, 2.85975, 4.94002), glm::vec3(2.36, 3.46, 0.5), colorCollbox);
        collboxes.insert(pair<int, pair<string, CollisionBox>>(6, pair<string, CollisionBox>("puerta", collbox)));
    }*/
    

    //OBJECT COLLISIONS
    //==========KEY COLLISTION==========//
    /*glm::vec3 keyPosition = models[1].getPosition();
    models[1].collbox.setPosition(keyPosition);
    models[1].collbox.setScale(glm::vec3(0.5, 0.5, 0.5));
    models[1].collbox.defineCollisionBox();*//*AQUI ES DONDE DA ERROR*/
    //asi para los demas objetos a animar(BORRAR):)
    /*models[1].collbox.setPosition(glm::vec3(1, 1, 1));
    models[1].collbox.setScale(glm::vec3(1, 1, 1));
    models[1].collbox.defineCollisionBox();*/

    //CREAMOS LOS LIGHTCUBES QUE ENREALIDAD SON COLLISION BOXES QUE NOS AYUDARAN A IDENTIFICAR LA POSICIÓN DE DONDE ESTAN
    glm::vec3 lScale(0.5);
    colorCollbox = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    collbox = CollisionBox(pointLightPositions[0], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(0, pair<string, CollisionBox>("LUZ1", collbox)));
    collbox = CollisionBox(pointLightPositions[1], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(1, pair<string, CollisionBox>("LUZ2", collbox)));
    collbox = CollisionBox(pointLightPositions[2], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(2, pair<string, CollisionBox>("LUZ3", collbox)));
    collbox = CollisionBox(pointLightPositions[3], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(3, pair<string, CollisionBox>("LUZ4", collbox)));

   //===============ADD WATER===============//
    plane = Plane("textures/water.jpg", 2048.0, 2048.0, 0.0, 0.0);  //=========ADD PATH OF THE WATER ON THE TEXT===========//

    plane.setPosition(glm::vec3(20.5, -0.5, -10.5));                  //========POSITION OF THE WATER========//
    plane.setAngles(glm::vec3(90.0, 0.0, 0.0));                 //========ANGLE OF WATER==============//
    plane.setScale(glm::vec3(150.0));                            //========SCALE OF WATER IMAGE=======//


    //==========RAIN==========//
    rain = QuadTexture("textures/rain.png", 525.0f, 525.0f, 0, 0);
    //DrawParticles()
    //==========BILLBOARD==========//
    //particles = Particles("textures/particulas.png");
    enemy = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy.setPosition(glm::vec3(18.7502, 0.0, -4.66011));//-4.0f, 0.2f, 0.0f
    enemy.setScale(1.5f);

    enemy2 = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy2.setPosition(glm::vec3(29.6004, 0.7, -7.67021));//-4.0f, 0.2f, 0.0f
    enemy2.setScale(1.5f);

    enemy3 = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy3.setPosition(glm::vec3(35.6599, 0.7, -3.02013));//-4.0f, 0.2f, 0.0f
    enemy3.setScale(1.5f);

    enemy4 = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy4.setPosition(glm::vec3(34.7399, 0.7, 1.26987));//-4.0f, 0.2f, 0.0f
    enemy4.setScale(1.5f);

    enemy5 = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy5.setPosition(glm::vec3(18.3802, 0.0, -9.28025));//-4.0f, 0.2f, 0.0f
    enemy5.setScale(1.5f);

    enemy6 = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy6.setPosition(glm::vec3(22.1803, 0.0, -2.48013));//-4.0f, 0.2f, 0.0f
    enemy6.setScale(1.5f);

    enemy7 = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy7.setPosition(glm::vec3(16.4001, 4.0, -2.5901));//-4.0f, 0.2f, 0.0f
    enemy7.setScale(1.5f);

    enemy8 = Billboard("textures/enemigo.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 453.5f, 768.0f);//907.0f, 1536.0f
    enemy8.setPosition(glm::vec3(21.1603, 3.2, -8.4102));//-4.0f, 0.2f, 0.0f
    enemy8.setScale(1.5f);
    
    
    
    
    
    ////=======PARTICLES=======//
    //rains = Particles("textures/particulas/rain.png");
    ////MOVEMENT VALUES//
    //m_rain.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    //m_rain.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f,  1.0f };
    //m_rain.SizeVariation = 0.3f;
    //m_rain.SizeBegin = 0.5f;
    //m_rain.SizeEnd = 0.0f;
    //m_rain.LifeTime = 1.0f;
    //m_rain.Velocity = { 0.2f, 1.0f, 1.0f };
    //m_rain.VelocityVariation = { 0.2f, 1.0f, 0.5f };
    //m_rain.Position = { 0.0f, 0.0f, 0.0f };
    

    //==========GAME START==========//
    //MessageBox(NULL, L"Encuentra las 3 llaves para poder escapar\nEnter o cualquier boton para continuar", L"START", MB_ICONHAND || MB_ICONHAND);

    
    
    

    //=======TEXT=======//
    Text = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    Text->Load("fonts/OpenSans-Italic.TTF", 60);
     
}
//:::: CONFIGURACIONES :::://

void loadEnviroment(Terrain *terrain, SkyBox *sky, glm::mat4 view, glm::mat4 projection)
{
    glm::mat4 model = mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, -2.5f, 0.0f));   // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(100.5f, 100.0f, 100.5f)); // it's a bit too big for our scene, so scale it down

    terrain->draw(model, view, projection);
    terrain->getShader()->setFloat("shininess", 100.0f);
    setMultipleLight(terrain->getShader(), pointLightPositions);

    glm::mat4 skyModel = mat4(1.0f);
    skyModel = glm::translate(skyModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    skyModel = glm::scale(skyModel, glm::vec3(40.0f, 40.0f, 40.0f));  // it's a bit too big for our scene, so scale it down
    sky->draw(skyModel, view, projection, skyPos);
    sky->getShader()->setFloat("shininess", 10.0f);
    setMultipleLight(sky->getShader(), pointLightPositions);

    //RENDER DE LIGHT CUBES
    if (renderLightingCubes)
        for (pair<int, pair<string, CollisionBox>> lights : lightcubes)
            lights.second.second.draw(view, projection);

    //======WATER ANIMATION=======//
    loadAnim(view, projection);

    //========RAIN ANIMATION========//
    //if (changeSky >= 60 && changeSky <= 60.1) {
    //    mainLight = vec3(0.2);
    //    //sky->reloadTexture("5");    //file name with the other sky:)
    //    //changeSprite = !changeSprite;
    //}
    //if (changeSky >= 30 && changeSky <= 30.1) {
    //    mainLight = vec3(0.25);
    //}
    //if (changeSky >= 60 && changeSky <= 60.1) {
    //    mainLight = vec3(0.2);
    //   // sky->reloadTexture("5");    //file name with the other sky:)
    //}
    //if (changeSky >= 90 && changeSky <= 90.1) {
    //    mainLight = vec3(0.15);
    //    //sky->reloadTexture("5");    //file name with the other sky:)
    //    changeSprite = !changeSprite;
    //}
    //if (changeSky >= 150 && changeSky <= 150.1) {
    //    mainLight = vec3(0.1);
    //}

   
    //glm::vec3(enemyPos) = enemy.getPosition();      //cambiar para utilizar un getPosition de la camara y un setPosition para
    //glm::vec3(cameraPos) = camera.Position;         //el enemigo(billboard y colision del billboard)
    //if (enemyPos.x < cameraPos.x && enemyPos.x > cameraPos.x) {
    //    //enemy.setPosition(glm::vec3(+1, +0, +0));
    //    enemyX += 1.0f;
    //    enemyPos.x = enemyX;
    //}
    //if (enemyPos.x > cameraPos.x) {
    //    enemyX -= 1.0f;
    //    enemyPos.x = enemyX;
    //}
    //if (enemyPos.y < cameraPos.y) {
    //    enemyY += 1.0f;
    //    enemyPos.y = enemyY;
    //}
    //if (enemyPos.y > cameraPos.y) {
    //    enemyY -= 1.0f;
    //    enemyPos.y = enemyY;
    //}
    //if (enemyPos.z < cameraPos.z) {
    //    enemyY += 1.0f;
    //    enemyPos.z = enemyZ;
    //}
    //if (enemyPos.z > cameraPos.z) {
    //    enemyY -= 1.0f;
    //    enemyPos.z = enemyZ;
    //}
    //if (enemyPos.x == cameraPos.x && enemyPos.y == cameraPos.y && enemyPos.z == cameraPos.z) {
    //    //activar el toque y la pantalla de perdido
    //    //MessageBox(NULL, L"Te atraparon, perdiste", L"PERDISTE", MB_ICONHAND || MB_ICONHAND);
    //}
    

}
void loadAnim(glm::mat4 view, glm::mat4 projection) {
    
    enemy.Draw(camera, round(enemyX), round(enemyY));
    enemy2.Draw(camera, round(enemyX), round(enemyY));
    enemy3.Draw(camera, round(enemyX), round(enemyY));
    enemy4.Draw(camera, round(enemyX), round(enemyY));
    enemy5.Draw(camera, round(enemyX), round(enemyY));
    enemy6.Draw(camera, round(enemyX), round(enemyY));
    enemy7.Draw(camera, round(enemyX), round(enemyY));
    enemy8.Draw(camera, round(enemyX), round(enemyY));
    //========WATER ANIMATION========//
    if (animWaterY <= 4.0f && animWaterX <= 4.0f && !is_water_out) {
        animWaterX += 0.001f;
        animWaterY += 0.001f;
    }
    else {
        is_water_out = true;
        if (animWaterY > 0.0f && animWaterX > 0.0f && is_water_out) {
            animWaterX -= 0.001f;
            animWaterY -= 0.001f;
        }
        else {
            is_water_out = false;
        }
    }
    //========RAIN ANIMATION========//
    //Activate when it´s night
    if (changeSprite) {
        if (spriteX <= 2.0f) {
            if (spriteX <= 4.0f) {
                spriteX += 0.3f;
            }
            else {
                spriteX = 0.0f;
                spriteY += 1.0f;
            }
        }
    else {
        spriteX = 0.0f;
        spriteY = 0.0f;
    }
        rain.changeSprite(round(spriteX), spriteY);
        rain.Draw(glm::vec2(0.0f, 0.0f), 3.0);
    }

    //==========SEGUIMIENTO DEL BILLBOARD==========//
   /* glm::vec3(cameraPos) = camera.Position;
    glm::vec3(enemyPos) = enemy.getPosition();
    enemigo prueba;
    glm::vec3(enemyNewPos) = cameraPos - enemyPos;
    if (isMoving == true) {
        glm::vec3(movimientoBill) = prueba.acercar(cameraPos, enemyPos);
        enemy.setPosition(movimientoBill);
        Sleep(80);
    }*/
    //Always active
    /*if (spriteX <= 2.0f) {
        if (spriteX <= 4.0f) {
            spriteX += 0.3f;
        }
        else {
            spriteX = 0.0f;
            spriteY += 1.0f;
        }
    }
    else {
        spriteX = 0.0f;
        spriteY = 0.0f;
    }
    rain.changeSprite(round(spriteX), spriteY);
    rain.Draw(glm::vec2(0.0f, 0.0f), 3.0);*/

    //========KEY ANIMATION=========//
    /*glm::vec3 keyPosition = models[1].getPosition();
    if (keyPosition.x < 5.0f && !moveKey) {
        keyPosition.x -= 0.010f;
    }
    else {
        moveKey = false;
    }*/
    
}
void drawModels(Shader *shader, glm::mat4 view, glm::mat4 projection)
{
    //DEFINIMOS EL BRILLO  DEL MATERIAL
    shader->setFloat("material.shininess", 60.0f);
    setMultipleLight(shader, pointLightPositions);   
    for (int i = 0; i < models.size(); i++)
    {
        //SI SE RECOGIO EL OBJETO
        shader->use();
        models[i].Draw(*shader);
        detectColls(&models[i].collbox, models[i].name, &camera, renderCollBox, collidedObject_callback);
    }
}
//void pickObjects(Shader* ourShader, Shader* selectShader, glm::mat4 projection) {
//    //IF THERE IS A COLLISION
//    glm::vec3 m = camera.Position + camera.Front * 1.2f;//POSITION OF CAMERA
//    if (pickKey && isColliding) {
//        selectShader->use();
//        setSimpleLight(selectShader);
//        pickModel[1].Draw(*selectShader);
//    }
//}

void setSimpleLight(Shader *shader)
{
    shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader->setInt("lightType", (int)lightType);
    shader->setVec3("light.position", lightPos);
    shader->setVec3("light.direction", lightDir);
    shader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader->setVec3("viewPos", camera.Position);
    shader->setFloat("light.constant", 0.2f);
    shader->setFloat("light.linear", 0.02f);
    shader->setFloat("light.quadratic", 0.032f);
    shader->setFloat("material.shininess", 60.0f);
}
void setMultipleLight(Shader *shader, vector<glm::vec3> pointLightPositions)
{
    shader->setVec3("viewPos", camera.Position);

    shader->setVec3("dirLights[0].direction", pointLightPositions[0]);
    shader->setVec3("dirLights[0].ambient", mainLight.x, mainLight.y, mainLight.z);
    shader->setVec3("dirLights[0].diffuse", mainLight.x, mainLight.y, mainLight.z);
    shader->setVec3("dirLights[0].specular", mainLight.x, mainLight.y, mainLight.z);

    shader->setVec3("dirLights[1].direction", pointLightPositions[1]);
    shader->setVec3("dirLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("dirLights[1].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[1].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("dirLights[2].direction", pointLightPositions[2]);
    shader->setVec3("dirLights[2].ambient", 0.3f, 0.5f, 0.5f);
    shader->setVec3("dirLights[2].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[2].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("dirLights[3].direction", pointLightPositions[3]);
    shader->setVec3("dirLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("dirLights[3].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[3].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("pointLights[0].position", pointLightPositions[0]);
    shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[0].constant", 1.0f);
    shader->setFloat("pointLights[0].linear", 0.09);
    shader->setFloat("pointLights[0].quadratic", 0.032);

    shader->setVec3("pointLights[1].position", pointLightPositions[1]);
    shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[1].constant", 1.0f);
    shader->setFloat("pointLights[1].linear", 0.09);
    shader->setFloat("pointLights[1].quadratic", 0.032);

    shader->setVec3("pointLights[2].position", pointLightPositions[2]);
    shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[2].constant", 1.0f);
    shader->setFloat("pointLights[2].linear", 0.09);
    shader->setFloat("pointLights[2].quadratic", 0.032);

    shader->setVec3("pointLights[3].position", pointLightPositions[3]);
    shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[3].constant", 1.0f);
    shader->setFloat("pointLights[3].linear", 0.09);
    shader->setFloat("pointLights[3].quadratic", 0.032);

    shader->setVec3("spotLights[0].position", pointLightPositions[0]);
    shader->setVec3("spotLights[0].direction", glm::vec3(0.2, 0.8, 0.2));
    shader->setVec3("spotLights[0].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[0].constant", 1.0f);
    shader->setFloat("spotLights[0].linear", 0.09);
    shader->setFloat("spotLights[0].quadratic", 0.032);
    shader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));

    // spotLight
    shader->setVec3("spotLights[1].position", pointLightPositions[1]);
    shader->setVec3("spotLights[1].direction", camera.Front);
    shader->setVec3("spotLights[1].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[1].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[1].constant", 1.0f);
    shader->setFloat("spotLights[1].linear", 0.09);
    shader->setFloat("spotLights[1].quadratic", 0.032);
    shader->setFloat("spotLights[1].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setVec3("spotLights[2].position", pointLightPositions[2]);
    shader->setVec3("spotLights[2].direction", camera.Front);
    shader->setVec3("spotLights[2].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[2].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[2].constant", 1.0f);
    shader->setFloat("spotLights[2].linear", 0.09);
    shader->setFloat("spotLights[2].quadratic", 0.032);
    shader->setFloat("spotLights[2].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[2].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setVec3("spotLights[3].position", pointLightPositions[3]);
    shader->setVec3("spotLights[3].direction", camera.Front);
    shader->setVec3("spotLights[3].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[3].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[3].constant", 1.0f);
    shader->setFloat("spotLights[3].linear", 0.09);
    shader->setFloat("spotLights[3].quadratic", 0.032);
    shader->setFloat("spotLights[3].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[3].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setInt("lightType", (int)lightType);
    shader->setInt("maxRenderLights", 1);
}

//void DrawParticles(glm::mat4 view, glm::mat4 projection) {
//    rains.OnUpdate(0.01f);
//    rains.Draw(glm::vec3(-5.5f, 0.5f, -2.5f), view, projection);
//
//    if (respawnCount >= 5) {
//        for (int i = 0; i < 25; i++)
//            rains.Emit(m_rain);
//        updateParticles = false;
//        respawnCount = 0;
//            
//    }
//}

void collisions()
{
    //TODO LO DE LAS COLISIONES VA AQUÍ
    detectColls(collboxes, &camera, renderCollBox, collidedObject_callback);
}