#include <vector>
#include <GL/GLUT.h>

using namespace std;

vector<GLfloat> readVertices(const char *filename);

void drawVertices(vector<GLfloat> vertices);
