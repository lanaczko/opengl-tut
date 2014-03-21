/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_utils.h"
/* ADD GLOBAL VARIABLES HERE LATER */
int screen_width=800, screen_height=600;
GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_fade;
//GLint uniform_m_transform;
GLuint vbo_cube_vertices;
GLuint ibo_cube_elements;
GLint uniform_mvp;

struct attributes {
  GLfloat coord3d[3];
  GLfloat v_color[3];
};

int init_resources(void)
{
  GLuint vs, fs;
  GLint link_ok;
  if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    return 0;
  }

  const char* attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  const char* uniform_name;
  uniform_name = "fade";
  uniform_fade = glGetUniformLocation(program, uniform_name);
  if (uniform_fade == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  struct attributes cube_attributes[] = {
    {{-1.0, -1.0,  1.0}, {1.0, 0.0, 0.0}},
    {{ 1.0, -1.0,  1.0}, {0.0, 1.0, 0.0}},
    {{ 1.0,  1.0,  1.0}, {0.0, 0.0, 1.0}},
    {{-1.0,  1.0,  1.0}, {1.0, 1.0, 1.0}},
    {{-1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
    {{ 1.0, -1.0, -1.0}, {0.0, 1.0, 0.0}},
    {{ 1.0,  1.0, -1.0}, {0.0, 0.0, 1.0}},
    {{-1.0,  1.0, -1.0}, {1.0, 1.0, 1.0}}
  };

  glGenBuffers(1, &vbo_cube_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_attributes), cube_attributes, GL_STATIC_DRAW);

  GLushort cube_elements[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    3, 2, 6,
    6, 7, 3,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // left
    4, 0, 3,
    3, 7, 4,
    // right
    1, 5, 6,
    6, 2, 1,
  };
  glGenBuffers(1, &ibo_cube_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (attribute_v_color == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

#if 0
  uniform_name = "m_transform";
  uniform_m_transform = glGetUniformLocation(program, uniform_name);
  if (uniform_m_transform == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }
#endif

  uniform_name = "mvp";
  uniform_mvp = glGetUniformLocation(program, uniform_name);
  if (uniform_mvp == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  return 1;
}
 
void onDisplay()
{
  /* Clear the background as white */
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  //glUniform1f(uniform_fade, 0.1);
  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord3d);
  glEnableVertexAttribArray(attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
  glVertexAttribPointer(
    attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    sizeof(struct attributes), // next coord3d appears every 5 floats
    0                  // offset of first element
  );

  glVertexAttribPointer(
    attribute_v_color, // attribute
    3,                 // number of elements per vertex, here (r,g,b)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    5 * sizeof(GLfloat),    // next color appears every 5 floats
    (GLvoid*) (2 * sizeof(GLfloat))  // offset of first element
  );
 
  /* Push each element in buffer_vertices to the vertex shader */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(attribute_coord3d);
  glDisableVertexAttribArray(attribute_v_color);
 
  /* Display the result */
  glutSwapBuffers();
}
 
void free_resources()
{
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_cube_vertices);
}

void onIdle()
{
  float move = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 5); // -1<->+1 every 5 seconds
  float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45° per second
  glm::vec3 axis_z(0, 0, 1);
  //glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), angle, axis_z);
  //glm::mat4 m_transform = glm::rotate(glm::mat4(1.0f), angle, axis_z) * glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0));

  float cur_fade = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*M_PI) / 5) / 2 + 0.5; // 0->1->0 every 5 seconds
  //float cur_fade = 1.0;

  glm::vec3 axis_y(0.0, 1.0, 0.0);
  glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, axis_y);

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
  glm::mat4 mvp = projection * view * model * anim;

  glUseProgram(program);

  glUniform1f(uniform_fade, cur_fade);
  //glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
  glutPostRedisplay();
}

void onReshape(int width, int height) {
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
}
 
int main(int argc, char* argv[])
{
  /* Glut-related initialising functions */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(screen_width, screen_height);
  glutCreateWindow("My First Triangle");

  // Enable alpha
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable depth
  glEnable(GL_DEPTH_TEST);

  glutReshapeFunc(onReshape);
 
  /* Extension wrangler initialising */
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return EXIT_FAILURE;
  }
 
  /* When all init functions run without errors,
  the program can initialise the resources */
  if (1 == init_resources())
  {
    /* We can display it if everything goes OK */
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onIdle);
    glutMainLoop();
  }
 
  /* If the program exits in the usual way,
  free resources and exit with a success */
  free_resources();
  return EXIT_SUCCESS;
}

