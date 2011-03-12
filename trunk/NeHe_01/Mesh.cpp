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

void Mesh::UpdateVerticesNormalsTexturesMaterials(
	Vertex *vertices, 
	Vertex *normals, 
	Texture *textures, 
	int textureCount, 
	TextureUV * textureCoords, 
	Material *materials, 
	int mCount ){

	this -> vertices = vertices;
	this -> normals = normals;
	this -> textures = textures;
	this -> noTextures = textureCount;
	this -> textureCoords = textureCoords;
	this -> materials = materials;
	this -> noMaterials = mCount;

}

void Mesh::BuildLists(){

	meshCL = glGenLists(1);

	glNewList(meshCL, GL_COMPILE);

	for(unsigned i = 0; i < noFaces; i++){
		Face f = faces[i];

		for(int k = 0; k < noMaterials; k++){
			Material mat = materials[k];
			if(mat.getName().compare(f.materialName) == 0){

				for(int l = 0; l < noTextures; l++){
					if(mat.getMap_Kd().compare(textures[l].name) == 0){
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, textures[l].tex);
						break;
					}
					if(l == noTextures - 1)
						glDisable(GL_TEXTURE_2D);
				}

				glColorMaterial(GL_FRONT, GL_SPECULAR);

				Vertex Ks = mat.getKs();
				GLfloat specular[] = { Ks.x, Ks.y, Ks.z, mat.getNs() };

				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

				Vertex Kd = mat.getKd();
				GLfloat diffuse[] = { Kd.x, Kd.y, Kd.z, 1.0f };

				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

				Vertex Ka = mat.getKa();
				GLfloat ambient[] = { Ka.x, Ka.y, Ka.z, 1.0f };

				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

			}

		}

		if(f.vertices.size() == 4)
			glBegin(GL_QUADS);
		else if(f.vertices.size() == 3)
			glBegin(GL_TRIANGLES);
		else
			glBegin(GL_POLYGON);

		{
			for(unsigned j = 0; j < f.vertices.size(); j++){

				if(f.textures[j] > 0){
					TextureUV t = textureCoords[f.textures[j]-1];
					glTexCoord2f(t.u, t.v);
				}
				
				if(f.normalIndex[j] > 0){
					Vertex n = normals[f.normalIndex[j]-1];
					glNormal3f(n.x, n.y, n.z);
				}

				Vertex v = vertices[f.vertices[j]-1];
				glVertex3f(v.x, v.y, v.z);

			}
		}
		glEnd();

	}

	glEndList();

}