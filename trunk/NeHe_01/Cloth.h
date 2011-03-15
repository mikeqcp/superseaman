#pragma once
#include "modelstructures.h"

class Cloth : public Model
{
private:

        Vertex gravity;

        void Verlet();
        void SatisfyConstraints();
        void AccumulateForces();

        vector<Edge> edges;

        Vertex *oldVertices;
        Vertex *verticesAcc;
        GLfloat *constraints;
        int *pinned;
		Vertex *pins;

public:

        Cloth();

        ~Cloth(void);

        void BuildCloth();

        void TimeStep();
};