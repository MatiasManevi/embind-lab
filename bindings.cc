#include "cpp_app/car_wash.h"
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <cstring>
#include <vector>

namespace car_wash_bindings {

// -----------------------------------------------------------------------------
// Design Decision 1: Wrapped Classes
// -----------------------------------------------------------------------------
// Wrapping myCar struct in a C++ class to avoid value_object serialization
// overhead and safely manage raw pointer lifecycle.
class Car {
 public:
  Car(const std::string& plate, float dirt_level) {
    car_ = new car_wash::myCar();
    car_->id = 0;
    car_->dirt_level = dirt_level;
    std::strncpy(car_->plate, plate.c_str(), 9);
    car_->plate[9] = '\0';
  }

  ~Car() { delete car_; }

  car_wash::myCar* get() const { return car_; }

  int GetId() const { return car_->id; }
  void SetId(int id) { car_->id = id; }

  float GetDirtLevel() const { return car_->dirt_level; }
  void SetDirtLevel(float level) { car_->dirt_level = level; }

  std::string GetPlate() const { return std::string(car_->plate); }
  void SetPlate(const std::string& plate) {
    std::strncpy(car_->plate, plate.c_str(), 9);
    car_->plate[9] = '\0';
  }

 private:
  car_wash::myCar* car_;
};

// -----------------------------------------------------------------------------
// Design Decision 2: WasmBuffer for Zero-Copy Arrays
// -----------------------------------------------------------------------------
// Zero-copy buffer using typed_memory_view so JS reads/writes directly
// into the WASM heap without copying.
template <typename T>
class WasmBuffer {
 public:
  WasmBuffer() : data_(0) {}

  explicit WasmBuffer(int size) : data_(size) {}

  static WasmBuffer FromArray(emscripten::val js_array) {
    const int length = js_array["length"].as<int>();
    WasmBuffer buffer;
    buffer.data_.resize(length);
    for (int i = 0; i < length; ++i) {
      buffer.data_[i] = js_array[i].as<T>();
    }
    return buffer;
  }

  uintptr_t GetPointer() {
    return reinterpret_cast<uintptr_t>(data_.data());
  }

  int GetElementCount() const { return static_cast<int>(data_.size()); }

  emscripten::val GetView() const {
    return emscripten::val(emscripten::typed_memory_view(data_.size(),
                                                         data_.data()));
  }

 private:
  std::vector<T> data_;
};

// -----------------------------------------------------------------------------
// Design Decision 3: Wrapper Functions for Complex Signatures
// -----------------------------------------------------------------------------

// Scenario 1: struct* -> class reference
void WashCarWrapper(Car& car) { car_wash::wash_car(car.get()); }

// Scenario 3: out parameters -> WasmBuffer
void ComputeRequiredResourcesWrapper(Car& car,
                                     WasmBuffer<float>& resources_out) {
  car_wash::compute_required_resources(
      car.get(), reinterpret_cast<float*>(resources_out.GetPointer()));
}

// runDiagnostics also uses WasmBuffer for the int* out parameter
void RunDiagnosticsWrapper(Car& car, WasmBuffer<int>& error_codes) {
  car_wash::run_diagnostics(car.get(),
                            reinterpret_cast<int*>(error_codes.GetPointer()),
                            error_codes.GetElementCount());
}

// -----------------------------------------------------------------------------
// Embind Registration
// -----------------------------------------------------------------------------
using emscripten::class_;
using emscripten::function;

EMSCRIPTEN_BINDINGS(car_wash) {
  // --- class_ (order: class_ before function) ---
  class_<Car>("Car")
      .constructor<const std::string&, float>()
      .property("id", &Car::GetId, &Car::SetId)
      .property("plate", &Car::GetPlate, &Car::SetPlate)
      .property("dirtLevel", &Car::GetDirtLevel, &Car::SetDirtLevel);

  // --- WasmBuffer<T> specializations ---
  class_<WasmBuffer<int>>("IntBuffer")
      .constructor<>()
      .constructor<int>()
      .class_function("FromArray", &WasmBuffer<int>::FromArray)
      .function("getPointer", &WasmBuffer<int>::GetPointer)
      .function("getView", &WasmBuffer<int>::GetView)
      .function("getElementCount", &WasmBuffer<int>::GetElementCount);

  class_<WasmBuffer<float>>("FloatBuffer")
      .constructor<>()
      .constructor<int>()
      .class_function("FromArray", &WasmBuffer<float>::FromArray)
      .function("getPointer", &WasmBuffer<float>::GetPointer)
      .function("getView", &WasmBuffer<float>::GetView)
      .function("getElementCount", &WasmBuffer<float>::GetElementCount);

  // --- function ---
  function("washCar", &WashCarWrapper);
  function("runDiagnostics", &RunDiagnosticsWrapper);
  function("computeRequiredResources", &ComputeRequiredResourcesWrapper);
}

}  // namespace car_wash_bindings