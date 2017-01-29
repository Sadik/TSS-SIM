#pragma once

#include "SAFault.h"

class SAFaultList
{
public:
    SAFaultList();

    std::vector<SAFault *> faultList() const;

    void addFault(SAFault *fault);

private:
    std::vector<SAFault*> m_faultList;
};
