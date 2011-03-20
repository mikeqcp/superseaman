#include "StdAfx.h"
#include "Mesh.h"


Mesh::Mesh(vector<Face> facesVector, string name) 
	: name(name){

	noFaces = facesVector.size();
	faces = new Face[noFaces];

	for(unsigned i = 0; i < noFaces; i++)
		faces[i] = facesVector[i];

	normalsInitialized = false;
	directMeshCLcreated = false;

	currentKeyFrame = 0;
	noKeyFrames = 1;

	meshCL = glGenLists(noKeyFrames);
	currentCL = meshCL;

}


Mesh::~Mesh(void){


}

void Mesh::DirectDraw(bool showNormals){

	if(!directMeshCLcreated){
		GLuint current;
		directMeshCL = glGenLists(noFaces);
		current = directMeshCL;

		for(unsigned i = 0; i < noFaces; i++){

			glNewList(current, GL_COMPILE);
			Face f = faces[i];
			bool skip = false;
			if(f.materialName.compare("(null)") == 0) skip = true;

			if(!skip){
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

						Vector3D Ks = mat.getKs();
						GLfloat specular[] = { Ks.x, Ks.y, Ks.z, mat.getNs() };

						glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

						Vector3D Kd = mat.getKd();
						GLfloat diffuse[] = { Kd.x, Kd.y, Kd.z, 1.0f };

						glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

						Vector3D Ka = mat.getKa();
						GLfloat ambient[] = { Ka.x, Ka.y, Ka.z, 1.0f };

						glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

					}

				}
			}

			glEndList();
			current++;
		}
		

		directMeshCLcreated = true;

	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	GLuint current = directMeshCL;
	for(unsigned i = 0; i < noFaces; i++){
		Face f = faces[i];

		if(f.materialName.compare("pins") == 0) {
			current++;
			continue;
		}
		if(f.materialName.compare("bompins") == 0){
			current++;
			continue;
		}

		glCallList(current++);

		glBegin(GL_TRIANGLES);

		{

			for(unsigned j = 0; j < f.vertices.size(); j++){

				if(f.textures[j] > 0){
					TextureUV t = textureCoords[f.textures[j]-1];
					glTexCoord2f(t.u, t.v);
				}
				
				if(f.normalIndex[j] > 0){
					Vector3D n = normals[f.normalIndex[j]-1];
					glNormal3f(n.x, n.y, n.z);
				}

				Vector3D v = vertices[f.vertices[j]-1];
				glVertex3f(v.x, v.y, v.z);

			}

			int vertNum = f.vertices.size()-1;

			for(int j = vertNum; j >= 0; j--){

				if(f.textures[j] > 0){
					TextureUV t = textureCoords[f.textures[j]-1];
					glTexCoord2f(t.u, t.v);
				}
				
				if(f.normalIndex[j] > 0){
					Vector3D n = normals[f.normalIndex[j]-1];
					glNormal3f(n.x*-1.0f, n.y*-1.0f, n.z*-1.0f);
				}

				Vector3D v = vertices[f.vertices[j]-1];
				glVertex3f(v.x, v.y, v.z);

			}

		}
		glEnd();

		if(showNormals){

			glBegin(GL_LINES);

			{
				for(unsigned j = 0; j < f.vertices.size(); j++){
				
					Vector3D n = normals[f.normalIndex[j]-1];
					Vector3D v = vertices[f.vertices[j]-1];
					glColor3f(1.0f, 1.0f, 1.0f);
					glVertex3f(v.x, v.y, v.z);
					glVertex3f(v.x - n.x, v.y - n.y, v.z - n.z);

				}
			}
			glEnd();

		}


	}
	glPolygonMode(GL_FRONT, GL_FILL);
}

void Mesh::Draw(){

	glCallList(currentCL);


}

void Mesh::UpdateVerticesNormalsTexturesMaterials(
	Vector3D *vertices, 
	int noVertices,
	Vector3D *normals, 
	Texture *textures, 
	int textureCount, 
	TextureUV * textureCoords, 
	Material *materials, 
	int mCount ){

	this -> vertices = vertices;
	this -> noVertices = noVertices;
	this -> normals = normals;
	this -> textures = textures;
	this -> noTextures = textureCount;
	this -> textureCoords = textureCoords;
	this -> materials = materials;
	this -> noMaterials = mCount;

}

void Mesh::BuildLists(){

	
	glNewList(currentCL, GL_COMPILE);

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

				Vector3D Ks = mat.getKs();
				GLfloat specular[] = { Ks.x, Ks.y, Ks.z, mat.getNs() };

				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

				Vector3D Kd = mat.getKd();
				GLfloat diffuse[] = { Kd.x, Kd.y, Kd.z, 1.0f };

				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

				Vector3D Ka = mat.getKa();
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
					Vector3D n = normals[f.normalIndex[j]-1];
					glNormal3f(n.x, n.y, n.z);
				}

				Vector3D v = vertices[f.vertices[j]-1];
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

