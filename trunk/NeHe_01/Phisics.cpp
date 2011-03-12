#include "StdAfx.h"
#include "Phisics.h"

static Phisics *s_instance;

static Phisics *instance()
{
    if (!s_instance)
        s_instance = new Phisics();
    return s_instance;
}

Phisics::Phisics(void)
{
}


Phisics::~Phisics(void)
{
}
