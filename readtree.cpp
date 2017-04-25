#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <GL/glut.h>
#include <vector>
#include <stack>

#include "agviewer.c"

using namespace std;

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
		res *= res;
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

// cout<<root->index<<" "<<root->level<<" "<<root->mainAngle<<" "<<root->xAngle<<" "<<root->yAngle<<" "<<root->zAngle;
// index, level, mainangle, xangle, yangle, zangle, child1, child2, child3
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

void traverseAndBuildTree(int index) {
	// cout<<"index = "<<index<<endl;
	// cout<<"N = "<<N<<endl;
	if(index <= N) {
		TreeNode* node = nodes[index];
		TreeNode* child1 = nodes[node->child1];
		TreeNode* child2 = nodes[node->child2];
		TreeNode* child3 = nodes[node->child3];
		// if(index == 0) {
		// 	cout<<"index1 = "<<node->index<<" begins"<<endl;
		// 	cout<<node->child1<<" "<<node->child2<<" "<<node->child3<<" "<<endl;
		// 	cout<<child1->level<<" "<<child2->level<<" "<<child3->level<<endl;
		// }

		float angle1y, angle1z;

		glCallList(BRANCH);
		glPushMatrix();
			// if(child1->mainAngle != child2->mainAngle || child1->mainAngle != child3->mainAngle) {
			// 	cout<<"lul ho kaa? "<<index<<endl;
			// 	cout<<child1->mainAngle<<" "<<child2->mainAngle<<" "<<child3->mainAngle<<endl;
			// }
			float mainAngle = child1->mainAngle;
			glRotatef(mainAngle, 0, 1, 0);
			glTranslatef(0, 1, 0);
			glScalef(0.7, 0.7, 0.7);

			glPushMatrix();
				angle1y = child1->yAngle;
				angle1z = child1->zAngle;
				glRotatef(angle1y, 0, 1, 0);
				glRotatef(angle1z, 0, 0, 1);
				if(child1->index != 0)
					traverseAndBuildTree(child1->index);
			glPopMatrix();
			glPushMatrix();
				angle1y = child2->yAngle;
				angle1z = child2->zAngle;
				glRotatef(angle1y, 0, 1, 0);
				glRotatef(angle1z, 0, 0, 1);
				if(child2->index != 0)
					traverseAndBuildTree(child2->index);
			glPopMatrix();
			glPushMatrix();
				angle1y = child3->yAngle;
				angle1z = child3->zAngle;
				glRotatef(angle1y, 0, 1, 0);
				glRotatef(angle1z, 0, 0, 1);
				if(child3->index != 0)
					traverseAndBuildTree(child3->index);
			glPopMatrix();
		glPopMatrix();
	}
	// cout<<"index1 = "<<index<<" ends"<<endl;

}


void createTree() {
	glPushMatrix();
		glCallList(TREEMAT);
		glTranslatef(0, -1, 0);
		traverseAndBuildTree(0);
	glPopMatrix();
}

// void PolarLookFrom(GLfloat dist, GLfloat elevation, GLfloat azimuth)
// {
//   glTranslatef(0, 0, -dist);
//   glRotatef(elevation, 1, 0, 0);
//   glRotatef(azimuth, 0, 1, 0);

// }

// void agvViewTransform(void)
// { 
//     PolarLookFrom(8.0, 30.0, 0.0);
// }

// void agvInit(int window)
// {
//   glutMouseFunc(agvHandleButton);
//   glutMotionFunc(agvHandleMotion);
//   glutKeyboardFunc(agvHandleKeys);
//   RedisplayWindow = glutGetWindow();
//   agvSetAllowIdle(window);
// }

// void agvMakeAxesList(int displaylistnum)
// {
//   int i,j;
//   GLfloat axes_ambuse[] =   { 0.5, 0.0, 0.0, 1.0 };
//   glNewList(displaylistnum, GL_COMPILE);
//   glPushAttrib(GL_LIGHTING_BIT);
//   glMatrixMode(GL_MODELVIEW);
//     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, axes_ambuse);
//     glBegin(GL_LINES);
//       glVertex3f(15, 0, 0); glVertex3f(-15, 0, 0);
//       glVertex3f(0, 15, 0); glVertex3f(0, -15, 0);
//       glVertex3f(0, 0, 15); glVertex3f(0, 0, -15);
//     glEnd();
//     for (i = 0; i < 3; i++) {
//       glPushMatrix();
//         glTranslatef(-10*(i==0), -10*(i==1), -10*(i==2));
//         for (j = 0; j < 21; j++) {
//           glutSolidCube(0.1);
//           glTranslatef(i==0, i==1, i==2);
// 	}
//       glPopMatrix();
//     }
//   glPopAttrib();
//   glEndList();  
// }

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
		gluCylinder(cyl, 0.1, 0.08, 1, 10, 2);
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
	// root = new TreeNode(0, 0.0, 0.0, 0.0, 0.0);
	// root->index = 0, root->child1 = 1, root->child2 = 2, root->child3 = 3;
	readTree();

	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow("TreeAnimation");

	agvInit(1);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	agvMakeAxesList(AXES);
	myGLInit();

	glutMainLoop();

	return 0;
	
}