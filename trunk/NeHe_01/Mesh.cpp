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

void Mesh::UpdateVerticesNormalsTextures(Vertex *vertices, Vertex *normals, Texture * textureCoords){

	this -> vertices = vertices;
	this -> normals = normals;
	this -> textureCoords = textureCoords;

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

				if(f.textures[j] > 0){
					Texture t = textureCoords[f.textures[j]-1];
					glEnable(GL_TEXTURE_2D);
					glTexCoord2d(t.u, t.v);
				} else 
					glDisable(GL_TEXTURE_2D);
				

				if(f.normalIndex[j] > 0){
					Vertex n = normals[f.normalIndex[j]-1];
					glNormal3d(n.x, n.y, n.z);
				}

				Vertex v = vertices[f.vertices[j]-1];
				glVertex3d(v.x, v.y, v.z);

			}
		}
		glEnd();

	}

	glEndList();

}