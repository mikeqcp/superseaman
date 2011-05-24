#include "Model.h"

Model::Model(){}

Model::Model(string fileName, glm::mat4 P, glm::mat4 V, glm::mat4 M, string vshaderFile, string fshaderFile): P(P), V(V), M(M){

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

	for(int i = 0; i < meshCount; i++){
	
		meshes[i].Draw();

	}

}

void Model::DrawReflection(){

	glUseProgram(shaderProgram);

	for(int i = 0; i < meshCount; i++){
		glm::mat4 prevM = meshes[i].GetModelMatrix();
		glm::mat4 refM = glm::scale(prevM, glm::vec3(1, -1, 1));

		meshes[i].Update(P, V, refM, lightPos);
		meshes[i].Draw();
		meshes[i].Update(P, V, prevM, lightPos);

	}

}

void Model::SetupUniformVariables(){

	//zmienne jednorodne dla calego modelu

}

void Model::Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	this ->P = P;
	this ->V = V;
	this ->M = M;
	this ->lightPos = lightPos;
	for(int i = 0; i < meshCount; i++)
		meshes[i].Update(P, V, M, lightPos);
	
}

void Model::UpdateMesh(string name, glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	for(int i = 0; i < meshCount; i++){
		if(meshes[i].GetName().compare(name) == 0)
			meshes[i].Update(P, V, M, lightPos);
	}

}

glm::vec4 *Model::GetSegment(string mname, string msname, int *length){

	glm::vec4 *result = NULL;
	*length = 0;

	for(int i = 0; i < meshCount; i++){
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

	for(int i = 0; i < meshCount; i++){

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

	locMaterial[0] = glGetUniformLocation(shaderProgram, "ambient");
	locMaterial[1] = glGetUniformLocation(shaderProgram, "diffuse");
	locMaterial[2] = glGetUniformLocation(shaderProgram, "specular");

	locVertex = glGetAttribLocation(shaderProgram, "vertex");
	locNormal = glGetAttribLocation(shaderProgram, "normal");

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

					materials[currentMaterial].setMap_Kd(split[1]);

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
	//unsigned not = vtextureCoords.size();
	
	unsigned iCount = vindices.size();
	originalVerticesCount = vvertices.size();
	verticesCount = iCount;
	indicesCount = vindices.size();
	normalsCount = verticesCount;
	meshCount = vmeshes.size();

	originalVertices = new glm::vec4[originalVerticesCount];
	vertices = new GLfloat[iCount*4];
	normals = new GLfloat[iCount*4];
	indices = new GLuint[iCount];
	meshes = new Mesh[meshCount];
	//textureCoordsTab = new TextureUV[not];

	glm::vec4 v, n;
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

		indices[i] = vindices[0]-1;
		
		vnormalsIndices.erase(vnormalsIndices.begin());
		vindices.erase(vindices.begin());

	}

	for(unsigned i = 0; i < originalVerticesCount; i++){
	
		originalVertices[i] = vvertices[0];
		vvertices.erase(vvertices.begin());

	}

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

	for(int i = 0; i < meshCount; i++){

		meshes[i].SetMaterials(materials, materialCount);

	}

	/*for(unsigned i = 0; i < not; i++){
		textureCoordsTab[i] = textureCoords[0];
		textureCoords.erase(textureCoords.begin());
	}*/

	/*for(unsigned i = 0; i < meshes.size(); i++){
		meshes[i].UpdateVerticesNormalsTexturesMaterials(
			verticesTab, 
			nov,
			normalsTab, 
			textures, 
			textureCount, 
			textureCoordsTab, 
			materials, 
			materialCount
			);

		meshes[i].BuildLists();
	}*/

}

void Model::UpdateRotation(GLfloat angle, glm::vec3 axis){

	this ->rotationAngle = angle;
	this ->rotationAxis = axis;

}

glm::mat4 Model::GetModelMatrix(){

	return glm::rotate(glm::mat4(1), rotationAngle, rotationAxis);

}