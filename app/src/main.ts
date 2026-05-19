import loadCarWash from "../../dist/car_wash.js"


async function runLab() {
  const outputElement = document.getElementById('output');
  if (!outputElement) return;

  const log = (msg: string) => {
    console.log(msg);
    outputElement.textContent += msg + '\n';
  };

  log('Loading WASM module...');
  const Module = await loadCarWash();
  log('WASM module loaded successfully.\n');

  // -------------------------------------------------------------------------
  // Decision 1: Wrapped classes for complex structs
  // -------------------------------------------------------------------------
  log('--- Decision 1: Wrapped Classes ---');
  const car = new Module.Car('XYZ-789', 85.5);
  log(`Car created with Id: ${car.id}`);
  log(`Car created with Plate: ${car.plate}`);
  log(`Car created with Dirt Level: ${car.dirtLevel}\n`);
  const car2 = new Module.Car('AB-723-GF', 100);
  log(`Car 2 created with Id: ${car2.id}`);
  log(`Car 2 created with Plate: ${car2.plate}`);
  log(`Car 2 created with Dirt Level: ${car2.dirtLevel}\n`);

  // -------------------------------------------------------------------------
  // Decision 2: WasmBuffer for zero-copy interaction
  // -------------------------------------------------------------------------
  log('--- Decision 2: WasmBuffer ---');
  const bufferSize = 5;
  const errorCodesBuffer = new Module.IntBuffer(bufferSize);

  // Pass the buffer to the wrapper function
  Module.runDiagnostics(car, errorCodesBuffer);

  // Get a TypedArray view of the WASM memory
  const view = errorCodesBuffer.getView();
  log('Diagnostic results (zero-copy view from JS):');
  for (let i = 0; i < errorCodesBuffer.getElementCount(); i++) {
    log(`  Diagnostic code [${i}]: ${view[i]}`);
  }

  // Demonstrate zero-copy: modify in JS, WASM would see it (if we had a
  // function to print it)
  view[0] = 999;
  log(`\nModified view[0] in JS to: ${view[0]}`);
  log('Any read/write operation on this view interacts directly with WASM heap memory.\n');

  // -------------------------------------------------------------------------
  // Decision 3: Wrapper Functions Scenarios
  // -------------------------------------------------------------------------
  log('--- Decision 3: Wrapper Functions Scenarios ---');

  // Scenario 1: Handling struct type conversions
  log('\nScenario 1: Handling struct type conversions');
  Module.washCar(car);
  log('Car washed.');
  log(`Post-wash Car Dirt Level: ${car.dirtLevel}`);

  // Scenario 2: Handling other unsupported types
  log('\nScenario 2: Handling other unsupported types');
  log(`Original Plate: ${car.plate}`);
  car.plate = 'ABC-123';
  log(`Updated Plate: ${car.plate}`);

  // Scenario 3: Handling Out Parameters
  log('\nScenario 3: Handling Out Parameters');
  const resourcesBuffer =
    Module.FloatBuffer.FromArray(new Float32Array([0, .2, .3]));
  car.dirtLevel = 75.0;
  Module.computeRequiredResources(car, resourcesBuffer);
  const resources = resourcesBuffer.getView();
  log(`Required Resources (Water L, Soap L, Time Min): [${resources[0]}, ${resources[1]}, ${resources[2]}]\n`);

  log('\nLab completed successfully.');
}

window.onload = runLab;
