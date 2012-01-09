#include "gmwaccount.h"

GMWAccount::GMWAccount(int id, const QString &decription):
    m_id(id), m_description(decription)
{
}

bool GMWAccount::isValid() const
{
    return m_id != -1 && !m_description.isEmpty();
}

int GMWAccount::id() const
{
    return m_id;
}

QString GMWAccount::description() const
{
    return m_description;
}

bool GMWAccount::operator ==(const GMWAccount &other) const
{
    return m_id == other.id();
}

bool GMWAccount::operator !=(const GMWAccount &other) const
{
    return m_id != other.id();
}
