#include "mocks.hpp"

void StringSink::add(const QString& str)
{
    strings << str;
}
