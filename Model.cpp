#include "Model.h"

Model::Model(){}

Model::Model(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile): P(P), V(V), M(M){

	clipPlane = glm::vec4(0, 0, 0, 0);
	SetupShaders(vshaderFile, fshaderFile);
	Load(fileName, 0, 0);
	verticesBufferType = GL_STATIC_DRAW;
	SetupBuffers();

}

Model::~Model(void){

	CleanShaders();
	CleanBuffers();

}

void Model::Draw(){
	
	glUseProgram(shaderProgram);

	SetupUniformVariables();

	for(unsigned i = 0; i < meshCount; i++){
	
		meshes[i].Draw();

	}

}

void Model::DrawReflection(){

	glUseProgram(shaderProgram);

	SetupUniformVariables();

	for(unsigned i = 0; i < meshCount; i++){
		glm::mat4 prevM = meshes[i].GetModelMatrix();
		glm::mat4 refM = glm::scale(prevM, glm::vec3(1, -1, 1));

		meshes[i].Update(P, V, refM, lightPos);
		meshes[i].Draw();
		meshes[i].Update(P, V, prevM, lightPos);

	}

}

void Model::SetupUniformVariables(){

	//zmienne jednorodne dla calego modelu
	glUniform3f(locLookAtPos, lookAtPos.x, lookAtPos.y, lookAtPos.z);
	glUniform3f(locViewPos, viewPos.x, viewPos.y, viewPos.z);
	glUniform4f(locClipPlane, clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
	glUniform1i(locMaterial[3], 0);
	glUniform1i(locMaterial[4], 1);
	glUniform1i(locMaterial[5], 2);
	glUniform1i(locMaterial[6], 3);

}

void Model::Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	this ->P = P;
	this ->V = V;
	this ->M = M;
	this ->lightPos = lightPos;
	for(unsigned i = 0; i < meshCount; i++)
		meshes[i].Update(P, V, M, lightPos);
	
}

void Model::UpdateMesh(string name, glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	for(unsigned i = 0; i < meshCount; i++){
		if(meshes[i].GetName().compare(name) == 0)
			meshes[i].Update(P, V, M, lightPos);
	}

}

glm::vec4 *Model::GetSegment(string mname, string msname, int *length){

	glm::vec4 *result = NULL;
	*length = 0;

	for(unsigned i = 0; i < meshCount; i++){
		if(meshes[i].GetName().compare(mname) == 0){
		
			MeshSegment ms = meshes[i].GetSegment(msname);
			if(ms.materialName != ""){
			
				result = new glm::vec4[ms.vertexCount];
				for(int j = 0, k = ms.begin; j < ms.vertexCount; j++, k++){
					result[j] = glm::vec4(
						vertices[k*4], 
						vertices[k*4 + 1], 
						vertices[k*4 + 2], 
						vertices[k*4 + 3]
					);
					*length = ms.vertexCount;
				}

			}
			break;

		}
			
	}

	return result;

}

