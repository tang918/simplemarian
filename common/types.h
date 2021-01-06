#pragma once
#include "common/shape.h"
namespace marian
{
size_t requiredBytes(const Shape& shape){
  return shape.elements()*sizeof(float);
}
} // namespace marian

