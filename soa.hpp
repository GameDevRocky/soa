#include <iostream>
#include <array>
#include <unordered_map>

static inline constexpr size_t default_capacity = 100'000;

template <typename T, size_t capacity = default_capacity>
struct Block{
    std::array<T, capacity> data;

    void Remove(size_t index, size_t last_active){
        std::swap(data[index], data[last_active]);
        data[last_active] = {};
    };

};

struct Tag{
    static inline size_t counter = 0;
    size_t id;
    Tag(){
        this->id = counter++;
    }
};

template<typename T, size_t capacity = default_capacity>
class Registry{

    std::unordered_map<Tag, Block> data;

    static Registry& Get(){
        static Registry registry;
        return registry;
    };
    
    template<typename T, size_t capacity = default_capacity>
    void CreateBlock(){

    };
    
    void Find


};





template<typename T, size_t capacity = default_capacity>
#define property