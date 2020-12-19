#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredDinding();

	BuildColoredAtap();

	BuildColoredKulkas();

	BuildColoredLemari();

	BuildColoredLaci();

	BuildColoredTV();

	BuildColoredPintuKayu();

	BuildColoredPintuKulkas();

	BuildColoredPlane();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth/2;
	double midY = screenHeight/2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);


	

}

void Demo::Update(double deltaTime) {
	
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredDinding();

	DrawColoredAtap();

	DrawColoredKulkas();

	DrawColoredLemari();

	DrawColoredLaci();

	DrawColoredPintuKayu();

	DrawColoredPintuKulkas();

	DrawColoredTV();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredDinding() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("dinding3.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		//DINDING DEPAN
		// front
		-10, -0.5,  -0.3, 0, 0,  // 0
		10, -0.5, -0.3, 1, 0,   // 1
		10, 8, -0.3, 1, 1,   // 2
		-10, 8, -0.3, 0, 1,   // 3

		// right
		10, 8, -0.3, 0, 0,  // 4 
		10,  8, -0.6, 1, 0,  // 5
		10, -0.5, -0.6, 1, 1,  // 6
		10, -0.5, -0.3, 0, 1,  // 7

		// back
		-10, -0.5,  -0.6, 0, 0,  // 8 
		10, -0.5, -0.6, 1, 0, // 9
		10,  8, -0.6, 1, 1, // 10
		-10,  8, -0.6, 0, 1,  // 11

		// left
		-10, -0.5,  -0.6, 0, 0, // 12
		-10, -0.5,  -0.3, 1, 0, // 13
		-10, 8, -0.3, 1, 1, // 14
		-10, -0.5,  -0.3, 0, 1, // 15

		// upper
		10, 8, -0.3, 0, 0,   // 16
		-10, 8, -0.3, 1, 0,  // 17
		-10,  8, -0.6,  1, 1,  // 18
		10,  8, -0.6, 0, 1,   // 19

		// bottom
		-10, -0.5,  -0.6, 0, 0, // 20
		10, -0.5, -0.6, 1, 0,  // 21
		10, -0.5, -0.3,1, 1,  // 22
		-10, -0.5,  -0.3, 0, 1, // 23

		//DINDING KIRI
		// front
		-10.3, -0.5,  10, 0, 0,  // 24
		-10, -0.5, 10, 1, 0,   // 25
		-10, 8, 10, 1, 1,   // 26
		-10.3, 8, 10, 0, 1,   // 27

		// right
		-10, 8, 10, 0, 0,  // 28
		-10,  8, -0.6, 1, 0,  // 29
		-10, -0.5, -0.6, 1, 1,  // 30
		-10, -0.5, 10, 0, 1,  // 31

		// back
		-10.3, -0.5,  -0.6, 0, 0,  // 32 
		-10, -0.5, -0.6, 1, 0, // 33
		-10,  8, -0.6, 1, 1, // 34
		-10.3,  8, -0.6, 0, 1,  // 35

		// left
		-10.3, -0.5,  -0.6, 0, 0, // 36
		-10.3, -0.5,  10, 1, 0, // 37
		-10.3, 8, 10, 1, 1, // 38
		-10.3,  8, -0.6, 0, 1, // 40

		// upper
		-10, 8, 10, 0, 0,   // 41
		-10.3, 8, 10, 1, 0,  // 42
		-10.3,  8, -0.6,  1, 1,  // 43
		-10,  8, -0.6, 0, 1,   // 44

		// bottom
		-10.3, -0.5,  -0.6, 0, 0, // 45
		-10, -0.5, -0.6, 1, 0,  // 46
		-10, -0.5, 10, 1, 1,  // 47
		-10.3, -0.5,  10, 0, 1, // 48

		//DINDING KANAN
		// front
		10, -0.5,  10, 0, 0,  // 49
		10.3, -0.5, 10, 1, 0,   // 50
		10.3, 8, 10, 1, 1,   // 51
		10, 8, 10, 0, 1,   // 52

		// right
		10, 8, 10, 0, 0,  // 53
		10,  8, -0.6, 1, 0,  // 54
		10, -0.5, -0.6, 1, 1,  // 55
		10, -0.5, 10, 0, 1,  // 56

		// back
		10.3, -0.5,  -0.6, 0, 0,  // 57
		10, -0.5, -0.6, 1, 0, // 58
		10,  8, -0.6, 1, 1, // 59
		10.3,  8, -0.6, 0, 1,  // 60

		// left
		10.3, -0.5,  -0.6, 0, 0, // 61
		10.3, -0.5,  10, 1, 0, // 62
		10.3, 8, 10, 1, 1, // 63
		10.3,  8, -0.6, 0, 1, // 64

		// upper
		10, 8, 10, 0, 0,   // 65
		10.3, 8, 10, 1, 0,  // 66
		10.3, 8, -0.6, 1, 1,  // 67
		10, 8, -0.6, 0, 1,   // 68

		// bottom
		10.3, -0.5, -0.6, 0, 0, // 69
		10, -0.5, -0.6, 1, 0,  // 70
		10, -0.5, 10, 1, 1,  // 71
		10.3, -0.5, 10, 0, 1, // 72

		//DINDING BELAKANG
		// front
		-10, -0.5, 10, 0, 0,  // 73
		10, -0.5, 10, 1, 0,   // 74
		10, 8, 10, 1, 1,   // 75
		-10, 8, 10, 0, 1,   // 76

		// right
		10, 8, 10, 0, 0,  // 77
		10, 8, 10.3, 1, 0,  // 78
		10, -0.5, 10, 1, 1,  // 79
		10, -0.5, 10.3, 0, 1,  // 80

		// back
		-10, -0.5, 10, 0, 0,  // 81
		10, -0.5, 10, 1, 0, // 82
		10, 8, 10, 1, 1, // 83
		-10, 8, 10, 0, 1,  // 84

		// left
		-10, -0.5, 10, 0, 0, // 85
		-10, -0.5, 10.3, 1, 0, // 86
		-10, 8, 10.3, 1, 1, // 87
		-10, -0.5, 10.3, 0, 1, // 88

		// upper
		10, 8, 10.3, 0, 0,   // 89
		-10, 8, 10.3, 1, 0,  // 90
		-10, 8, 10, 1, 1,  // 91
		10, 8, 10, 0, 1,   // 92

		// bottom
		-10, -0.5, 10, 0, 0, // 93
		10, -0.5, 10, 1, 0,  // 94
		10, -0.5, 10.3, 1, 1,  // 95
		-10, -0.5, 10.3, 0, 1, // 96
	};

	unsigned int indices[] = {
		//DINDING DEPAN
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

		//DINDING KIRI
		24,  25,  26,  24,  26,  27,   // front
		28,  29,  30,  28,  30,  31,   // right
		32,  33,  34, 32,  34, 35,  // back
		36, 38, 37, 36, 39, 38,  // left
		40, 42, 41, 40, 43, 42,  // upper
		44, 46, 45, 44, 47, 46,   // bottom

		//DINDING KANAN
		48,  49,  50,  48,  50,  51,   // front
		52,  53,  54,  52,  54,  55,   // right
		56,  57,  58, 56,  58, 59,  // back
		60, 62, 61, 60, 63, 62,  // left
		64, 66, 65, 64, 67, 66,  // upper
		68, 70, 69, 68, 71, 70,   // bottom

		//DINDING BELAKANG
		72,  73,  74,  72,  74,  75,   // front
		76,  77,  78,  76,  78,  79,   // right
		80,  81,  82, 80,  82, 83,  // back
		84, 86, 85, 84, 87, 86,  // left
		88, 90, 89, 88, 91, 90,  // upper
		92, 94, 93, 92, 95, 94,   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredDinding()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredAtap() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("lantai2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-10,  8.01,  10, 0, 0,  // 0 <- 4
		10,  8.01, 10, 1, 0,   // 1 <- 5
		10, 8.2, 10, 1, 1,   // 2 <- 1
		-10.5, 8.2, 10, 0, 1,   // 3 <- 0

		// right
		10, 8.2, 10, 0, 0,  // 4 = 2
		10,  8.2, -0.3, 1, 0,  // 5 = 10
		10, 8.01, -0.3, 1, 1,  // 6 = 9
		10,  8.01, 10, 0, 1,  // 7 = 1

		// back
		-10, 8.01,  -0.3, 0, 0,  // 8 <- 7 
		10, 8.01, -0.3, 1, 0, // 9 <- 6
		10,  8.2, -0.3, 1, 1, // 10 <- 2
		-10,  8.2, -0.3, 0, 1,  // 11 <- 3

		// left
		-10, 8.01,  -0.3, 0, 0, // 12 = 8
		-10,  8.01,  10, 1, 0, // 13 = 0
		-10, 8.2, 10, 1, 1, // 14 = 3
		-10,  8.2, -0.3, 0, 1, // 15 = 11

		// upper
		10, 8.2, 10, 0, 0,   // 16 = 2
		-10, 8.2, 10, 1, 0,  // 17 = 3
		-10,  8.2, -0.3, 1, 1,  // 18 = 11
		10,  8.2, -0.3, 0, 1,   // 19 = 10

		// bottom
		-10, 8.01,  -0.3, 0, 0, // 20 = 8
		10, 8.01, -0.3, 1, 0,  // 21 = 9
		10,  8.01, 10, 1, 1,  // 22 = 1
		-10,  8.01,  10, 0, 1, // 23 = 0
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glGenBuffers(1, &EBO9);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO9);

	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredAtap()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO9); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredKulkas() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("tv2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-5.5,  -0.5,  0.5, 0, 0,  // 0 <- 4
		-4,  -0.5, 0.5, 1, 0,   // 1 <- 5
		-4, 2.5, 0.5, 1, 1,   // 2 <- 1
		-5.5, 2.5, 0.5, 0, 1,   // 3 <- 0

		// right
		-4,  2.5, 0.5, 0, 0,  // 4 = 2
		-4,  2.5, -0.2, 1, 0,  // 5 = 10
		-4, -0.5, -0.2, 1, 1,  // 6 = 9
		-4,  -0.5, 0.5, 0, 1,  // 7 = 1

		// back
		-5.5, -0.5,  -0.2, 0, 0,  // 8 <- 7 
		-4, -0.5, -0.2, 1, 0, // 9 <- 6
		-4,  2.5, -0.2, 1, 1, // 10 <- 2
		-5.5,  2.5, -0.2, 0, 1,  // 11 <- 3

		// left
		-5.5, -0.5,  -0.2, 0, 0, // 12 = 8
		-5.5,  -0.5,  0.5, 1, 0, // 13 = 0
		-5.5, 2.5, 0.5, 1, 1, // 14 = 3
		-5.5,  2.5, -0.2, 0, 1, // 15 = 11

		// upper
		-4, 2.5, 0.5, 0, 0,   // 16 = 2
		-5.5, 2.5, 0.5, 1, 0,  // 17 = 3
		-5.5,  2.5, -0.2, 1, 1,  // 18 = 11
		-4,  2.5, -0.2, 0, 1,   // 19 = 10

		// bottom
		-5.5, -0.5,  -0.2, 0, 0, // 20 = 8
		-4, -0.5, -0.2, 1, 0,  // 21 = 9
		-4,  -0.5, 0.5, 1, 1,  // 22 = 1
		-5.5,  -0.5,  0.5, 0, 1, // 23 = 0
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredKulkas()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredLemari() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("lemari3.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-2.7,  -0.5,  0.5, 0, 0,  // 0 <- 4
		0.5,  -0.5, 0.5, 1, 0,   // 1 <- 5
		0.5, 3, 0.5, 1, 1,   // 2 <- 1
		-2.7, 3, 0.5, 0, 1,   // 3 <- 0

		// right
		0.5, 3, 0.5, 0, 0,  // 4 = 2
		0.5,  3, -0.2, 1, 0,  // 5 = 10
		0.5, -0.5, -0.2, 1, 1,  // 6 = 9
		0.5,  -0.5, 0.5, 0, 1,  // 7 = 1

		// back
		-2.7, -0.5,  -0.2, 0, 0,  // 8 <- 7 
		0.5, -0.5, -0.2, 1, 0, // 9 <- 6
		0.5,  3, -0.2, 1, 1, // 10 <- 2
		-2.7,  3, -0.2,  0, 1,  // 11 <- 3

		// left
		-2.7, -0.5,  -0.2, 0, 0, // 12 = 8
		-2.7,  -0.5,  0.5, 1, 0, // 13 = 0
		-2.7, 3, 0.5, 1, 1, // 14 = 3
		-2.7,  3, -0.2,  0, 1, // 15 = 11

		// upper
		0.5, 3, 0.5, 0, 0,   // 16 = 2
		-2.7, 3, 0.5, 1, 0,  // 17 = 3
		-2.7,  3, -0.2,  1, 1,  // 18 = 11
		0.5,  3, -0.2, 0, 1,   // 19 = 10

		// bottom
		-2.7, -0.5,  -0.2, 0, 0, // 20 = 8
		0.5, -0.5, -0.2, 1, 0,  // 21 = 9
		0.5,  -0.5, 0.5, 1, 1,  // 22 = 1
		-2.7,  -0.5,  0.5, 0, 1, // 23 = 0
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredLemari()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredLaci() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("lemari3.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		1.8,  -0.5,  0.5, 0, 0,  // 0 <- 4
		4.5,  -0.5, 0.5, 1, 0,   // 1 <- 5
		4.5, 0.5, 0.5, 1, 1,   // 2 <- 1
		1.8, 0.5, 0.5, 0, 1,   // 3 <- 0

		// right
		4.5, 0.5, 0.5, 0, 0,  // 4 = 2
		4.5,  0.5, -0.2, 1, 0,  // 5 = 10
		4.5, -0.5, -0.2, 1, 1,  // 6 = 9
		4.5,  -0.5, 0.5, 0, 1,  // 7 = 1

		// back
		1.8, -0.5,  -0.2, 0, 0,  // 8 <- 7 
		4.5, -0.5, -0.2, 1, 0, // 9 <- 6
		4.5,  0.5, -0.2, 1, 1, // 10 <- 2
		1.8,  0.5, -0.2,  0, 1,  // 11 <- 3

		// left
		1.8, -0.5,  -0.2, 0, 0, // 12 = 8
		1.8,  -0.5,  0.5, 1, 0, // 13 = 0
		1.8, 0.5, 0.5, 1, 1, // 14 = 3
		1.8,  0.5, -0.2, 0, 1, // 15 = 11

		// upper
		4.5, 0.5, 0.5, 0, 0,   // 16 = 2
		1.8, 0.5, 0.5, 1, 0,  // 17 = 3
		1.8,  0.5, -0.2,  1, 1,  // 18 = 11
		4.5,  0.5, -0.2, 0, 1,   // 19 = 10

		// bottom
		1.8, -0.5,  -0.2, 0, 0, // 20 = 8
		4.5, -0.5, -0.2, 1, 0,  // 21 = 9
		4.5,  -0.5, 0.5, 1, 1,  // 22 = 1
		1.8,  -0.5,  0.5, 0, 1, // 23 = 0
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredLaci()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredTV() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("TV.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		//TV
		// front
		2.1,  0.7,  0.25, 0, 0,  // 0 <- 4
		4.2,  0.7, 0.25, 1, 0,   // 1 <- 5
		4.2, 2.0, 0.25, 1, 1,   // 2 <- 1
		2.1, 2.0, 0.25, 0, 1,   // 3 <- 0

		// right
		4.2, 2.0, 0.25,  0, 0,  // 4 = 2
		4.2,  2.0, 0.05, 1, 0,  // 5 = 10
		4.2, 0.7, 0.05, 1, 1,  // 6 = 9
		4.2,  0.7, 0.25, 0, 1,  // 7 = 1

		// back
		2.1, 0.7,  0.05, 0, 0,  // 8 <- 7
		4.2, 0.7, 0.05, 1, 0, // 9 <- 6
		4.2,  2.0, 0.05, 1, 1, // 10 <- 2
		2.1,  2.0, 0.05, 0, 1,  // 11 <- 3

		// left
		2.1, 0.7,  0.05, 0, 0, // 12 = 8
		2.1,  0.7,  0.25, 1, 0, // 13 = 0
		2.1, 2.0, 0.25, 1, 1, // 14 = 3
		2.1,  2.0, 0.05, 0, 1, // 15 = 11

		// upper
		4.2, 2.0, 0.25, 0, 0,   // 16 = 2
		2.1, 2.0, 0.25, 1, 0,  // 17 = 3
		2.1,  2.0, 0.05,  1, 1,  // 18 = 11
		4.2,  2.0, 0.05, 0, 1,   // 19 = 10

		// bottom
		2.1, 0.7,  0.05, 0, 0, // 20 = 8
		4.2, 0.7, 0.05, 1, 0,  // 21 = 9
		4.2,  0.7, 0.25, 1, 1,  // 22 = 1
		2.1,  0.7,  0.25, 0, 1, // 23 = 0

		//TATAKAN TENGAH
		// front
		3.05,  0.55,  0.25, 0, 0,  // 24
		3.25,  0.55, 0.25, 1, 0,   // 25
		3.25, 0.7, 0.25, 1, 1,  // 26
		3.05, 0.7, 0.25, 0, 1,   // 27

		// right
		3.25, 0.7, 0.25, 0, 0,  // 28
		3.25,  0.7, 0.05, 1, 0,  // 29
		3.25, 0.55, 0.05, 1, 1,  // 30
		3.25,  0.55, 0.25, 0, 1,  // 31

		// back
		3.05, 0.55,  0.05, 0, 0,  // 32 
		3.25, 0.55, 0.05, 1, 0, // 33
		3.25,  0.7, 0.05, 1, 1, // 34
		3.05,  0.7, 0.05, 0, 1,  // 35

		// left
		3.05, 0.55,  0.05, 0, 0, // 36 
		3.05,  0.55,  0.25, 1, 0, // 37
		3.05, 0.7, 0.25, 1, 1, // 38
		3.05,  0.7, 0.05, 0, 1, // 39

		// upper
		3.25, 0.7, 0.25, 0, 0,   // 40
		3.05, 0.7, 0.25, 1, 0,  // 41
		3.05,  0.7, 0.05,  1, 1,  // 42
		3.25,  0.7, 0.05, 0, 1,   // 43

		// bottom
		3.05, 0.55,  0.05, 0, 0, // 44
		3.25, 0.55, 0.05, 1, 0,  // 45
		3.25, 0.55, 0.05, 1, 1,  // 46
		3.05,  0.55,  0.25, 0, 1, // 47

		//TATAKAN BAWAH 
		// front
		2.6,  0.5,  0.45, 0, 0,  // 48
		3.7,  0.5, 0.45, 1, 0,   // 49
		3.7, 0.55, 0.45, 1, 1,  // 50
		2.6, 0.55, 0.45, 0, 1,   // 51

		// right
		3.7, 0.55, 0.45, 0, 0,  // 52
		3.7,  0.55, -0.15, 1, 0,  // 53
		3.7, 0.5, -0.15, 1, 1,  // 54
		3.7,  0.5, 0.45, 0, 1,  // 55

		// back
		2.6, 0.5,  -0.15, 0, 0,  // 56 
		3.7, 0.5, -0.15, 1, 0, // 57
		3.7,  0.55, -0.15, 1, 1, // 58
		2.6,  0.55, -0.15, 0, 1,  // 59

		// left
		2.6, 0.5,  -0.15, 0, 0, // 60
		2.6,  0.5,  0.45, 1, 0, // 61
		2.6, 0.55, 0.45, 1, 1, // 62
		2.6,  0.55, -0.15, 0, 1, // 63

		// upper
		3.7, 0.55, 0.45, 0, 0,   // 64
		2.6, 0.55, 0.45, 1, 0,  // 65
		2.6, 0.55, -0.15, 1, 1,  // 66
		3.7, 0.55, -0.15, 0, 1,   // 67

		// bottom
		2.6, 0.5, -0.15, 0, 0, // 68
		3.7, 0.5, -0.15, 1, 0,  // 69
		3.7, 0.5, 0.45, 1, 1,  // 70
		2.6, 0.5, 0.45, 0, 1, // 71
	};

	unsigned int indices[] = {
		//TV
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

		//TATAKAN TENGAH
		24,  25,  26,  24,  26,  27,   // front
		28,  29,  30,  28,  30,  31,   // right
		32,  33,  34, 32,  34, 35,  // back
		36, 38, 37, 36, 39, 38,  // left
		40, 42, 41, 40, 43, 42,  // upper
		44, 46, 45, 44, 47, 46,   // bottom

		//TATAKAN BAWAH
		48,  49,  50,  48,  50,  51,   // front
		52,  53,  54,  52,  54,  55,   // right
		56,  57,  58, 56,  58, 59,  // back
		60, 62, 61, 60, 63, 62,  // left
		64, 66, 65, 64, 67, 66,  // upper
		68, 70, 69, 68, 71, 70,   // bottom
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredTV()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPintuKayu() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pintuKayu3.jpeg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// PINTU LEMARI KIRI
		-2.6,  -0.4,  0.51, 0, 0,  // 0 <- 4
		-1.2,  -0.4, 0.51, 1, 0,   // 1 <- 5
		-1.2, 2.9, 0.51, 1, 1,   // 2 <- 1
		-2.6, 2.9, 0.51, 0, 1,   // 3 <- 0

		// PINTU LEMARI KANAN
		-1.0,  -0.4,  0.51, 0, 0,  // 0 <- 4
		0.4,  -0.4, 0.51, 1, 0,   // 1 <- 5
		0.4, 2.9, 0.51, 1, 1,   // 2 <- 1
		-1.0, 2.9, 0.51, 0, 1,   // 3 <- 0

		// PINTU LACI
		1.9,  -0.4,  0.51, 0, 0,  // 0 <- 4
		4.4,  -0.4, 0.51, 1, 0,   // 1 <- 5
		4.4, 0.4, 0.51, 1, 1,   // 2 <- 1
		1.9, 0.4, 0.51, 0, 1,   // 3 <- 0
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // pintu lemari kiri
		4,  5,  6,  4,  6,  7,   // pintu lemari kanan
		8,  9,  10, 8,  10, 11,  // pintu laci
	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredPintuKayu()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPintuKulkas() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pintuKulkas2.jpeg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// PINTU LEMARI KULKAS BAWAH
		-5.4,  -0.4,  0.51, 0, 0,  // 0 <- 4
		-4.1,  -0.4, 0.51, 1, 0,   // 1 <- 5
		-4.1, 1.19, 0.51, 1, 1,   // 2 <- 1
		-5.4, 1.19, 0.51, 0, 1,   // 3 <- 0

		// PINTU LEMARI KULKAS ATAS
		-5.4,  1.2,  0.51, 0, 0,  // 0 <- 4
		-4.1,  1.2, 0.51, 1, 0,   // 1 <- 5
		-4.1, 2.4, 0.51, 1, 1,   // 2 <- 1
		-5.4, 2.4, 0.51, 0, 1,   // 3 <- 0
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // pintu lemari bawah
		4,  5,  6,  4,  6,  7,   // pintu lemari atas
	};

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredPintuKulkas()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lantai2.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-10.3, -0.5, 10,  0,  0,
		 10, -0.5, 10, 50,  0,
		 10, -0.5,  -0.6, 50, 50,
		-10.3, -0.5,  -0.6,  0, 50,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}


int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Camera: Free Camera Implementation", 900, 900, false, false);
}