// Copyright 2026, Rocklyn Clarke & Elias Lopes
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Rocklyn Clarke & Elias Lopes nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <typeindex>
#include <unordered_map>
#include <vector>
#include <string>
#include <typeinfo>

#pragma once


#define DOD_CLASS(ClassName) class ClassName : public DOD::Handle<ClassName>

#define DOD_PUBLIC_FIELD(Type, Name)                    \
    struct Name##_tag {};                               \
                                                        \
    void set_##Name(Type value) {                       \
        this->template Set<Name##_tag>(value);          \
    }                                                   \
                                                        \
    Type get_##Name() const {                           \
        return this->template Get<Name##_tag, Type>();  \
    }

namespace DOD
{

    /**
     * @brief Key for context storage map
     *
     * @tparam Class
     * @tparam Tag
     */
    template<typename Class, typename Tag>
    struct FieldKey {};


    /**
     * @brief Parent storage column type to avoid requiring
     *  type for context storage map
     *
     */
    struct StorageColumn
    {
        virtual ~StorageColumn() = default;
    };

    /**
     * @brief Vector of types to store
     *  data of the same field contiguosly
     *
     * @tparam T
     */
    template<typename T>
    struct TypedStorageColumn
    {
        std::vector<T> values;
    };


    template<typename Derived>
    class Handle;


    /**
     * @brief Top level data manager for all
     *  objects that will leverage DOD memory
     *  management.
     *
     */
    class DODContext
    {

    private:

        // Map that stores array of data for each field
        // key is FieldKey<ClassName, FieldTag>
        std::unordered_map<
                    std::type_index,
                    std::unique_ptr<StorageColumn>
                    > storageMap;

        // Tracks number of instances of a type
        // have been created by this context
        std::unordered_map<std::type_index, unsigned int> classInstancesCount;

    public:

        // @Improvements: Add support for arg-constructors
        /**
         * @brief Creates a single DOD_CLASS instance
         *
         * @tparam T
         * @return Handle<T>
         */
        template<typename T>
        inline Handle<T> Create()
        {
            // Create handler controller
            Handle<T> handle;

            // Attach context
            handle.context = this;

            // Find current objects index
            handle.index = this->classInstancesCount.count(typeid(T));
        }


        /**
         * @brief Retrieve a value from class types
         *  SOA storage
        *
         * @tparam Derived
         * @tparam Tag
         * @tparam T
         * @param index
         * @return T
         */
        template<typename Derived, typename Tag, typename T>
        inline T Get(const unsigned int& index)
        {
            FieldKey<Derived, Tag> key {};

            // Check if any instances have been made at all
            if (this->classInstancesCount.count(typeid(Derived)) == 0)
              throw std::invalid_argument("No instances of class have been stored by this DOD context.");

            try
            {
                // Extract data by field key and index
                TypedStorageColumn<T> columnStruct = this->storageMap.at(key);

                if (0 > index >= columnStruct.values.size())
                  throw std::invalid_argument("Index out of bounds of field data.");

                T data = columnStruct.values.at(index);
                return data;
            }
            catch(...)
            {
                throw std::runtime_error("Unable to fetch field from storage at index " +
                    std::to_string(index));
            }
        }


        /**
         * @brief Sets a field of a class inside
         *  context storage
        *
         * @tparam Derived
         * @tparam Tag
         * @tparam T
         * @param index
         * @param value
         */
        template<typename Derived, typename Tag, typename T>
        inline void Set(const unsigned int& index, T value)
        {
            const FieldKey<Derived, Tag> key {};

            TypedStorageColumn<T> columns;

            // Get data column from storage
            // Note: May be setting column data for the first time
            if (storageMap.count(key) == 0)
                storageMap[key] = columns;
            else
                columns = storageMap[key];

            const unsigned int columnSize = columns.values.size();

            // Add initial value to front or back
            if ((index == 0 && columnSize == 0) || index == columnSize)
            {
                columns.values.push_back(value);
                return;
            }
            else if (0 <= index && index < columnSize)
            {
                columns.values[index] = value;
                return;
            }
            else
            {
                throw std::invalid_argument("Index out of bounds of field data.");
            }
        }

    };

    /**
     * @brief Parent class that all generated
     *  DOD class handlers will inherit from
     *
     * @tparam T
     */
    template<typename Derived>
    class Handle
    {

    public:
        DODContext* context;  // ptr to context that owns DOD_CLASS
        uint32_t index;       // Index of where current instance sits inside context

    protected:
        /**
         * @brief Setter method that sets a DOD_FIELD
         *  inside context storage
         *
         * @tparam Tag
         * @tparam T
         * @param value
         */
        template<typename Tag, typename T>
        void Set(T value)
        {
            context->Set<Derived, Tag>(index, value);
        }

        /**
         * @brief Getter method that retrieves
         *  data from context storage
         *
         * @tparam Tag
         * @tparam T
         * @return T
         */
        template<typename Tag, typename T>
        T Get() const
        {
            return context->Get<Derived, Tag, T>(index);
        }
    };


}
