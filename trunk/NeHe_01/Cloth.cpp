#include "StdAfx.h"
#include "Cloth.h"


Cloth::Cloth()
{
	showNormals = false;	
    gravity.x = 0.0f;
    gravity.y = -4.0f;
    gravity.z = 10.0f;
                
}

void Cloth::Draw(){

	meshes[0].DirectDraw(showNormals);

}

void Cloth::BuildCloth(){

        edges = createEdgeList();
        noVertices = VerticesCount();

        oldVertices = new Vertex[noVertices];
        verticesAcc = new Vertex[noVertices];
		masses = new short[noVertices];
		int count = 0;
        for(int i = 0; i < noVertices; i++){

                oldVertices[i] = verticesTab[i];
				masses[i] = 1;

        }

		vector<int> outline = getOutline();
        while(!outline.empty()){

			int e = outline.at(0);
            outline.erase(outline.begin());

			masses[e-1] = 0;

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

	delete [] masses;

}

void Cloth::Verlet(){

        GLfloat fTimeStep = 0.01f;

        for(int i = 0; i < noVertices; i++){

			    Vertex v = verticesTab[i];
                Vertex temp = v;
                Vertex old = oldVertices[i];
                Vertex a = verticesAcc[i];

                v.x += v.x - old.x + a.x *fTimeStep*fTimeStep*(GLfloat)masses[i];
                v.y += v.y - old.y + a.y *fTimeStep*fTimeStep*(GLfloat)masses[i];
                v.z += v.z - old.z + a.z *fTimeStep*fTimeStep*(GLfloat)masses[i];

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
				short m1 = masses[e.v1];
				short m2 = masses[e.v2];

                //GLfloat deltalength = (c + dotProduct/c)/2;
				GLfloat deltalength = sqrt(dotProduct);
                GLfloat diff = (deltalength - c)/deltalength;

                v1.x = v1.x + delta.x*0.5f*diff*(GLfloat)m1;
                v1.y = v1.y + delta.y*0.5f*diff*(GLfloat)m1;
                v1.z = v1.z + delta.z*0.5f*diff*(GLfloat)m1;

                v2.x = v2.x - delta.x*0.5f*diff*(GLfloat)m2;
                v2.y = v2.y - delta.y*0.5f*diff*(GLfloat)m2;
                v2.z = v2.z - delta.z*0.5f*diff*(GLfloat)m2;

                verticesTab[e.v1] = v1;
                verticesTab[e.v2] = v2;
        }

}


void Cloth::TimeStep(){

        AccumulateForces();
        Verlet();
        SatisfyConstraints();
		meshes[0].ComputeNormals();

}

void Cloth::ToogleNormals(){

	showNormals = !showNormals;

}

void Cloth::FlipWind(){

	gravity.z *= -1;

}