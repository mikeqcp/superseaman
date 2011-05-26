#include "Mesh.h"

Mesh::Mesh(){
}

Mesh::Mesh(vector<MeshSegment> ms, string name) : name(name){

	segmentsCount = ms.size();
	segments = new MeshSegment[segmentsCount];

	for(int i = 0; i < segmentsCount; i++){
	
		segments[i] = ms[i];
		segments[i].texture = 0;

	}

	enableTexturing = 0;

}


Mesh::~Mesh(void)
{
}


void Mesh::Draw(){

	for(int i = 0; i < segmentsCount; i++){

		if(materials[segments[i].materialIndex].getMap_Kd() != ""){

			enableTexturing = 1;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, segments[i].texture);
			

		} else enableTexturing = 0;

		SetupUniformVariables(segments[i].materialIndex);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, segments[i].begin, segments[i].vertexCount);

		glBindVertexArray(0);

	}

}

void Mesh::Update(glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec4 lightPos){

	this ->P = P;
	this ->V = V;
	this ->M = M;
	this ->lightPos = lightPos;

}

void Mesh::SetupUniformVariables(int index){

	glUniformMatrix4fv(locP,1, false, glm::value_ptr(P));
	glUniformMatrix4fv(locV,1, false, glm::value_ptr(V));
	glUniformMatrix4fv(locM,1, false, glm::value_ptr(M));

	glUniform4f(locLighPos, lightPos.x, lightPos.y, lightPos.z, lightPos.w);

	glm::vec4 ambient = materials[index].getKa();
	glUniform4f(locations[0], ambient.r, ambient.g, ambient.b, ambient.a);

	glm::vec4 diffuse = materials[index].getKd();
	glUniform4f(locations[1], diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	
	glUniform1i(locEnableTexturing, enableTexturing);

}

void Mesh::SetMaterials(Material *materials, int materialCount){ 

	this ->materials = materials; 

	for(int i = 0; i < segmentsCount; i++){
	
		for(int j = 0; j < materialCount; j++){
		
			if(materials[j].getName().compare(segments[i].materialName) == 0){
			
				segments[i].materialIndex = j;
				break;

			}

		}

	}

}

MeshSegment Mesh::GetSegment(string name){

	for(int i = 0; i < segmentsCount; i++){
	
		if(segments[i].materialName == name)
			return segments[i];

	}

	MeshSegment ms;
	ms.materialName = "";

	return ms;

}

void Mesh::SetTextures(Texture *textures, unsigned textureCount, GLuint locEnableTexturing){ 

	this ->textures = textures;
	this ->textureCount = textureCount;
	this ->locEnableTexturing = locEnableTexturing;

	for(int i = 0; i < segmentsCount; i++){
	
		for(int j = 0; j < textureCount; j++){
		
			if(materials[segments[i].materialIndex].getMap_Kd() == textures[j].name){
			
				segments[i].texture = textures[j].tex;

			}

		}

	}

};