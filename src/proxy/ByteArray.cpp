#include "proxy/ByteArray.hpp"

#include <sstream>

void __index(LuaStack& stack)
{
    QByteArray& array = stack.as<QByteArray&>(1);
    stack.shift();

    if (stack.type(1) == lua::type::number) {
        auto index = stack.as<int>(1);
        stack.clear();
        if (index < 1 || index > array.size()) {
            std::stringstream str;
            str << "Index must be within range [1, " << array.size() << "], but was given " << index;
            throw LuaException(str.str());
        }
        int data = array.at(index - 1);
        if (data < 0) {
            // Convert the signed char to an unsigned one (since the byte is stored as two's complement)
            data = 256 + data;
        }
        lua::push<int>(stack, data);
        return;
    }

    lua::push(stack, lua::value::nil);
}

int __len(QByteArray& arr)
{
    return arr.size();
}

QString __tostring(QByteArray& arr)
{
    return QString(arr);
}

namespace rainback {
namespace proxy {

void initializeByteArray(LuaStack& stack, QByteArray& array)
{
    stack.pushMetatable(1);
    auto mt = stack.saveAndPop();

    mt["__index"] = __index;
    mt["__len"] = __len;
    mt["__tostring"] = __tostring;
}

} // namespace proxy
} // namespace rainback
