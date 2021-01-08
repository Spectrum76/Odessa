#include "pch.h"
#include "Types.h"

bool Vertex::operator==(const Vertex& other) const
{
    return Position == other.Position && TexCoord == other.TexCoord && Normal == other.Normal;
}
