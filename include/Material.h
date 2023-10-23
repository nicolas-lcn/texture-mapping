#ifndef MATERIAL_H_DEFINED
#define MATERIAL_H_DEFINED
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <stdexcept>
#include <vector>

#define SKYBOX 0;
#define MESH 1;


struct Material {
	// Shader program
	GLuint m_program;
	GLuint m_programMesh;
	GLuint m_programSkyBox;
	GLuint m_programReflection;

	// Material parameters
	glm::vec4 m_color;
	glm::vec3 m_lightDiffuse;
	glm::vec3 m_lightSpecular;
	glm::vec3 m_lightPos;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	int m_type;

	float m_shininess;
	GLint m_texture;
	GLint m_normalMap;
	GLint m_cubeMap;

	GLuint skyboxVAO, skyboxVBO;
	GLuint cubeVAO, cubeVBO;

	

	inline void check() {
		if (m_programMesh == 0 || m_programSkyBox == 0) {
			throw std::runtime_error("Shader program not initialized");
		}
	}

	Material(): m_program(0) {

	}

	virtual ~Material();

	virtual void init();

	virtual void clear();

	void bind(int type);

	virtual void internalBind();

	void setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix);

	GLint getAttribute(const std::string& in_attributeName);

	GLint getUniform(const std::string& in_uniformName);
};

#endif
