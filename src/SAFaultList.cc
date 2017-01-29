#include "SAFaultList.h"

SAFaultList::SAFaultList()
{

}

std::vector<SAFault *> SAFaultList::faultList() const
{
    return m_faultList;
}

void SAFaultList::addFault(SAFault* fault)
{
    m_faultList.push_back(fault);
}
