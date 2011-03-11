#include "StdAfx.h"
#include "Mesh.h"


Mesh::Mesh(vector<Face> facesVector, string name) 
	: name(name){

	noFaces = facesVector.size();
	faces = new Face[noFaces];

	for(unsigned i = 0; i < noFaces; i++)
		faces[i] = facesVector[i];

}


Mesh::~Mesh(void){


}

void Mesh::Draw(){

	glCallList(meshCL);	

}

void Mesh::UpdateVerticesNormals(Vertex *vertices, Vertex *normals){

	this -> vertices = vertices;
	this -> normals = normals;
	this -> noVertices = noVertices;
	this -> noNormals = noNormals;

}

void Mesh::BuildLists(){

	meshCL = glGenLists(1);

	glNewList(meshCL, GL_COMPILE);

	for(unsigned i = 0; i < noFaces; i++){
		Face f = faces[i];

		if(f.vertices.size() == 4)
			glBegin(GL_QUADS);
		else if(f.vertices.size() == 3)
			glBegin(GL_TRIANGLES);
		else
			glBegin(GL_POLYGON);

		{
			for(unsigned j = 0; j < f.vertices.size(); j++){

				Vertex v = vertices[f.vertices[j]-1];
				Vertex n = normals[f.normalIndex[j]-1];

				glNormal3d(n.x, n.y, n.z);
				glVertex3d(v.x, v.y, v.z);

			}
		}
		glEnd();

	}

	glEndList();

}