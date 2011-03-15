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

void Mesh::DirectDraw(){

	//glPolygonMode(GL_BACK, GL_LINE);

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


		glBegin(GL_LINES);

		{
			for(unsigned j = 0; j < f.vertices.size(); j++){
				
				Vertex n = normals[f.normalIndex[j]-1];
				Vertex v = vertices[f.vertices[j]-1];
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(v.x, v.y, v.z);
				glVertex3f(v.x + n.x, v.y + n.y, v.z+n.z);

			}
		}
		glEnd();


	}
	glPolygonMode(GL_FRONT, GL_FILL);
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

vector<Edge> Mesh::createEdgeList(int noVertices){

	vector<Edge> edges;

	bool **incMatrix = new bool*[noVertices];
	for(int i = 0; i < noVertices; i++)
		incMatrix[i] = new bool[noVertices];

	for(int i = 0; i < noVertices; i++)
		for(int j = i; j < noVertices; j++)
			incMatrix[i][j] = false;

	for(unsigned i = 0; i < noFaces; i++){

		for(unsigned j = 0; j < faces[i].vertices.size(); j++){

			int a, b;

			if(j == faces[i].vertices.size() - 1){

				a = faces[i].vertices[j]-1;
				b = faces[i].vertices[0]-1;

			} else {

				a = faces[i].vertices[j]-1;
				b = faces[i].vertices[j+1]-1;

			}

			incMatrix[a][b] = true;
			incMatrix[b][a] = true;

		}

	}

	for(int i = 0; i < noVertices; i++){
		for(int j = i; j < noVertices; j++){
			if(incMatrix[i][j]){
				Edge edge;
				edge.v1 = i;
				edge.v2 = j;
				edges.push_back(edge);
			}
		}
	}

	for(int i = 0; i < noVertices; i++)
		delete [] incMatrix[i];

	delete [] incMatrix;

	return edges;

}

void Mesh::ComputeNormals(int noVertices){

	int counter = 0;
	normals = new Vertex[noFaces*3];
	for(unsigned i = 0; i < noFaces; i++){

		faces[i].normalIndex.clear();

		Vertex a = vertices[faces[i].vertices[0]-1];
		Vertex b = vertices[faces[i].vertices[1]-1];
		Vertex c = vertices[faces[i].vertices[2]-1];

		Vertex normal;
		Vertex U, V;

		U.x = a.x - b.x;
		U.y = a.y - b.y;
		U.z = a.z - b.z;

		V.x = c.x - b.x;
		V.y = c.y - b.y;
		V.z = c.z - b.z;

		normal.x = U.y * V.z - U.z * V.y;
		normal.y = U.z * V.x - U.x * V.z;
		normal.z = U.x * V.y - U.y * V.x;

		GLfloat length = sqrt(normal.x * normal.x + normal.y*normal.y + normal.z*normal.z);

		normal.x *= 1.0f/length;
		normal.y *= 1.0f/length;
		normal.z *= 1.0f/length;

		normals[counter++] = normal;
		faces[i].normalIndex.push_back(counter);
		normals[counter++] = normal;
		faces[i].normalIndex.push_back(counter);
		normals[counter++] = normal;
		faces[i].normalIndex.push_back(counter);

	}

}