void Mesh::ComputeNormals(){

	int counter = 0;

	if(!normalsInitialized){
		normals = new Vector3D[noVertices];
		dividers = new short[noVertices];
		for(unsigned i = 0; i < noVertices; i++)
			dividers[i] = 1;

		for(unsigned i = 0; i < noFaces; i++){

			if(faces[i].materialName.compare("pins") == 0 || faces[i].materialName.compare("bompins") == 0)
			continue;

			for(unsigned j = 0; j < faces[i].vertices.size(); j++)
				dividers[j]++;

		}

		normalsInitialized = true;
	}

	for(unsigned i = 0; i < noFaces; i++){

		if(faces[i].materialName.compare("pins") == 0 || faces[i].materialName.compare("bompins") == 0)
			continue;

		faces[i].normalIndex.clear();

		if(faces[i].vertices.size() < 3){

			faces[i].normalIndex.push_back(-1);

			faces[i].normalIndex.push_back(-1);

			continue;
		}

		int ind1 = faces[i].vertices[0]-1;
		int ind2 = faces[i].vertices[1]-1;
		int ind3 = faces[i].vertices[2]-1;

		Vector3D a = vertices[ind1];
		Vector3D b = vertices[ind2];
		Vector3D c = vertices[ind3];

		Vector3D normal;
		Vector3D U, V;

		U = a - b;
		V = c - b;

		normal.x = U.y * V.z - U.z * V.y;
		normal.y = U.z * V.x - U.x * V.z;
		normal.z = U.x * V.y - U.y * V.x;

		normal.normalize();
		normal.negate();
		
		int index;
		for(unsigned j = 0; j < faces[i].vertices.size(); j++){
		
			index = faces[i].vertices[j];
			if(normals[index] == 0)
				normals[index] = normal;
			else 
				normals[index] += normal;
		}

		for(unsigned j = 0; j < faces[i].vertices.size(); j++){
		
			index = faces[i].vertices[j];
			faces[i].normalIndex.push_back(index+1);

		}

	}

	for(unsigned i = 0; i < noVertices; i++){

		normals[i] /= (GLfloat)dividers[i];

		normals[i].normalize();
		GLfloat length = normals[i].length();

	}

}

vector<int> Mesh::getOutline(){

	vector<int> outline;

	for(unsigned i = 0; i < noFaces; i++){

		if(faces[i].materialName.compare("pins") == 0 || faces[i].materialName.compare("bompins") == 0){
			for(unsigned j = 0; j < faces[i].vertices.size(); j++)
				outline.push_back(faces[i].vertices[j]);

		}

	}

	return outline;

}

vector<int> Mesh::getBomPins(){

	vector<int> outline;

	for(unsigned i = 0; i < noFaces; i++){

		if(faces[i].materialName.compare("bompins") == 0){
			for(unsigned j = 0; j < faces[i].vertices.size(); j++)
				outline.push_back(faces[i].vertices[j]);

		}

	}

	return outline;

}

void Mesh::rotate(GLfloat angle, Vector3D v){

	MatrixBase rotMatrix;
	rotMatrix.createRotation(v, (int)angle);

	GLfloat **vector = new GLfloat*[1];
		vector[0] = new GLfloat[4];

	for(int i = 0; i < noVertices; i++){

		vector[0][0] = vertices[i].x;
		vector[0][1] = vertices[i].y;
		vector[0][2] = vertices[i].z;
		vector[0][3] = 1.0f;

		MatrixBase m1(vector, 1, 4);

		MatrixBase result = m1 * rotMatrix;

		vertices[i].x = result.get(0, 0);
		vertices[i].y = result.get(0, 1);
		vertices[i].z = result.get(0, 2);

	}

	delete [] vector[0];
	delete [] vector;
	
	ComputeNormals();

}

void Mesh::createRotation(Vector3D v){

	currentKeyFrame = 0;
	noKeyFrames = 361;

	glDeleteLists(meshCL, 1);
	meshCL = glGenLists(noKeyFrames);
	currentCL = meshCL;

	for(int i = 0; i < noKeyFrames; i++){

		BuildLists();
		rotate(1, v);
		currentCL++;

	}

	currentCL = meshCL;

}

void Mesh::NextKeyFrame(){

	if(currentKeyFrame == noKeyFrames+1){

		currentKeyFrame = 0;
		currentCL = meshCL;

	} else {

		currentKeyFrame++;
		currentCL++;

	}

}