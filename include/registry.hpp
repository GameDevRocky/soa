// Copyright 2026, Elias Lopes & Rocklyn Clarke
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of Elias Lopes & Rocklyn Clarke nor the names of its
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <array>
#include <limits>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace dod {

/**
 *
 * Tracker: Represents the location of the
 * object inside its type registry
 */
struct Handle {
  // a large positive int for an out of bounds index
  const static unsigned int invalidIndex =
      std::numeric_limits<unsigned int>::max();

  // position of where object data
  // is inside registry rows
  unsigned int slot = invalidIndex;

  unsigned int generation = 0;
};

// interface for all feild data
// so StorageMap will accept all data types
struct IFieldBlock {
  virtual ~IFieldBlock() = default;
};

template <typename T, int maxCapacity>
struct TypedFieldBlock : public IFieldBlock {
  std::array<T, maxCapacity> data;
};

// Lookup table used to track which rows
// an objects data maps to based of slot
template <unsigned int maxCapacity>
using SlotLookupTable = std::array<unsigned int, maxCapacity>;

// Contains blocks of object data
// aligned in parrellel array form
using StorageMap =
    std::unordered_map<std::type_index, std::unique_ptr<IFieldBlock>>;

template <class Owner, unsigned int maxCapacity> class Registry {

public:
  // tracks which field row each
  // object slot maps maps to
  SlotLookupTable<maxCapacity> m_slotTable;

  // Stores class instances
  // data inside arrays
  StorageMap m_storageMap;

  Registry();

  /**
   * Registers a field block with room for maxCapacity values.
   * @tparam Tag Unique type identifying the field within this registry.
   * @tparam T Type of the values stored in the block.
   * @throws std::invalid_argument If Tag is already registered.
   */
  template <typename Tag, typename T> void CreateStorageField() {

    const std::type_index indexType = typeid(Tag);

    // throw error if field exists already
    if (m_storageMap.count(indexType) != 0) {
      throw std::invalid_argument(
          "Cannot create storage field, field already exists in storageMap.");
    }

    m_storageMap.emplace(indexType,
                         std::make_unique<TypedFieldBlock<T, maxCapacity>>());
  }

  /**
   * Looks up a registered field block by tag and checks its value type.
   * @tparam Tag Type identifying the registered field.
   * @tparam T Expected value type.
   * @return Mutable reference to the field block.
   * @throws std::out_of_range If Tag is unregistered.
   * @throws std::invalid_argument If the registered block has a different type.
   */
  template <typename Tag, typename T> TypedFieldBlock<T, maxCapacity> &Get() {

    // Get the field block at the tag key
    const auto &field = m_storageMap.at(std::type_index(typeid(Tag)));

    // cast to a typed field
    auto *block = dynamic_cast<TypedFieldBlock<T, maxCapacity> *>(field.get());

    // block not found
    if (block == nullptr) {
      throw std::invalid_argument("Storage field type does not match its tag.");
    }

    return *block;
  }

  /**
   * Looks up a value at a compile-time row index within a field block.
   * @tparam Tag Type identifying the registered field.
   * @tparam T Expected value type.
   * @tparam Index Row index, which must be less than maxCapacity.
   * @return Mutable reference to the value at Index.
   * @throws std::out_of_range If Tag is unregistered.
   * @throws std::invalid_argument If the registered block has a different type.
   */
  template <typename Tag, typename T, unsigned int Index> T &Get() {
    static_assert(Index < maxCapacity, "Storage field index is out of range.");
    return Get<Tag, T>().data[Index];
  }

  /**
   * Assigns a value at a compile-time row index within a field block.
   * @tparam Tag Type identifying the registered field.
   * @tparam T Expected value type.
   * @tparam Index Row index, which must be less than maxCapacity.
   * @tparam Value Type deduced from the supplied value.
   * @param value Value assignable to the field's value type.
   * @throws std::out_of_range If Tag is unregistered.
   * @throws std::invalid_argument If the registered block has a different type.
   */
  template <typename Tag, typename T, unsigned int Index,
            typename ValueType = T>
  void Set(ValueType &&value) {
    Get<Tag, T, Index>() = std::forward<ValueType>(value);
  }
};

} // namespace dod
