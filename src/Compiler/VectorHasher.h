#ifndef VECTORHASHER_H
#define VECTORHASHER_H

#include <vector>

namespace Core {
    template <typename T>
    struct VectorHasher {
        size_t operator()(const std::vector<T>& V) const noexcept {
            size_t hash = V.size();
            for(auto &i : V) {
                hash ^= std::hash<T>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };
}

#endif
