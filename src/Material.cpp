// Local includes
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Context.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

Material::~Material() {
	clear();
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	// TODO : Change shader by your
    m_programSkyBox = load_shaders("../shaders/unlit/vertex-skybox.glsl", "../shaders/unlit/fragment-skybox.glsl");
    m_programMesh = load_shaders("../shaders/unlit/vertex-phong.glsl", "../shaders/unlit/fragment-phong.glsl");
    m_programReflection = load_shaders("../shaders/unlit/vertex-reflection.glsl", "../shaders/unlit/fragment-reflection.glsl");
    float skyboxVertices[] = {
    // positions
    -3.0f,  3.0f, -3.0f,
    -3.0f, -3.0f, -3.0f,
     3.0f, -3.0f, -3.0f,
     3.0f, -3.0f, -3.0f,
     3.0f,  3.0f, -3.0f,
    -3.0f,  3.0f, -3.0f,

    -3.0f, -3.0f,  3.0f,
    -3.0f, -3.0f, -3.0f,
    -3.0f,  3.0f, -3.0f,
    -3.0f,  3.0f, -3.0f,
    -3.0f,  3.0f,  3.0f,
    -3.0f, -3.0f,  3.0f,

     3.0f, -3.0f, -3.0f,
     3.0f, -3.0f,  3.0f,
     3.0f,  3.0f,  3.0f,
     3.0f,  3.0f,  3.0f,
     3.0f,  3.0f, -3.0f,
     3.0f, -3.0f, -3.0f,

    -3.0f, -3.0f,  3.0f,
    -3.0f,  3.0f,  3.0f,
     3.0f,  3.0f,  3.0f,
     3.0f,  3.0f,  3.0f,
     3.0f, -3.0f,  3.0f,
    -3.0f, -3.0f,  3.0f,

    -3.0f,  3.0f, -3.0f,
     3.0f,  3.0f, -3.0f,
     3.0f,  3.0f,  3.0f,
     3.0f,  3.0f,  3.0f,
    -3.0f,  3.0f,  3.0f,
    -3.0f,  3.0f, -3.0f,

    -3.0f, -3.0f, -3.0f,
    -3.0f, -3.0f,  3.0f,
     3.0f, -3.0f, -3.0f,
     3.0f, -3.0f, -3.0f,
    -3.0f, -3.0f,  3.0f,
     3.0f, -3.0f,  3.0f
    };
   
    std::vector<std::string> faces
    {
        "../data/skybox/right.jpg",
        "../data/skybox/left.jpg",
        "../data/skybox/top.jpg",
        "../data/skybox/bottom.jpg",
        "../data/skybox/front.jpg",
        "../data/skybox/back.jpg"
    };
    m_cubeMap = loadCubeMap(faces);
    //skybox
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    m_color = {1.0, 1.0, 1.0, 1.0};
    m_texture = loadTexture2DFromFilePath("../data2/metalrose.png");
    m_normalMap = loadTexture2DFromFilePath("../data2/rainbow.jpg");
    m_lightPos = {4.0, 4.0, 4.0};
    m_lightDiffuse = {1.0, 1.0, 1.0};
    m_lightSpecular = {1.0, 1.0, 1.0};
    m_ambient = {1.0, 1.0, 1.0};
    m_diffuse = {1.0, 1.0, 1.0};
    m_specular = {1.0, 1.0, 1.0};
    m_shininess = 16.0f;
    
	check();
	
    
}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
    if(m_type == 0)
    {
    	glDeleteVertexArrays(1, &skyboxVAO);
    	glDeleteBuffers(1, &skyboxVBO);
    }
}	

void Material::bind(int type) {
	
    check();
    m_type = type;
    if(m_type == 0)  m_program = m_programSkyBox;
    else if (m_type == 1)  m_program = m_programMesh;
    else m_program = m_programReflection;
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind() {
	// // bind parameters
	
	// TODO : Add your custom parameters here
    if(m_type == 0)
    {
        glDepthFunc(GL_LEQUAL);
    	glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }
    else if(m_type == 1)
    {
        GLint color = getUniform("color");
        glUniform4fv(color, 1, glm::value_ptr(m_color));
        GLint lightpos = getUniform("light.pos");
        glUniform3fv(lightpos, 1, glm::value_ptr(m_lightPos));
        GLint lightDiffuse = getUniform("light.diffuse");
        glUniform3fv(lightDiffuse, 1, glm::value_ptr(m_lightDiffuse));
        GLint lightSpecular = getUniform("light.specular");
        glUniform3fv(lightSpecular, 1, glm::value_ptr(m_lightSpecular));
        GLint ambient = getUniform("material.ambient");
        glUniform3fv(ambient, 1, glm::value_ptr(m_ambient));
        GLint diffuse = getUniform("material.diffuse");
        glUniform3fv(diffuse, 1, glm::value_ptr(m_diffuse));
        GLint specular = getUniform("material.specular");
        glUniform3fv(specular, 1, glm::value_ptr(m_specular));
        GLint shininess = getUniform("material.shininess");
        glUniform1f(shininess, m_shininess);
        

        if (m_texture != -1) {

         glUniform1i(getUniform("colorTexture"), 0);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, m_texture);    
        }
        

        if (m_normalMap!= -1){
         glUniform1i(getUniform("normalMap"), 1);
         glActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_2D, m_normalMap);  
        }
    }
    else if (m_type == 2)
    {
        GLint cameraposition = getUniform("cameraPos");
        glUniform3fv(cameraposition, 1, glm::value_ptr(Context::camera.position));

        glUniform1i(getUniform("skybox"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

    
    }
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
