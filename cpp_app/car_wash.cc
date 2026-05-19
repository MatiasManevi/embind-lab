#include "car_wash.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

namespace car_wash
{

  void wash_car(myCar *car)
  {
    if (!car)
      return;
    // Simulate washing
    car->dirt_level = 0.0f;
  }

  void compute_required_resources(myCar *car, float *resources_out)
  {
    if (!car || !resources_out)
      return;
    // Compute resources based on dirt level
    resources_out[0] = car->dirt_level * 1.5f; // Water liters
    resources_out[1] = car->dirt_level * 0.2f; // Soap liters
    resources_out[2] = car->dirt_level * 0.5f; // Time minutes
  }

  void run_diagnostics(myCar *car, int *error_codes, int count)
  {
    if (!car || !error_codes)
      return;
    for (int i = 0; i < count; ++i)
    {
      // Fill with some mock diagnostic codes based on car ID
      error_codes[i] = car->id + (std::rand() % 11) * i;
    }
  }

} // namespace car_wash
