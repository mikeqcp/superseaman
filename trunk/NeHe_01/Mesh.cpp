#include "StdAfx.h"
#include "Mesh.h"


Mesh::Mesh(vector<Vertex> *verticesVector,vector<Vertex> *normalsVector , vector<Face> facesVector, string name) 
	: name(name), vertices(verticesVector), normals(normalsVector){

	noFaces = facesVector.size();
	faces = new Face[noFaces];

	for(unsigned i = 0; i < noFaces; i++)
		faces[i] = facesVector[i];

}


Mesh::~Mesh(void){


}

void Mesh::Draw(){

	for(unsigned i = 0; i < noFaces; i++){
		Face f = faces[i];

		if(f.vertices.size() == 4)
			glBegin(GL_QUADS);
		else if(f.vertices.size() == 3)
			glBegin(GL_TRIANGLES);

		{
			Vertex n = normals -> at(f.normalIndex-1);
			glNormal3f(n.x, n.y, n.z);

			for(unsigned j = 0; j < f.vertices.size(); j++){

				Vertex v = vertices -> at(f.vertices[j]-1);
				
				glVertex3f(v.x, v.y, v.z);

			}
		}
		glEnd();

	}

}