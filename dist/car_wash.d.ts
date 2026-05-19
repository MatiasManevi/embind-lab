// TypeScript bindings for emscripten-generated code.  Automatically generated at compile time.
interface WasmModule {
}

type EmbindString = ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string;
export interface ClassHandle {
  isAliasOf(other: ClassHandle): boolean;
  delete(): void;
  deleteLater(): this;
  isDeleted(): boolean;
  // @ts-ignore - If targeting lower than ESNext, this symbol might not exist.
  [Symbol.dispose](): void;
  clone(): this;
}
export interface Car extends ClassHandle {
  id: number;
  get plate(): string;
  set plate(value: EmbindString);
  dirtLevel: number;
}

export interface IntBuffer extends ClassHandle {
  getPointer(): number;
  getView(): any;
  getElementCount(): number;
}

export interface FloatBuffer extends ClassHandle {
  getPointer(): number;
  getView(): any;
  getElementCount(): number;
}

interface EmbindModule {
  Car: {
    new(_0: EmbindString, _1: number): Car;
  };
  IntBuffer: {
    new(): IntBuffer;
    new(_0: number): IntBuffer;
    FromArray(_0: any): IntBuffer;
  };
  FloatBuffer: {
    new(): FloatBuffer;
    new(_0: number): FloatBuffer;
    FromArray(_0: any): FloatBuffer;
  };
  washCar(_0: Car): void;
  runDiagnostics(_0: Car, _1: IntBuffer): void;
  computeRequiredResources(_0: Car, _1: FloatBuffer): void;
}

export type MainModule = WasmModule & EmbindModule;
export default function MainModuleFactory (options?: unknown): Promise<MainModule>;
