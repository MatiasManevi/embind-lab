#ifndef THIRD_PARTY_WASM_CAR_WASH_CAR_WASH_H_
#define THIRD_PARTY_WASM_CAR_WASH_CAR_WASH_H_

#include <string>
#include <vector>

namespace car_wash
{

  // A complex C-style struct simulating MuJoCo's C API
  struct myCar
  {
    int id;
    float dirt_level;
    char plate[10];
  };

  // C-style functions simulating MuJoCo's C API
  void wash_car(myCar *car);
  void compute_required_resources(myCar *car, float *resources_out);
  void run_diagnostics(myCar *car, int *error_codes, int count);

} // namespace car_wash

#endif // THIRD_PARTY_WASM_CAR_WASH_CAR_WASH_H_
