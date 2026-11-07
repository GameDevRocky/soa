#include <iostream>
#include <array>
#include <unordered_map>

namespace std{
    template<>
    struct std::hash<Tag>{
            std::size_t operator()(const Tag& key) const noexcept{
                return std::hash<std::size_t>{}(key.id);
            }
        };
};

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

    bool operator ==(const Tag& other) const noexcept {
        return id == other.id;
    }

    Tag(){
        this->id = counter++;
    }
};

template<typename T, size_t capacity = default_capacity>
class Registry{
    static inline size_t active_count = 0; 
    std::unordered_map<Tag, Block> data;

    static Registry& Get(){
        static Registry registry;
        return registry;
    };
    
    template<typename T, size_t capacity = default_capacity>
    void CreateBlock(){
        Block* block = new Block<T, capacity>();

    };

    void RemoveObject(const std::size_t& index){
        for (const auto& [tag, block] : data){
            block.Remove(index);
        }
    }

};


class Handle{
    std::size_t slot;
    std::size_t generation;

    Handle() = default;
    Handle(std::size_t s, std::size_t g) : slot(s), generation(g){

    } 
};



template<std::size_t capacity = default_capacity>
class Object{
    

    protected:
        const Handle handle;


}


#define dod_property(data_type, property_name) \
    struct property_name##_field_tag {}; \
    soa::Property<data_type, property_name##_field_tag> property_name