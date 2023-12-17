#include "KeyIds.hpp"

std::ostream &operator<<(std::ostream &os, const KeyId id)
{
    switch (id)
    {
    case KeyId::NONE:
        return os << "KeyID::NONE";
        break;
    case KeyId::LEFT:
        return os << "KeyID::LEFT";
        break;
    case KeyId::RIGHT:
        return os << "KeyID::RIGHT";
        break;
    case KeyId::ENTER:
        return os << "KeyID::ENTER";
        break;
    case KeyId::BACK:
        return os << "KeyID::BACK";
        break;
    case KeyId::TASK1:
        return os << "KeyID::TASK1";
        break;
    case KeyId::TASK2:
        return os << "KeyID::TASK2";
        break;
    case KeyId::TASK3:
        return os << "KeyID::TASK3";
        break;
    case KeyId::TASK4:
        return os << "KeyID::TASK4";
        break;
    default:
        return os << "Unknown KeyID";
        break;
    }
}
