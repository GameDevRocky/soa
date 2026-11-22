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

namespace dod {

// Custom excpetion classes

/*
 * Top level class that all DOD classes
 * will inherit from, owns the life cycle
 * of all class registries
 *
 * @Owner: Class that will be managed
 */
template <class Owner> class Object {

public:
  // Disable copy constructor and copy assignment
  // from public client scope
  Object(const Object &) = delete;
  Object &operator=(const Object &) = delete;

  Handle DODHandle() const noexcept;

protected:
  /**
   * Creates a single registry for class type
   */
  Object();

  /**
   * Destory an instance
   */
  ~Object() noexcept;

  // Only allow these operations for
  // inherited classes
  Object(Object &&other) noexcept;
  Object &operator=(Object &&other) noexcept;

private:
  // object instance handle
  Handle m_handle;
};

} // namespace dod