void Model::SetupBuffers(){

	/*
	*	Model's vertex buffer setup
	*/

	glGenBuffers(1, &bufVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bufVertices);
	glBufferData(GL_ARRAY_BUFFER, 
		verticesCount* sizeof(GLfloat)*4,
		vertices,
		GL_STATIC_DRAW);

	/*
	*	Model's normal buffer setup
	*/

	glGenBuffers(1, &bufNormals);
	glBindBuffer(GL_ARRAY_BUFFER, bufNormals);
	glBufferData(GL_ARRAY_BUFFER,
		verticesCount*sizeof(GLfloat)*4,
		normals,
		GL_STATIC_DRAW);

	/*
	*	Model's texture coords buffer setup
	*/

	glGenBuffers(1, &bufTexCoords);
	glBindBuffer(GL_ARRAY_BUFFER, bufTexCoords);
	glBufferData(GL_ARRAY_BUFFER,
		verticesCount*sizeof(GLfloat)*2,
		texturesCoords,
		GL_STATIC_DRAW);

	/*
	* Model's TBN buffers
	*/

	glGenBuffers(1, &bufTBNCol1);
	glBindBuffer(GL_ARRAY_BUFFER, bufTBNCol1);
	glBufferData(GL_ARRAY_BUFFER,
		verticesCount*sizeof(GLfloat)*3,
		matTBNcol1,
		GL_STATIC_DRAW);

	glGenBuffers(1, &bufTBNCol2);
	glBindBuffer(GL_ARRAY_BUFFER, bufTBNCol2);
	glBufferData(GL_ARRAY_BUFFER,
		verticesCount*sizeof(GLfloat)*3,
		matTBNcol2,
		GL_STATIC_DRAW);

	glGenBuffers(1, &bufTBNCol3);
	glBindBuffer(GL_ARRAY_BUFFER, bufTBNCol3);
	glBufferData(GL_ARRAY_BUFFER,
		verticesCount*sizeof(GLfloat)*3,
		matTBNcol3,
		GL_STATIC_DRAW);
		
	/*
	*	Model's vao setup
	*/

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, bufVertices);
	glEnableVertexAttribArray(locVertex);
	glVertexAttribPointer(locVertex, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufNormals);
	glEnableVertexAttribArray(locNormal);
	glVertexAttribPointer(locNormal, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufTexCoords);
	glEnableVertexAttribArray(locTexCoord);
	glVertexAttribPointer(locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufTBNCol1);
	glEnableVertexAttribArray(locTBN[0]);
	glVertexAttribPointer(locTBN[0], 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufTBNCol2);
	glEnableVertexAttribArray(locTBN[1]);
	glVertexAttribPointer(locTBN[1], 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufTBNCol3);
	glEnableVertexAttribArray(locTBN[2]);
	glVertexAttribPointer(locTBN[2], 3, GL_FLOAT, GL_FALSE, 0, 0);

	for(unsigned i = 0; i < meshCount; i++){

		meshes[i].SetVao(vao);
		meshes[i].SetLocations(locP, locV, locM, locLighPos, locMaterial);
		meshes[i].Update(P, V, M, lightPos);

	}

}

void Model::CleanBuffers(){

	glDeleteBuffers(1, &bufVertices);
	glDeleteBuffers(1, &bufNormals);
	glDeleteBuffers(1, &bufIndices);
	glDeleteVertexArrays(1, &vao);

}


void Model::SetupShaders(string vshaderFile, string fshaderFile){

	vertexShader = LoadShader(GL_VERTEX_SHADER, vshaderFile);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fshaderFile);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}

	locP = glGetUniformLocation(shaderProgram, "P");
	locV = glGetUniformLocation(shaderProgram, "V");
	locM = glGetUniformLocation(shaderProgram, "M");
	locLighPos = glGetUniformLocation(shaderProgram, "lightPos");
	locLookAtPos = glGetUniformLocation(shaderProgram, "lookAtPos");
	locViewPos = glGetUniformLocation(shaderProgram, "viewPos");

	locMaterial[0] = glGetUniformLocation(shaderProgram, "ambient");
	locMaterial[1] = glGetUniformLocation(shaderProgram, "diffuse");
	locMaterial[2] = glGetUniformLocation(shaderProgram, "specular");
	locMaterial[3] = glGetUniformLocation(shaderProgram, "map_Ka");
	locMaterial[4] = glGetUniformLocation(shaderProgram, "map_Kd");
	locMaterial[5] = glGetUniformLocation(shaderProgram, "map_Ks");
	locMaterial[6] = glGetUniformLocation(shaderProgram, "map_N");

	locClipPlane = glGetUniformLocation(shaderProgram, "clipPlane");

	locEnableTexturing[0] = glGetUniformLocation(shaderProgram, "enableTexturingA");
	locEnableTexturing[1] = glGetUniformLocation(shaderProgram, "enableTexturingD");
	locEnableTexturing[2] = glGetUniformLocation(shaderProgram, "enableTexturingS");
	locEnableTexturing[3] = glGetUniformLocation(shaderProgram, "enableTexturingN");

	locTexCoord = glGetAttribLocation(shaderProgram, "texCoord");

	locVertex = glGetAttribLocation(shaderProgram, "vertex");
	locNormal = glGetAttribLocation(shaderProgram, "normal");

	locTBN[0] = glGetAttribLocation(shaderProgram, "tbnCol1");
	locTBN[1] = glGetAttribLocation(shaderProgram, "tbnCol2");
	locTBN[2] = glGetAttribLocation(shaderProgram, "tbnCol3");


}


