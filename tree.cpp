#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <GL/glut.h>
#include <vector>

#define MAXLEVEL 8

using namespace std;

int idx = 0;

struct TreeNode {
	int level, index;
	float mainAngle;
	float xAngle, yAngle, zAngle;
	vector<TreeNode*> children;

	TreeNode() {}
	TreeNode(int l, float m, float x, float y, float z) {
		level = l, mainAngle = m;
		xAngle = x, yAngle = y, zAngle = z;
		index = ++idx;
	}
};

TreeNode* root;


void traverseTree(TreeNode* root) {
	cout<<root->index<<" "<<root->level<<" "<<root->mainAngle<<" "<<root->xAngle<<" "<<root->yAngle<<" "<<root->zAngle;
	if(root->children.size() > 0) {
		for(int i=0; i<root->children.size(); i++) {
			cout<<" "<<root->children[i]->index;
		}
		cout<<endl;
	} else {
		cout<<" 0 0 0"<<endl;
	}
	for(int i=0; i<root->children.size(); i++) {
		traverseTree(root->children[i]);
	}
}

typedef enum {
	NA,
	TREE,
	BRANCH,
	LEAF,
	TREEMAT,
	AXES
} DisplayLists;

long treeSeed;
int LEVEL = 4; 

void makeTree(int level, TreeNode* root) {
	long seed;
	float mainAngle;
	float angle1y, angle1z;
	float angle2y, angle2z;
	float angle3y, angle3z;

	if(level <= LEVEL) {
		glCallList(BRANCH);
		glPushMatrix();
			mainAngle = drand48()*180;
			glRotatef(mainAngle, 0, 1, 0);
			glTranslatef(0, 1, 0);
			glScalef(0.7, 0.7, 0.7);

			seed = (long) drand48()*ULONG_MAX;
			glPushMatrix();
				angle1y = 110 + drand48()*40;
				angle1z = 30 + drand48()*20;
				// Tree Tree
				TreeNode* node1 = new TreeNode(level+1, mainAngle, 0.0, angle1y, angle1z);
				glRotatef(angle1y, 0, 1, 0);
				glRotatef(angle1z, 0, 0, 1);
				makeTree(level+1, node1);
			glPopMatrix();

			srand48(seed);
			seed = (long) drand48()*ULONG_MAX;
			glPushMatrix();
				angle2y = -130 + drand48()*40;
				angle2z = 30 + drand48()*20;
				// Tree Tree
				TreeNode* node2 = new TreeNode(level+1, mainAngle, 0.0, angle2y, angle2z);
				glRotatef(angle2y, 0, 1, 0);
				glRotatef(angle2z, 0, 0, 1);
        		makeTree(level + 1, node2);
      		glPopMatrix();

      		srand48(seed);
      		glPushMatrix();
      			angle3y = -20 + drand48()*40;
				angle3z = 30 + drand48()*20;
				// Tree Tree
				TreeNode* node3 = new TreeNode(level+1, mainAngle, 0.0, angle3y, angle3z);
        		glRotatef(angle3y, 0, 1, 0);
        		glRotatef(angle3z, 0, 0, 1);
        		makeTree(level + 1, node3);
      		glPopMatrix();
    	glPopMatrix();

    	root->children.push_back(node1);
    	root->children.push_back(node2);
    	root->children.push_back(node3);
	}
}

void createLists() {
	GLUquadricObj * cyl = gluNewQuadric();

	glNewList(BRANCH, GL_COMPILE);
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(cyl, 0.1, 0.08, 1, 10, 2);
	glPopMatrix();
	glEndList();
}

void myGLInit() {
	treeSeed = time(NULL);

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

void createTree() {
	srand48(treeSeed);
	glNewList(TREE, GL_COMPILE);
		glPushMatrix();
		glCallList(TREEMAT);
		glTranslatef(0, -1, 0);
		makeTree(0, root);
		glPopMatrix();

		traverseTree(root);

	glEndList();
}

void PolarLookFrom(GLfloat dist, GLfloat elevation, GLfloat azimuth)
{
  glTranslatef(0, 0, -dist);
  glRotatef(elevation, 1, 0, 0);
  glRotatef(azimuth, 0, 1, 0);

}

void agvViewTransform(void)
{ 
    PolarLookFrom(8.0, 30.0, 0.0);
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

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();

	agvViewTransform();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	createTree();

	glCallList(TREE);

	glutSwapBuffers();
	glFlush();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow("Tree Animation");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	myGLInit();

	root = (TreeNode*) malloc(sizeof(TreeNode));
	root->index = 0, root->level = 0, root->mainAngle = 0.0;
	root->xAngle = 0.0, root->yAngle = 0.0, root->zAngle = 0.0;

	glutMainLoop();
	return 0;

}