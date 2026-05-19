import { defineConfig } from "vite";

export default defineConfig({
  base: "./",
  build: {
    outDir: "../app-dist",
    emptyOutDir: true,
  },
  server: {
    open: true,
    fs: {
      allow: ['..'],
    },
    headers: {
      "Cross-Origin-Opener-Policy": "same-origin",
      "Cross-Origin-Embedder-Policy": "require-corp",
    },
  },
});
