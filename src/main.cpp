#include "gl_tool/glBlueprints.hpp"
//#include "gl_tool/glCamera.h"
#include <camera_ex.hpp>
#include "gl_tool/glCommonTools.hpp"
#include "gl_tool/glShader.h"
//#include "gl_tool/glTexture.h"
#include "gl_tool/glModule.h"
#include "gl_tool/glVertexObjects.h"
#include "phy.h"
#include "learn-opengl/Model.h"
#include <reactphysics3d/reactphysics3d.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <cmath>
#include <iostream>
using namespace glTool;
// 线框模式
#define POLYGON_MODE 0
unsigned int loadCubemap(std::vector<std::string> faces);
const GLfloat base = 100000.0f;
GLfloat skyboxVertices[] = {
	// positions
	-base, base, -base,
	-base, -base, -base,
	base, -base, -base,
	base, -base, -base,
	base, base, -base,
	-base, base, -base,

	-base, -base, base,
	-base, -base, -base,
	-base, base, -base,
	-base, base, -base,
	-base, base, base,
	-base, -base, base,

	base, -base, -base,
	base, -base, base,
	base, base, base,
	base, base, base,
	base, base, -base,
	base, -base, -base,

	-base, -base, base,
	-base, base, base,
	base, base, base,
	base, base, base,
	base, -base, base,
	-base, -base, base,

	-base, base, -base,
	base, base, -base,
	base, base, base,
	base, base, base,
	-base, base, base,
	-base, base, -base,

	-base, -base, -base,
	-base, -base, base,
	base, -base, -base,
	base, -base, -base,
	-base, -base, base,
	base, -base, base};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
rp3d::PhysicsCommon physicsCommon;

// 窗口大小
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;

//
float carX = 0, carY = 0, carZ = 0, carR = 0;
Camera cam(glm::vec3(carX - 2 * sinf(carR), 2.0, carZ - 2 * cosf(carR)),
		   glm::vec3(carX, 2.0f, carR), glm::vec3(0, 1, 0),
		   glm::perspective(70.0f, (float)SCR_WIDTH / SCR_HEIGHT, 0.1f,
							100000.0f));
CameraEX cam_ex;
Phy phy;
GLuint loadTexture(GLchar *path, GLboolean alpha);

class Fuck : public glTool::Blueprint
{
public:
	float *vertexPointer;
	unsigned int *indexPointer;
	int vertexArraySize, indexArraySize;
	Fuck(){};
	void init(const vector<learn_opengl::Vertex> &vertices, const vector<GLuint> &indices, const vector<learn_opengl::Texture> &textures)
	{

		vertexArraySize = vertices.size() * 3;
		indexArraySize = indices.size();
		int vertexArrayIndex = 0, indexArrayIndex = 0;

		vertexPointer = new float[vertexArraySize];
		indexPointer = new unsigned int[indexArraySize];

		for (std::vector<learn_opengl::Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			vertexPointer[vertexArrayIndex++] = it->Position.x;
			vertexPointer[vertexArrayIndex++] = it->Position.y;
			vertexPointer[vertexArrayIndex++] = it->Position.z;
		}

		for (std::vector<GLuint>::const_iterator it = indices.begin(); it != indices.end(); ++it)
		{
			indexPointer[indexArrayIndex++] = *it;
		}
		usage = GL_TRIANGLES;
		VBO.data(vertexArraySize * sizeof(float), vertexPointer, GL_STATIC_DRAW);
		EBO.data(indexArraySize * sizeof(GLuint), indexPointer, GL_STATIC_DRAW);
	}
	const float *getVertixBufferData() const
	{
		return vertexPointer;
	}
	const unsigned int *getVertixElementData() const
	{
		return indexPointer;
	}
	int getVertixBufferSize() const
	{
		return sizeof(float) * vertexArraySize;
	};
	int getVertixElementSize() const { return sizeof(unsigned int) * indexArraySize; };
};

