#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <GL/glut.h>
#include <vector>
#include <stack>

#include "agviewer.c"

#define V struct vector3f

using namespace std;

struct vector3f
{
	float x, y, z;
	vector3f() { };
	vector3f(float gx, float gy, float gz) : x(gx), y(gy), z(gz) { };
};

void normalize(V & v)
{
	float norm = sqrt((v.x)*(v.x)+(v.y)*(v.y)+(v.z)*(v.z));
	if(norm > 0.0)
	{
		v.x = v.x/norm;
		v.y = v.y/norm;
		v.z = v.z/norm;
	}
}

float dotproduct(V v1, V v2) {
	float ret = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return ret;
}

V crossproduct(V u, V v) {
	V cross(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
	return cross;
}

struct TreeNode {
	int level, index;
	float mainAngle;
	float xAngle, yAngle, zAngle;
	vector<TreeNode*> children;
	int child1, child2, child3;
	TreeNode() {}
	TreeNode(int l, float m, float x, float y, float z) {
		level = l, mainAngle = m;
		xAngle = x, yAngle = y, zAngle = z;
		// index = ++idx;
	}
};

float power(float a, int n) {
	float res = 1;
	for(int i=0; i<n; i++) {
		res *= a;
	}
	return res;
}

typedef enum {
	NA,
	TREE,
	BRANCH,
	LEAF,
	TREEMAT,
	AXES
} DisplayLists;

TreeNode* root;
vector<TreeNode*> nodes;
int N; // numNodes
V windForce;
float oscCoeff = 0.5, bendingCoeff = 75;
float elasticMod = 40.5;
float thisTime = 0.0;

void readTree() {
	cin>>N;
	cout<<"N = "<<N<<endl;
	int nn = N+1;
	// nodes.push_back(root);
	int idx, level, child1, child2, child3;
	float mainAngle, xAngle, yAngle, zAngle;
	while(nn--) {
		cin>>idx>>level>>mainAngle>>xAngle>>yAngle>>zAngle>>child1>>child2>>child3;
		TreeNode* node = new TreeNode(level, mainAngle, xAngle, yAngle, zAngle);
		node->index = idx; node->child1 = child1, node->child2 = child2, node->child3 = child3;
		nodes.push_back(node);
	}
	return;
}

void traverseAndBuildTree(int index, float mAngle, float anglex, float angley, float anglez) {
	// cout<<"time = "<<thisTime<<endl;
	if(index <= N) {
		TreeNode* node = nodes[index];
		TreeNode* child1 = nodes[node->child1];
		TreeNode* child2 = nodes[node->child2];
		TreeNode* child3 = nodes[node->child3];
		float angle1y, angle1z;

		bendingCoeff = 75 + (rand() % 6);

		float t = 0.09*power(0.7, node->level);
		float l = power(0.7, node->level);
		float k = 100*(elasticMod*bendingCoeff*t*t*t)/(4*l*l);

		float Px = windForce.x*(1 + oscCoeff*sin(thisTime + bendingCoeff));
		float Pz = windForce.z*(1 + oscCoeff*sin(thisTime + bendingCoeff));
		float dx = Px/k;
		float dz = Pz/k;
		float thetaZ = 2*asin(dx/l);
		float thetaX = 2*asin(dz/l);
		if(node->level == 4)
			cout<<"theta = "<<(thetaX)<<" "<<(thetaZ)<<endl;

		glCallList(BRANCH);
		glPushMatrix();
			float mainAngle = child1->mainAngle;
			glRotatef(mainAngle, 0, 1, 0);
			glTranslatef(0, 2, 0);
			glScalef(0.7, 0.7, 0.7);

			glPushMatrix();
				//rotate wind
				glRotatef(-mAngle - mainAngle, 0, 1, 0);
				glRotatef(-anglex, 1, 0, 0);
				glRotatef(-angley, 0, 1, 0);
				glRotatef(-anglez, 0, 0, 1);
				glRotatef(thetaX, 1, 0, 0);
				glRotatef(thetaZ, 0, 0, 1);
				glRotatef(anglez, 0, 0, 1);
				glRotatef(angley, 0, 1, 0);
				glRotatef(anglex, 1, 0, 0);
				glRotatef(mAngle + mainAngle, 0, 1, 0);
				//rotate wind
				angle1y = child1->yAngle;
				angle1z = child1->zAngle;
				glRotatef(angle1y, 0, 1, 0);
				glRotatef(angle1z, 0, 0, 1);
				if(child1->index != 0)
					traverseAndBuildTree(child1->index, mAngle + mainAngle, anglex + thetaX, angley + angle1y, anglez + angle1z + thetaZ);
			glPopMatrix();
			glPushMatrix();
				//rotate wind
				glRotatef(-mAngle - mainAngle, 0, 1, 0);
				glRotatef(-anglex, 1, 0, 0);
				glRotatef(-angley, 0, 1, 0);
				glRotatef(-anglez, 0, 0, 1);
				glRotatef(thetaX, 1, 0, 0);
				glRotatef(thetaZ, 0, 0, 1);
				glRotatef(anglez, 0, 0, 1);
				glRotatef(angley, 0, 1, 0);
				glRotatef(anglex, 1, 0, 0);
				glRotatef(mAngle + mainAngle, 0, 1, 0);
				//rotate wind
				angle1y = child2->yAngle;
				angle1z = child2->zAngle;
				glRotatef(angle1y, 0, 1, 0);
				glRotatef(angle1z, 0, 0, 1);
				if(child2->index != 0)
					traverseAndBuildTree(child2->index, mAngle + mainAngle, anglex + thetaX, angley + angle1y, anglez + angle1z + thetaZ);
			glPopMatrix();
			glPushMatrix();
				//rotate wind
				glRotatef(-mAngle - mainAngle, 0, 1, 0);
				glRotatef(-anglex, 1, 0, 0);
				glRotatef(-angley, 0, 1, 0);
				glRotatef(-anglez, 0, 0, 1);
				glRotatef(thetaX, 1, 0, 0);
				glRotatef(thetaZ, 0, 0, 1);
				glRotatef(anglez, 0, 0, 1);
				glRotatef(angley, 0, 1, 0);
				glRotatef(anglex, 1, 0, 0);
				glRotatef(mAngle + mainAngle, 0, 1, 0);
				//rotate wind
				angle1y = child3->yAngle;
				angle1z = child3->zAngle;
				glRotatef(angle1y, 0, 1, 0);
				glRotatef(angle1z, 0, 0, 1);
				if(child3->index != 0)
					traverseAndBuildTree(child3->index, mAngle + mainAngle, anglex + thetaX, angley + angle1y, anglez + angle1z + thetaZ);
			glPopMatrix();
		glPopMatrix();
	}
}


void createTree() {
	glPushMatrix();
		glCallList(TREEMAT);
		glTranslatef(0, -2, 0);
		traverseAndBuildTree(0, 0.0, 0.0, 0.0, 0.0);
	glPopMatrix();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();

	agvViewTransform();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	thisTime+= 1;
	// cout<<"Creating Tree with time = "<<thisTime<<endl;
	createTree();

	glCallList(TREE);

	glutSwapBuffers();
	glutPostRedisplay();
	glFlush();
}

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w/h, 0.01, 100);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
}

void createLists() {
	GLUquadricObj * cyl = gluNewQuadric();

	glNewList(BRANCH, GL_COMPILE);
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(cyl, 0.1, 0.08, 2, 10, 2);
	glPopMatrix();
	glEndList();
}

void myGLInit() {

	GLfloat l_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat l_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat l_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat l_position[] = { 0.0, 0.3, 0.3, 0.0 };

	GLfloat lmodel_ambient[] = {0.4, 0.4, 0.4, 1.0};

	GLfloat tree_amb_diff[] =   { 0.4, 0.25, 0.1, 1.0 };
	GLfloat tree_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat tree_shininess[] = { 0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, l_position);
    
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glNewList(TREEMAT, GL_COMPILE);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tree_amb_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, tree_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, tree_shininess);
	glEndList();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	createLists();

	glFlush();
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	readTree();

	glutInit(&argc, argv);
	glutInitWindowSize(1280, 640);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow("TreeAnimation");

	agvInit(1);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	agvMakeAxesList(AXES);
	myGLInit();

	windForce.x = 2.0, windForce.y = 0.0, windForce.z = 0.0;

	glutMainLoop();

	return 0;
	
}

