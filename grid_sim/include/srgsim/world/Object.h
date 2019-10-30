#pragma once

#include "srgsim/SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>
#include <essentials/Identifier.h>

namespace srgsim
{

class Cell;

class Object
{
public:
    Object(Type type, essentials::IdentifierConstPtr id,  State state = State::Undefined);
    virtual ~Object();

    essentials::IdentifierConstPtr getID() const;
    Type getType() const;
    void setType(Type type);
    State getState() const;
    void setState(State state);

    Cell* getCell();
    void setCell(Cell* cell);
    void deleteCell();

    friend std::ostream& operator<<(std::ostream& os, const srgsim::Object& obj)
    {
        os << "ID: " << obj.id << " Type: " << static_cast<int>(obj.type) << " State: " << static_cast<int>(obj.state) << std::endl;
        return os;
    }
protected:
    Cell* cell;
    Type type;
    State state;
    essentials::IdentifierConstPtr id;
};

} // namespace srgsim
