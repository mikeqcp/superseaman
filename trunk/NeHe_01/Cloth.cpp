#include "StdAfx.h"
#include "Cloth.h"


Cloth::Cloth()
{

        gravity.x = 0.4f;
        gravity.y = -2.0f;
        gravity.z = 0;
                
}

void Cloth::Draw(){

	meshes[0].DirectDraw();

}

void Cloth::BuildCloth(){

        edges = createEdgeList();
        noVertices = VerticesCount();

        oldVertices = new Vertex[noVertices];
        verticesAcc = new Vertex[noVertices];

        pinned = new int[3];

        for(int i = 0; i < noVertices; i++){

                oldVertices[i] = verticesTab[i];
				if(verticesTab[i].y == -1.0f && verticesTab[i].z == -1.0f)
					pinned[0] = i;

				else if(verticesTab[i].y == -1.0f && verticesTab[i].z == 1.0f)
					pinned[1] = i;

				else if(verticesTab[i].y == 1.0f && verticesTab[i].z == -1.0f)
					pinned[2] = i;
        }

        constraints = new GLfloat[edges.size()];
        vector<Edge> ed = edges;
        int i = 0;
        while(!ed.empty()){

                Edge e = ed.at(0);
                ed.erase(ed.begin());
                
                Vertex v1 = verticesTab[e.v1];
                Vertex v2 = verticesTab[e.v2];
                Vertex delta;
                
                delta.x = v2.x - v1.x;
                delta.y = v2.y - v1.y;
                delta.z = v2.z - v1.z;

                GLfloat dotProduct = delta.x *delta.x + delta.y*delta.y + delta.z * delta.z;

                constraints[i] = sqrt(dotProduct);

                i++;

        }



}

Cloth::~Cloth(void)
{

}

void Cloth::Verlet(){

        GLfloat fTimeStep = 0.02f;

        for(int i = 0; i < noVertices; i++){

			    Vertex v = verticesTab[i];
                Vertex temp = v;
                Vertex old = oldVertices[i];
                Vertex a = verticesAcc[i];

                v.x += v.x - old.x + a.x *fTimeStep*fTimeStep;
                v.y += v.y - old.y + a.y *fTimeStep*fTimeStep;
                v.z += v.z - old.z + a.z *fTimeStep*fTimeStep;

                verticesTab[i] = v;
                oldVertices[i] = temp;
		}

}

void Cloth::AccumulateForces(){

        for(int i = 0; i < noVertices; i++)
                verticesAcc[i] = gravity;

}

void Cloth::SatisfyConstraints(){

        vector<Edge> ed = edges;
        int i = 0;
        while(!ed.empty()){

                Edge e = ed.at(0);
                ed.erase(ed.begin());

                Vertex v1 = verticesTab[e.v1];
                Vertex v2 = verticesTab[e.v2];

                Vertex t1 = v1;
                Vertex t2 = v2;

                Vertex delta;

                delta.x = v2.x - v1.x;
                delta.y = v2.y - v1.y;
                delta.z = v2.z - v1.z;

                GLfloat dotProduct = delta.x *delta.x + delta.y*delta.y + delta.z * delta.z;
				GLfloat c = constraints[i++];

                //GLfloat deltalength = (c + dotProduct/c)/2;
				GLfloat deltalength = sqrt(dotProduct);
                GLfloat diff = (deltalength - c)/deltalength;

                v1.x = v1.x + delta.x*0.5f*diff;
                v1.y = v1.y + delta.y*0.5f*diff;
                v1.z = v1.z + delta.z*0.5f*diff;

                v2.x = v2.x - delta.x*0.5f*diff;
                v2.y = v2.y - delta.y*0.5f*diff;
                v2.z = v2.z - delta.z*0.5f*diff;

                verticesTab[e.v1] = v1;
                verticesTab[e.v2] = v2;
        }

		verticesTab[pinned[0]].x = 0;
		verticesTab[pinned[0]].y = -1;
		verticesTab[pinned[0]].z = -1;

		verticesTab[pinned[1]].x = 0;
		verticesTab[pinned[1]].y = -1;
		verticesTab[pinned[1]].z = 1;

		verticesTab[pinned[2]].x = 0;
		verticesTab[pinned[2]].y = 1;
		verticesTab[pinned[2]].z = -1;

}


void Cloth::TimeStep(){

        AccumulateForces();
        Verlet();
        SatisfyConstraints();
		meshes[0].ComputeNormals(noVertices);

}