void draw(GLFWwindow *window)
{
	cam_ex.setperspective((float)SCR_WIDTH / SCR_HEIGHT, 0.1, 1000);
	cam_ex.fov = 70;
	glEnable(GL_DEPTH_TEST);

	// Create the world settings
	rp3d::PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 10;
	settings.isSleepingEnabled = false;
	settings.gravity = rp3d::Vector3(0, -9.81, 0);

	// Create the physics world with your settings
	rp3d::PhysicsWorld *world = physicsCommon.createPhysicsWorld(settings);
	world->enableSleeping(false);

	// 编译着色器
	// 纯色
	Shader shader0("shader/0-light_material/vertex.glsl",
				   "shader/0-light_material/fragment.glsl",
				   "shader/0-light_material/geometry.glsl");
	// 纹理
	Shader shader1("shader/texure2-light_material/vertex.glsl",
				   "shader/texure2-light_material/fragment.glsl",
				   "shader/texure2-light_material/geometry.glsl");
	Shader skyboxShader("shader/skybox/vertex.glsl",
						"shader/skybox/fragment.glsl");
	Shader shader2("shader/0-light_material/vertex.glsl",
				   "shader/0-light_material/fragment.glsl",
				   "shader/0-light_material/geometry.glsl");

	Shader explodeshader("shader/explode/vertex.glsl", "shader/explode/fragment.glsl", "shader/explode/geometry.glsl");

	CubeBp cubeBp;
	Part<1> earth(cubeBp);
	GLuint trackID = loadTexture((GLchar *)"textures/earth.png", true);

	learn_opengl::Model car_mod((GLchar *)"./model/car/capsule.obj");
	learn_opengl::Model rock_mod((GLchar *)"./model/rock/rock.obj");
	Fuck fuck[car_mod.meshes.size()];
	Part<0> car_part[car_mod.meshes.size()];
	glTool::Module car;
	Fuck fuckRock[rock_mod.meshes.size()];
	Part<0> rock_part[rock_mod.meshes.size()];
	glTool::Module rock;

	// Giving Track a Collision Body.
	// Initial position and orientation of the collision body
	rp3d::Vector3 trackPosition(0.0, -1, 0.0);
	rp3d::Quaternion trackOrientation = rp3d::Quaternion::identity();
	rp3d::Transform trackTransform(trackPosition, trackOrientation);
	// Create a collision body in the world
	rp3d::RigidBody *trackBody;
	trackBody = world->createRigidBody(trackTransform);
	trackBody->setType(rp3d::BodyType::STATIC);

	// Half extents of the box in the x, y and z directions
	const rp3d::Vector3 halfExtents(100.0, 2.0, 100.0);
	// Create the box shape
	rp3d::BoxShape *boxShape = physicsCommon.createBoxShape(halfExtents);
	rp3d::Collider *trackCollider = trackBody->addCollider(boxShape, rp3d::Transform::identity());

	// Giving Rock a rigid body.
	// Initial position and orientation of the rigid body
	rp3d::Vector3 rockPosition(10.0, 0.0, 0.0);
	rp3d::Quaternion rockOrientation = rp3d::Quaternion::identity();
	rp3d::Transform rockTransform(rockPosition, rockOrientation);
	// Create a rigid body in the world
	rp3d::RigidBody *rockBody = world->createRigidBody(rockTransform);

	int indexBase = 0;
	int indiceBase = 0;
	float small[3] = {100, 100, 100}, large[3] = {-100, -100, -100};
	for (int i = 0; i < rock_mod.meshes.size(); i++)
	{
		fuckRock[i].init(rock_mod.meshes[i].vertices, rock_mod.meshes[i].indices, rock_mod.meshes[i].textures);
		rock_part[i].setBlueprint(fuckRock[i]);
		rock.addSubstance(car_part[i]);
		// ReactPhysics
		for (int index = 0; index < fuckRock[i].vertexArraySize; ++index)
		{
			// printf("%d\n",index);
			// vertex[index + indexBase] = fuck[i].vertexPointer[index];
			if (small[index % 3] > fuckRock[i].vertexPointer[index])
				small[index % 3] = fuckRock[i].vertexPointer[index];
			if (large[index % 3] < fuckRock[i].vertexPointer[index])
				large[index % 3] = fuckRock[i].vertexPointer[index];
		}
	}

	float vertices[24];
	vertices[0] = small[0];
	vertices[1] = small[1];
	vertices[2] = large[2];
	vertices[3] = large[0];
	vertices[4] = small[1];
	vertices[5] = large[2];
	vertices[6] = large[0];
	vertices[7] = small[1];
	vertices[8] = small[2];
	vertices[9] = small[0];
	vertices[10] = small[1];
	vertices[11] = small[2];
	vertices[12] = small[0];
	vertices[13] = large[1];
	vertices[14] = large[2];
	vertices[15] = large[0];
	vertices[16] = large[1];
	vertices[17] = large[2];
	vertices[18] = large[0];
	vertices[19] = large[1];
	vertices[20] = small[2];
	vertices[21] = small[0];
	vertices[22] = large[1];
	vertices[23] = small[2];

	// Array with the vertices indices for each face of the mesh
	int indices[24];
	indices[0] = 0;
	indices[1] = 3;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 4;
	indices[5] = 5;
	indices[6] = 6;
	indices[7] = 7;
	indices[8] = 0;
	indices[9] = 1;
	indices[10] = 5;
	indices[11] = 4;
	indices[12] = 1;
	indices[13] = 2;
	indices[14] = 6;
	indices[15] = 5;
	indices[16] = 2;
	indices[17] = 3;
	indices[18] = 7;
	indices[19] = 6;
	indices[20] = 0;
	indices[21] = 4;
	indices[22] = 7;
	indices[23] = 3;

	// Description of the six faces of the convex mesh
	rp3d::PolygonVertexArray::PolygonFace *rockFaces = new rp3d::PolygonVertexArray::PolygonFace[6];
	rp3d::PolygonVertexArray::PolygonFace *rockFace = rockFaces;
	for (int f = 0; f < 6; f++)
	{
		// First vertex of the face in the indices array
		rockFace->indexBase = f * 4;
		// Number of vertices in the face
		rockFace->nbVertices = 4;
		rockFace++;
	}
	// Create the polygon vertex array
	rp3d::PolygonVertexArray *polygonVertexArray = new rp3d::PolygonVertexArray(8, vertices, 3 * sizeof(float),
																				indices, sizeof(int), 6, rockFaces,
																				rp3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
																				rp3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	// Create the polyhedron mesh
	rp3d::PolyhedronMesh *rockMesh = physicsCommon.createPolyhedronMesh(polygonVertexArray);

	// Create the convex mesh collision shape
	rp3d::ConvexMeshShape *rockMeshShape = physicsCommon.createConvexMeshShape(rockMesh);
	rp3d::Collider *rockCollider = rockBody->addCollider(rockMeshShape, rockTransform);
	rockBody->updateMassPropertiesFromColliders();
	rockBody->setType(rp3d::BodyType::DYNAMIC);

	// Giving Car a rigid body.
	// Initial position and orientation of the rigid body
	rp3d::Vector3 carPosition(0.0, 1.0, 0.0);
	rp3d::Quaternion carOrientation = rp3d::Quaternion::identity();
	rp3d::Transform carTransform(carPosition, carOrientation);
	// Create a rigid body in the world
	rp3d::RigidBody *carBody = world->createRigidBody(carTransform);

	indexBase = 0;
	indiceBase = 0;
	small[0] = 100, small[1] = 100, small[2] = 100;
	large[1] = -100, large[0] = -100, large[2] = -100;
	for (int i = 0; i < car_mod.meshes.size(); i++)
	{
		fuck[i].init(car_mod.meshes[i].vertices, car_mod.meshes[i].indices, car_mod.meshes[i].textures);
		// for(learn_opengl::Vertex v :car_mod.meshes[i].vertices)
		// 	printf("v %f %f %f\n",v.Position.x,v.Position.y,v.Position.z);
		// for(GLuint f :car_mod.meshes[i].indices)
		// 	printf("%d ",f);
		// Original
		car_part[i].setBlueprint(fuck[i]);
		car.addSubstance(car_part[i]);
		// ReactPhysics
		for (int index = 0; index < fuck[i].vertexArraySize; ++index)
		{
			// printf("%d\n",index);
			// vertex[index + indexBase] = fuck[i].vertexPointer[index];
			if (small[index % 3] > fuck[i].vertexPointer[index])
				small[index % 3] = fuck[i].vertexPointer[index];
			if (large[index % 3] < fuck[i].vertexPointer[index])
				large[index % 3] = fuck[i].vertexPointer[index];
		}
	}

	vertices[0] = small[0];
	vertices[1] = small[1];
	vertices[2] = large[2];
	vertices[3] = large[0];
	vertices[4] = small[1];
	vertices[5] = large[2];
	vertices[6] = large[0];
	vertices[7] = small[1];
	vertices[8] = small[2];
	vertices[9] = small[0];
	vertices[10] = small[1];
	vertices[11] = small[2];
	vertices[12] = small[0];
	vertices[13] = large[1];
	vertices[14] = large[2];
	vertices[15] = large[0];
	vertices[16] = large[1];
	vertices[17] = large[2];
	vertices[18] = large[0];
	vertices[19] = large[1];
	vertices[20] = small[2];
	vertices[21] = small[0];
	vertices[22] = large[1];
	vertices[23] = small[2];

	// Array with the vertices indices for each face of the mesh
	indices[0] = 0;
	indices[1] = 3;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 4;
	indices[5] = 5;
	indices[6] = 6;
	indices[7] = 7;
	indices[8] = 0;
	indices[9] = 1;
	indices[10] = 5;
	indices[11] = 4;
	indices[12] = 1;
	indices[13] = 2;
	indices[14] = 6;
	indices[15] = 5;
	indices[16] = 2;
	indices[17] = 3;
	indices[18] = 7;
	indices[19] = 6;
	indices[20] = 0;
	indices[21] = 4;
	indices[22] = 7;
	indices[23] = 3;

	// Description of the six faces of the convex mesh
	rp3d::PolygonVertexArray::PolygonFace *polygonFaces = new rp3d::PolygonVertexArray::PolygonFace[6];
	rp3d::PolygonVertexArray::PolygonFace *face = polygonFaces;
	for (int f = 0; f < 6; f++)
	{
		// First vertex of the face in the indices array
		face->indexBase = f * 4;
		// Number of vertices in the face
		face->nbVertices = 4;
		face++;
	}
	// Create the polygon vertex array
	polygonVertexArray = new rp3d::PolygonVertexArray(8, vertices, 3 * sizeof(float),
													  indices, sizeof(int), 6, polygonFaces,
													  rp3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
													  rp3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	// Create the polyhedron mesh
	rp3d::PolyhedronMesh *polyhedronMesh = physicsCommon.createPolyhedronMesh(polygonVertexArray);

	// Create the convex mesh collision shape
	rp3d::ConvexMeshShape *carMeshShape = physicsCommon.createConvexMeshShape(polyhedronMesh);
	rp3d::Collider *carCollider = carBody->addCollider(carMeshShape, carTransform);
	carBody->updateMassPropertiesFromColliders();
	carBody->setType(rp3d::BodyType::KINEMATIC);

	earth.setLocation(0, -1, 0);
	// texture
	earth.setAttribute(
		1, 2,
		[](float x, float y, float z, float *tex)
		{
			tex[0] = x < 0 ? 0 : 1;
			tex[1] = z < 0 ? 0 : 1;
			return;
		},
		GL_STATIC_DRAW);
	earth.setScaling(100, 1, 100);
	//car.setScaling(2, 0.8, 1);
	float tnow = 0.01f;
// 线框模式
#if POLYGON_MODE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// Skybox Shader Configuration
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);

	std::string basedir = "textures/skybox/";
	std::vector<std::string> faces{
		basedir + "right.jpg",
		basedir + "left.jpg",
		basedir + "top.jpg",
		basedir + "bottom.jpg",
		basedir + "front.jpg",
		basedir + "back.jpg"};

	GLuint cubemapTexture = loadCubemap(faces);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// controlFPS(60);
	double oldtime = glfwGetTime(), newtime;
	earth.draw(shader1.ID, "trans");

	// Prepare the car.

	// Constant physics time step
	const float timeStep = 1.0f / 60.0f;
	time_t previousFrameTime = time(NULL);
	double accumulator = 0.0f;
	rp3d::Transform prevTransform = carBody->getTransform();

	// Get the current material of the body
	rp3d::Material &material = carCollider->getMaterial();
	// Change the bounciness of the body
	material.setBounciness(0.4);
	// Change the friction coefficient of the body
	material.setFrictionCoefficient(0.0);

	// Get the current material of the body
	rp3d::Material &trackMaterial = trackCollider->getMaterial();
	// Change the bounciness of the body
	trackMaterial.setBounciness(0.4);
	// Change the friction coefficient of the body
	trackMaterial.setFrictionCoefficient(0.0);
	bool flag = false, explode = false;
	clock_t explodeTime = 0;
	//carBody->setWorldLocation(-10,1,0);
	while (!glfwWindowShouldClose(window))
	{
		// Get the current system time
		time_t currentFrameTime = time(NULL);
		// Compute the time difference between the two frames
		double deltaTime = difftime(currentFrameTime, previousFrameTime);
		// Update the previous time
		previousFrameTime = currentFrameTime;
		// Add the time difference in the accumulator
		accumulator += deltaTime;
		// While there is enough accumulated time to take
		// one or several physics steps
		//	while (accumulator >= timeStep)
		{
			// Update the Dynamics world with a constant time step
			world->update(timeStep);
			// Decrease the accumulated time
			accumulator -= timeStep;
		}
		if (world->testOverlap(rockBody, carBody))
		{
			flag = true;
			rockBody->removeCollider(rockCollider);
			explode = true;
		}
		// Compute the time interpolation factor
		rp3d::decimal factor = accumulator / timeStep;

		/*
		// Get the updated transform of the body
		rp3d::Transform currTransform = carBody->getTransform();

		// Compute the interpolated transform of the rigid body
		rp3d::Transform interpolatedTransform = rp3d::Transform::interpolateTransforms(prevTransform,
																					   currTransform, factor);

		// Now you can render your body using the interpolated transform here
		// Get the OpenGL matrix array of the transform
		float matrix[16];
		currTransform.getOpenGLMatrix(matrix);

		// Update the previous transform
		prevTransform = currTransform;

		glm::mat4 trans = glm::mat4(matrix[0], matrix[1], matrix[2], matrix[3],
									matrix[4], matrix[5], matrix[6], matrix[7],
									matrix[8], matrix[9], matrix[10], matrix[11],
									matrix[12], matrix[13], matrix[14], matrix[15]);
		glm::vec4 carLocation(0, 0, 0, 1);
		rp3d::Vector3 axis;
		float angle;
		currTransform.getOrientation().getRotationAngleAxis(angle, axis);

		carLocation = trans * carLocation;

		carX = carLocation.x / carLocation.w;
		carY = carLocation.y / carLocation.w;
		carZ = carLocation.z / carLocation.w;
		if (angle > 0.001)
			car.setRotation(axis[0], axis[1], axis[2], angle / 3.1416 * 180);
			*/
		cam.setCenter(carX, carY + 2.0f, carZ);
		cam.setEye(carX - 6 * sinf(carR + 3.1416 / 2), carY + 4.0f,
				   carZ - 6 * cosf(carR + 3.1416 / 2));
		cam.setperspective(70.0f, (float)SCR_WIDTH / SCR_HEIGHT, 0.1f,
						   100000.0f);
					
		car.setLocation(carX, carY, carZ);
		car.setRotation(0,1,0,carR*180/3.14);
		carBody->setTransform(rp3d::Transform(rp3d::Vector3(carX, carY, carZ),rp3d::Quaternion(0,1,0,0)));
		if (!flag)
		{

			rp3d::Transform rockcurrTransform = rockBody->getTransform();
			float rockmatrix[16];
			rockcurrTransform.getOpenGLMatrix(rockmatrix);
			glm::mat4 rocktrans = glm::mat4(rockmatrix[0], rockmatrix[1], rockmatrix[2], rockmatrix[3],
									rockmatrix[4],rockmatrix[5], rockmatrix[6], rockmatrix[7],
									rockmatrix[8], rockmatrix[9], rockmatrix[10], rockmatrix[11],
									rockmatrix[12], rockmatrix[13], rockmatrix[14], rockmatrix[15]);
			glm::vec4 rockLocation(10, 0, 0, 1);
			rockLocation=rocktrans*rockLocation;
			rock.setLocation( rockLocation.x/rockLocation.w, rockLocation.y/rockLocation.w, rockLocation.z/rockLocation.w);
			printf("%f %f %f\n", rockLocation.x/rockLocation.w, rockLocation.y/rockLocation.w, rockLocation.z/rockLocation.w);
		}
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader0.use();
		shader0.setMat4("camera", cam.getMat());
		shader0.setVec3("viewPos", cam.eye);
		shader0.setVec3("light.pos", 50, 50, 0);
		shader0.setVec3("material.ambient", 0.2, 0.5, 0.5);
		shader0.setVec3("material.diffuse", 0.2, 0.5, 0.5);
		shader0.setVec3("material.specular", 0.2, 0.2, 0.2);
		shader0.setFloat("material.shininess", 0.25);
		car.draw(shader0.ID, "trans");

		if (!flag)
		{
			shader2.use();
			shader2.setMat4("camera", cam.getMat());
			shader2.setVec3("viewPos", cam.eye);
			shader2.setVec3("light.pos", 50, 50, 0);
			shader2.setVec3("material.ambient", 0.2, 0.5, 0.5);
			shader2.setVec3("material.diffuse", 0.2, 0.5, 0.5);
			shader2.setVec3("material.specular", 0.2, 0.2, 0.2);
			shader2.setFloat("material.shininess", 0.25);
			rock.draw(shader2.ID, "trans");
		}

		shader1.use();
		shader1.setMat4("camera", cam.getMat());
		shader1.setVec3("viewPos", cam.eye);
		shader1.setVec3("light.pos", 50, 50, 0);
		shader1.setVec3("material.ambient", 0, 0, 0);
		shader1.setVec3("material.diffuse", 1, 1, 1);
		shader1.setVec3("material.specular", 1, 1, 1);
		shader1.setFloat("material.shininess", 1024);
		glBindTexture(GL_TEXTURE_2D, trackID);
		earth.draw(shader1.ID, "trans");

		skyboxShader.use();
		skyboxShader.setMat4("view", cam.getMat());
		skyboxShader.setMat4("projection", cam.perspective);

		// skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		// explode
		if (explode)
		{
			printf("explode!\n");
			if (!explodeTime)
			{
				explodeTime = clock();
			}
			explodeshader.use();
			explodeshader.setMat4("projection", cam.perspective);
			explodeshader.setMat4("view", cam.getMat());

			// explode model
			explodeshader.use();
			glm::mat4 model = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
			model = glm::translate(model, glm::vec3(10.0, 0.0, 0.0));
			const GLfloat boss_scale = 5.0f;
			model = glm::scale(model, glm::vec3(boss_scale, boss_scale, boss_scale));
			explodeshader.setMat4("model", model);
			explodeshader.setFloat("dis", (clock() - explodeTime) / 100.0f);
			rock.draw(explodeshader.ID, "trans");
		}
		// -------------------------------------------------------------------------------
		// controlFPS();
		printf("%s\n", phy.debugmsg().c_str());
		glfwSwapBuffers(window);
		// 捕获事件
		processInput(window);
		//车辆变化
		newtime = glfwGetTime();
		phy.nextFrerame();
		carX += phy.V() * sinf(carR + 3.1416 / 2) * (newtime - oldtime)*10 ;
		carZ += phy.V() * cosf(carR + 3.1416 / 2) * (newtime - oldtime) *10;
		carR += phy.R() * phy.V() * (newtime - oldtime)*10;
		//carBody->setLinearVelocity(rp3d::Vector3(phy.V() * sinf(carR + 3.1416 / 2), 0.0f, 0.0f)); //phy.V() * cosf(carR + 3.1416 / 2)
		//carBody->setAngularVelocity( phy.R() * rp3d::Vector3(phy.V() * sinf(carR),0.0f, 0.0f) ); //phy.V() * cosf(carR )
		//carBody->applyForceAtLocalPosition( rp3d::Vector3 (70*phy._w,0,0),rp3d::Vector3 (1,0,0) ); //-40*std::abs(phy._w)
		//if(carY<1.0f) carBody->applyForceAtLocalPosition( rp3d::Vector3 (0,0.0f,-200* phy.R()), rp3d::Vector3 (1,0,0) ); //-100* std::abs(phy.R())

		oldtime = newtime;
		glfwPollEvents();
		// printf("Test Overlap: %d\n", world->testOverlap(carBody, trackBody));
		//printf("%f %f %f\n", carX, carY, carZ);
		// break;
	}
	return;
}
int main()
{
	// 初始化
	GLFWwindow *window =
		glfwGladInitCreateWindow(SCR_WIDTH, SCR_HEIGHT, "demo");
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	draw(window);
	glfwTerminate();
	return 0;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	// 重置
	phy.reset();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (phy.V() >= -0.1)
			phy.setW(3);
		else
			phy.setB(0.9);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (phy.V() <= 0.1)
			phy.setW(-1);
		else
			phy.setB(0.9);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		phy.setR(1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		phy.setR(-1);
	}
}

GLuint loadTexture(GLchar *path, GLboolean alpha)
{
	//Generate texture ID and load texture data
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char *image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT); // Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			SOIL_free_image_data(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}