GLuint Model::LoadShader(GLenum shaderType, string fileName){

	Helper helper;

	GLuint shader = glCreateShader(shaderType);
	const GLchar* shaderSource = helper.ReadBinaryFile((char *)fileName.c_str());

	glShaderSource(shader, 1, &shaderSource, NULL);

	glCompileShader(shader);

	int infologLength = 0, charsWritten = 0;
	char* infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if(infologLength > 1){
	
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);

		printf("%s\n", infoLog);
		delete [] infoLog;

	}

	return shader;
}

void Model::CleanShaders() {

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);

}

void Model::Load(string fileName, Texture *textures, int textureCount){

	Helper helper;

	this ->textures = textures;
	this ->textureCount = textureCount;

	int loaded = 0;
	string name;

	vector<glm::vec4> vvertices;
	vector<GLuint> vindices;
	vector<glm::vec4> vnormals;
	vector<GLuint> vnormalsIndices;
	vector<glm::vec2> vtextureCoords;
	vector<GLuint> vtextureCoordsIndices;
	vector<MeshSegment> ms;
	vector<Mesh> vmeshes;

	materials = NULL;
	string materialFileName;
	string currentMaterialName;

	int begin = 0, counter = 0;

	ifstream file;
	file.open(fileName);
	{
		if(file.is_open()){
			while(!file.eof()){

				string read;
				getline(file, read);
				vector<string> split = helper.SplitString(read);

				if(split[0].compare("o") == 0 || file.eof()){

					if(loaded){

						if(file.eof()){
						
							//TODO upewnic sie, czy segmenty dobrze sa tworzone

						}
						
						MeshSegment newMS;
						newMS.begin = begin;
						newMS.vertexCount = counter;
						newMS.materialName = currentMaterialName;
						ms.push_back(newMS);

						Mesh *newMesh = new Mesh(ms, name);
						vmeshes.push_back(*newMesh);
						delete newMesh;
						ms.clear();

					}
					loaded++;
					name.clear();
					if(!file.eof())
						name = split[1];
					
						
					
				} else if(split[0].compare("v") == 0){

					GLfloat x, y, z;
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);

					glm::vec4 v(x, y, z, 1);
					vvertices.push_back(v);

				} else if(split[0].compare("vn") == 0){

					GLfloat x, y, z;
					sscanf_s(split[1].c_str(), "%f", &x);
					sscanf_s(split[2].c_str(), "%f", &y);
					sscanf_s(split[3].c_str(), "%f", &z);

					glm::vec4 v(x, y, z, 1);
					vnormals.push_back(v);

				} else if(split[0].compare("vt") == 0){

					GLfloat u, v;
					sscanf_s(split[1].c_str(), "%f", &u);
					sscanf_s(split[2].c_str(), "%f", &v);

					glm::vec2 t(u, v);

					vtextureCoords.push_back(t);

				} else if(split[0].compare("f") == 0){
				
					int ind, normal, tex;
					for(unsigned i = 1; i < split.size(); i++){

						int first = split[i].find_first_of('/');
						int last =  split[i].find_last_of('/');

						counter++;

						if(first == string::npos){

							sscanf_s(split[i].c_str(), "%d", &ind);
							vindices.push_back(ind);
							vnormalsIndices.push_back(0);
							vtextureCoordsIndices.push_back(0);

						} else {

							string sub = split[i].substr(0, first);
						
							sscanf_s(sub.c_str(), "%d", &ind);
							vindices.push_back(ind);

							sub = split[i].substr(last + 1);
							sscanf_s(sub.c_str(), "%d", &normal);
							vnormalsIndices.push_back(normal);

							if (first + 1 != last){

								sub = split[i].substr(first + 1, last);
								sscanf_s(sub.c_str(), "%d", &tex);
								vtextureCoordsIndices.push_back(tex);

							} else 
								vtextureCoordsIndices.push_back(0);

						} 

					}

				} else if(split[0].compare("mtllib") == 0){

					materialFileName = split[1];

				} else if(split[0].compare("usemtl") == 0){

					if(counter){
						MeshSegment newMS;
						newMS.begin = begin;
						newMS.vertexCount = counter;
						newMS.materialName = currentMaterialName;
						ms.push_back(newMS);
						begin += counter;
						counter = 0;
					}

					currentMaterialName = split[1];

				}
			}
		}
	}
	file.close();

	//Materials data
	ifstream materialFile;
	materialCount = 0;
	int currentMaterial = -1;
	materialFile.open("Models/" + materialFileName);
	{
		if(materialFile.is_open()){
			while(!materialFile.eof()){

				string read;
				getline(materialFile, read);
				vector<string> split = helper.SplitString(read);

				if(split[0].compare("#") == 0){

					if(split.size() == 4 && split[1].compare("Material") == 0 && split[2].compare("Count:") == 0){

						sscanf_s(split[3].c_str(), "%d", &materialCount);
						materialCount++;
						if(materialCount > 0)
							materials = new Material[materialCount];

					}

				} else if (split[0].compare("newmtl") == 0){

					currentMaterial++;
					materials[currentMaterial].setName(split[1]);

				} else if (split[0].compare("Ns") == 0){

					GLfloat Ns;
					sscanf_s(split[1].c_str(), "%f", &Ns);
					materials[currentMaterial].setNs(Ns);

				} else if (split[0].compare("Ni") == 0){

					GLfloat Ni;
					sscanf_s(split[1].c_str(), "%f", &Ni);
					materials[currentMaterial].setNi(Ni);

				} else if (split[0].compare("d") == 0){

					GLfloat d;
					sscanf_s(split[1].c_str(), "%f", &d);
					materials[currentMaterial].setD(d);

				} else if (split[0].compare("illum") == 0){

					int illum;
					sscanf_s(split[1].c_str(), "%d", &illum);
					materials[currentMaterial].setIllum(illum);

				} else if (split[0].compare("Ka") == 0){

					GLfloat r, g, b;
					
					sscanf_s(split[1].c_str(), "%f", &r);
					sscanf_s(split[2].c_str(), "%f", &g);
					sscanf_s(split[3].c_str(), "%f", &b);
					
					glm::vec4 Ka(r, g, b, 1);
					materials[currentMaterial].setKa(Ka);

				} else if (split[0].compare("Kd") == 0){

					GLfloat r, g, b;
					
					sscanf_s(split[1].c_str(), "%f", &r);
					sscanf_s(split[2].c_str(), "%f", &g);
					sscanf_s(split[3].c_str(), "%f", &b);
					
					glm::vec4 Kd(r, g, b, 1);
					materials[currentMaterial].setKd(Kd);

				} else if (split[0].compare("Ks") == 0){

					GLfloat r, g, b;
					
					sscanf_s(split[1].c_str(), "%f", &r);
					sscanf_s(split[2].c_str(), "%f", &g);
					sscanf_s(split[3].c_str(), "%f", &b);
					
					glm::vec4 Ks(r, g, b, 1);
					materials[currentMaterial].setKs(Ks);

				} else if (split[0].compare("map_Kd") == 0){

					materials[currentMaterial].setMap_Kd(split[1].substr(0, split[1].find_last_of('.')));

				} else if (split[0].compare("map_Ks") == 0){

					materials[currentMaterial].setMap_Ks(split[1].substr(0, split[1].find_last_of('.')));

				} else if (split[0].compare("map_Ka") == 0){

					materials[currentMaterial].setMap_Ka(split[1].substr(0, split[1].find_last_of('.')));

				} else if (split[0].compare("map_Kn") == 0){

					materials[currentMaterial].setMap_Kn(split[1].substr(0, split[1].find_last_of('.')));

				}
			}
		}
	}
	materialFile.close();

	materials[materialCount - 1].setName("(null)");
	materials[materialCount - 1].setKa(glm::vec4(0));
	materials[materialCount - 1].setKd(glm::vec4(1));
	materials[materialCount - 1].setKs(glm::vec4(0));

	
	//unsigned nov = vvertices.size();
	//unsigned non = vnormalsIndices.size();
	unsigned texCoordCount = vtextureCoords.size();
	
	unsigned iCount = vindices.size();
	originalVerticesCount = vvertices.size();
	verticesCount = iCount;
	indicesCount = vindices.size();
	normalsCount = verticesCount;
	meshCount = vmeshes.size();

	originalVertices = new glm::vec4[originalVerticesCount];
	glm::vec2 *originalTexCoord = new glm::vec2[texCoordCount];
	vertices = new GLfloat[iCount*4];
	normals = new GLfloat[iCount*4];
	texturesCoords = new GLfloat[iCount*2];
	indices = new GLuint[iCount];
	GLuint *texIndices = new GLuint[iCount];
	meshes = new Mesh[meshCount];
	matTBNcol1 = new GLfloat[iCount*3];
	matTBNcol2 = new GLfloat[iCount*3];
	matTBNcol3 = new GLfloat[iCount*3];
	tangent = new glm::vec3[originalVerticesCount];
	bitangent = new glm::vec3[originalVerticesCount];
	norm = new glm::vec3[originalVerticesCount];

	for(unsigned i = 0; i < originalVerticesCount; i++)
		originalVertices[i] = vvertices[i];

	for(unsigned i = 0; i < texCoordCount; i++)
		originalTexCoord[i] = vtextureCoords[i];

	glm::vec4 v, n;
	glm::vec2 t;
	for(unsigned i = 0; i < iCount; i++){

		v = vvertices[vindices[0]-1];
		vertices[i*4] = v.x;
		vertices[i*4 + 1] = v.y;
		vertices[i*4 + 2] = v.z;
		vertices[i*4 + 3] = 1;

		n = vnormals[vnormalsIndices[0]-1];
		normals[i*4] = n.x;
		normals[i*4 + 1] = n.y;
		normals[i*4 + 2] = n.z;
		normals[i*4 + 3] = 1;

		if(texCoordCount > 0 && vtextureCoordsIndices[0] > 0){

			t = vtextureCoords[vtextureCoordsIndices[0]-1];

			texturesCoords[i*2] = t.x;
			texturesCoords[i*2 + 1] = t.y;

		}

		texIndices[i] = vtextureCoordsIndices[0]-1;
		indices[i] = vindices[0]-1;
		
		vnormalsIndices.erase(vnormalsIndices.begin());
		vindices.erase(vindices.begin());
		vtextureCoordsIndices.erase(vtextureCoordsIndices.begin());
	}

	//liczenie tangent i bitangent

	

	for(int i = 0; i < originalVerticesCount; i++){

		tangent[i] = glm::vec3(0);
		bitangent[i] = glm::vec3(0);

	}

	if(texCoordCount > 0){
		int ip1, ip2, ip3, it1, it2, it3;
		glm::vec3 p1, p2, p3, v1, v2, T, B, N;
		glm::vec2 t1, t2, t3, c1, c2;
		GLfloat W;
		for(int i = 0; i < indicesCount; i+=3){
	
			if(texIndices[i] >= 0){

				ip1 = indices[i];
				ip2 = indices[i+1];
				ip3 = indices[i+2];

				it1 = texIndices[i];
				it2 = texIndices[i+1];
				it3 = texIndices[i+2];

				p1 = glm::vec3(originalVertices[ip1]);
				p2 = glm::vec3(originalVertices[ip2]);
				p3 = glm::vec3(originalVertices[ip3]);

				t1 = originalTexCoord[it1];
				t2 = originalTexCoord[it2];
				t3 = originalTexCoord[it3];

				v1 = p2 - p1;
				v2 = p3 - p1;

				c1 = t2 - t1;
				c2 = t3 - t1;

				W = c1.x * c2.y - c2.x * c1.y;

				T = (v1*c2.y - v2*c1.y)/W;
				B = (v2*c1.x - v1*c2.x)/W;

				tangent[ip1] += T;
				tangent[ip2] += T;
				tangent[ip3] += T;

				bitangent[ip1] += B;
				bitangent[ip2] += B;
				bitangent[ip3] += B;

			}

		}
		glm::vec3 t, b, n;
		glm::mat3 invTBN;
		for(int i = 0; i < originalVerticesCount; i++){

			tangent[i] = glm::normalize(tangent[i]);
			bitangent[i] = glm::normalize(bitangent[i]);

			norm[i] = glm::normalize(glm::cross(glm::vec3(tangent[i]), glm::vec3(bitangent[i])));

			invTBN = glm::inverse(glm::mat3(tangent[i], bitangent[i], norm[i]));

			tangent[i] = glm::vec3(invTBN[0][0], invTBN[1][0], invTBN[2][0]);
			bitangent[i] = glm::vec3(invTBN[0][1], invTBN[1][1], invTBN[2][1]);
			norm[i] = glm::vec3(invTBN[0][2], invTBN[1][2], invTBN[2][2]);
		}

		int index;
		for(int i = 0; i < iCount; i++){

			index = indices[i];
		
			t = tangent[index];
			b = bitangent[index];
			n = norm[index];
			
			matTBNcol1[i*3] = t.x;
			matTBNcol1[i*3+1] = t.y;
			matTBNcol1[i*3+2] = t.z;

			matTBNcol2[i*3] = b.x;
			matTBNcol2[i*3+1] = b.y;
			matTBNcol2[i*3+2] = b.z;

			matTBNcol3[i*3] = n.x;
			matTBNcol3[i*3+1] = n.y;
			matTBNcol3[i*3+2] = n.z;

		}

	} else {

		int index;
		for(int i = 0; i < iCount; i++){

			matTBNcol1[i*3] = 0;
			matTBNcol1[i*3+1] = 0;
			matTBNcol1[i*3+2] = 0;

			matTBNcol2[i*3] = 0;
			matTBNcol2[i*3+1] = 0;
			matTBNcol2[i*3+2] = 0;

			matTBNcol3[i*3] = 0;
			matTBNcol3[i*3+1] = 0;
			matTBNcol3[i*3+2] = 0;

		}

	}
	

	//----------------------------

	for(unsigned i = 0; i < meshCount; i++){
	
		meshes[i] = vmeshes[0];
		vmeshes.erase(vmeshes.begin());

	}

	vvertices.clear();
	vindices.clear();
	vnormals.clear();
	vnormalsIndices.clear();
	vtextureCoords.clear();
	vtextureCoordsIndices.clear();
	vmeshes.clear();

	for(unsigned i = 0; i < meshCount; i++){

		meshes[i].SetMaterials(materials, materialCount);

	}

}

void Model::UpdateRotation(GLfloat angle, glm::vec3 axis){

	this ->rotationAngle = angle;
	this ->rotationAxis = axis;

}

glm::mat4 Model::GetModelMatrix(){

	return glm::rotate(glm::mat4(1), rotationAngle, rotationAxis);

}

void Model::SetTextures(Texture *textures, unsigned textureCount){ 
	
	this ->textures = textures; 
	this ->textureCount = textureCount; 

	for(unsigned i = 0; i < meshCount; i++){
	
		meshes[i].SetTextures(textures, textureCount, locEnableTexturing);

	}
};